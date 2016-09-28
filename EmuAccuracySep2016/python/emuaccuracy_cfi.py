import FWCore.ParameterSet.Config as cms

emuaccuracy = cms.EDAnalyzer('EmuAccuracySep2016',
    unpHitTag = cms.InputTag('emtfStage2Digis'),
    unpTrackTag = cms.InputTag('emtfStage2Digis'),
    #emuHitTag = cms.InputTag('simEmtfDigis', 'EMTF'),
    #emuTrackTag = cms.InputTag('simEmtfDigis', 'EMTF'),
    emuHitTag = cms.InputTag('simEmtfDigisData'),
    emuTrackTag = cms.InputTag('simEmtfDigisData'),
    verbosity = cms.untracked.int32(0),
)
