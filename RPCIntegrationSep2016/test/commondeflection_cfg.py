import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

# Message logger
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

# Number of events
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:l1NtupleMC_RAW2DIGI_sep.root'),
)

process.options = cms.untracked.PSet()


# Plugin: RPCIntegration
process.load("L1TMuonEndCapStudies.RPCIntegrationSep2016.commondeflection_cfi")
#process.commondeflection.outFileName = "histos.root"
process.commondeflection.verbosity = 1


# Paths
process.p = cms.Path(process.commondeflection)

