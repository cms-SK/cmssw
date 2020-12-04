#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgoV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_TPClusterAlgoV1_h

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgo.h"

namespace ecalPh2 {

class TPClusterAlgoV1 : public TPClusterAlgo {
 public:
  TPClusterAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper);
  ~TPClusterAlgoV1() override {};

  void processEvent(const EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters) override;

 private:
  const bool doCluster_;
  const bool doSwissCross_;
  const unsigned int iEtaDiffMax_;
  const unsigned int iPhiDiffMax_;
  const bool useLDSpikesInSum_;
  const int seedEtThreshold_;
  const int clusterCrystalEtThreshold_;
  const int swissCrossCrystalEtThreshold_;
  const unsigned int tpClusterEtThreshold_;
  unsigned int nTPInCluster_;
  const float swissCrossSpikeThreshold_;
};

} // namespace ecalPh2
#endif
