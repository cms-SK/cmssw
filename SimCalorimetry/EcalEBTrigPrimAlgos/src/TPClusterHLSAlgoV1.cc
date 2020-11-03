///
/// \class ecalPh2::TPClusterHLSAlgoV1
///
/// \author: Thomas Reis
/// 
/// Version: V1
/// Calls the HLS algorithm for the BCP clustering algorithm after preparing the data.
///

#include <array>
#include <iostream>

#include "ap_int.h"
#include "EbSwissCross.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterHLSAlgoV1.h"

using namespace ecalPh2::hls::bcpswisscross;

ecalPh2::TPClusterHLSAlgoV1::TPClusterHLSAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper) : TPClusterAlgo(ecalBcpPayloadParamsHelper)
{
}

void ecalPh2::TPClusterHLSAlgoV1::processEvent(const EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters)
{
  std::cout << "Processing TPClusterHLSAlgoV1" << std::endl;
  std::cout << "This TP collection has size: " << ebTPs.size() << std::endl;

  // Dummy algo
}

