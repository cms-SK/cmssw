///
/// \class ecalPh2::BCPPayloadV1
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP FW payload version 1 class.
///

#include <iostream>
#include <vector>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadV1.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgoFactory.h"

void ecalPh2::BCPPayloadV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs)
{
  std::cout << "Processing BCPPayloadV1" << std::endl;
  spikeTaggerLDAlgo_->processEvent(ebDigis, ebTPs);
}

void ecalPh2::BCPPayloadV1::createAlgos(const edm::EventSetup &eventSetup)
{
  ecalPh2::SpikeTaggerLDAlgoFactory spikeTaggerLDFactory;

  spikeTaggerLDAlgo_ = spikeTaggerLDFactory.create(ecalBcpPayloadParamsHelper_, eventSetup);
}

