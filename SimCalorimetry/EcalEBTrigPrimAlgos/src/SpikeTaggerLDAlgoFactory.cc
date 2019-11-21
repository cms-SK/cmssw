///
/// \class bcp::SpikeTaggerLDAlgoFactory
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP spike tagger LD algorithm factory. Based on the
/// firmware version and a type string it selects the appropriate algorithms.
///

#include <iomanip>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgoFactory.h"

#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDIdealAlgoV1.h"

bcp::SpikeTaggerLDAlgoFactory::ReturnType bcp::SpikeTaggerLDAlgoFactory::create(const edm::ParameterSet& config, const edm::EventSetup &eventSetup)
{
  ReturnType spikeTaggerLDAlgo;

  std::unique_ptr<EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper;

  const auto configSource = config.getParameter<std::string>("configSource");
  if (configSource == "fromES") {
    const auto &paramsRcd = eventSetup.get<EcalBcpPayloadParamsRcd>();
    edm::ESHandle<EcalBcpPayloadParams> paramsHandle;
    paramsRcd.get(paramsHandle);

    ecalBcpPayloadParamsHelper = std::make_unique<EcalBcpPayloadParamsHelper>(*paramsHandle.product());
  } else if (configSource == "fromModuleConfig") {
    ecalBcpPayloadParamsHelper->createFromPSet(config);
  } else {
    edm::LogError("bcp::SpikeTaggerLDAlgoFactory") << "Unknown configuration source '" << configSource << "'";
  }
  const auto algoType = ecalBcpPayloadParamsHelper->spikeTaggerLdType();
  const auto fwVersion = ecalBcpPayloadParamsHelper->version();

  // factory
  if (algoType == "ideal") {
    if (fwVersion >= 1) {
      edm::LogInfo("bcp::SpikeTaggerLDAlgoFactory") << "Creating ideal spike tagger LD algo for FW version 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
      spikeTaggerLDAlgo = std::make_unique<bcp::SpikeTaggerLDIdealAlgoV1>(config, eventSetup);
    } else {
      edm::LogError("bcp::SpikeTaggerLDAlgoFactory") << "Invalid ideal spike tagger LD algo FW version: 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
    }
  } else if (algoType == "hls") {
    if (fwVersion >= 1) {
      edm::LogInfo("bcp::SpikeTaggerLDAlgoFactory") << "Creating HLS spike tagger LD algo for FW version 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
      edm::LogWarning("bcp::SpikeTaggerLDAlgoFactory") << "HLS spike tagger LD algo not implemented yet";
    } else {
      edm::LogError("bcp::SpikeTaggerLDAlgoFactory") << "Invalid HLS spike tagger LD algo FW version: 0x" << std::hex << std::setfill('0') << std::setw(8) << fwVersion << std::dec;
    }
  } else {
    edm::LogError("bcp::SpikeTaggerLDAlgoFactory") << "Invalid spike tagger LD algo type '" << algoType << "'";
  }

  return spikeTaggerLDAlgo;
}

