#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterHLSAlgoV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterHLSAlgoV1_h

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgo.h"

namespace ecalph2 {

  class TPClusterHLSAlgoV1 : public TPClusterAlgo {
  public:
    TPClusterHLSAlgoV1(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper);
    ~TPClusterHLSAlgoV1() override {};

    void processEvent(const EcalEBTrigPrimDigiCollection &ebTPs,
                      std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters) override;
  };

}  // namespace ecalph2
#endif
