///
/// \class ecalph2::BCPPayloadFactory
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
#include <sstream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadFactory.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadV1.h"

ecalph2::BCPPayloadFactory::ReturnType ecalph2::BCPPayloadFactory::create(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup)
{
  const auto fwVersion = ecalBcpPayloadParamsHelper->fwVersion();

  // FW string for messages
  std::stringstream fwStrStream;
  fwStrStream << "0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
  const auto fwStr = fwStrStream.str();

  // create payload
  ReturnType payload;
  if (fwVersion >= 1) {
    edm::LogInfo("ecalph2::BCPPayloadFactory") << "Creating BCP payload for FW version " << fwStr;
    payload = std::make_unique<ecalph2::BCPPayloadV1>(ecalBcpPayloadParamsHelper, eventSetup);
  } else {
    edm::LogError("ecalph2::BCPPayloadFactory") << "No BCP payload to create for FW version " << fwStr;
  }

  return payload;
}

