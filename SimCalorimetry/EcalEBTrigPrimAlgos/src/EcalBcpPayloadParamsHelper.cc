///
/// \class EcalBcpPayloadParamsHelper
///
/// \author: Thomas Reis
/// 19.11.2019
///
///
/// This class implements the BCP FW payload parameter interface.
/// Modifications here are easier since the object in the CondFormats
/// package is not changed.
///

#include <utility>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper() : EcalBcpPayloadParams() {
  nodes_.resize(NUM_NODES);
}

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper(const EcalBcpPayloadParams &params) : EcalBcpPayloadParams(params) {
  nodes_.resize(NUM_NODES);
}

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper(const edm::ParameterSet &config) {
  nodes_.resize(NUM_NODES);
  createFromPSet(config);
}

void EcalBcpPayloadParamsHelper::createFromPSet(const edm::ParameterSet &config)
{
  version_ = config.getParameter<unsigned int>("fwVersion");

  setSamplesOfInterest(config.getParameter<std::vector<edm::ParameterSet>>("samplesOfInterest"));

  // algo parameters        
  const auto algoConfigs = config.getParameter<std::vector<edm::ParameterSet>>("algoConfigs");
  for (const auto &algoConfig : algoConfigs) {
    const auto algo = algoConfig.getParameter<std::string>("algo");
    if (algo == "spikeTaggerLd") {
      setSpikeTaggerLdType(algoConfig.getParameter<std::string>("type"));
      setPerCrystalSpikeTaggerParams(algoConfig.getParameter<std::vector<edm::ParameterSet>>("perCrystalParams"));
    } else if (algo == "someOtherAlgo") {
      // get some other algo configuration parameters
    } else {
      edm::LogWarning("EcalBcpPayloadParamsHelper") << "Unknown algorithm '" << algo << "'";
    }
  }
}

// Global parameters
unsigned int EcalBcpPayloadParamsHelper::sampleOfInterest(const EBDetId &detId) const
{
  const auto nodesIt = crystalNodes_.find(detId.rawId());
  return nodesIt->at(kCrystalAlgoParams).uparams_.size() > UIdx::kSampleOfInterest
      ? nodesIt->at(kCrystalAlgoParams).uparams_[UIdx::kSampleOfInterest] : 0;
}

void EcalBcpPayloadParamsHelper::setSampleOfInterest(const EBDetId &detId, const unsigned int soi)
{
  const auto rawId = detId.rawId();

  // make sure that all nodes exist
  crystalNodes_[rawId].resize(NUM_CRYSTAL_NODES);

  // set parameters for this crystal
  if (crystalNodes_[rawId][kCrystalAlgoParams].uparams_.size() > UIdx::kSampleOfInterest) {
    crystalNodes_[rawId][kCrystalAlgoParams].uparams_[UIdx::kSampleOfInterest] = soi;
  } else {
    crystalNodes_[rawId][kCrystalAlgoParams].uparams_.push_back(soi);
  }
}

// Spike tagger LD parameters
std::string EcalBcpPayloadParamsHelper::spikeTaggerLdType() const
{
  return nodes_[kGlobalSpikeTaggerLdParams].sparams_.size() > SIdx::kSpikeTaggerLdType
      ? nodes_[kGlobalSpikeTaggerLdParams].sparams_[SIdx::kSpikeTaggerLdType] : "";
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdType(const std::string &type)
{
  if (nodes_[kGlobalSpikeTaggerLdParams].sparams_.size() > SIdx::kSpikeTaggerLdType) {
    nodes_[kGlobalSpikeTaggerLdParams].sparams_[SIdx::kSpikeTaggerLdType] = type;
  } else {
    nodes_[kGlobalSpikeTaggerLdParams].sparams_.emplace_back(type);
  }
}

double EcalBcpPayloadParamsHelper::spikeTaggerLdThreshold(const EBDetId &detId) const
{
  const auto nodesIt = crystalNodes_.find(detId.rawId());
  return nodesIt->at(kCrystalSpikeTaggerLdParams).dparams_.size() > DIdx::kSpikeThreshold
      ? nodesIt->at(kCrystalSpikeTaggerLdParams).dparams_[DIdx::kSpikeThreshold] : 0.;
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdThreshold(const EBDetId &detId, const double &thr)
{
  const auto rawId = detId.rawId();

  // make sure that all nodes exist
  crystalNodes_[rawId].resize(NUM_CRYSTAL_NODES);

  // set parameters for this crystal
  if (crystalNodes_[rawId][kCrystalSpikeTaggerLdParams].dparams_.size() > DIdx::kSpikeThreshold) {
    crystalNodes_[rawId][kCrystalSpikeTaggerLdParams].dparams_[DIdx::kSpikeThreshold] = thr;
  } else {
    crystalNodes_[rawId][kCrystalSpikeTaggerLdParams].dparams_.push_back(thr);
  }
}

std::vector<double> EcalBcpPayloadParamsHelper::spikeTaggerLdWeights(const EBDetId &detId) const
{
  const auto nodesIt = crystalNodes_.find(detId.rawId());
  return nodesIt->at(kCrystalSpikeTaggerLdWeights).dparams_;
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdWeights(const EBDetId &detId, const std::vector<double> &weights)
{
  const auto rawId = detId.rawId();

  // make sure that all nodes exist
  crystalNodes_[rawId].resize(NUM_CRYSTAL_NODES);

  // set parameters for this crystal
  crystalNodes_[rawId][kCrystalSpikeTaggerLdWeights].dparams_ = weights;
}

// print parameters to stream:
void EcalBcpPayloadParamsHelper::print(std::ostream &out) const
{
  out << "ECAL BCP payload parameters" << std::endl;
  out << "Version " << std::hex << version_ << std::dec << std::endl;
  out << "Global parameters:" << std::endl;
  // TODO: output of per crystal parameters in usable format
  //out << "  Sample of interest " << this->sampleOfInterest() << std::endl;
  out << "Spike tagger LD parameters:" << std::endl;
  out << "  Type " << this->spikeTaggerLdType() << std::endl;
  //out << "  Spike threshold " << this->spikeTaggerLdThreshold() << std::endl;
  //out << "  Polynomial weights (ascending order)" << std::endl;
  //for (const auto weight : this->spikeTaggerLdWeights()) {
  //  out << "    " << weight << std::endl;
  //}
}

std::ostream & operator<<(std::ostream &out, const EcalBcpPayloadParamsHelper &params)
{
  params.print(out);
  return out;
}

void EcalBcpPayloadParamsHelper::setSamplesOfInterest(const std::vector<edm::ParameterSet> &pSets)
{
  for (const auto &pSet : pSets) {
    int ietaMin, ietaMax;
    int iphiMin, iphiMax;
    parseCrystalRange(pSet.getParameter<std::string>("ietaRange"), ietaMin, ietaMax);
    parseCrystalRange(pSet.getParameter<std::string>("iphiRange"), iphiMin, iphiMax, false);
    for (int ieta = ietaMin; ieta <= ietaMax; ++ieta) {
      // skip non-existing ieta == 0 crystal
      if (ieta == 0) continue;
      for (int iphi = iphiMin; iphi <= iphiMax; ++iphi) {
        if (EBDetId::validDetId(ieta, iphi)) {
          const EBDetId detId(ieta, iphi);
          this->setSampleOfInterest(detId, pSet.getParameter<unsigned int>("sampleOfInterest"));
        }
      }
    }
  }
}

void EcalBcpPayloadParamsHelper::setPerCrystalSpikeTaggerParams(const std::vector<edm::ParameterSet> &pSets)
{
  for (const auto &pSet : pSets) {
    int ietaMin, ietaMax;
    int iphiMin, iphiMax;
    parseCrystalRange(pSet.getParameter<std::string>("ietaRange"), ietaMin, ietaMax);
    parseCrystalRange(pSet.getParameter<std::string>("iphiRange"), iphiMin, iphiMax, false);
    for (int ieta = ietaMin; ieta <= ietaMax; ++ieta) {
      // skip non-existing ieta == 0 crystal
      if (ieta == 0) continue;
      for (int iphi = iphiMin; iphi <= iphiMax; ++iphi) {
        if (EBDetId::validDetId(ieta, iphi)) {
          const EBDetId detId(ieta, iphi);
          this->setSpikeTaggerLdThreshold(detId, pSet.getParameter<double>("spikeThreshold"));
          this->setSpikeTaggerLdWeights(detId, pSet.getParameter<std::vector<double>>("weights"));
        }
      }
    }
  }
}

void EcalBcpPayloadParamsHelper::parseCrystalRange(const std::string &rangeStr, int &iMin, int &iMax, const bool isEta)
{
  const auto divPos = rangeStr.find(":");
  const auto minStr = rangeStr.substr(0, divPos);
  const auto maxStr = rangeStr.substr(divPos + 1);

  const int min = isEta ? -1 * EBDetId::MAX_IETA : EBDetId::MIN_IPHI;
  const int max = isEta ? EBDetId::MAX_IETA : EBDetId::MAX_IPHI;

  iMin = minStr != "" ? std::stoi(minStr) : min;
  iMax = maxStr != "" ? std::stoi(maxStr) : max;
}

