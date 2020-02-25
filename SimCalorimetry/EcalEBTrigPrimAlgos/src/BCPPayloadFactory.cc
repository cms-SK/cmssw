///
/// \class ecalPh2::BCPPayloadFactory
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
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadFactory.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadV1.h"

ecalPh2::BCPPayloadFactory::ReturnType ecalPh2::BCPPayloadFactory::create(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup)
{
  ReturnType payload;
  if (ecalBcpPayloadParamsHelper->fwVersion() >= 1) {
    payload = std::make_unique<ecalPh2::BCPPayloadV1>(ecalBcpPayloadParamsHelper, eventSetup);
  } else {
    edm::LogError("ecalPh2::BCPPayloadFactory") << "Invalid BCP payload FW version: 0x" << std::hex << std::setfill('0') << std::setw(8) << ecalBcpPayloadParamsHelper->fwVersion() << std::dec;
  }
  return payload;
}

