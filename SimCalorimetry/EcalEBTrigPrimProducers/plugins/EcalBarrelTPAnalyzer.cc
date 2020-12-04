// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalBarrelTPAnalyzer
// 
/**\class EcalBarrelTPAnalyzer EcalBarrelTPAnalyzer.cc SimCalorimetry/EcalEBTrigPrimProducers/plugins/EcalBarrelTPAnalyzer.cc

 Description: Ecal barrel trigger primitive analyzer

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
//         Created:  Thu, 19 Mar 2020 08:40:37 GMT
//
//

#include <iostream>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveCluster.h"

#include <TH1I.h>
#include <TH1F.h>
#include <TH2I.h>
#include <TH2F.h>
#include <TTree.h>
#include <TFile.h>

//
// class declaration
//

class EcalBarrelTPAnalyzer : public edm::one::EDAnalyzer<> {
 public:
  explicit EcalBarrelTPAnalyzer(const edm::ParameterSet &);
  ~EcalBarrelTPAnalyzer() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

 private:
  void analyze(const edm::Event&, const edm::EventSetup&) override;

  const edm::ParameterSet config_;
  unsigned int soi_;
  bool compareWithTPColl2_;
  bool compareWithTPClusterColl2_;

  edm::Service<TFileService> fs_;

  // TP1 histograms
  TH1I *hNumTPs1_;
  TH1I *hTPEncodedEt1_;
  TH1I *hTPL1aSpike1_;
  TH1I *hTPTime1_;
  TH1F *hTPAuxSpikeLD1_;
  TH2I *h2TPEncodedEtVsCh1_;
  TH2I *h2TPEncodedEtVsSpike1_;
  TH2F *h2TPAuxSpikeLDVsL1aSpike1_;

  // TP2 histograms
  TH1I *hNumTPs2_;
  TH1I *hTPEncodedEt2_;
  TH1I *hTPL1aSpike2_;
  TH1I *hTPTime2_;
  TH1F *hTPAuxSpikeLD2_;
  TH2I *h2TPEncodedEtVsCh2_;
  TH2I *h2TPEncodedEtVsSpike2_;
  TH2F *h2TPAuxSpikeLDVsL1aSpike2_;

  // TP1 vs. TP2 histograms
  TH2I *h2TPEncodedEt1Vs2_;
  TH2I *h2TPL1aSpike1Vs2_;
  TH2I *h2TPTime1Vs2_;
  TH2F *h2TPAuxSpikeLD1Vs2_;

  // TP clusters 1 histograms
  TH1I *hNumTPClusters1_;
  TH1I *hTPClusterHwEt1_;
  TH1I *hTPClusterHwTime1_;
  TH1I *hTPClusterHwEta1_;
  TH1I *hTPClusterHwPhi1_;
  TH1I *hTPClusterNumOfCrystals1_;
  TH1I *hTPClusterSpike1_;

  // TP clusters 2 histograms
  TH1I *hNumTPClusters2_;
  TH1I *hTPClusterHwEt2_;
  TH1I *hTPClusterHwTime2_;
  TH1I *hTPClusterHwEta2_;
  TH1I *hTPClusterHwPhi2_;
  TH1I *hTPClusterNumOfCrystals2_;
  TH1I *hTPClusterSpike2_;

  TTree *tree_;
  unsigned long long event_;
  unsigned int lumiBlock_;
  unsigned int run_;
  int numTPs1_;
  std::vector<int> tpEncodedEt1_;
  std::vector<int> tpL1aSpike1_;
  std::vector<int> tpTime1_;
  std::vector<float> tpAuxSpikeLD1_;
  int numTPs2_;
  std::vector<int> tpEncodedEt2_;
  std::vector<int> tpL1aSpike2_;
  std::vector<int> tpTime2_;
  std::vector<float> tpAuxSpikeLD2_;
  int numTPClusters1_;
  std::vector<int> tpClusterHwEt1_;
  std::vector<int> tpClusterHwTime1_;
  std::vector<int> tpClusterHwEta1_;
  std::vector<int> tpClusterHwPhi1_;
  std::vector<int> tpClusterNumOfCrystals1_;
  std::vector<int> tpClusterSpike1_;
  int numTPClusters2_;
  std::vector<int> tpClusterHwEt2_;
  std::vector<int> tpClusterHwTime2_;
  std::vector<int> tpClusterHwEta2_;
  std::vector<int> tpClusterHwPhi2_;
  std::vector<int> tpClusterNumOfCrystals2_;
  std::vector<int> tpClusterSpike2_;

  edm::EDGetTokenT<EcalEBTrigPrimDigiCollection> ebTPToken1_;
  edm::EDGetTokenT<EcalEBTrigPrimDigiCollection> ebTPToken2_;
  edm::EDGetTokenT<EcalEBTriggerPrimitiveClusterCollection> ebTPClusterToken1_;
  edm::EDGetTokenT<EcalEBTriggerPrimitiveClusterCollection> ebTPClusterToken2_;

  std::string tpCollName1_;
  std::string tpCollName2_;
  std::string tpClusterCollName1_;
  std::string tpClusterCollName2_;
};

EcalBarrelTPAnalyzer::EcalBarrelTPAnalyzer(const edm::ParameterSet& iConfig) :
  config_(iConfig),
  soi_(iConfig.getParameter<unsigned int>("sampleOfInterest")),
  compareWithTPColl2_(false),
  compareWithTPClusterColl2_(false),
  ebTPToken1_(consumes<EcalEBTrigPrimDigiCollection>(iConfig.getParameter<edm::InputTag>("barrelTPColl1"))),
  ebTPToken2_(consumes<EcalEBTrigPrimDigiCollection>(iConfig.getParameter<edm::InputTag>("barrelTPColl2"))),
  ebTPClusterToken1_(consumes<EcalEBTriggerPrimitiveClusterCollection>(iConfig.getParameter<edm::InputTag>("barrelTPClusterColl1"))),
  ebTPClusterToken2_(consumes<EcalEBTriggerPrimitiveClusterCollection>(iConfig.getParameter<edm::InputTag>("barrelTPClusterColl2"))),
  tpCollName1_(iConfig.getUntrackedParameter<std::string>("tpCollName1", "collection 1")),
  tpCollName2_(iConfig.getUntrackedParameter<std::string>("tpCollName2", "collection 2")),
  tpClusterCollName1_(iConfig.getUntrackedParameter<std::string>("tpClusterCollName1", "collection 1")),
  tpClusterCollName2_(iConfig.getUntrackedParameter<std::string>("tpClusterCollName2", "collection 2"))
{
  tree_ = fs_->make<TTree>("ecalBarrelTPTree", "ecalBarrelTPTree");

  tree_->Branch("event", &event_, "event/l");
  tree_->Branch("lumiBlock", &lumiBlock_, "lumiBlock/i");
  tree_->Branch("run", &run_, "run/i");

  // TPs of collection 1
  tree_->Branch("numTPs1", &numTPs1_, "numTPs1/I");
  tree_->Branch("tpEncodedEt1", &tpEncodedEt1_);
  tree_->Branch("tpL1aSpike1", &tpL1aSpike1_);
  tree_->Branch("tpTime1", &tpTime1_);
  tree_->Branch("tpAuxSpikeLD1", &tpAuxSpikeLD1_);

  hNumTPs1_ = fs_->make<TH1I>("numTPs1", ("number of TPs in " + tpCollName1_ + ";# TP;").c_str(), 100, 0, 100);
  hTPEncodedEt1_ = fs_->make<TH1I>("tpEncodedEt1", ("encoded E_{T} of sample of interest in " + tpCollName1_ + ";encoded E_{T};").c_str(), 1024, 0, 1024);
  hTPL1aSpike1_ = fs_->make<TH1I>("tpL1aSpike1", ("spike flag of sample of interest in " + tpCollName1_ + ";spike;").c_str(), 2, 0, 2);
  hTPTime1_ = fs_->make<TH1I>("tpTime1", ("time of sample of interest in " + tpCollName1_ + ";time;").c_str(), 32, 0, 32);
  hTPAuxSpikeLD1_ = fs_->make<TH1F>("tpAuxSpikeLD1", ("spike LD of sample of interest in " + tpCollName1_ + ";LD;").c_str(), 200, -2., 2.);
  h2TPEncodedEtVsCh1_ = fs_->make<TH2I>("tpEncodedEtVsCh1", ("encoded E_{T} vs. TP index of sample of interest in " + tpCollName1_ + ";encoded E_{T};TP index").c_str(), 1024, 0, 1024, 20, 0, 20);
  h2TPEncodedEtVsSpike1_ = fs_->make<TH2I>("tpEncodedEtVsSpike1", ("encoded E_{T} vs. spike flag of sample of interest in " + tpCollName1_ + ";encoded E_{T};spike").c_str(), 1024, 0, 1024, 2, 0, 2);
  h2TPAuxSpikeLDVsL1aSpike1_ = fs_->make<TH2F>("tpAuxSpikeLDVsL1aSpike1", ("spike LD vs. spike flag of sample of interest in " + tpCollName1_ + ";LD;spike").c_str(), 200, -2., 2., 2, 0., 2.);

  // TPs of collection 2 if available
  if (iConfig.getParameter<edm::InputTag>("barrelTPColl2").label() != "") {
    compareWithTPColl2_ = true;

    tree_->Branch("numTPs2", &numTPs2_, "numTPs2/I");
    tree_->Branch("tpEncodedEt2", &tpEncodedEt2_);
    tree_->Branch("tpL1aSpike2", &tpL1aSpike2_);
    tree_->Branch("tpTime2", &tpTime2_);
    tree_->Branch("tpAuxSpikeLD2", &tpAuxSpikeLD2_);

    hNumTPs2_ = fs_->make<TH1I>("numTPs2", ("number of TPs in " + tpCollName2_ + ";# TP;").c_str(), 100, 0, 100);
    hTPEncodedEt2_ = fs_->make<TH1I>("tpEncodedEt2", ("encoded E_{T} of sample of interest in " + tpCollName2_ + ";encoded E_{T};").c_str(), 1024, 0, 1024);
    hTPL1aSpike2_ = fs_->make<TH1I>("tpL1aSpike2", ("spike flag of sample of interest in " + tpCollName2_ + ";spike;").c_str(), 2, 0, 2);
    hTPTime2_ = fs_->make<TH1I>("tpTime2", ("time of sample of interest in " + tpCollName2_ + ";time;").c_str(), 32, 0, 32);
    hTPAuxSpikeLD2_ = fs_->make<TH1F>("tpAuxSpikeLD2", ("spike LD in " + tpCollName2_ + ";LD;").c_str(), 200, -2., 2.);
    h2TPEncodedEtVsCh2_ = fs_->make<TH2I>("tpEncodedEtVsCh2", ("encoded E_{T} vs. TP index of sample of interest in " + tpCollName2_ + ";encoded E_{T};TP index").c_str(), 1024, 0, 1024, 20, 0, 20);
    h2TPEncodedEtVsSpike2_ = fs_->make<TH2I>("tpEncodedEtVsSpike2", ("encoded E_{T} vs. spike flag of sample of interest in " + tpCollName2_ + ";encoded E_{T};spike").c_str(), 1024, 0, 1024, 2, 0, 2);
    h2TPAuxSpikeLDVsL1aSpike2_ = fs_->make<TH2F>("tpAuxSpikeLDVsL1aSpike2", ("spike LD vs. spike flag of sample of interest in " + tpCollName2_ + ";LD;spike").c_str(), 200, -2., 2., 2, 0., 2.);

    h2TPEncodedEt1Vs2_ = fs_->make<TH2I>("tpEncodedEt1Vs2", ("encoded E_{T} flag of sample of interest in " + tpCollName1_ + " vs. " + tpCollName2_ + ";" + tpCollName1_ + " encoded E_{T};" + tpCollName2_ + " encoded E_{T}").c_str(), 1024, 0, 1024, 1024, 0, 1024);
    h2TPL1aSpike1Vs2_ = fs_->make<TH2I>("tpL1aSpike1Vs2", ("spike flag of sample of interest in " + tpCollName1_ + " vs. " + tpCollName2_ + ";" + tpCollName1_ + " spike;" + tpCollName2_ + " spike").c_str(), 2, 0, 2, 2, 0, 2);
    h2TPTime1Vs2_ = fs_->make<TH2I>("tpTime1Vs2", ("time of sample of interest in " + tpCollName1_ + " vs. " + tpCollName2_ + ";" + tpCollName1_ + " time;" + tpCollName2_ + " time").c_str(), 32, 0, 32, 32, 0, 32);
    h2TPAuxSpikeLD1Vs2_ = fs_->make<TH2F>("tpAuxSpikeLD1Vs2", ("spike LD of sample of interest in " + tpCollName1_ + " vs. " + tpCollName2_ + ";" + tpCollName1_ + " LD;" + tpCollName2_ + " LD").c_str(), 100, -1., 1., 100, -1., 1.);
  }

  // TP clusters of collection 1
  tree_->Branch("numTPClusters1", &numTPClusters1_, "numTPClusters1/I");
  tree_->Branch("tpClusterHwEt1", &tpClusterHwEt1_);
  tree_->Branch("tpClusterHwTime1", &tpClusterHwTime1_);
  tree_->Branch("tpClusterHwEta1", &tpClusterHwEta1_);
  tree_->Branch("tpClusterHwPhi1", &tpClusterHwPhi1_);
  tree_->Branch("tpClusterNumOfCrystals1", &tpClusterNumOfCrystals1_);
  tree_->Branch("tpClusterSpike1", &tpClusterSpike1_);

  hNumTPClusters1_ = fs_->make<TH1I>("numTPClusters1", ("number of TP clusters, " + tpClusterCollName1_ + ";# TP clusters;").c_str(), 20, 0, 20);
  hTPClusterHwEt1_ = fs_->make<TH1I>("tpClusterHwEt1", ("HW E_{T}, " + tpClusterCollName1_ + ";HW E_{T};").c_str(), 1025, -1, 1024); // HW ET -1 is for crystals that are not the seed of a cluster but are a spike
  hTPClusterHwTime1_ = fs_->make<TH1I>("tpClusterHwTime1", ("HW time, " + tpClusterCollName1_ + ";HW time;").c_str(), 32, 0, 32);
  hTPClusterHwEta1_ = fs_->make<TH1I>("tpClusterHwEta1", ("i#eta, " + tpClusterCollName1_ + ";i#eta;").c_str(), 256, 0, 256);
  hTPClusterHwPhi1_ = fs_->make<TH1I>("tpClusterHwPhi1", ("i#phi," + tpClusterCollName1_ + ";i#phi;").c_str(), 256, 0, 256);
  hTPClusterNumOfCrystals1_ = fs_->make<TH1I>("tpClusterNumOfCrystals1", ("number of crystals per cluster, " + tpClusterCollName1_ + ";# crystals;").c_str(), 256, 0, 256);
  hTPClusterSpike1_ = fs_->make<TH1I>("tpClusterSpike1", ("swiss cross spike, " + tpClusterCollName1_ + ";swiss cross spike;").c_str(), 2, 0, 2);

  // TP clusters of collection 2 if available
  if (iConfig.getParameter<edm::InputTag>("barrelTPClusterColl2").label() != "") {
    compareWithTPClusterColl2_ = true;

    tree_->Branch("numTPClusters2", &numTPClusters2_, "numTPClusters2/I");
    tree_->Branch("tpClusterHwEt2", &tpClusterHwEt2_);
    tree_->Branch("tpClusterHwTime2", &tpClusterHwTime2_);
    tree_->Branch("tpClusterHwEta2", &tpClusterHwEta2_);
    tree_->Branch("tpClusterHwPhi2", &tpClusterHwPhi2_);
    tree_->Branch("tpClusterNumOfCrystals2", &tpClusterNumOfCrystals2_);
    tree_->Branch("tpClusterSpike2", &tpClusterSpike2_);

    hNumTPClusters2_ = fs_->make<TH1I>("numTPClusters2", ("number of TP clusters in " + tpClusterCollName2_ + ";# TP clusters;").c_str(), 20, 0, 20);
    hTPClusterHwEt2_ = fs_->make<TH1I>("tpClusterHwEt2", ("HW E_{T}, " + tpClusterCollName2_ + ";HW E_{T};").c_str(), 1025, -1, 1024); // HW ET -1 is for crystals that are not the seed of a cluster but are a spike
    hTPClusterHwTime2_ = fs_->make<TH1I>("tpClusterHwTime2", ("HW time, " + tpClusterCollName2_ + ";HW time;").c_str(), 32, 0, 32);
    hTPClusterHwEta2_ = fs_->make<TH1I>("tpClusterHwEta2", ("i#eta, " + tpClusterCollName2_ + ";i#eta;").c_str(), 256, 0, 256);
    hTPClusterHwPhi2_ = fs_->make<TH1I>("tpClusterHwPhi2", ("i#phi, " + tpClusterCollName2_ + ";i#phi;").c_str(), 256, 0, 256);
    hTPClusterNumOfCrystals2_ = fs_->make<TH1I>("tpClusterNumOfCrystals2", ("number of crystals per cluster, " + tpClusterCollName2_ + ";# crystals;").c_str(), 256, 0, 256);
    hTPClusterSpike2_ = fs_->make<TH1I>("tpClusterSpike2", ("swiss cross spike, " + tpClusterCollName2_ + ";swiss cross spike;").c_str(), 2, 0, 2);
  }
}

EcalBarrelTPAnalyzer::~EcalBarrelTPAnalyzer()
{
}

//
// member functions
//

// ------------ method called to analyze the data  ------------
void EcalBarrelTPAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // clear vectors for branches
  tpEncodedEt1_.clear();
  tpL1aSpike1_.clear();
  tpTime1_.clear();
  tpAuxSpikeLD1_.clear();
  tpEncodedEt2_.clear();
  tpL1aSpike2_.clear();
  tpTime2_.clear();
  tpAuxSpikeLD2_.clear();
  tpClusterHwEt1_.clear();
  tpClusterHwTime1_.clear();
  tpClusterHwEta1_.clear();
  tpClusterHwPhi1_.clear();
  tpClusterNumOfCrystals1_.clear();
  tpClusterSpike1_.clear();
  tpClusterHwEt2_.clear();
  tpClusterHwTime2_.clear();
  tpClusterHwEta2_.clear();
  tpClusterHwPhi2_.clear();
  tpClusterNumOfCrystals2_.clear();
  tpClusterSpike2_.clear();

  // Get inputs
  edm::Handle<EcalEBTrigPrimDigiCollection> ebTPs1;
  edm::Handle<EcalEBTrigPrimDigiCollection> ebTPs2;
  edm::Handle<EcalEBTriggerPrimitiveClusterCollection> ebTPClusters1;
  edm::Handle<EcalEBTriggerPrimitiveClusterCollection> ebTPClusters2;

  iEvent.getByToken(ebTPToken1_, ebTPs1);
  if (compareWithTPColl2_ == true) {
    iEvent.getByToken(ebTPToken2_, ebTPs2);
  }
  iEvent.getByToken(ebTPClusterToken1_, ebTPClusters1);
  if (compareWithTPClusterColl2_ == true) {
    iEvent.getByToken(ebTPClusterToken2_, ebTPClusters2);
  }

  event_ = iEvent.eventAuxiliary().event();
  lumiBlock_ = iEvent.eventAuxiliary().luminosityBlock();
  run_ = iEvent.eventAuxiliary().run();

  // TPs of collection 1
  numTPs1_ = ebTPs1->size();
  tpEncodedEt1_.reserve(numTPs1_);
  tpL1aSpike1_.reserve(numTPs1_);
  tpTime1_.reserve(numTPs1_);
  tpAuxSpikeLD1_.reserve(numTPs1_);

  // fill histograms for TPs
  // histograms for coll1
  hNumTPs1_->Fill(numTPs1_);
  for (auto it = ebTPs1->begin(); it != ebTPs1->end(); ++it) {
    const auto sample = it->sample(static_cast<int>(soi_));
    const auto aux = it->auxiliary(static_cast<int>(soi_));

    tpEncodedEt1_.emplace_back(sample.encodedEt());
    tpL1aSpike1_.emplace_back(sample.l1aSpike());
    tpTime1_.emplace_back(sample.time());
    tpAuxSpikeLD1_.emplace_back(aux.spikeLD());

    hTPEncodedEt1_->Fill(sample.encodedEt());
    hTPL1aSpike1_->Fill(sample.l1aSpike());
    hTPTime1_->Fill(sample.time());
    hTPAuxSpikeLD1_->Fill(aux.spikeLD());
    const auto i = std::distance(ebTPs1->begin(), it);
    h2TPEncodedEtVsCh1_->Fill(sample.encodedEt(), i);
    h2TPEncodedEtVsSpike1_->Fill(sample.encodedEt(), sample.l1aSpike());
    h2TPAuxSpikeLDVsL1aSpike1_->Fill(aux.spikeLD(), sample.l1aSpike());
  }

  // TPs of collection 2 if available
  if (compareWithTPColl2_ == true) {
    numTPs2_ = ebTPs2->size();
    tpEncodedEt2_.reserve(numTPs2_);
    tpL1aSpike2_.reserve(numTPs2_);
    tpTime2_.reserve(numTPs2_);
    tpAuxSpikeLD2_.reserve(numTPs1_);

    // histograms for coll2
    hNumTPs2_->Fill(numTPs2_);
    auto it1 = ebTPs1->begin();
    for (auto it = ebTPs2->begin(); it != ebTPs2->end(); ++it) {
      const auto sample = it->sample(static_cast<int>(soi_));
      const auto aux = it->auxiliary(static_cast<int>(soi_));

      tpEncodedEt2_.emplace_back(sample.encodedEt());
      tpL1aSpike2_.emplace_back(sample.l1aSpike());
      tpTime2_.emplace_back(sample.time());
      tpAuxSpikeLD2_.emplace_back(aux.spikeLD());

      hTPEncodedEt2_->Fill(sample.encodedEt());
      hTPL1aSpike2_->Fill(sample.l1aSpike());
      hTPTime2_->Fill(sample.time());
      hTPAuxSpikeLD2_->Fill(aux.spikeLD());
      const auto i = std::distance(ebTPs2->begin(), it);
      h2TPEncodedEtVsCh2_->Fill(sample.encodedEt(), i);
      h2TPEncodedEtVsSpike2_->Fill(sample.encodedEt(), sample.l1aSpike());
      h2TPAuxSpikeLDVsL1aSpike2_->Fill(aux.spikeLD(), sample.l1aSpike());

      // comparison histograms of coll1 with coll2
      if (it1 != ebTPs1->end()) {
        const auto sample1 = it1->sample(static_cast<int>(soi_));
        const auto aux1 = it1->auxiliary(static_cast<int>(soi_));
        h2TPEncodedEt1Vs2_->Fill(sample1.encodedEt(), sample.encodedEt());
        h2TPL1aSpike1Vs2_->Fill(sample1.l1aSpike(), sample.l1aSpike());
        h2TPTime1Vs2_->Fill(sample1.time(), sample.time());
        h2TPAuxSpikeLD1Vs2_->Fill(aux1.spikeLD(), aux.spikeLD());
        ++it1;
      }
    }
  }

  // TP clusters of collection 1
  numTPClusters1_ = ebTPClusters1->size();
  tpClusterHwEt1_.reserve(numTPClusters1_);
  tpClusterHwTime1_.reserve(numTPClusters1_);
  tpClusterHwEta1_.reserve(numTPClusters1_);
  tpClusterHwPhi1_.reserve(numTPClusters1_);
  tpClusterNumOfCrystals1_.reserve(numTPClusters1_);
  tpClusterSpike1_.reserve(numTPClusters1_);

  // fill histograms for TP clusters
  hNumTPClusters1_->Fill(numTPClusters1_);
  for (auto it = ebTPClusters1->cbegin(); it != ebTPClusters1->cend(); ++it) {
    tpClusterHwEt1_.emplace_back(it->hwEt());
    tpClusterHwTime1_.emplace_back(it->hwTime());
    tpClusterHwEta1_.emplace_back(it->hwEta());
    tpClusterHwPhi1_.emplace_back(it->hwPhi());
    tpClusterNumOfCrystals1_.emplace_back(it->numberOfCrystals());
    tpClusterSpike1_.emplace_back(it->spike());

    hTPClusterHwEt1_->Fill(it->hwEt());
    hTPClusterHwTime1_->Fill(it->hwTime());
    hTPClusterHwEta1_->Fill(it->hwEta());
    hTPClusterHwPhi1_->Fill(it->hwPhi());
    hTPClusterNumOfCrystals1_->Fill(it->numberOfCrystals());
    hTPClusterSpike1_->Fill(it->spike());
  }

  // TP clusters of collection 2 if available
  if (compareWithTPClusterColl2_ == true) {
    numTPClusters2_ = ebTPClusters2->size();
    tpClusterHwEt2_.reserve(numTPClusters2_);
    tpClusterHwTime2_.reserve(numTPClusters2_);
    tpClusterHwEta2_.reserve(numTPClusters2_);
    tpClusterHwPhi2_.reserve(numTPClusters2_);
    tpClusterNumOfCrystals2_.reserve(numTPClusters2_);
    tpClusterSpike2_.reserve(numTPClusters2_);

    hNumTPClusters2_->Fill(ebTPClusters2->size());
    for (auto it = ebTPClusters2->cbegin(); it != ebTPClusters2->cend(); ++it) {
      tpClusterHwEt2_.emplace_back(it->hwEt());
      tpClusterHwTime2_.emplace_back(it->hwTime());
      tpClusterHwEta2_.emplace_back(it->hwEta());
      tpClusterHwPhi2_.emplace_back(it->hwPhi());
      tpClusterNumOfCrystals2_.emplace_back(it->numberOfCrystals());
      tpClusterSpike2_.emplace_back(it->spike());

      hTPClusterHwEt2_->Fill(it->hwEt());
      hTPClusterHwTime2_->Fill(it->hwTime());
      hTPClusterHwEta2_->Fill(it->hwEta());
      hTPClusterHwPhi2_->Fill(it->hwPhi());
      hTPClusterNumOfCrystals2_->Fill(it->numberOfCrystals());
      hTPClusterSpike2_->Fill(it->spike());
    }
  }
  tree_->Fill();
}


// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EcalBarrelTPAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalBarrelTPAnalyzer);
