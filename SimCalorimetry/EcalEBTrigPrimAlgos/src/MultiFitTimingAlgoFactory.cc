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

  // factory
  if (algoType == "dummy") {
    if (fwVersion >= 1) {
      edm::LogInfo("ecalPh2::MultiFitTimingAlgoFactory") << "Creating multifit timing algo for FW version 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
      multiFitTimingAlgo = std::make_unique<ecalPh2::MultiFitTimingAlgoV1>(ecalBcpPayloadParamsHelper, eventSetup);
    } else {
      edm::LogError("ecalPh2::MultiFitTimingAlgoFactory") << "Invalid FW version for multifit timing algo: 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
    }
  } else {
    edm::LogError("ecalPh2::MultiFitTimingAlgoFactory") << "Invalid multifit timing algo type '" << algoType << "'";
  }

  return multiFitTimingAlgo;
}

