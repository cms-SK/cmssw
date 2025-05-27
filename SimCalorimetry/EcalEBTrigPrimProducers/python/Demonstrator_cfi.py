# configuration for Setup

import FWCore.ParameterSet.Config as cms

DemonstratorSK_params = cms.PSet (

  InputTag       = cms.InputTag( "simEcalUnsuppressedDigis" ),      # inout tag for ecal barrel adc counts

  InfraGap       = cms.int32( 6 ),                                  # number of clock tics between pakets
  NumEvents      = cms.int32( 6 ),                                  # packet size in BX
  NumFramesBX    = cms.int32( 9 ),                                  # number of clock tics per bx

  NumTotal       = cms.int32( 61200 ),                              # total number of ADCs
  NumCounts      = cms.int32(    16 ),                              # number of samples per ADC

  PedestalADC    = cms.int32  ( 12   ),                             # ADC counter offset
  ThresholdPeak  = cms.int32  ( 10   ),                             # adc count offest above noise
  ThresholdLD    = cms.double ( -0.1 ),                             #
  WeightsLD      = cms.vdouble( 1.5173, -2.1034, 1.8117 ),          #


  NumSamples     = cms.int32(  4 ),                                 # number of samples per bx
  WidthADC       = cms.int32( 14 ),                                 # number of bits used per ADC count
  WidthSK        = cms.int32(  1 ),                                 # number of bits used per SK flag

  NumADCs        = cms.int32( 75 ),                                 # number of ADCs
  MuxedADCs      = cms.int32(  5 ),                                 # number of ADC counts muxed to one channel
  MuxedSKs       = cms.int32( 75 ),                                 # number of SK flags muxed to one channel

  SlicesADCs     = cms.vint32( 35, 35 ),                            # muxed input object split into two frames with those number of bits
  SlicesSKs      = cms.vint32( 64, 11 ),                            # muxed output object split into two frames with those number of bits

  ChannelsIn     = cms.vint32(),                                    # input channel maping, leave blank for default maping
  ChannelsOut    = cms.vint32(),                                    # output channel maping, leave blank for default maping

  RunTime        = cms.double( 2.5 ),                               # modelsim simulatio time in us
  Dir            = cms.string( "/heplnw039/tschuh/work/proj/ld/" ), # path to ipbb project area
  txtInput       = cms.string( "in.txt"   ),                        # file name containing input data
  txtOutputSim   = cms.string( "sim.txt"  ),                        # file name containing simulated output data
  txtOutputEmu   = cms.string( "emu.txt"  ),                        # file name containing emulated output data
  txtOutputDiff  = cms.string( "diff.txt" ),                        # file name containing sim vs emu diff output

)
