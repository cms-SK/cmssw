import FWCore.ParameterSet.Config as cms

process = cms.Process("EBTPGTest")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023D35Reco_cff') # for MTD samples
process.load('Configuration.Geometry.GeometryExtended2023D35_cff') # for MTD samples
#process.load('Configuration.Geometry.GeometryExtended2023D41Reco_cff') # for L1 TDR samples
#process.load('Configuration.Geometry.GeometryExtended2023D41_cff') # for L1 TDR samples
process.load('Configuration.StandardSequences.MagneticField_cff')

process.MessageLogger.categories = cms.untracked.vstring('EBPhaseIITPStudies', 'FwkReport')
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
   reportEvery = cms.untracked.int32(1)
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(2000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #'/store/mc/PhaseIIMTDTDRAutumn18DR/SingleE_FlatPt-2to100/FEVT/NoPU_103X_upgrade2023_realistic_v2-v1/70000/B0464430-CA74-7844-A36A-A5EF21C3D7C0.root',
        #'/store/mc/PhaseIIMTDTDRAutumn18DR/SingleE_FlatPt-2to100/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/70000/FF17CBE6-81E5-8D43-B58B-6DF17222820E.root',
        '/store/mc/PhaseIIMTDTDRAutumn18DR/NeutrinoGun_E_10GeV/FEVT/PU200_103X_upgrade2023_realistic_v2-v1/280000/EFFCC733-2B7F-C645-929D-505B1E0949D6.root'
    ),
)

# All this stuff just runs the various EG algorithms that we are studying
                         
# ---- Global Tag :
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'PH2_1K_FB_V3::All', '')
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

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
    configSource = cms.string("fromModuleConfig"), # use "fromDB" for parameters from DB

    # configuration below is only active when configSource is set to "fromModuleConfig"
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
        cms.PSet(
            algo = cms.string("spikeTaggerLd"),
            type = cms.string("ideal"), # ideal, hls
            perCrystalParams = cms.VPSet(
                cms.PSet(
                    ietaRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
                    iphiRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
                    spikeThreshold = cms.double(-0.1),
                    weights = cms.vdouble(1.5173, -2.1034, 1.8117, -0.6451)
                )
            )
        ),
    )
)

process.simEcalBarrelTPDigis = cms.EDProducer("EcalBarrelTPProducer",
    configSource = cms.string("fromES"), # use "fromES" for parameters from ES Producer
                                         # use "fromModuleConfig" for parameters from this module configuration
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","","HLT"),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","ebDigis"),
#    barrelEcalDigis = cms.InputTag("selectDigi","selectedEcalEBDigiCollection"),
    barrelEcalDigis = cms.InputTag("simEcalDigis","ebDigis"),

    # configuration below is only active when configSource is set to "fromModuleConfig"
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
        cms.PSet(
            algo = cms.string("spikeTaggerLd"),
            type = cms.string("ideal"), # ideal, hls
            perCrystalParams = cms.VPSet(
                cms.PSet(
                    ietaRange = cms.string("-85:85"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
                    iphiRange = cms.string("1:360"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
                    spikeThreshold = cms.double(-0.1),
                    weights = cms.vdouble(1.5173, -2.1034, 1.8117, -0.6451)
                )
            )
        ),
    )
)

process.pNancy = cms.Path(process.simEcalEBTriggerPrimitiveDigis+process.simEcalBarrelTPDigis)

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

