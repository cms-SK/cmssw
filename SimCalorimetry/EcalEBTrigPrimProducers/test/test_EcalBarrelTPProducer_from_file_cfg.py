import FWCore.ParameterSet.Config as cms

process = cms.Process("EBTPGTest")

process.load('Configuration.StandardSequences.Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")

process.MessageLogger.categories = cms.untracked.vstring('EBPhaseIITPStudies', 'FwkReport')
process.MessageLogger.cerr.FwkReport = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

process.source = cms.Source("EcalBCPFileInputSource",
    fileNames = cms.untracked.vstring('file:../../../hls/testbench_data/rx_tbspikes0p9_ld2_1000evts.txt'),
    firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID([]),
    runNumber = cms.untracked.uint32(1),
    firstEventNumber = cms.untracked.uint32(1),
    startSample = cms.untracked.uint32(4)
)

process.ecalBcpPayloadParamsSource = cms.ESSource("EmptyESSource",
    recordName = cms.string('EcalBcpPayloadParamsRcd'),
    iovIsRunNotTime = cms.bool(True),
    firstValid = cms.vuint32(1)
)

sampleOfInterest = 6

process.ecalBcpPayloadParamsEsProducer = cms.ESProducer("EcalBcpPayloadParamsESProducer",
    fwVersion = cms.uint32(1),

    # samples of interest configurable for each crystal
    samplesOfInterest = cms.VPSet(
        cms.PSet(
            ietaRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
            iphiRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
            sampleOfInterest = cms.uint32(sampleOfInterest)
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
    )
)

process.simEcalBarrelTPDigis = cms.EDProducer("EcalBarrelTPProducer",
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","","HLT"),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","ebDigis"),
#    barrelEcalDigis = cms.InputTag("selectDigi","selectedEcalEBDigiCollection"),
    barrelEcalDigis = cms.InputTag("source"),

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

process.simEcalBarrelTPDigisIdealSpikeTagger = cms.EDProducer("EcalBarrelTPProducer",
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","","HLT"),
#    barrelEcalDigis = cms.InputTag("simEcalUnsuppressedDigis","ebDigis"),
#    barrelEcalDigis = cms.InputTag("selectDigi","selectedEcalEBDigiCollection"),
    barrelEcalDigis = cms.InputTag("source"),

    configSource = cms.string("fromModuleConfig"), # use "fromES" for parameters from ES Producer
                                         # use "fromModuleConfig" for parameters below from this module configuration
    # configuration below is only active when configSource is set to "fromModuleConfig"
    fwVersion = cms.uint32(1),

    # samples of interest configurable for each crystal
    samplesOfInterest = cms.VPSet(
        cms.PSet(
            ietaRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "-85:42" (user defined), "1:" (positive side), ":" (whole EB eta range)
            iphiRange = cms.string(":"), # Example range formats "ietaMin:ietaMax", e.g. "90:270" (user defined), ":180" (MIN_IPHI:180), ":" (MIN_IPHI:MAX_IPHI)
            sampleOfInterest = cms.uint32(sampleOfInterest)
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

process.TFileService = cms.Service("TFileService",
   fileName = cms.string("histos.root"),
   closeFileFast = cms.untracked.bool(True)
)

process.ecalBarrelTPAnalyzer = cms.EDAnalyzer("EcalBarrelTPAnalyzer",
   barrelTPColl1 = cms.InputTag("simEcalBarrelTPDigisIdealSpikeTagger"),
   barrelTPColl2 = cms.InputTag("simEcalBarrelTPDigis"),
   barrelTPClusterColl1 = cms.InputTag("simEcalBarrelTPDigisIdealSpikeTagger"),
   barrelTPClusterColl2 = cms.InputTag("simEcalBarrelTPDigis"),
   sampleOfInterest = cms.uint32(sampleOfInterest)
)

process.p = cms.Path(process.simEcalBarrelTPDigis+process.simEcalBarrelTPDigisIdealSpikeTagger+process.ecalBarrelTPAnalyzer)

process.Out = cms.OutputModule( "PoolOutputModule",
    fileName = cms.untracked.string( "EBTP_PhaseII_filetest_uncompEt_spikeflag.root" ),
    fastCloning = cms.untracked.bool( False ),
    outputCommands = cms.untracked.vstring("keep *")
)

process.end = cms.EndPath( process.Out )

