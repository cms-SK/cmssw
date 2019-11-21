#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_PayloadFactory_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_PayloadFactory_h

#include <memory>

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/Payload.h"

namespace bcp {

class PayloadFactory {
 public:
  PayloadFactory() {};
  ~PayloadFactory() {};

  typedef std::unique_ptr<Payload> ReturnType;

  ReturnType create(const unsigned int fwVersion, const edm::ParameterSet& config, const edm::EventSetup &eventSetup);
};

} // namespace bcp
#endif
