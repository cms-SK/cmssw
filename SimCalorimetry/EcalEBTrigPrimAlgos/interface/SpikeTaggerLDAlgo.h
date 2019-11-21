#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgo_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgo_h

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

namespace bcp {

class SpikeTaggerLDAlgo {
 public:
  SpikeTaggerLDAlgo(const edm::ParameterSet& config, const edm::EventSetup &eventSetup) {};
  virtual ~SpikeTaggerLDAlgo() {};

  virtual void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) = 0;
};

} // namespace bcp
#endif
