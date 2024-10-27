#ifndef SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDIdealAlgoV1_h
#define SimCalorimetry_EcalEBTrigPrimAlgos_SpikeTaggerLDIdealAlgoV1_h

#include <array>
#include <vector>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimCalorimetry/EcalEBTrigPrimAlgos/interface/SpikeTaggerLDAlgo.h"

namespace ecalph2 {

class SpikeTaggerLDIdealAlgoV1 : public SpikeTaggerLDAlgo {
 public:
  SpikeTaggerLDIdealAlgoV1(const std::shared_ptr<ecalph2::EcalBcpPayloadParamsHelper> ecalBcpPayloadParamsHelper, const edm::EventSetup &eventSetup);
  ~SpikeTaggerLDIdealAlgoV1() override {};

  void processEvent(const EBDigiCollection &ebDigis, EcalEBTrigPrimDigiCollection &ebTPs) override;

 private:
  // returns true if the signal shape for this crystal matches a spike
  bool isSpike();

  unsigned int peakIdx_;

  std::array<float, 4> gains_;

  double spikeThreshold_;
  std::vector<double> weights_;

  float calcLD(const EBDataFrame &frame) const;
  float calcRMinus1Poly(const EBDataFrame &frame) const;
};

} // namespace ecalph2
#endif
