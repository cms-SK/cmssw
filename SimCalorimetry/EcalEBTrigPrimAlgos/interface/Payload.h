#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_Payload_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_Payload_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

namespace bcp {

class Payload {
 public:
  Payload(const unsigned int fwVersion, const edm::ParameterSet& config, const edm::EventSetup &eventSetup) : fwVersion_(fwVersion) {};
  virtual ~Payload() {};

  virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) = 0;

 protected:
  virtual void createAlgos(const edm::ParameterSet& config, const edm::EventSetup &eventSetup) = 0;

  unsigned int fwVersion_;
};

} // namespace bcp
#endif
