///
/// \class ecalPh2::TPClusterAlgoV1
///
/// \author: Thomas Reis
/// 
/// Version: V1
/// Implements the TP clustering algorithm and calculates the swiss cross
/// spike flag. Clusters are either added if they pass the cluster threshold
/// or if the seed crystal was found to be a spike by the swiss cross method.
///

#include <cmath>
#include <iostream>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/TPClusterAlgoV1.h"

ecalPh2::TPClusterAlgoV1::TPClusterAlgoV1(const std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper) :
  TPClusterAlgo(ecalBcpPayloadParamsHelper),
  doCluster_(true),
  doSwissCross_(true),
  iEtaDiffMax_(1),
  iPhiDiffMax_(1),
  useLDSpikesInSum_(false),
  seedEtThreshold_(1),
  clusterCrystalEtThreshold_(1),
  swissCrossCrystalEtThreshold_(1),
  tpClusterEtThreshold_(1),
  swissCrossSpikeThreshold_(0.95)
{
  // hard coded flags, cluster size, and thresholds for the moment
  // TODO: get parameters from configuration

  nTPInCluster_ = (2 * iEtaDiffMax_ + 1) * (2 * iPhiDiffMax_ + 1);
}

void ecalPh2::TPClusterAlgoV1::processEvent(const EcalEBTrigPrimDigiCollection &ebTPs, std::vector<EcalEBTriggerPrimitiveCluster> &ebTPClusters)
{
  std::cout << "Processing TPClusterAlgoV1" << std::endl;
  std::cout << "This TP collection has size: " << ebTPs.size() << std::endl;

  for (size_t i = 0; i < ebTPs.size(); ++i) {
    bool goodSeed = true; // start with the assumption that this is a good seed
    const auto ebTP = ebTPs[i];
    auto ebTPId = ebTP.id();
    //std::cout << "TP for clustering " << i << ": rawId=" << ebTPId.rawId() << ", ieta=" << ebTPId.ieta() << ", iphi=" << ebTPId.iphi() << std::endl;

    // get the algo parameters for this crystal
    const auto peakIdx = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebTPId);
    const auto seedEt = ebTP[peakIdx].encodedEt();
    if (seedEt < seedEtThreshold_) {
      continue;
    }

    unsigned int nTPAdded = 1;
    unsigned int nSwissCrossChecked = 0;
    unsigned int nSwissCrossAdded = 0;
    unsigned int sum = seedEt;
    unsigned int swissCrossSum = 0;

    // it is not a good seed if it has an LD spike flag and the LD flags are not ignored
    if (ebTP[peakIdx].l1aSpike() and not useLDSpikesInSum_) {
      goodSeed = false;
    }

    // loop over TPs to find adjacent ones
    for (size_t j = 0; j < ebTPs.size(); ++j) {
      if (ebTPId == ebTPs[j].id()) {
        continue;
      }

      const auto distEta = EBDetId::distanceEta(ebTPs[j].id(), ebTPId);
      const auto distPhi = EBDetId::distancePhi(ebTPs[j].id(), ebTPId);
      const auto encodedEt = ebTPs[j][peakIdx].encodedEt();
      const auto l1aSpike = ebTPs[j][peakIdx].l1aSpike();

      // calculate cluster ET sum
      if (doCluster_ and encodedEt >= clusterCrystalEtThreshold_ and std::abs(distEta) <= iEtaDiffMax_ and std::abs(distPhi) <= iPhiDiffMax_) {
        //std::cout << "seed et: " << seedEt << ", seed ieta: " << ebTPId.ieta() << ", seed iphi: " << ebTPId.iphi() << ", et: " << encodedEt << ", distEta: " << distEta << ", distPhi: " << distPhi << ", spike: " << ebTPs[j][peakIdx].l1aSpike() << ", goodSeed: " << goodSeed << std::endl;
        if (goodSeed) {
          if (useLDSpikesInSum_ or (l1aSpike == 0 and not useLDSpikesInSum_)) {
            // If a crystal with a higher ET than the seed crystal is found in the
            // cluster area no cluster is made around the seed crystal.
            // A cluster may be made around the higher crystal in a later
            // iteration of the outer for loop. The current seed could still be
            // declared a spike based on the swiss cross method.
            if (seedEt < encodedEt) {
              goodSeed = false;
            } else {
              sum += encodedEt;
            }
          }
        }
        ++nTPAdded;
      }

      // calculate swiss cross sum
      if (doSwissCross_ and std::abs(distEta) + std::abs(distPhi) == 1) {
        if (encodedEt >= swissCrossCrystalEtThreshold_ and (useLDSpikesInSum_ or (l1aSpike == 0 and not useLDSpikesInSum_))) {
          swissCrossSum += encodedEt;
          ++nSwissCrossAdded;
        }
        ++nSwissCrossChecked;
      }

      // break if all desired crystals were already considered
      if (nSwissCrossChecked == 4) {
        if (nTPAdded >= nTPInCluster_ or not goodSeed) {
          break;
        }
      }
    }

    // swiss cross spike estimation 1 - E_4/E_seed > threshold
    float swissCross = 0;
    bool spike = false;
    if (doSwissCross_) {
      swissCross = 1 - static_cast<float>(swissCrossSum) / static_cast<float>(seedEt);
      spike = swissCross > swissCrossSpikeThreshold_;
    }

    // add a cluster object if the cluster passes the threshold or if the seed is a spike
    if ((doCluster_ and goodSeed and sum >= tpClusterEtThreshold_) or spike) {
      const auto time = ebTP[peakIdx].time();
      const auto ieta = ebTPId.ieta();
      const auto iphi = ebTPId.iphi();

      // if this is just a spike use the energy sum and number of added crystals from the swiss cross
      if (spike and not (doCluster_ and goodSeed and sum >= tpClusterEtThreshold_)) {
        sum = swissCrossSum + seedEt;
        nTPAdded = nSwissCrossAdded + 1; // add also the seed crystal since it is in the cluster ET as well
      }

      // set Et to maximum in case of 10 bit overflow
      constexpr unsigned int etMax = (1 << 10) - 1;
      if (sum > etMax) {
        std::cout << "ET overflow. sum=" << sum << " > " << etMax << ". Setting ET to " << etMax << std::endl;
        sum = etMax;
      }
      std::cout << "Adding TP cluster et=" << sum << ", ieta=" << ieta << ", iphi=" << iphi << ", number of crystals=" << nTPAdded << ", spike=" << spike << ", swiss cross=" << swissCross << std::endl;
      ebTPClusters.emplace_back(EcalEBTriggerPrimitiveCluster(sum, time, ieta, iphi, nTPAdded, spike));
    }
  }
}

