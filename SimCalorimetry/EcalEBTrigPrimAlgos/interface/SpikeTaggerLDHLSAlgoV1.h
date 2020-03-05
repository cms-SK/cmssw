#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDHLSAlgoV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDHLSAlgoV1_h

#include <array>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace ecalPh2 {

class SpikeTaggerLDHLSAlgoV1 : public SpikeTaggerLDAlgo {
 public:
  SpikeTaggerLDHLSAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
  ~SpikeTaggerLDHLSAlgoV1() override {};

  void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) override;

 private:
  unsigned int peakIdx_;

  std::array<float, 4> gains_;
};

} // namespace ecalPh2
#endif
