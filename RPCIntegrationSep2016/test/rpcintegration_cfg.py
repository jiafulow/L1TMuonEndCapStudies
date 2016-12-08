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
    fileNames = cms.untracked.vstring('file:l1NtupleMC_RAW2DIGI_sep.root'),
)

process.options = cms.untracked.PSet()


# Plugin: EmuAccuracy
process.load("L1TMuonEndCapStudies.RPCIntegrationSep2016.rpcintegration_cfi")
process.rpcintegration.outFileName = "histos.root"
process.rpcintegration.verbosity = 0


# Paths
process.p = cms.Path(process.rpcintegration)

