#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadFactory_h

#include <memory>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayload.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalph2 {

class BCPPayloadFactory {
 public:
  BCPPayloadFactory() {};
  ~BCPPayloadFactory() {};

  typedef std::unique_ptr<BCPPayload> ReturnType;

  ReturnType create(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
};

} // namespace ecalph2
#endif
