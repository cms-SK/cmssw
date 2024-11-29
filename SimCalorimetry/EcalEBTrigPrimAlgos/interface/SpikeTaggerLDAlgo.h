#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgo_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgo_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

namespace ecalph2 {

  class SpikeTaggerLDAlgo {
  public:
    SpikeTaggerLDAlgo(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper,
                      const edm::EventSetup &eventSetup)
        : ecalBcpPayloadParamsHelper_(ecalBcpPayloadParamsHelper) {};
    virtual ~SpikeTaggerLDAlgo() {};

    virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) = 0;

  protected:
    // object holding the configuration
    const std::shared_ptr<EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper_;
  };

}  // namespace ecalph2
#endif
