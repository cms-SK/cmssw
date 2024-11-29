///
/// \class ecalph2::MultiFitTimingAlgoV1
///
/// \author: Thomas Reis
///
/// Version: V1
/// Implements a dummy multifit timing algo. It just takes the sample from the
/// digis times the gain and uses it in the TP. The time remains untouched.
///

#include <iostream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgoV1.h"

ecalph2::MultiFitTimingAlgoV1::MultiFitTimingAlgoV1(
    const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper,
    const edm::EventSetup &eventSetup)
    : MultiFitTimingAlgo(ecalBcpPayloadParamsHelper, eventSetup) {}

void ecalph2::MultiFitTimingAlgoV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) {
  std::cout << "Processing MultiFitTimingAlgoV1" << std::endl;

  for (size_t i = 0; i < ebDigis.size(); ++i) {
    EBDataFrame ebDigi = ebDigis[i];
    auto ebDigiId = ebDigi.id();
    //std::cout << "digi " << i << ": rawId=" << ebDigiId.rawId() << ", nSamples=" << ebDigi.size() << ", ieta=" << ebDigiId.ieta() << ", iphi=" << ebDigiId.iphi() << std::endl;

    // get the algo parameters for this crystal
    //TODO per crystal gains from ES
    gains_ = {{12., 1., 2., 12.}};  // TIA gains (Currently old gain values. Phase 2 will have only two gains.)
    peakIdx_ = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebDigiId);

    // dummy ET calculation
    const auto adcCounts = ebDigi[peakIdx_].adc() >> 2;  // Go from 12 bit ADC counts to 10 bit ET
    const auto gainId = ebDigi[peakIdx_].gainId();
    float linearlisedCounts = gains_[gainId] * adcCounts;
    //std::cout << "digi " << i << ", sample " << peakIdx_ << ": ADC counts=" << adcCounts << ", gain id=" << gainId << ", lin. counts=" << linearlisedCounts << std::endl;

    // setting the TP sample
    auto ebTPPeakSample = ebTPs[i][peakIdx_];
    auto encodedEt = linearlisedCounts;
    auto l1aSpike = ebTPPeakSample.l1aSpike();
    auto time = ebTPPeakSample.time();
    //std::cout << "ebTPPeakSample encodedEt=" << encodedEt << ", l1aSpike=" << l1aSpike << ", time=" << time << std::endl;
    ebTPs[i].setSample(peakIdx_, EcalEBTriggerPrimitiveSample(encodedEt, l1aSpike, time));
  }
}
