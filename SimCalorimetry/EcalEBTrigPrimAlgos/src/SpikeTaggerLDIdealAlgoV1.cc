///
/// \class ecalPh2::SpikeTaggerLDIdealAlgoV1
///
/// \author: Thomas Reis
/// 
/// Version: V1
/// Implements the ideal BCP linear discriminant spike tagger described in the
/// ECAL phase 2 TDR.
///

#include <iostream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDIdealAlgoV1.h"

ecalPh2::SpikeTaggerLDIdealAlgoV1::SpikeTaggerLDIdealAlgoV1(const edm::ParameterSet& config, const edm::EventSetup &eventSetup) : SpikeTaggerLDAlgo(config, eventSetup)
{
  const auto configSource = config.getParameter<std::string>("configSource");
  if (configSource == "fromES") {
    // getting algo parameters from ES
    const auto &paramsRcd = eventSetup.get<EcalBcpPayloadParamsRcd>();
    edm::ESHandle<EcalBcpPayloadParams> paramsHandle;
    paramsRcd.get(paramsHandle);

    ecalBcpPayloadParamsHelper_ = std::make_unique<EcalBcpPayloadParamsHelper>(*paramsHandle.product());
  } else if (configSource == "fromModuleConfig") {
    ecalBcpPayloadParamsHelper_->createFromPSet(config);
  } else {
    edm::LogError("ecalPh2::SpikeTaggerLDIdealAlgoV1") << "Unknown configuration source '" << configSource << "'";
  }
}

void ecalPh2::SpikeTaggerLDIdealAlgoV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs)
{
  std::cout << "Processing SpikeTaggerLDIdealAlgoV1" << std::endl;
  std::cout << "This frame has size: " << ebDigis.size() << std::endl;

  for (size_t i = 0; i < ebDigis.size(); ++i) {
    EBDataFrame ebDigi = ebDigis[i];
    auto ebDigiId = ebDigi.id();
    std::cout << "digi " << i << ": rawId=" << ebDigiId.rawId() << ", nSamples=" << ebDigi.size() << ", ieta=" << ebDigiId.ieta() << ", iphi=" << ebDigiId.iphi() << std::endl;

    // get the algo parameters for this crystal
    //TODO per crystal gains from ES
    gains_ = { {12., 1., 2., 12.} }; // TIA gains (Currently old gain values. Phase 2 will have only two gains.)
    peakIdx_ = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebDigiId);
    spikeThreshold_ = ecalBcpPayloadParamsHelper_->spikeTaggerLdThreshold(ebDigiId);
    weights_ = ecalBcpPayloadParamsHelper_->spikeTaggerLdWeights(ebDigiId);

    // loop over samples
    for (int j = 0; j < ebDigi.size(); ++j) {
      const auto adcCounts = ebDigi[j].adc();
      const auto gainId = ebDigi[j].gainId();
      float linearlisedCounts = gains_[gainId] * adcCounts;
      std::cout << "digi " << i << ", sample " << j << ": ADC counts=" << adcCounts << ", gain id=" << gainId << ", lin. counts=" << linearlisedCounts << std::endl;
    }
    const auto ld = calcLD(ebDigi);
    std::cout << "LD=" << ld << ", spike=" << (ld < spikeThreshold_) << std::endl;

    // setting the TP sample
    auto ebTPPeakSample = ebTPs[i][peakIdx_];
    auto encodedEt = ebTPPeakSample.encodedEt();
    auto l1aSpike = ebTPPeakSample.l1aSpike() & (ld < spikeThreshold_);
    auto time = ebTPPeakSample.time();
    ebTPs[i].setSample(peakIdx_, EcalEBTriggerPrimitiveSample(encodedEt, l1aSpike, time));
  }
}

float ecalPh2::SpikeTaggerLDIdealAlgoV1::calcLD(const EBDataFrame &frame) const
{
  const auto sPlus1 = gains_[frame[peakIdx_ + 1].gainId()] * frame[peakIdx_ + 1].adc();
  const auto sMax = gains_[frame[peakIdx_].gainId()] * frame[peakIdx_].adc();
  const auto rPlus1 = sPlus1 / sMax;

  return rPlus1 - calcRMinus1Poly(frame);
}

float ecalPh2::SpikeTaggerLDIdealAlgoV1::calcRMinus1Poly(const EBDataFrame &frame) const
{
  const auto sMinus1 = gains_[frame[peakIdx_ - 1].gainId()] * frame[peakIdx_ - 1].adc();
  const auto sMax = gains_[frame[peakIdx_].gainId()] * frame[peakIdx_].adc();
  const auto rMinus1 = sMinus1 / sMax;
  float rMinus1Pow = 1.;
  float rMinus1Poly = 0.;
  for (const auto weight : weights_) {
    rMinus1Poly += weight * rMinus1Pow;
    rMinus1Pow *= rMinus1;
  }

  return rMinus1Poly;
}
