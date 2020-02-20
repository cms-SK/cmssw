#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayload_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_BCPPayload_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

namespace ecalPh2 {

class BCPPayload {
 public:
  BCPPayload(const unsigned int fwVersion, const edm::ParameterSet& config, const edm::EventSetup &eventSetup) : fwVersion_(fwVersion) {};
  virtual ~BCPPayload() {};

  virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) = 0;

 protected:
  virtual void createAlgos(const edm::ParameterSet& config, const edm::EventSetup &eventSetup) = 0;

  unsigned int fwVersion_;
};

} // namespace ecalPh2
#endif
