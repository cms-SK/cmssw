///
/// \class ecalph2::BCPPayloadV1
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
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/MultiFitTimingAlgoFactory.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgoFactory.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgoFactory.h"

void ecalph2::BCPPayloadV1::processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters)
{
  std::cout << "Processing BCPPayloadV1" << std::endl;
  multiFitTimingAlgo_->processEvent(ebDigis, ebTPs);
  spikeTaggerLDAlgo_->processEvent(ebDigis, ebTPs);
  tpClusterAlgo_->processEvent(ebTPs, ebTPClusters);
}

void ecalph2::BCPPayloadV1::createAlgos(const edm::EventSetup &eventSetup)
{
  ecalph2::MultiFitTimingAlgoFactory multiFitTimingAlgoFactory;
  ecalph2::SpikeTaggerLDAlgoFactory spikeTaggerLDAlgoFactory;
  ecalph2::TPClusterAlgoFactory tpClusterAlgoFactory;

  multiFitTimingAlgo_ = multiFitTimingAlgoFactory.create(ecalBcpPayloadParamsHelper_, eventSetup);
  spikeTaggerLDAlgo_ = spikeTaggerLDAlgoFactory.create(ecalBcpPayloadParamsHelper_, eventSetup);
  tpClusterAlgo_ = tpClusterAlgoFactory.create(ecalBcpPayloadParamsHelper_);
}

