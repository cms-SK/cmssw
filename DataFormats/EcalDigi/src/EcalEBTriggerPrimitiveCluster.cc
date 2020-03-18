#include "DataFormats/EcalDigi/interface/EcalEBTriggerPrimitiveCluster.h"

bool EcalEBTriggerPrimitiveCluster::operator==(const EcalEBTriggerPrimitiveCluster& rhs) const
{
  return hwEt_ == rhs.hwEt() &&
         hwTime_ == rhs.hwTime() &&
         hwEta_ == rhs.hwEta() &&
         hwPhi_ == rhs.hwPhi() &&
         numberOfCrystals_ == rhs.numberOfCrystals() &&
         spike_ == rhs.spike();
}

