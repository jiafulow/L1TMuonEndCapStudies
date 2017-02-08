#!/bin/bash

./submit.sh NoAnalyzer SingleMu_Pt1To1000_FlatRandomOneOverPt singlemu emtf 2016 137
#./submit.sh NoAnalyzer SingleMu_Pt1To1000_FlatRandomOneOverPt singlemu emtf 2016 229
#./submit.sh NoAnalyzer SingleMu_Pt1To1000_FlatRandomOneOverPt singlemu emtf 2016 1369

# Testing
#./submit.sh NoAnalyzer SingleMu_Pt1To1000_FlatRandomOneOverPt_foo singlemu emtf 2016 1

# hadd
#hadd -f histos_add_20161220.root blt_projects/NoAnalyzer/SingleMu_Pt1To1000_FlatRandomOneOverPt/out/histos_*.root
