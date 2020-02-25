#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgoFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgoFactory_h

#include <memory>
#include <string>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace ecalPh2 {

class SpikeTaggerLDAlgoFactory {
 public:
  SpikeTaggerLDAlgoFactory() {};
  ~SpikeTaggerLDAlgoFactory() {};

  typedef std::unique_ptr<SpikeTaggerLDAlgo> ReturnType;

  ReturnType create(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
};

} // namespace ecalPh2
#endif
