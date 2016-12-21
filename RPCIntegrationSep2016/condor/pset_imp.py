import FWCore.ParameterSet.Config as cms
import pset; process = pset.process

def tweak_input(njobs, jobid):
  filename = "input.txt"
  lines = tuple(open(filename))
  def chunk(l,n,i):
    m=(len(l)+n-1)/n
    return l[(i-1)*m:i*m]
  lines = chunk(lines, njobs, jobid)
  process.source.fileNames = lines

def tweak_output(njobs, jobid):
  #process.RAWSIMoutput.fileName = process.RAWSIMoutput.fileName._value.replace('.root', '_%i.root' % jobid)
  #process.TFileService.fileName = process.TFileService.fileName._value.replace('.root', '_%i.root' % jobid)
  process.rpcintegration.outFileName = process.rpcintegration.outFileName._value.replace('.root', '_%i.root' % jobid)

def tweak_maxEvents(njobs, jobid):
  process.maxEvents.input = -1
  process.MessageLogger.cerr.FwkReport.reportEvery = 1000

def tweak_pset(njobs, jobid):
  tweak_input(njobs, jobid)
  tweak_output(njobs, jobid)
  tweak_maxEvents(njobs, jobid)

