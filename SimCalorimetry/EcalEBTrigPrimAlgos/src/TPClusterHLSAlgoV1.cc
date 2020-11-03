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

  // Since one FPGA handles 300 channels
  // (ecalPh2::hls::bcpswisscross::nchannels_eta * ecalPh2::hls::bcpswisscross::nchannels_phi)
  // the HLS swissCross function expects TPs from as many channels.
  // In addition TPs from the surrounding FPGAs are expected as well.
  std::array<std::array<input::indata, nchannels_phi>, nchannels_eta> data;
  std::array<std::array<input::indata, nchannels_phi>, nchannels_neighbour_eta> data_etap;
  std::array<std::array<input::indata, nchannels_phi>, nchannels_neighbour_eta> data_etam;
  std::array<std::array<input::indata, nchannels_neighbour_phi>, nchannels_eta> data_phip;
  std::array<std::array<input::indata, nchannels_neighbour_phi>, nchannels_eta> data_phim;

  // loop over FPGAs, each processing nchannel_eta * nchannel_phi
  // create 2d array with dimension nchannel_eta * nchannel_phi as input and run swissCross
  constexpr unsigned int nchannels_fpga(nchannels_eta * nchannels_phi);
  for (size_t fpgaIdx = 0; fpgaIdx <= (ebTPs.size() - 1) / nchannels_fpga; ++fpgaIdx) {
    // zero all channels
    for (unsigned int i = 0; i < nchannels_eta; ++i) {
      for (unsigned int j = 0; j < nchannels_phi; ++j) {
        data[i][j] = {0, 0, false};
      }
      for (unsigned int j = 0; j < nchannels_neighbour_phi; ++j) {
        data_phip[i][j] = {0, 0, false};
        data_phim[i][j] = {0, 0, false};
      }
    }
    for (unsigned int i = 0; i < nchannels_neighbour_eta; ++i) {
      for (unsigned int j = 0; j < nchannels_phi; ++j) {
        data_etap[i][j] = {0, 0, false};
        data_etam[i][j] = {0, 0, false};
      }
    }

    // loop over ebTPs of nchannels and prepare set the data arrays
    for (size_t i = fpgaIdx * nchannels_fpga; i < (fpgaIdx + 1) * nchannels_fpga and i < ebTPs.size(); ++i) {
      const auto ebTP = ebTPs[i];
      auto ebTPId = ebTP.id();
      //std::cout << "TP for clustering " << i << ": rawId=" << ebTPId.rawId() << ", ieta=" << ebTPId.ieta() << ", iphi=" << ebTPId.iphi() << std::endl;

      // get the algo parameters for this crystal
      const auto peakIdx = ecalBcpPayloadParamsHelper_->sampleOfInterest(ebTPId);

      const auto ieta = ebTPId.ieta();
      const auto iphi = ebTPId.iphi();
      data[ieta][iphi].et = static_cast<ap_uint<10>>(ebTP[peakIdx].encodedEt());
      data[ieta][iphi].time = static_cast<ap_uint<5>>(ebTP[peakIdx].time());
      data[ieta][iphi].spike = ebTP[peakIdx].l1aSpike();
    }

    // Run the HLS swissCross function.
    std::array<output::outdata, nclusters_max> scOutput;
    std::cout << "Running the HLS spike tagger algo" << std::endl;
    scOutput = swissCross(data, data_etap, data_etam, data_phip, data_phim);

    // setting the TPClusters
    for (const auto scCluster : scOutput) {
      if (scCluster.et > 0 || scCluster.spike) {
        std::cout << "Adding TP cluster et=" << scCluster.et << ", ieta=" << scCluster.eta << ", iphi=" << scCluster.phi << ", number of crystals=" << scCluster.ncrystals << ", spike=" << scCluster.spike << std::endl;
        ebTPClusters.emplace_back(EcalEBTriggerPrimitiveCluster(scCluster.et, scCluster.time, scCluster.eta, scCluster.phi, scCluster.ncrystals, scCluster.spike));
      }
    }
  }
}

