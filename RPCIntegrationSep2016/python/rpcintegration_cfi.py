import FWCore.ParameterSet.Config as cms

rpcintegration = cms.EDAnalyzer('RPCIntegrationSep2016',
    emuHitTag = cms.InputTag('simEmtfDigis'),
    emuTrackTag = cms.InputTag('simEmtfDigis'),
    genPartTag = cms.InputTag('genParticles'),
    outFileName = cms.string('histos.root'),
    verbosity = cms.untracked.int32(0),
)
