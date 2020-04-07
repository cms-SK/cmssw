// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalBarrelTPProducer
// 
/**\class EcalBarrelTPProducer EcalBarrelTPProducer.cc SimCalorimetry/EcalEBTrigPrimProducers/plugins/EcalBarrelTPProducer.cc

 Description: Ecal barrel trigger primitive emulator

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
//         Created:  Tue, 05 Nov 2019 11:35:37 GMT
//
//


// system include files
#include <iostream>
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/BCPPayloadFactory.h"

//
// class declaration
//

class EcalBarrelTPProducer : public edm::stream::EDProducer<> {
 public:
  explicit EcalBarrelTPProducer(const edm::ParameterSet&);
  ~EcalBarrelTPProducer();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

 private:
  virtual void beginStream(edm::StreamID) override;
  virtual void produce(edm::Event&, const edm::EventSetup&) override;
  virtual void endStream() override;

  virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;

  // ----------member data ---------------------------
  const edm::ParameterSet config_;

  unsigned int fwVersion_;

  edm::ESGetToken<EcalBcpPayloadParams, EcalBcpPayloadParamsRcd> esEbPayloadParamsToken_;
  edm::EDGetTokenT<EBDigiCollection> ebDigiToken_;
  edm::EDPutTokenT<EcalEBTrigPrimDigiCollection> ebTPToken_;
  edm::EDPutTokenT<EcalEBTriggerPrimitiveClusterCollection> ebTPClusterToken_;

  std::shared_ptr<ecalPh2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper_;
  std::unique_ptr<ecalPh2::BCPPayload> payload_;
};

//
// constructors and destructor
//
EcalBarrelTPProducer::EcalBarrelTPProducer(const edm::ParameterSet& iConfig) :
  config_(iConfig),
  fwVersion_(0),
  esEbPayloadParamsToken_(esConsumes<EcalBcpPayloadParams, EcalBcpPayloadParamsRcd, edm::Transition::BeginRun>()),
  ebDigiToken_(consumes<EBDigiCollection>(iConfig.getParameter<edm::InputTag>("barrelEcalDigis"))),
  ebTPToken_(produces<EcalEBTrigPrimDigiCollection>()),
  ebTPClusterToken_(produces<EcalEBTriggerPrimitiveClusterCollection>())
{
}


EcalBarrelTPProducer::~EcalBarrelTPProducer()
{
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
EcalBarrelTPProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // get the input data from the event
  edm::Handle<EBDigiCollection> ebDigisHandle;
  iEvent.getByToken(ebDigiToken_, ebDigisHandle);

  // prepare the output collections
  // one TP per ebDigi id
  EcalEBTrigPrimDigiCollection ebTPs;
  ebTPs.reserve(ebDigisHandle->size());
  for (size_t i = 0; i < ebDigisHandle->size(); ++i) {
    ebTPs.emplace_back(EcalEBTriggerPrimitiveDigi((*ebDigisHandle)[i].id()));
  }
  EcalEBTriggerPrimitiveClusterCollection ebTPClusters;

  // process event in payload algorithms
  payload_->processEvent(*ebDigisHandle, ebTPs, ebTPClusters);

  // set TP object and put it in the event
  auto tpOut = std::make_unique<EcalEBTrigPrimDigiCollection>(ebTPs);
  auto tpClusterOut = std::make_unique<EcalEBTriggerPrimitiveClusterCollection>(ebTPClusters);
  iEvent.put(ebTPToken_, std::move(tpOut));
  iEvent.put(ebTPClusterToken_, std::move(tpClusterOut));
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void
EcalBarrelTPProducer::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
EcalBarrelTPProducer::endStream() {
}

// ------------ method called when starting to processes a run  ------------
void
EcalBarrelTPProducer::beginRun(edm::Run const&, edm::EventSetup const &eventSetup)
{
  // get the configuration
  const auto configSource = config_.getParameter<std::string>("configSource");
  if (configSource == "fromES") {
    const auto &esParams = eventSetup.getData(esEbPayloadParamsToken_);
    ecalBcpPayloadParamsHelper_ = std::make_shared<ecalPh2::EcalBcpPayloadParamsHelper>(esParams);
  } else if (configSource == "fromModuleConfig") {
    ecalBcpPayloadParamsHelper_ = std::make_shared<ecalPh2::EcalBcpPayloadParamsHelper>(config_);
  } else {
    edm::LogError("EcalBarrelTPProducer") << "Unknown configuration source '" << configSource << "'";
  }

  // rebuild the payload algos if the FW version has changed
  auto newFwVersion = ecalBcpPayloadParamsHelper_->fwVersion();
  if (newFwVersion != fwVersion_) {
    fwVersion_ = newFwVersion;

    // build payload depending on current FW version
    ecalPh2::BCPPayloadFactory factory;
    payload_ = factory.create(ecalBcpPayloadParamsHelper_, eventSetup);
  }
}

 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EcalBarrelTPProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(EcalBarrelTPProducer);
