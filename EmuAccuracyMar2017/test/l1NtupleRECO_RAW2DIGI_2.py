# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1NtupleRECO -s RAW2DIGI --era=Run2_2016 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleAODRAWEMU --conditions=auto:run2_data -n 200 --data --no_exec --no_output --filein=/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/148D2F0D-639F-E611-8ABC-FA163EE13E98.root
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
    input = cms.untracked.int32(200)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/148D2F0D-639F-E611-8ABC-FA163EE13E98.root'),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1NtupleRECO nevts:200'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_data', '')

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.endjob_step)

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

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion


## JFTEST
# Input source
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
#process.source.fileNames = cms.untracked.vstring(fileNames)
process.source.fileNames = cms.untracked.vstring(["root://cmsxrootd.fnal.gov//store/data/Run2016H/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/284/036/00000/08B0B1C0-989F-E611-A9CE-02163E0143C2.root"])
process.maxEvents.input = 1000
process.TFileService.fileName = "L1Ntuple_2.root"
#process.simEmtfDigis.verbosity = 1

# Emulator settings
process.simGmtStage2Digis.autoBxRange = False
process.simEmtfDigis.spPAParams16.BugMode7CLCT = True
process.simEmtfDigis.spPAParams16.BugNegPt = True
process.simEmtfDigis.spTBParams16.BugME11Dupes = True
process.simEmtfDigis.RPCEnable = False
process.simEmtfDigis.spTBParams16.ThetaWindow = 4
#process.simEmtfDigis.spPAParams16.ReadPtLUTFile = True

# Dump python
with open("dump.py", "w") as f:
    f.write(process.dumpPython())
## END JFTEST

