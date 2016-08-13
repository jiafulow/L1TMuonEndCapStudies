import FWCore.ParameterSet.Config as cms

emuaccuracy = cms.EDAnalyzer('EmuAccuracy',
    unpTrackTag = cms.InputTag('emtfStage2Digis'),
    emuTrackTag = cms.InputTag('simEmtfDigis','EMTF'),
)
