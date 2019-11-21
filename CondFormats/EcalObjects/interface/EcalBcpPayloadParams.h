#ifndef CondFormats_EcalObjects_EcalBcpPayloadParams_h
#define CondFormats_EcalObjects_EcalBcpPayloadParams_h

#include <string>
#include <vector>

#include "CondFormats/Serialization/interface/Serializable.h"

class EcalBcpPayloadParams {
 public:
  EcalBcpPayloadParams() : version_(0) {};
  ~EcalBcpPayloadParams() {};

  // generic node that holds parameters
  class Node {
   public:
    Node() {};
    ~Node() {};
  
    std::vector<double> dparams_;
    std::vector<unsigned int> uparams_;
    std::vector<int> iparams_;
    std::vector<std::string> sparams_;
  
    COND_SERIALIZABLE;
  };

  unsigned int version() const { return version_; };
  void setVersion(const unsigned int version) { version_ = version; };

 protected:
  unsigned int version_;
  std::vector<Node> nodes_;

  COND_SERIALIZABLE;
};

#endif
