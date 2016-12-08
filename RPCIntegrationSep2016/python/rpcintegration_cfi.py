import FWCore.ParameterSet.Config as cms

rpcintegration = cms.EDAnalyzer('RPCIntegrationSep2016',
    #unpHitTag = cms.InputTag('emtfStage2Digis'),
    #unpTrackTag = cms.InputTag('emtfStage2Digis'),
    ##emuHitTag = cms.InputTag('simEmtfDigis', 'EMTF'),
    ##emuTrackTag = cms.InputTag('simEmtfDigis', 'EMTF'),
    #emuHitTag = cms.InputTag('simEmtfDigis'),
    #emuTrackTag = cms.InputTag('simEmtfDigis'),
    emuHitTag2 = cms.InputTag('simEmtfDigis'),
    emuTrackTag2 = cms.InputTag('simEmtfDigis'),
    genPartTag = cms.InputTag('genParticles'),
    outFileName = cms.string('histos.root'),
    verbosity = cms.untracked.int32(0),
)
