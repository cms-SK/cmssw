// -*- C++ -*-
//
// Package:    SimCalorimetry/EcalEBTrigPrimProducers
// Class:      EcalBCPFileInputSource
// 
/**\class EcalBCPFileInputSource EcalBCPFileInputSource.cc SimCalorimetry/EcalEBTrigPrimProducers/plugins/EcalBCPFileInputSource.cc

 Description: Produces Ecal Barrel Calorimeter Processor inputs (Phase2 EcalDigis) from text files

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Thomas Reis
//         Created:  Tue, 10 Mar 2020 15:45:37 GMT
//
//


// system include files
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Sources/interface/ProducerSourceFromFiles.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalMGPASample.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

//
// class declaration
//

class EcalBCPFileInputSource : public edm::ProducerSourceFromFiles {
 public:
  explicit EcalBCPFileInputSource(const edm::ParameterSet&, edm::InputSourceDescription const&);
  ~EcalBCPFileInputSource();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

 private:
  bool setRunAndEventInfo(edm::EventID& id, edm::TimeValue_t& time, edm::EventAuxiliary::ExperimentType& eType) override;
  virtual void produce(edm::Event&) override;

  void openFile();
  void readHeader();

  // ----------member data ---------------------------
  std::string fname_;
  std::ifstream fstream_;

  unsigned long runnr_;
  unsigned long evtnr_;
  unsigned int startSample_;
  int skipUntilEventNr_;

  std::string line_;
  unsigned int nchannels_;

  std::vector<EBDetId> detIds_;
  EBDigiCollection ebDigis_;
  edm::EDPutTokenT<EBDigiCollection> ebDigiToken_;
};

//
// constructors and destructor
//
EcalBCPFileInputSource::EcalBCPFileInputSource(const edm::ParameterSet& iConfig, const edm::InputSourceDescription &iSrcDesc) :
  edm::ProducerSourceFromFiles(iConfig, iSrcDesc, true),
  runnr_(iConfig.getUntrackedParameter<unsigned int>("runNumber")),
  evtnr_(iConfig.getUntrackedParameter<unsigned int>("firstEventNumber")),
  startSample_(iConfig.getUntrackedParameter<unsigned int>("startSample")),
  skipUntilEventNr_(evtnr_ + iConfig.getUntrackedParameter<int>("skipEvents", 0)),
  nchannels_(0),
  ebDigiToken_(produces<EBDigiCollection>())
{
  if (fileNames().empty()) {
    throw cms::Exception("FileOpenError") << "No input file";
  }
  fname_ = fileNames()[0].substr(fileNames()[0].find(":") + 1);
  openFile();
  readHeader();
}


EcalBCPFileInputSource::~EcalBCPFileInputSource()
{
  if (fstream_.is_open()) {
    fstream_.close();
  }
}


//
// member functions
//

void EcalBCPFileInputSource::openFile()
{
  if (!fstream_.is_open()) {
    fstream_.open(fname_);
    if (!fstream_.good()) {
      throw cms::Exception("FileOpenError") << "Failed to open input file " << fname_;
    }
  }
}

void EcalBCPFileInputSource::readHeader()
{
  nchannels_ = 0;
  if (fstream_.good()) {
    while (getline(fstream_, line_)) {
      std::istringstream strstream(line_);

      std::string labelstr;

      if (line_.find("Channels") != std::string::npos) {
        strstream >> labelstr >> nchannels_;
        std::cout << "Found " << nchannels_ << " channel(s) in the input file." << std::endl;
      } else if (line_.find("Chan:") != std::string::npos) {
        std::string chan;
        strstream >> labelstr;
        // interpret letters as eta and numbers a phi
        for (unsigned int i = 0; i < nchannels_; ++i) {
          strstream >> chan;
          int ieta = chan[0] - 'A' + 1;
          int iphi = chan[1] - '0';
          std::cout << "DetId: " << chan << ", ieta: " << ieta << ", iphi: " << iphi << std::endl;
          EBDetId detId(ieta, iphi);
          detIds_.emplace_back(detId);
        }
        continue;
      } else if (line_.find("Frame") != std::string::npos) {
        return;
      }
    }
  }
}

bool
EcalBCPFileInputSource::setRunAndEventInfo(edm::EventID& id, edm::TimeValue_t& time, edm::EventAuxiliary::ExperimentType& eType)
{
  // stop if there are no more data in the file
  if (fstream_.eof()) {
    return false;
  }

  do {
    id = edm::EventID(runnr_, id.luminosityBlock(), evtnr_);

    EBDigiCollection ebDigis;
    ebDigis.reserve(nchannels_);
    for (unsigned int i = 0; i < nchannels_; ++i) {
      ebDigis.push_back(detIds_[i].rawId());
    }

    unsigned int s = 0;
    while (not fstream_.eof()) {
      std::istringstream strstream(line_);
      std::cout << "sample " << s << ", line: " << line_;

      // Only take as many frames as the ECAL data format can handle
      if (s >= startSample_ and s < startSample_ + EcalDataFrame::MAXSAMPLES) {
        std::cout << ", --> take";
        std::string labelstr;
        std::string framenumberstr;
        std::string colonstr;
        int adc;
        bool first_sample;
        strstream >> labelstr >> framenumberstr >> colonstr;
        for (unsigned int i = 0; i < nchannels_; ++i) {
          strstream >> adc >> first_sample;
          EcalMGPASample sample(adc, 1); // EcalMGPASample applies a mask of 0xFFF on adc count (max. adc: 4095)
          static_cast<EcalDataFrame>(ebDigis[i]).setSample(s - startSample_, sample);
        }
      }
      std::cout << std::endl;
      ++s;
      getline(fstream_, line_);
      // if first_sample is 1 this is already the line of the next event
      if (line_.rfind("1") == line_.size() - 1) {
        break;
      }
    }

    //for (unsigned int i = 0; i < ebDigis.size(); ++i) {
    //  EBDataFrame ebDigi = ebDigis[i];
    //  for (int j = 0; j < ebDigi.size(); ++j) {
    //    std::cout << "channel " << i << ", sample " << j << ", adc: " << ebDigi[j].adc() << std::endl;
    //  }
    //}

    ebDigis_.swap(ebDigis);
    ++evtnr_;
  } while (static_cast<long>(evtnr_) <= skipUntilEventNr_);


  return true;
}


// ------------ method called to produce the data  ------------
void
EcalBCPFileInputSource::produce(edm::Event& iEvent)
{
  // set digi object and put it in the event
  auto ebDigiOut = std::make_unique<EBDigiCollection>(ebDigis_);
  iEvent.put(ebDigiToken_, std::move(ebDigiOut));
}

 
// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
EcalBCPFileInputSource::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_INPUT_SOURCE(EcalBCPFileInputSource);
