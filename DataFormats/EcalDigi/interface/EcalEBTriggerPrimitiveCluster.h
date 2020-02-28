#ifndef DataFormats_EcalDigi_EcalEBTriggerPrimitiveCluster_h
#define DataFormats_EcalDigi_EcalEBTriggerPrimitiveCluster_h

class EcalEBTriggerPrimitiveCluster {
 public:
  EcalEBTriggerPrimitiveCluster() : hwEt_(0), hwTime_(0), hwEta_(0), hwPhi_(0), numberOfCrystals_(0), spike_(false) {}
  EcalEBTriggerPrimitiveCluster(int hwEt, int hwTime, int hwEta, int hwPhi, int numberOfCrystals, bool spike) : hwEt_(hwEt), hwTime_(hwTime), hwEta_(hwEta), hwPhi_(hwPhi), numberOfCrystals_(numberOfCrystals), spike_(spike) {}

  void setHwEt(const int hwEt) { hwEt_ = hwEt; }
  bool hwEt() const { return hwEt_; }

  void setHwTime(const int hwTime) { hwTime_ = hwTime; }
  bool hwTime() const { return hwTime_; }

  void setHwEta(const int hwEta) { hwEta_ = hwEta; }
  bool hwEta() const { return hwEta_; }

  void setHwPhi(const int hwPhi) { hwPhi_ = hwPhi; }
  bool hwPhi() const { return hwPhi_; }

  void setNumberOfCrystals(const int numberOfCrystals) { numberOfCrystals_ = numberOfCrystals; }
  bool numberOfCrystals() const { return numberOfCrystals_; }

  void setSpike(const bool spike) { spike_ = spike; }
  bool spike() const { return spike_; }

 private:
  // CMS-TDR-015, Table 3.5
  int hwEt_;
  int hwTime_;
  int hwEta_;
  int hwPhi_;
  int numberOfCrystals_;
  bool spike_;
};

#endif
