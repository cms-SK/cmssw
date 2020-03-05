///
/// \class ecalPh2::MultiFitTimingAlgoFactory
///
/// \author: Thomas Reis
///
///
/// This class implements the multifit timing algorithm factory for the BCP.
/// Based on the firmware version and a type string it selects the appropriate
/// algorithms.
///

#include <iomanip>
#include <sstream>
#include <string>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgoFactory.h"

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgoV1.h"

ecalPh2::MultiFitTimingAlgoFactory::ReturnType ecalPh2::MultiFitTimingAlgoFactory::create(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup)
{
  ReturnType multiFitTimingAlgo;

  //const auto algoType = ecalBcpPayloadParamsHelper->multiFitTimingAlgoType();
  const auto algoType = std::string("dummy");
  const auto fwVersion = ecalBcpPayloadParamsHelper->fwVersion();

  // FW string for messages
  std::stringstream fwStrStream;
  fwStrStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
  const auto fwStr = fwStrStream.str();

  // factory
  if (algoType == "dummy") {
    if (fwVersion >= 1) {
      edm::LogInfo("ecalPh2::MultiFitTimingAlgoFactory") << "Creating multifit timing algo for FW version " << fwStr;
      multiFitTimingAlgo = std::make_unique<ecalPh2::MultiFitTimingAlgoV1>(ecalBcpPayloadParamsHelper, eventSetup);
    } else {
      edm::LogError("ecalPh2::MultiFitTimingAlgoFactory") << "No multifit timing algo to create for FW version " << fwStr;
    }
  } else {
    edm::LogError("ecalPh2::MultiFitTimingAlgoFactory") << "Unknown multifit timing algo type '" << algoType << "'";
  }

  return multiFitTimingAlgo;
}

