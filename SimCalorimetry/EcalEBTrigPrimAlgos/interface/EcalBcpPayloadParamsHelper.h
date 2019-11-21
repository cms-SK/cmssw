#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_EcalBcpPayloadParamsHelper_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_EcalBcpPayloadParamsHelper_h

#include <iostream>
#include <string>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"
#include "CondFormats/EcalObjects/interface/EcalBcpPayloadParams.h"

class EcalBcpPayloadParamsHelper : public EcalBcpPayloadParams {
 public:
  EcalBcpPayloadParamsHelper();
  EcalBcpPayloadParamsHelper(const EcalBcpPayloadParams &params);
  EcalBcpPayloadParamsHelper(const edm::ParameterSet& config);
  ~EcalBcpPayloadParamsHelper() {};

  void createFromPSet(const edm::ParameterSet& config);

  // Global parameters
  unsigned int sampleOfInterest() const;
  void setSampleOfInterest(const unsigned int soi);

  // Spike tagger LD parameters
  std::string spikeTaggerLdType() const;
  void setSpikeTaggerLdType(const std::string &type);

  double spikeTaggerLdThreshold() const;
  void setSpikeTaggerLdThreshold(const double &thr);

  std::vector<double> spikeTaggerLdWeights() const;
  void setSpikeTaggerLdWeights(const std::vector<double> &weights);

  // print parameters to stream:
  void print(std::ostream &out) const;
  friend std::ostream & operator<<(std::ostream &out, const EcalBcpPayloadParamsHelper &params);

 private:
  // Defines the content of each node
  // New nodes can only be added before NUM_NODES
  enum EcalBcpPayloadParamNode {
    kGlobalAlgoParams = 0,
    kSpikeTaggerLdParams,
    kSpikeTaggerLdWeights,
    NUM_NODES
  };

  // index of variabe inside a node vector
  enum DIdx {kSpikeThreshold = 0};
  enum UIdx {kSampleOfInterest = 0};
  enum IIdx {};
  enum SIdx {kSpikeTaggerLdType = 0};
};

typedef EcalCondObjectContainer<EcalBcpPayloadParamsHelper> EcalBcpPayloadParamsHelperContainer;

#endif
