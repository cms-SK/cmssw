#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadV1_h

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayload.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace ecalPh2 {

class BCPPayloadV1 : public BCPPayload {
 public:
  BCPPayloadV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup) : BCPPayload(ecalBcpPayloadParamsHelper, eventSetup) { createAlgos(eventSetup); };
  virtual ~BCPPayloadV1() {};

  void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) override;

 protected:
  void createAlgos(const edm::EventSetup &eventSetup) override;

  std::unique_ptr<SpikeTaggerLDAlgo> spikeTaggerLDAlgo_;
};

} // namespace ecalPh2
#endif
