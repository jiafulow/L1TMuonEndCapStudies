#!/usr/bin/env python

if __name__ == '__main__':
  import argparse
  import os

  # Setup argument parser
  parser = argparse.ArgumentParser()
  parser.add_argument("pset", type=str, help="pset file")
  parser.add_argument("infile", type=str, help="input file")
  parser.add_argument("outfile", type=str, help="output file")
  parser.add_argument("nevents", type=int, help="number of events")
  options = parser.parse_args()

  writeme = []
  if options.pset.endswith(".py"):
    pass
  else:
    raise Exception("pset is not .py")

  if options.infile.endswith(".root"):
    s = "process.source.fileNames = ['file:%s']" % options.infile
  else:
    raise Exception("infile is not .root")
  writeme.append(s)

  if options.outfile.endswith(".root"):
    s = "process.rpcintegration.outFileName = '%s'" % options.outfile
  else:
    raise Exception("outfile is not .root")
  writeme.append(s)

  s = "process.maxEvents.input = %i" % options.nevents
  writeme.append(s)

  with open(options.pset, "a") as f:
    for s in writeme:
      f.write(s+"\n")

