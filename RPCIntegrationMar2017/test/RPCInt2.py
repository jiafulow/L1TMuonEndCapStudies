# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/TSG-PhaseIISpring17GS-00001-fragment.py --step GEN,SIM,DIGI:pdigi_valid,L1,L1TrackTrigger,DIGI2RAW,RAW2DIGI --mc --eventcontent RAWSIM --datatier GEN-SIM-DIGI-RAW --nThreads 4 --conditions 90X_upgrade2023_realistic_v9 --beamspot HLLHC14TeV --geometry Extended2023D4 --era Phase2C2_timing --pileup NoPileUp -n 100 --no_exec
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Phase2C2_timing)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2023D4Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2023D4_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedHLLHC14TeV_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1TrackTrigger_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("EmptySource")

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('Configuration/GenProduction/python/TSG-PhaseIISpring17GS-00001-fragment.py nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(9),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(20971520),
    fileName = cms.untracked.string('l1NtupleMC_GEN_SIM_DIGI_L1_L1TrackTrigger_DIGI2RAW_RAW2DIGI.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '90X_upgrade2023_realistic_v9', '')

process.generator = cms.EDProducer("FlatRandomPtGunProducer",
    AddAntiParticle = cms.bool(True),
    PGunParameters = cms.PSet(
        MaxEta = cms.double(2.6),
        MaxPhi = cms.double(3.14159265359),
        MaxPt = cms.double(100),
        MinEta = cms.double(-2.6),
        MinPhi = cms.double(-3.14159265359),
        MinPt = cms.double(8),
        PartID = cms.vint32(-13)
    ),
    Verbosity = cms.untracked.int32(0),
    firstRun = cms.untracked.uint32(1),
    psethack = cms.string('single mu Pt8to100')
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.L1TrackTrigger_step = cms.Path(process.L1TrackTrigger)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.raw2digi_step = cms.Path(process.RawToDigi)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.L1TrackTrigger_step,process.digi2raw_step,process.raw2digi_step,process.endjob_step,process.RAWSIMoutput_step)

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(4)
process.options.numberOfStreams=cms.untracked.uint32(0)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path)._seq = process.generator * getattr(process,path)._seq 


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion



# ______________________________________________________________________________
# Modify output
process.RAWSIMoutput.outputCommands = ['drop *', 'keep *_genParticles_*_*', 'keep *_simCscTriggerPrimitiveDigis_*_*', 'keep *_simMuonRPCDigis_*_*', 'keep *_simMuonGEMDigis_*_*', 'keep *_simEmtfDigis_*_*']

# My paths and schedule definitions
if True:
    process.simMuonRPCDigis.doBkgNoise               = False
    process.simMuonGEMDigis.doBkgNoise               = False
process.load('L1Trigger.L1TMuonEndCap.fakeEmtfParams_cff')
#from L1Trigger.L1TMuonEndCap.simEmtfDigis_cfi import simEmtfDigisMC
#process.simEmtfDigis = simEmtfDigisMC
process.simEmtfDigis.verbosity = cms.untracked.int32(0)
if True:
    #process.simCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi")
    #process.simCscTriggerPrimitiveDigis.CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")

    process.simEmtfDigis.spPCParams16.ZoneBoundaries = [0,36,54,96,127]
    process.simEmtfDigis.spPCParams16.UseNewZones    = True
    process.simEmtfDigis.spPCParams16.FixME11Edges   = True
    #process.simEmtfDigis.spPCParams16.CoordLUTDir    = 'ph_lut_v2'
    process.simEmtfDigis.GEMEnable                   = True
process.step1 = cms.Path(process.simEmtfDigis)
#process.schedule = cms.Schedule(process.step1, process.RAWSIMoutput_step)
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.digitisation_step,process.L1simulation_step,process.L1TrackTrigger_step,process.digi2raw_step,process.raw2digi_step,process.step1,process.endjob_step,process.RAWSIMoutput_step)


# Configure framework report and summary
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Dump the full python config
with open("dump.py", "w") as f:
    f.write(process.dumpPython())

