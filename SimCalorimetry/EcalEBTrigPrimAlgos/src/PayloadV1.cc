///
/// \class bcp::PayloadV1
///
/// \author: Thomas Reis
///
///
/// This class implements the BCP FW payload version 1 class.
///

#include <iostream>
#include <vector>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/PayloadV1.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgoFactory.h"

void bcp::PayloadV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs)
{
  std::cout << "Processing PayloadV1" << std::endl;
  spikeTaggerLDAlgo_->processEvent(ebDigis, ebTPs);
}

void bcp::PayloadV1::createAlgos(const edm::ParameterSet& config, const edm::EventSetup &eventSetup)
{
  bcp::SpikeTaggerLDAlgoFactory spikeTaggerLDFactory;

  spikeTaggerLDAlgo_ = spikeTaggerLDFactory.create(config, eventSetup);
}

