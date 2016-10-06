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
    fileNames = cms.untracked.vstring('file:SingleMuon_Run2016F_RAW2DIGI.root'),
    #fileNames = cms.untracked.vstring('file:SingleMuon_Run2016F_RAW2DIGI.15k.root'),
)

process.options = cms.untracked.PSet()


# Plugin: EmuAccuracy
process.load("L1TMuonEndCapStudies.EmuAccuracySep2016.emuaccuracy_cfi")
#process.emuaccuracy.emuHitTag = cms.InputTag('simEmtfDigis', 'EMTF')
#process.emuaccuracy.emuTrackTag = cms.InputTag('simEmtfDigis', 'EMTF')
process.emuaccuracy.verbosity = 0

# Paths
process.p = cms.Path(process.emuaccuracy)

