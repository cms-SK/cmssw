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
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgoFactory.h"

void ecalPh2::BCPPayloadV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters)
{
  std::cout << "Processing BCPPayloadV1" << std::endl;
  spikeTaggerLDAlgo_->processEvent(ebDigis, ebTPs);
  tpClusterAlgo_->processEvent(ebTPs, ebTPClusters);
}

void ecalPh2::BCPPayloadV1::createAlgos(const edm::EventSetup &eventSetup)
{
  ecalPh2::SpikeTaggerLDAlgoFactory spikeTaggerLDAlgoFactory;
  ecalPh2::TPClusterAlgoFactory tpClusterAlgoFactory;

  spikeTaggerLDAlgo_ = spikeTaggerLDAlgoFactory.create(ecalBcpPayloadParamsHelper_, eventSetup);
  tpClusterAlgo_ = tpClusterAlgoFactory.create(ecalBcpPayloadParamsHelper_);
}

