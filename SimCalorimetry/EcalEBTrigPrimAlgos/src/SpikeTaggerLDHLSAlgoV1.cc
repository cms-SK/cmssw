///
/// \class ecalPh2::SpikeTaggerLDHLSAlgoV1
///
/// \author: Thomas Reis
/// 
/// Version: V1
/// Calls the HLS algorithm for the BCP linear discriminant spike tagger after preparing the data.
///

#include <array>
#include <iostream>

#include "ap_int.h"
#include "EbSpikeTaggerLd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDHLSAlgoV1.h"

using namespace ecalPh2::hls::bcpspiketagger;

ecalPh2::SpikeTaggerLDHLSAlgoV1::SpikeTaggerLDHLSAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup) : SpikeTaggerLDAlgo(ecalBcpPayloadParamsHelper, eventSetup)
{
}

void ecalPh2::SpikeTaggerLDHLSAlgoV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs)
{
  std::cout << "Processing SpikeTaggerLDHLSAlgoV1" << std::endl;
  // do not run if there are no inputs
  if (ebDigis.empty()) {
    std::cout << "This frame is empty. Not running the SpikeTaggerLDHLSAlgoV1 algorithm." << std::endl;
    return;
  } else {
    std::cout << "This frame has size: " << ebDigis.size() << std::endl;
  }

  // Since one FPGA handles 300 channels (ecalPh2::hls::bcpspiketagger::nchannels)
  // the HLS findSpikes function expects samples from as many channels each
  // clock cycle. Three arrays are filled with samples from channels as they
  // are in the ebDigis and once 300 channels are reached the findSpikes function is called.

  // loop over FPGAs, each processing nchannels
  for (size_t fpgaIdx = 0; fpgaIdx <= (ebDigis.size() - 1) / nchannels; ++fpgaIdx) {

    // three consecutive samples for all nchannels
    std::array<std::array<input::indata, nchannels>, 3> samplesArray;
    // loop over ebDigis of nchannels
    for (size_t i = fpgaIdx * nchannels; i < (fpgaIdx + 1) * nchannels and i < ebDigis.size(); ++i) {
      EBDataFrame ebDigi = ebDigis[i];
      auto ebDigiId = ebDigi.id();
      //std::cout << "digi " << i << ": rawId=" << ebDigiId.rawId() << ", nSamples=" << ebDigi.size() << ", ieta=" << ebDigiId.ieta() << ", iphi=" << ebDigiId.iphi() << std::endl;

      // get the algo parameters for this crystal
      //TODO per crystal gains from ES
      gains_ = { {12., 1., 2., 12.} }; // TIA gains (Currently old gain values. Phase 2 will have only two gains.)
      peakIdx_ = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebDigiId);

      // loop over three consecutive samples around the peak
      for (size_t j = peakIdx_ - 1; j <= peakIdx_ + 1 and j < static_cast<size_t>(ebDigi.size()); ++j) {
        const auto adcCounts = ebDigi[j].adc();
        const auto gainId = ebDigi[j].gainId();
        float linearlisedCounts = gains_[gainId] * adcCounts;
        //std::cout << "digi " << i << ", sample " << j << ": ADC counts=" << adcCounts << ", gain id=" << gainId << ", lin. counts=" << linearlisedCounts << std::endl;
        samplesArray[j - peakIdx_ + 1][i % nchannels].sample = static_cast<ap_uint<13>>(linearlisedCounts);
        samplesArray[j - peakIdx_ + 1][i % nchannels].first_sample = (j == peakIdx_ - 1);
        j == peakIdx_ ? samplesArray[j - peakIdx_ + 1][i % nchannels].peak = true : samplesArray[j - peakIdx_ + 1][i % nchannels].peak = false;
      }
    }

    // Run the HLS findSpikes function.
    // After three calls of the function the output contains the spike flag calculated from the three samples.
    std::array<output::outdata, nchannels> sfOutput;
    for (const auto samples : samplesArray) {
      std::cout << "running the HLS spike tagger algo" << std::endl;
      sfOutput = findSpikes(samples);
    }

    // setting the TP samples
    for (size_t i = fpgaIdx * nchannels; i < (fpgaIdx + 1) * nchannels and i < ebDigis.size(); ++i) {
      peakIdx_ = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebDigis[i].id());
      auto ebTPPeakSample = ebTPs[i][peakIdx_];
      auto encodedEt = ebTPPeakSample.encodedEt();
      auto l1aSpike = ebTPPeakSample.l1aSpike() | sfOutput[i % nchannels].spike;
      auto time = ebTPPeakSample.time();
      std::cout << "ebTPPeakSample encodedEt=" << encodedEt << ", l1aSpike=" << l1aSpike << ", time=" << time << std::endl;
      ebTPs[i].setSample(peakIdx_, EcalEBTriggerPrimitiveSample(encodedEt, l1aSpike, time));
    }
  }
}

