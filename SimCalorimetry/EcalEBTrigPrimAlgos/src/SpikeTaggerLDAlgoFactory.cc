///
/// \class ecalph2::SpikeTaggerLDAlgoFactory
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP spike tagger LD algorithm factory. Based on the
/// firmware version and a type string it selects the appropriate algorithms.
///

#include <iomanip>
#include <sstream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgoFactory.h"

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDHLSAlgoV1.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDIdealAlgoV1.h"

ecalph2::SpikeTaggerLDAlgoFactory::ReturnType ecalph2::SpikeTaggerLDAlgoFactory::create(
    const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper,
    const edm::EventSetup &eventSetup) {
  ReturnType spikeTaggerLDAlgo;

  const auto algoType = ecalBcpPayloadParamsHelper->spikeTaggerLdType();
  const auto fwVersion = ecalBcpPayloadParamsHelper->fwVersion();

  // FW string for messages
  std::stringstream fwStrStream;
  fwStrStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
  const auto fwStr = fwStrStream.str();

  // factory
  if (algoType == "ideal") {
    if (fwVersion >= 1) {
      edm::LogInfo("ecalph2::SpikeTaggerLDAlgoFactory")
          << "Creating ideal spike tagger LD algo for FW version " << fwStr;
      spikeTaggerLDAlgo = std::make_unique<ecalph2::SpikeTaggerLDIdealAlgoV1>(ecalBcpPayloadParamsHelper, eventSetup);
    } else {
      edm::LogError("ecalph2::SpikeTaggerLDAlgoFactory")
          << "No ideal spike tagger LD algo to create for FW version " << fwStr;
    }
  } else if (algoType == "hls") {
    if (fwVersion >= 1) {
      edm::LogInfo("ecalph2::SpikeTaggerLDAlgoFactory") << "Creating HLS spike tagger LD algo for FW version " << fwStr;
      spikeTaggerLDAlgo = std::make_unique<ecalph2::SpikeTaggerLDHLSAlgoV1>(ecalBcpPayloadParamsHelper, eventSetup);
    } else {
      edm::LogError("ecalph2::SpikeTaggerLDAlgoFactory")
          << "No HLS spike tagger LD algo to create for FW version " << fwStr;
    }
  } else {
    edm::LogError("ecalph2::SpikeTaggerLDAlgoFactory") << "Unknown spike tagger LD algo type '" << algoType << "'";
  }

  return spikeTaggerLDAlgo;
}
