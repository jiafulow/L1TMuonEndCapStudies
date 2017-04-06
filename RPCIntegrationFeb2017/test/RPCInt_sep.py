# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1NtupleMC --step RAW2DIGI --mc --eventcontent RAWSIM --era Run2_2016 --conditions auto:run2_mc --customise L1Trigger/Configuration/customiseReEmul.L1TReEmulMCFromRAW --customise L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleRAWEMUGEN_MC --filein /store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/26CA310A-4164-E611-BE48-001E67248566.root --no_exec -n 100
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(20000)
    #input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/EC6D71F4-2F64-E611-A229-001EC9ADC0B4.root'),
    #fileNames = cms.untracked.vstring('/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/26CA310A-4164-E611-BE48-001E67248566.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1NtupleMC nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('l1NtupleMC_RAW2DIGI.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step,process.RAWSIMoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulMCFromRAW 

#call to customisation function L1TReEmulMCFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulMCFromRAW(process)

## Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
#from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleRAWEMUGEN_MC 
#
##call to customisation function L1NtupleRAWEMUGEN_MC imported from L1Trigger.L1TNtuples.customiseL1Ntuple
#process = L1NtupleRAWEMUGEN_MC(process)

# End of customisation functions


# Modify output
process.RAWSIMoutput.outputCommands = ['drop *', 'keep *_genParticles_*_*', 'keep *_simCscTriggerPrimitiveDigis_*_*', 'keep *_simMuonRPCDigis_*_*', 'keep *_simEmtfDigis_*_*']

# Modify source
fileNames = [
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/26CA310A-4164-E611-BE48-001E67248566.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/EC6D71F4-2F64-E611-A229-001EC9ADC0B4.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/FC81006E-3A64-E611-BE81-001EC9ADDD7B.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/C0A70950-4C64-E611-8C76-008CFA165F30.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/926CFD80-4F64-E611-9344-008CFA1112B0.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/745A1421-4B64-E611-85A7-008CFA580778.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/58240974-4664-E611-AAEF-00259076082A.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/14CEDA3A-4864-E611-A74C-002590574922.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/C0EF161F-4B64-E611-A6BE-00259070E27A.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/60000/94ECA959-4D64-E611-9FC7-008CFA11134C.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/027C504B-8F64-E611-929A-0025904B21E2.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/044809A8-5664-E611-8C2F-0025904B2A8E.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/060F5CB0-8264-E611-8B9F-0025904E405A.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/08D335B2-8664-E611-BCEA-0025904B2334.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/0C7F3E84-6564-E611-B422-0025904B2334.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/0E09109A-7464-E611-B845-0025904E4034.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/0E146694-8C64-E611-8019-002590821168.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/1093A1BA-4F64-E611-A6EF-0025904CC418.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/1262865A-6364-E611-9A9E-003048C7BA3C.root",
    "/store/mc/RunIISpring16DR80/SingleMu_Pt1To1000_FlatRandomOneOverPt/GEN-SIM-RAW/NoPURAW_NZS_withHLT_80X_mcRun2_asymptotic_v14-v1/40000/129B9075-8264-E611-A6AD-00259081ED0A.root",
]
process.source.fileNames = cms.untracked.vstring(fileNames)


# My paths and schedule definitions
#process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
#from L1Trigger.L1TMuonEndCap.simEmtfDigis_cfi import simEmtfDigisMC
#process.simEmtfDigis = simEmtfDigisMC
process.simEmtfDigis.verbosity = cms.untracked.int32(0)
if True:
    process.simCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi")
    process.simCscTriggerPrimitiveDigis.CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")

    process.simEmtfDigis.RPCEnable                   = True
    process.simEmtfDigis.spPCParams16.ZoneBoundaries = [0,36,54,96,127]
    process.simEmtfDigis.spPCParams16.UseNewZones    = True
    process.simEmtfDigis.spPCParams16.CoordLUTDir    = 'ph_lut_v2'
    process.simEmtfDigis.spPCParams16.FixME11Edges   = True
process.step1 = cms.Path((process.simCscTriggerPrimitiveDigis) + process.simEmtfDigis)
process.schedule = cms.Schedule(process.step1, process.RAWSIMoutput_step)


# Configure framework report and summary
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Dump the full python config
with open("dump.py", "w") as f:
    f.write(process.dumpPython())

