// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalBcpPayloadParamsESProducer
// 
/**\class EcalBcpPayloadParamsESProducer

 Description: Produces the configuration parameters for the BCP payload

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis <Thomas.Reis@stfc.ac.uk>
//         Created:  Tue, 19 Nov 2019 16:15:26 GMT
//
//

// system include files
#include <iostream>
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/DataRecord/interface/EcalBcpPayloadParamsRcd.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

//
// class declaration
//

class EcalBcpPayloadParamsESProducer : public edm::ESProducer {
 public:
  EcalBcpPayloadParamsESProducer(const edm::ParameterSet&);
  ~EcalBcpPayloadParamsESProducer() override;

  using ReturnType = std::unique_ptr<EcalBcpPayloadParams>;

  ReturnType produce(const EcalBcpPayloadParamsRcd&);
 private:
  EcalBcpPayloadParams params_;
};

//
// constructors and destructor
//
EcalBcpPayloadParamsESProducer::EcalBcpPayloadParamsESProducer(const edm::ParameterSet& iConfig)
{
  setWhatProduced(this);

  ecalph2::EcalBcpPayloadParamsHelper paramsHelper(iConfig);
  params_ = static_cast<EcalBcpPayloadParams>(paramsHelper);
}

EcalBcpPayloadParamsESProducer::~EcalBcpPayloadParamsESProducer()
{
}

//
// member functions
//

// ------------ method called to produce the data  ------------
EcalBcpPayloadParamsESProducer::ReturnType
EcalBcpPayloadParamsESProducer::produce(const EcalBcpPayloadParamsRcd& iRecord)
{
  auto product = std::make_unique<EcalBcpPayloadParams>(params_);
  return product;
}

//define this as a plug-in
DEFINE_FWK_EVENTSETUP_MODULE(EcalBcpPayloadParamsESProducer);
