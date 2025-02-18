#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayloadV1_h

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayload.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgo.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgo.h"

namespace ecalph2 {

  class BCPPayloadV1 : public BCPPayload {
  public:
    BCPPayloadV1(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper,
                 const edm::EventSetup &eventSetup)
        : BCPPayload(ecalBcpPayloadParamsHelper, eventSetup) {
      createAlgos(eventSetup);
    };
    virtual ~BCPPayloadV1() {};

    void processEvent(const EBDigiCollection &ebDigis,
                      EcalEBTrigPrimDigiCollection &ebTPs,
                      std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters) override;

  protected:
    void createAlgos(const edm::EventSetup &eventSetup) override;

    std::unique_ptr<MultiFitTimingAlgo> multiFitTimingAlgo_;
    std::unique_ptr<SpikeTaggerLDAlgo> spikeTaggerLDAlgo_;
    std::unique_ptr<TPClusterAlgo> tpClusterAlgo_;
  };

}  // namespace ecalph2
#endif
