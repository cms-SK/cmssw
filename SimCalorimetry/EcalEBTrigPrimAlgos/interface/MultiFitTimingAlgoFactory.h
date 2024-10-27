#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgoFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_MultiFitTimingAlgoFactory_h

#include <memory>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgo.h"

namespace ecalph2 {

class MultiFitTimingAlgoFactory {
 public:
  MultiFitTimingAlgoFactory() {};
  ~MultiFitTimingAlgoFactory() {};

  typedef std::unique_ptr<MultiFitTimingAlgo> ReturnType;

  ReturnType create(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
};

} // namespace ecalph2
#endif
