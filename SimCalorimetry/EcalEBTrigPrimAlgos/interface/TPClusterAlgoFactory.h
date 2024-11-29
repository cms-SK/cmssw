#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgoFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgoFactory_h

#include <memory>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgo.h"

namespace ecalph2 {

  class TPClusterAlgoFactory {
  public:
    TPClusterAlgoFactory() {};
    ~TPClusterAlgoFactory() {};

    typedef std::unique_ptr<TPClusterAlgo> ReturnType;

    ReturnType create(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper);
  };

}  // namespace ecalph2
#endif
