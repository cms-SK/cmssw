#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_PayloadV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_PayloadV1_h

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/Payload.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace bcp {

class PayloadV1 : public Payload {
 public:
  PayloadV1(const unsigned int fwVersion, const edm::ParameterSet& config, const edm::EventSetup &eventSetup) : Payload(fwVersion, config, eventSetup) { createAlgos(config, eventSetup); };
  virtual ~PayloadV1() {};

  void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) override;

 protected:
  void createAlgos(const edm::ParameterSet& config, const edm::EventSetup &eventSetup) override;

  std::unique_ptr<SpikeTaggerLDAlgo> spikeTaggerLDAlgo_;
};

} // namespace bcp
#endif
