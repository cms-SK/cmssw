#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgoFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDAlgoFactory_h

#include <memory>
#include <string>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace bcp {

class SpikeTaggerLDAlgoFactory {
 public:
  SpikeTaggerLDAlgoFactory() {};
  ~SpikeTaggerLDAlgoFactory() {};

  typedef std::unique_ptr<SpikeTaggerLDAlgo> ReturnType;

  ReturnType create(const edm::ParameterSet& config, const edm::EventSetup &eventSetup);
};

} // namespace bcp
#endif
