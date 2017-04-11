import FWCore.ParameterSet.Config as cms

rpcintegration2 = cms.EDAnalyzer('RPCIntegration2Sep2016',
    emuHitTag = cms.InputTag('simEmtfDigis'),
    emuTrackTag = cms.InputTag('simEmtfDigis'),
    genPartTag = cms.InputTag('genParticles'),
    outFileName = cms.string('ntuple.root'),
    verbosity = cms.untracked.int32(0),
)
