# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple --step RAW2DIGI --data --eventcontent RAW --era Run2_2016 --conditions auto:run2_data --customise L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleAODRAWEMU --filein /store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00148DAF-A684-E611-960A-02163E0134A9.root --no_exec -n 100
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    #input = cms.untracked.int32(1000)
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00148DAF-A684-E611-960A-02163E0134A9.root'),
    secondaryFileNames = cms.untracked.vstring(),
    eventsToProcess = cms.untracked.VEventRange(),
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1Ntuple nevts:100'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string(''),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    fileName = cms.untracked.string('l1Ntuple_RAW2DIGI_sep_r284036.root'),
    outputCommands = process.RAWEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWoutput_step = cms.EndPath(process.RAWoutput)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step,process.RAWoutput_step)

# customisation of the process.

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseReEmul
from L1Trigger.Configuration.customiseReEmul import L1TReEmulFromRAW 

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleAODRAWEMU 

#call to customisation function L1NtupleAODRAWEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleAODRAWEMU(process)

# End of customisation functions


# Modify output
process.RAWoutput.outputCommands = ['drop *', 'keep *_emtfStage2Digis_*_*', 'keep *_muonCSCDigis_*_*', 'keep *_muonRPCDigis_*_*', 'keep *_simEmtfDigis_*_*']

# Modify source
fileNames = [
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/08B0B1C0-989F-E611-A9CE-02163E0143C2.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/148D2F0D-639F-E611-8ABC-FA163EE13E98.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/14EE7A3D-639F-E611-81EE-02163E013441.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/1679AB7B-629F-E611-A800-FA163EF7B5F0.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/1A76ED83-629F-E611-BB70-FA163E36568E.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/1A85378F-629F-E611-A252-FA163EB8539A.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/1ADB1929-9F9F-E611-8C51-02163E013465.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/1E0CCA19-929F-E611-8C33-02163E011B45.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/22D93BB1-629F-E611-B13F-FA163E125AAF.root",
    "/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/38372FF6-A19F-E611-84CD-02163E013690.root",
]
process.source.fileNames = cms.untracked.vstring(fileNames)

eventsToProcess = [

]
process.source.eventsToProcess = cms.untracked.VEventRange(eventsToProcess)


# My paths and schedule definitions
process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
from L1TriggerSep2016.L1TMuonEndCap.simEmtfDigis_cfi import simEmtfDigisData
process.simEmtfDigis = simEmtfDigisData
process.simEmtfDigis.verbosity = cms.untracked.int32(0)
if True:
    #process.simEmtfDigis.spGCParams16.UseSecondEarliest = False
    #process.simEmtfDigis.spPCParams16.FixZonePhi = False
    #process.simEmtfDigis.spPRParams16.UseSymmetricalPatterns = False
    #process.simEmtfDigis.spPAParams16.Bug9BitDPhi = True
    process.simEmtfDigis.spPAParams16.BugMode7CLCT = True
    process.simEmtfDigis.spPAParams16.BugNegPt = True
    process.simEmtfDigis.spPAParams16.BugGMTPhi = True
    process.simEmtfDigis.spTBParams16.BugME11Dupes = True
process.step1 = cms.Path((process.emtfStage2Digis) + (process.muonCSCDigis+process.muonRPCDigis) + process.simEmtfDigis)
process.schedule = cms.Schedule(process.step1, process.RAWoutput_step)


# Configure framework report and summary
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Dump the full python config
with open("dump.py", "w") as f:
    f.write(process.dumpPython())

