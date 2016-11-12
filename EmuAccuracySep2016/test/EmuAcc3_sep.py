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
    fileName = cms.untracked.string('l1Ntuple_RAW2DIGI_sep.root'),
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
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00148DAF-A684-E611-960A-02163E0134A9.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00469FF0-C684-E611-88F8-02163E013620.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/008389AD-A684-E611-9B1D-FA163E480423.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00AABA9A-B684-E611-B508-FA163EFD691C.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00DA5315-AA84-E611-8AA9-02163E01199A.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/00FD3DF7-A184-E611-9841-02163E01347D.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/0227E1C7-CF84-E611-A271-FA163E06F9DB.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/022E63C2-BF84-E611-91B7-02163E0126D3.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02551D95-CF84-E611-9B27-FA163E2A69B6.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/0256DF81-CA84-E611-8FBC-FA163EC4F635.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02610FBA-B184-E611-A29F-02163E014516.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/0274B279-AF84-E611-81A8-02163E013456.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/029DFCBF-C384-E611-8B59-FA163E817AEE.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02C048C3-BA84-E611-9C9C-02163E012A71.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02CCF8FE-D684-E611-A09B-02163E0144D3.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02DE684F-AA84-E611-8C9E-02163E0144AD.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02E8F87D-9684-E611-A172-FA163E0B3058.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/02FCAE61-AC84-E611-8F3D-02163E01474B.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/041D7730-AC84-E611-AF34-02163E014485.root",
    "/store/express/Run2016H/ExpressPhysics/FEVT/Express-v2/000/281/707/00000/0434525C-AC84-E611-B798-FA163EC7FB6E.root",
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
    #process.simEmtfDigis.spPRParams16.UseSecondEarliest = False
    process.simEmtfDigis.spPCParams16.FixZonePhi = False
    process.simEmtfDigis.spPRParams16.UseSymmetricalPatterns = False
    process.simEmtfDigis.spPAParams16.Bug9BitDPhi = True
    process.simEmtfDigis.spPAParams16.BugMode7CLCT = True
    process.simEmtfDigis.spPAParams16.BugNegPt = True
process.step1 = cms.Path((process.emtfStage2Digis) + (process.muonCSCDigis+process.muonRPCDigis) + process.simEmtfDigis)
process.schedule = cms.Schedule(process.step1, process.RAWoutput_step)


# Configure framework report and summary
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Dump the full python config
with open("dump.py", "w") as f:
    f.write(process.dumpPython())

