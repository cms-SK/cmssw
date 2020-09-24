#ifndef DataFormats_EcalDigi_EcalEBTriggerPrimitiveDigi_h
#define DataFormats_EcalDigi_EcalEBTriggerPrimitiveDigi_h

#include <ostream>
#include <vector>
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveSample.h"
#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveAuxiliary.h"

/** \class EcalEBTriggerPrimitiveDigi
\author N. Marinelli - Univ. of Notre Dame


*/

class EcalEBTriggerPrimitiveDigi {
public:
  typedef EBDetId key_type;  ///< For the sorted collection

  EcalEBTriggerPrimitiveDigi();  // for persistence
  EcalEBTriggerPrimitiveDigi(const EBDetId& id);

  void swap(EcalEBTriggerPrimitiveDigi& rh) {
    std::swap(id_, rh.id_);
    std::swap(size_, rh.size_);
    std::swap(data_, rh.data_);
  }

  const EBDetId& id() const { return id_; }
  int size() const { return size_; }

  const EcalEBTriggerPrimitiveSample& operator[](int i) const { return data_[i]; }
  const EcalEBTriggerPrimitiveSample& sample(int i) const { return data_[i]; }

  void setSize(int size);
  void setSample(int i, const EcalEBTriggerPrimitiveSample& sam);
  void setSampleValue(int i, uint16_t value) { data_[i].setValue(value); }

  static const int MAXSAMPLES = 20;

  /// get the 10 bits Et of interesting sample
  int encodedEt() const;

  /// Spike flag
  bool l1aSpike() const;

  /// Time info
  int time() const;

  /// True if debug mode (# of samples > 1)
  bool isDebug() const;

  /// Gets the interesting sample
  int sampleOfInterest() const;

  /// Get the auxiliary data
  const EcalEBTriggerPrimitiveAuxiliary& auxiliary(int i) const { return auxData_[i]; }
  /// Set the auxiliary data
  void setAuxiliary(int i, const EcalEBTriggerPrimitiveAuxiliary& aux) { auxData_[i] = aux; }

private:
  EBDetId id_;
  int size_;
  std::vector<EcalEBTriggerPrimitiveSample> data_;
  std::vector<EcalEBTriggerPrimitiveAuxiliary> auxData_;
};

inline void swap(EcalEBTriggerPrimitiveDigi& lh, EcalEBTriggerPrimitiveDigi& rh) { lh.swap(rh); }

std::ostream& operator<<(std::ostream& s, const EcalEBTriggerPrimitiveDigi& digi);

#endif
