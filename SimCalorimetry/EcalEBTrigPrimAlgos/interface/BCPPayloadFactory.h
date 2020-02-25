#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadFactory_h

#include <memory>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayload.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalPh2 {

class BCPPayloadFactory {
 public:
  BCPPayloadFactory() {};
  ~BCPPayloadFactory() {};

  typedef std::unique_ptr<BCPPayload> ReturnType;

  ReturnType create(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
};

} // namespace ecalPh2
#endif
