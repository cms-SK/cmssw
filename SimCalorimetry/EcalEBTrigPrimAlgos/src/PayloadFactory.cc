///
/// \class bcp::PayloadFactory
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP FW payload factory. Based on the firmware
/// version it selects the appropriate payload algorithm. A new payload
/// implementation is needed if algo blocks in the payload are added or
/// removed, or if the data format between algo blocks changes.
///

#include <iomanip>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/PayloadFactory.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/PayloadV1.h"

bcp::PayloadFactory::ReturnType bcp::PayloadFactory::create(const unsigned int fwVersion, const edm::ParameterSet& config, const edm::EventSetup &eventSetup)
{
  ReturnType payload;
  if (fwVersion >= 1) {
    payload = std::make_unique<bcp::PayloadV1>(fwVersion, config, eventSetup);
  } else {
    edm::LogError("bcp::PayloadFactory") << "Invalid BCP payload FW version: 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
  }
  return payload;
}

