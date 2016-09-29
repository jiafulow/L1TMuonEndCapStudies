# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: l1Ntuple --step RAW2DIGI --data --eventcontent RAW --era Run2_2016 --conditions auto:run2_data --geometry Extended2016,Extended2016Reco --customise L1Trigger/Configuration/customiseReEmul.L1TEventSetupForHF1x1TPs --customise L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --customise L1Trigger/Configuration/customiseUtils.L1TTurnOffUnpackStage2GtGmtAndCalo --filein /store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A015A25D-A058-E611-9DF0-02163E011E0F.root --no_exec -n 100
import FWCore.ParameterSet.Config as cms

from Configuration.StandardSequences.Eras import eras

process = cms.Process('RAW2DIGI',eras.Run2_2016)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A015A25D-A058-E611-9DF0-02163E011E0F.root'),
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
    fileName = cms.untracked.string('SingleMuon_Run2016F_RAW2DIGI_std.root'),
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
from L1Trigger.Configuration.customiseReEmul import L1TEventSetupForHF1x1TPs,L1TReEmulFromRAW 

#call to customisation function L1TEventSetupForHF1x1TPs imported from L1Trigger.Configuration.customiseReEmul
process = L1TEventSetupForHF1x1TPs(process)

#call to customisation function L1TReEmulFromRAW imported from L1Trigger.Configuration.customiseReEmul
process = L1TReEmulFromRAW(process)

# Automatic addition of the customisation function from L1Trigger.L1TNtuples.customiseL1Ntuple
from L1Trigger.L1TNtuples.customiseL1Ntuple import L1NtupleEMU 

#call to customisation function L1NtupleEMU imported from L1Trigger.L1TNtuples.customiseL1Ntuple
process = L1NtupleEMU(process)

# Automatic addition of the customisation function from L1Trigger.Configuration.customiseUtils
from L1Trigger.Configuration.customiseUtils import L1TTurnOffUnpackStage2GtGmtAndCalo 

#call to customisation function L1TTurnOffUnpackStage2GtGmtAndCalo imported from L1Trigger.Configuration.customiseUtils
process = L1TTurnOffUnpackStage2GtGmtAndCalo(process)

# End of customisation functions


# Modify output
process.RAWoutput.outputCommands = ['drop *','keep *_emtfStage2Digis_*_*','keep *_muonRPCDigis_*_*','keep *_simEmtfDigis_*_*','keep *_simEmtfDigisData_*_*',]

# Modify source
fileNames = [
    '/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A015A25D-A058-E611-9DF0-02163E011E0F.root', #event 1686648178
    '/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A058A03A-A058-E611-8713-02163E011935.root', #events 1540061587, 1539957230, 1539578840, 1541093157
    '/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A09AEB8F-6858-E611-8CA7-02163E0120FD.root',
    '/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A0AC8119-A058-E611-97CB-02163E0144B4.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A0B13946-A258-E611-BF04-FA163E7FF867.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A0D1D95D-A258-E611-A313-02163E011CF3.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A0F95926-A058-E611-9C3C-FA163EFD21E0.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A238DA3D-A958-E611-A2B1-FA163E5E6331.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A23A6520-6C58-E611-9C48-FA163E2B692D.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A257A474-6658-E611-8997-02163E01277C.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A260D642-9E58-E611-B854-02163E0146EB.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A26C4C4B-A058-E611-B94C-02163E012734.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A27A21E4-6058-E611-AA82-02163E0126A5.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A28E14D1-A858-E611-919B-02163E013521.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A293DEC1-A258-E611-BC67-02163E012A8B.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A2C1DF8E-6058-E611-97AB-02163E011C7C.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A2EAD6A7-9F58-E611-B8BC-02163E012BDF.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A2EDDAAA-7258-E611-A21D-02163E011F43.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A436D444-9E58-E611-B23B-02163E014383.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A443DE6E-A858-E611-B5D2-FA163E9B76AC.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A45E0E67-A158-E611-B31E-02163E011B4C.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A4A30E6B-A858-E611-ACB0-FA163E7C8087.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A4C6D79B-A958-E611-9DF7-02163E014540.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A6348D34-A058-E611-8D35-FA163E240241.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A63C0E45-6B58-E611-AF91-FA163E321AE0.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A63CA23F-A058-E611-8D2F-02163E01458D.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A6411D5A-A158-E611-9856-02163E0142F0.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A64A5F21-A858-E611-82FE-FA163E0106C3.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A65520BB-6A58-E611-B878-02163E01339B.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A67742EB-6958-E611-8631-02163E0146F1.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A693442A-A858-E611-9D30-FA163EC4B7FA.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A6B9E36D-6D58-E611-A8B2-02163E0144CB.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A6EFD2CD-A158-E611-A4D0-02163E01279E.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A82FD570-A158-E611-A088-02163E01479A.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A877891A-6F58-E611-8D50-FA163E54BB3A.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A8B6DDED-A858-E611-AD29-02163E01376B.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A8BFFC36-A158-E611-8446-02163E014138.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A8C022A1-A158-E611-ADCE-02163E0143B8.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A8F88968-6D58-E611-9CDB-02163E01273A.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A8FD691B-A858-E611-AFD0-FA163EFA5241.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AA50AB92-A958-E611-87C1-FA163E92D1B7.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AA756529-A258-E611-9254-02163E011FD9.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AA8B5B8D-A058-E611-8676-02163E0134DF.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AA926338-A058-E611-AFA8-02163E0142E8.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AAC45613-6458-E611-8DCD-02163E01478E.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC0E0F81-6A58-E611-8D78-02163E01436E.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC1DB962-A758-E611-AADD-02163E0135EA.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC3AEADE-6958-E611-9E95-FA163E5259C4.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC75B745-A058-E611-8B98-FA163EBE7364.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC80044D-6458-E611-90E2-FA163E2EB753.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC8071B1-A158-E611-B6B0-FA163E13BCD6.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AC9891B5-A158-E611-83E2-02163E01295F.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/ACD65B8B-A558-E611-856A-02163E0145CE.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AE122615-5F58-E611-A40C-02163E011A03.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AE1FDCDA-A858-E611-BB28-02163E014653.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AE650ABC-A858-E611-99EC-02163E0138C5.root',
    #'/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/AEF030CB-A158-E611-B8F0-02163E01259B.root',
]
process.source.fileNames = cms.untracked.vstring(fileNames)

eventsToProcess = [
    #'278018:1539957230',
    #'278018:1540061587',
    #'278018:1540745931',
    #'278018:1541093157',
    #'278018:1686648178',
    #'278018:1686541662',
    #'278018:1687278667',
    #'278018:1686541662',
    #'278018:1686727950',
    #'278018:1687217520',
    #'278018:1686654434',
    #'278018:1686692705',
    #'278018:1687701259',
    #'278018:1686379804',
    #'278018:1687597236',
    #'278018:1687746030',
    #'278018:1686648178',
    #'278018:1687317481',
    #'278018:1686296629',
    #'278018:1686169492',
    #'278018:1685942945',
]
process.source.eventsToProcess = cms.untracked.VEventRange(eventsToProcess)

# Plugin: simEmtfDigis
if True:
    process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
    process.simEmtfDigis.CSCInput = cms.InputTag('emtfStage2Digis')
    process.simEmtfDigis.GenInput = cms.InputTag('genParticles')
    process.simEmtfDigis.GMTInput = cms.InputTag('gtDigis')
    #process.simEmtfDigis.isData = cms.bool(True)
    process.step1 = cms.Path((process.gtDigis)+(process.emtfStage2Digis)+(process.muonCSCDigis+process.muonRPCDigis)+(process.simEmtfDigis))
    #process.step1 = cms.Path((process.csctfDigis+process.gtDigis)+(process.emtfStage2Digis)+(process.muonCSCDigis+process.muonRPCDigis)+(process.simCscTriggerPrimitiveDigis+process.simEmtfDigis))
    process.schedule = cms.Schedule(process.step1, process.RAWoutput_step)

# Plugin: simEmtfDigis (NEW)
if False:
    #process.load("L1TriggerSep2016.L1TMuonEndCap.simEmtfDigis_cfi")
    if True:
        process.load('EventFilter.L1TRawToDigi.emtfStage2Digis_cfi')
        from L1TriggerSep2016.L1TMuonEndCap.simEmtfDigis_cfi import simEmtfDigisData
        process.simEmtfDigisData = simEmtfDigisData
    process.simEmtfDigisData.verbosity = 0
    process.step1 = cms.Path((process.gtDigis)+(process.emtfStage2Digis)+(process.muonCSCDigis+process.muonRPCDigis)+(process.simEmtfDigisData))
    process.schedule = cms.Schedule(process.step1, process.RAWoutput_step)

# Configure framework report and summary
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Dump the full python config
with open("dump.py", "w") as f:
    f.write(process.dumpPython())

