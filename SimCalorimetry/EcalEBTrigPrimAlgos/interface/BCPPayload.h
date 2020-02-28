#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayload_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayload_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveCluster.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalPh2 {

class BCPPayload {
 public:
  BCPPayload(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup) : ecalBcpPayloadParamsHelper_(ecalBcpPayloadParamsHelper) {};
  virtual ~BCPPayload() {};

  virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters) = 0;

 protected:
  // object holding the configuration
  const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper_;

  virtual void createAlgos(const edm::EventSetup &eventSetup) = 0;
};

} // namespace ecalPh2
#endif
