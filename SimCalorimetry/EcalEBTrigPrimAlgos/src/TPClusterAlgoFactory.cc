///
/// \class ecalPh2::TPClusterAlgoFactory
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP crystal clustering algorithm factory. Based on the
/// firmware version and a type string it selects the appropriate algorithms.
///

#include <iomanip>
#include <string>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgoFactory.h"

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgoV1.h"

ecalPh2::TPClusterAlgoFactory::ReturnType ecalPh2::TPClusterAlgoFactory::create(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper)
{
  ReturnType tpClusterAlgo;

  //const auto algoType = ecalBcpPayloadParamsHelper->tpClusterAlgoType();
  const auto algoType = std::string("crystalSumWithSwissCrossSpike");
  const auto fwVersion = ecalBcpPayloadParamsHelper->fwVersion();

  // factory
  if (algoType == "crystalSumWithSwissCrossSpike") {
    if (fwVersion >= 1) {
      edm::LogInfo("ecalPh2::TPClusterAlgoFactory") << "Creating crystal sum TP clustering algo with swiss cross spike estimation for FW version 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
      tpClusterAlgo = std::make_unique<ecalPh2::TPClusterAlgoV1>(ecalBcpPayloadParamsHelper);
    } else {
      edm::LogError("ecalPh2::TPClusterAlgoFactory") << "Invalid FW version for crystal sum TP clustering algo with swiss cross spike estimation: 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
    }
  } else {
    edm::LogError("ecalPh2::TPClusterAlgoFactory") << "Invalid TP clustering LD algo type '" << algoType << "'";
  }

  return tpClusterAlgo;
}

