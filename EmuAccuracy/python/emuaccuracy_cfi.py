import FWCore.ParameterSet.Config as cms

emuaccuracy = cms.EDAnalyzer('EmuAccuracy',
    unpHitTag = cms.InputTag('emtfStage2Digis'),
    emuHitTag = cms.InputTag('simEmtfDigis', 'EMTF'),
    unpTrackTag = cms.InputTag('emtfStage2Digis'),
    emuTrackTag = cms.InputTag('simEmtfDigis', 'EMTF'),
)
