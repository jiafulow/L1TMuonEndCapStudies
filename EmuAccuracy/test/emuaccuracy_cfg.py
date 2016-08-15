import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    #fileNames = cms.untracked.vstring(
    #    'file:myfile.root'
    #)
    #fileNames = cms.untracked.vstring('/store/data/Run2016F/SingleMuon/RAW/v1/000/278/018/00000/A015A25D-A058-E611-9DF0-02163E011E0F.root'),
    fileNames = cms.untracked.vstring('file:SingleMuon_Run2016F_RAW2DIGI.root'),
)

from L1TMuonEndCapStudies.EmuAccuracy.emuaccuracy_cfi import emuaccuracy
process.emuaccuracy = emuaccuracy

#process.MessageLogger.categories.append('EmuAccuracy')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000


process.p = cms.Path(process.emuaccuracy)
