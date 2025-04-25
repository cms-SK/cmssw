# EDAnalyzer testing Spike Killer emulator

import FWCore.ParameterSet.Config as cms
from SimCalorimetry.EcalEBTrigPrimProducers.Demonstrator_cfi import DemonstratorSK_params

DemonstratorSK = cms.EDAnalyzer("sk::Demonstrator", DemonstratorSK_params)
