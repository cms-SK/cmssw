#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgoV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgoV1_h

#include <array>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgo.h"

namespace ecalPh2 {

class MultiFitTimingAlgoV1 : public MultiFitTimingAlgo {
 public:
  MultiFitTimingAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
  ~MultiFitTimingAlgoV1() override {};

  void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) override;

 private:
  unsigned int peakIdx_;

  std::array<float, 4> gains_;
};

} // namespace ecalPh2
#endif
