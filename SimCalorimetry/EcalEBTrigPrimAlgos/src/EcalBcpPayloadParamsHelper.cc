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

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/EcalBcpPayloadParamsHelper.h"

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper() : EcalBcpPayloadParams() {
  nodes_.resize(NUM_NODES);
}

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper(const EcalBcpPayloadParams &params) : EcalBcpPayloadParams(params) {
  nodes_.resize(NUM_NODES);
}

EcalBcpPayloadParamsHelper::EcalBcpPayloadParamsHelper(const edm::ParameterSet &config) {
  this->createFromPSet(config);
}

void EcalBcpPayloadParamsHelper::createFromPSet(const edm::ParameterSet &config)
{
  version_ = config.getParameter<unsigned int>("fwVersion");
  this->setSampleOfInterest(config.getParameter<unsigned int>("sampleOfInterest"));

  // algo parameters        
  const auto algoConfigs = config.getParameter<std::vector<edm::ParameterSet>>("algoConfigs");
  for (const auto &algoConfig : algoConfigs) {
    const auto algo = algoConfig.getParameter<std::string>("algo");
    if (algo == "spikeTaggerLd") {
      this->setSpikeTaggerLdType(algoConfig.getParameter<std::string>("type"));
      this->setSpikeTaggerLdThreshold(algoConfig.getParameter<double>("spikeThreshold"));
      this->setSpikeTaggerLdWeights(algoConfig.getParameter<std::vector<double>>("weights"));
    } else if (algo == "someOtherAlgo") {
      // get some other algo configuration parameters
    } else {
      edm::LogWarning("EcalBcpPayloadParamsHelper") << "Unknown algorithm '" << algo << "'";
    }
  }
}

// Global parameters
unsigned int EcalBcpPayloadParamsHelper::sampleOfInterest() const
{
  return nodes_[kGlobalAlgoParams].uparams_.size() > UIdx::kSampleOfInterest
      ? nodes_[kGlobalAlgoParams].uparams_[UIdx::kSampleOfInterest] : 0;
}

void EcalBcpPayloadParamsHelper::setSampleOfInterest(const unsigned int soi)
{
  if (nodes_[kGlobalAlgoParams].uparams_.size() > UIdx::kSampleOfInterest) {
    nodes_[kGlobalAlgoParams].uparams_[UIdx::kSampleOfInterest] = soi;
  } else {
    nodes_[kGlobalAlgoParams].uparams_.push_back(soi);
  }
}

// Spike tagger LD parameters
std::string EcalBcpPayloadParamsHelper::spikeTaggerLdType() const
{
  return nodes_[kSpikeTaggerLdParams].sparams_.size() > SIdx::kSpikeTaggerLdType
      ? nodes_[kSpikeTaggerLdParams].sparams_[SIdx::kSpikeTaggerLdType] : "";
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdType(const std::string &type)
{
  if (nodes_[kSpikeTaggerLdParams].sparams_.size() > SIdx::kSpikeTaggerLdType) {
    nodes_[kSpikeTaggerLdParams].sparams_[SIdx::kSpikeTaggerLdType] = type;
  } else {
    nodes_[kSpikeTaggerLdParams].sparams_.emplace_back(type);
  }
}

double EcalBcpPayloadParamsHelper::spikeTaggerLdThreshold() const
{
  return nodes_[kSpikeTaggerLdParams].dparams_.size() > DIdx::kSpikeThreshold
      ? nodes_[kSpikeTaggerLdParams].dparams_[DIdx::kSpikeThreshold] : 0.;
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdThreshold(const double &thr)
{
  if (nodes_[kSpikeTaggerLdParams].dparams_.size() > DIdx::kSpikeThreshold) {
    nodes_[kSpikeTaggerLdParams].dparams_[DIdx::kSpikeThreshold] = thr;
  } else {
    nodes_[kSpikeTaggerLdParams].dparams_.push_back(thr);
  }
}

std::vector<double> EcalBcpPayloadParamsHelper::spikeTaggerLdWeights() const
{
  return nodes_[kSpikeTaggerLdWeights].dparams_;
}

void EcalBcpPayloadParamsHelper::setSpikeTaggerLdWeights(const std::vector<double> &weights)
{
  nodes_[kSpikeTaggerLdWeights].dparams_ = weights;
}

// print parameters to stream:
void EcalBcpPayloadParamsHelper::print(std::ostream &out) const
{
  out << "ECAL BCP payload parameters" << std::endl;
  out << "Version " << std::hex << version_ << std::dec << std::endl;
  out << "Global parameters:" << std::endl;
  out << "  Sample of interest " << this->sampleOfInterest() << std::endl;
  out << "Spike tagger LD parameters:" << std::endl;
  out << "  Type " << this->spikeTaggerLdType() << std::endl;
  out << "  Spike threshold " << this->spikeTaggerLdThreshold() << std::endl;
  out << "  Polynomial weights (ascending order)" << std::endl;
  for (const auto weight : this->spikeTaggerLdWeights()) {
    out << "    " << weight << std::endl;
  }
}

std::ostream & operator<<(std::ostream &out, const EcalBcpPayloadParamsHelper &params)
{
  params.print(out);
  return out;
}

