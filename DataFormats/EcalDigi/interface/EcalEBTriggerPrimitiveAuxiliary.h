#ifndef DataFormats_EcalDigi_EcalEBTriggerPrimitiveAuxiliary_h
#define DataFormats_EcalDigi_EcalEBTriggerPrimitiveAuxiliary_h

#include "DataFormats/Common/interface/Ptr.h"

class EcalEBTriggerPrimitiveAuxiliary {
public:
  EcalEBTriggerPrimitiveAuxiliary();

  /// get the spike tagger LD value
  float spikeLD() const { return spikeLD_; }
  /// set the spike tagger LD value
  void setSpikeLD(float value) { spikeLD_ = value; }

private:
  float spikeLD_;
};

#endif
