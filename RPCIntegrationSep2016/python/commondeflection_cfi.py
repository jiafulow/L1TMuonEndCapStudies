import FWCore.ParameterSet.Config as cms

commondeflection = cms.EDAnalyzer('CommonDeflectionAngleProducerSep2016',
    emuHitTag = cms.InputTag('simEmtfDigis'),
    emuTrackTag = cms.InputTag('simEmtfDigis'),
    verbosity = cms.untracked.int32(0),
)
