#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgo_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgo_h

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveCluster.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalph2 {

  class TPClusterAlgo {
  public:
    TPClusterAlgo(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper)
        : ecalBcpPayloadParamsHelper_(ecalBcpPayloadParamsHelper) {};
    virtual ~TPClusterAlgo() {};

    virtual void processEvent(const EcalEBTrigPrimDigiCollection &ebTPs,
                              std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters) = 0;

  protected:
    // object holding the configuration
    const std::shared_ptr<EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper_;
  };

}  // namespace ecalph2
#endif
