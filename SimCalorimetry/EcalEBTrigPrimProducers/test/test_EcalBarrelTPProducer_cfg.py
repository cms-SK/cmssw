import FWCore.ParameterSet.Config as cms

process = cms.Process("EBTPGTest")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')
process.load( 'Configuration.Geometry.GeometryExtendedRun4D110Reco_cff' ) 
process.load( 'Configuration.Geometry.GeometryExtendedRun4D110_cff' )
#process.load( 'Configuration.Geometry.GeometryExtendedRun4D98Reco_cff' ) 
#process.load( 'Configuration.Geometry.GeometryExtendedRun4D98_cff' )

process.load('Configuration.StandardSequences.MagneticField_cff')

#process.MessageLogger.categories = cms.untracked.vstring('EBPhaseIITPStudies', 'FwkReport')
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
   reportEvery = cms.untracked.int32(1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        '/store/mc/Phase2Spring24DIGIRECOMiniAOD/DYToLL_M-50_TuneCP5_14TeV-pythia8/GEN-SIM-DIGI-RAW-MINIAOD/PU200_Trk1GeV_140X_mcRun4_realistic_v4-v1/2810000/47946cea-bd28-463f-ba64-0757207dd773.root'
        #'/store/relval/CMSSW_14_0_0_pre2/RelValTTbar_14TeV/GEN-SIM-DIGI-RAW/PU_133X_mcRun4_realistic_v1_STD_2026D98_PU200_RV229-v1/2580000/0b2b0b0b-f312-48a8-9d46-ccbadc69bbfd.root'
    ),
)

# All this stuff just runs the various EG algorithms that we are studying
                         
# ---- Global Tag :
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '133X_mcRun4_realistic_v1', '')


process.simEcalEBTriggerPrimitiveDigis = cms.EDProducer("EcalEBTrigPrimProducer",
    BarrelOnly = cms.bool(True),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","","HLT"),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","ebDigis"),
    barrelEcalDigis = cms.InputTag("simEcalDigis","ebDigis"),
#    barrelEcalDigis = cms.InputTag("selectDigi","selectedEcalEBDigiCollection"),
    binOfMaximum = cms.int32(6), ## optional from release 200 on, from 1-10
    TcpOutput = cms.bool(False),
    Debug = cms.bool(False),
    Famos = cms.bool(False),
    nOfSamples = cms.int32(1)
)

process.ecalBcpPayloadParamsSource = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalBcpPayloadParamsRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

process.ecalBcpPayloadParamsEsProducer = cms.ESProducer("EcalBcpPayloadParamsESProducer",
    fwVersion = cms.uint32(1),

    # samples of interest configurable for each crystal
    samplesOfInterest = cms.VPSet(
        cms.PSet(
            ietaRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
            iphiRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
            sampleOfInterest = cms.uint32(6)
        )
    ),

    # configuration PSets for the individual payload algorithms
    algoConfigs = cms.VPSet(
        #cms.PSet(
        #    algo = cms.string("spikeTaggerLd"),
        #    type = cms.string("ideal"), # ideal, hls
        #    perCrystalParams = cms.VPSet(
        #        cms.PSet(
        #            ietaRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
        #            iphiRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
        #            spikeThreshold = cms.double(-0.1),
        #            weights = cms.vdouble(1.5173, -2.1034, 1.8117, -0.6451)
        #        )
        #    )
        #),
        cms.PSet(
            algo = cms.string("spikeTaggerLd"),
            type = cms.string("hls"), # ideal, hls
            perCrystalParams = cms.VPSet(
            )
        ),
        cms.PSet(
            algo = cms.string("tpClusterAlgo"),
            type = cms.string("crystalSumWithSwissCrossSpike"), # crystalSumWithSwissCrossSpike, hls
            # the rest of the parameters is currently hardcoded in TPClusterAlgoV1.cc
        ),

    )
)

process.simEcalBarrelTPDigis = cms.EDProducer("EcalBarrelTPProducer",
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","","HLT"),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","ebDigis"),
#    barrelEcalDigis = cms.InputTag("selectDigi","selectedEcalEBDigiCollection"),
    barrelEcalDigis = cms.InputTag("simEcalDigis","ebDigis"),

    configSource = cms.string("fromES"), # use "fromES" for parameters from ES Producer
                                         # use "fromModuleConfig" for parameters below from this module configuration
    # configuration below is only active when configSource is set to "fromModuleConfig"
    fwVersion = cms.uint32(1),

    # samples of interest configurable for each crystal
    samplesOfInterest = cms.VPSet(
    ),

    # configuration PSets for the individual payload algorithms
    algoConfigs = cms.VPSet(
    )
)

process.p = cms.Path(process.simEcalEBTriggerPrimitiveDigis+process.simEcalBarrelTPDigis)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "EBTP_PhaseII_TESTDF_uncompEt_spikeflag.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring("keep *_EcalEBTrigPrimProducer_*_*",
                                           "keep *_TriggerResults_*_*",
                                           "keep *_ecalRecHit_EcalRecHitsEB_*",
                                           "keep *_simEcalDigis_ebDigis_*",
                                           "keep *_selectDigi_selectedEcalEBDigiCollection_*",
                                           "keep *_g4SimHits_EcalHitsEB_*",
                                           "keep *_simEcalEBTriggerPrimitiveDigis_*_*",
                                           "keep *_simEcalBarrelTPDigis_*_*")
)

process.end = cms.EndPath( process.Out )



#print process.dumpPython()
#dump_file = open("dump_file.py", "w")
#

