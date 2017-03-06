import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# Message logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    #fileNames = cms.untracked.vstring('file:l1Ntuple_RAW2DIGI.root'),
    #fileNames = cms.untracked.vstring('file:l1Ntuple_RAW2DIGI_sep.root'),
    fileNames = cms.untracked.vstring('file:l1Ntuple_RAW2DIGI_sep_1.root', 'file:l1Ntuple_RAW2DIGI_sep_2.root', 'file:l1Ntuple_RAW2DIGI_sep_3.root', 'file:l1Ntuple_RAW2DIGI_sep_4.root', 'file:l1Ntuple_RAW2DIGI_sep_5.root', 'file:l1Ntuple_RAW2DIGI_sep_6.root'),
)

process.options = cms.untracked.PSet()


# Plugin: EmuAccuracy
process.load("L1TMuonEndCapStudies.EmuAccuracySep2016.emuaccuracy_cfi")
#process.emuaccuracy.emuHitTag = cms.InputTag('simEmtfDigis', '')
#process.emuaccuracy.emuTrackTag = cms.InputTag('simEmtfDigis', '')
#process.emuaccuracy.emuHitTag2 = cms.InputTag('simEmtfDigis', 'CSC')
#process.emuaccuracy.emuTrackTag2 = cms.InputTag('simEmtfDigis', '')
process.emuaccuracy.verbosity = 0

# Paths
process.p = cms.Path(process.emuaccuracy)

