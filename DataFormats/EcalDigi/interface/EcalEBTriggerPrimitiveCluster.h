#ifndef DataFormats_EcalDigi_EcalEBTriggerPrimitiveCluster_h
#define DataFormats_EcalDigi_EcalEBTriggerPrimitiveCluster_h

#include <vector>

class EcalEBTriggerPrimitiveCluster {
 public:
  EcalEBTriggerPrimitiveCluster() : hwEt_(0), hwTime_(0), hwEta_(0), hwPhi_(0), numberOfCrystals_(0), spike_(false) {}
  EcalEBTriggerPrimitiveCluster(int hwEt, int hwTime, int hwEta, int hwPhi, int numberOfCrystals, bool spike) : hwEt_(hwEt), hwTime_(hwTime), hwEta_(hwEta), hwPhi_(hwPhi), numberOfCrystals_(numberOfCrystals), spike_(spike) {}

  void setHwEt(const int hwEt) { hwEt_ = hwEt; }
  int hwEt() const { return hwEt_; }

  void setHwTime(const int hwTime) { hwTime_ = hwTime; }
  int hwTime() const { return hwTime_; }

  void setHwEta(const int hwEta) { hwEta_ = hwEta; }
  int hwEta() const { return hwEta_; }

  void setHwPhi(const int hwPhi) { hwPhi_ = hwPhi; }
  int hwPhi() const { return hwPhi_; }

  void setNumberOfCrystals(const int numberOfCrystals) { numberOfCrystals_ = numberOfCrystals; }
  int numberOfCrystals() const { return numberOfCrystals_; }

  void setSpike(const bool spike) { spike_ = spike; }
  bool spike() const { return spike_; }

  bool operator==(const EcalEBTriggerPrimitiveCluster& rhs) const;
  inline bool operator!=(const EcalEBTriggerPrimitiveCluster& rhs) const { return !(operator==(rhs)); };

 private:
  // CMS-TDR-015, Table 3.5
  int hwEt_;
  int hwTime_;
  int hwEta_;
  int hwPhi_;
  int numberOfCrystals_;
  bool spike_;
};

typedef std::vector<EcalEBTriggerPrimitiveCluster> EcalEBTriggerPrimitiveClusterCollection;
#endif
