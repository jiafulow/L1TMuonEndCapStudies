#!/bin/bash

# ______________________________________________________________________________
# Step 0: parse input arguments, set stageout areas

# Expects 6 input arguments
if [ "$#" -ne 6 ]; then
  echo "Usage: $0 [ANALYZER] [DATASET] [DATASETGROUP] [SELECTION] [PERIOD] [NJOBS]"
  exit 1
fi

ANALYZER=$1
DATASET=$2
DATASETGROUP=$3
SELECTION=$4
PERIOD=$5
NJOBS=$6

BLT="blt_projects"
SRCPATH="sourceFiles"
JOBPATH="${BLT}/${ANALYZER}/${DATASET}"

#MACHINE="fnal.gov"
#STORAGE="/eos/uscms/store/group/l1upgrades/L1MuonTrigger/"
#SRMSTORAGE="srm://cmseos.fnal.gov:8443/srm/v2/server?SFN=/eos/uscms/store/group/l1upgrades/L1MuonTrigger/"
MACHINE="ufl.edu"
STORAGE="/cms/data/store/user/jiafulow/L1MuonTrigger/"
SRMSTORAGE="srm://srm.ihepa.ufl.edu:8443/srm/v2/server?SFN=/cms/data/store/user/jiafulow/L1MuonTrigger/"
#MACHINE="uscms.org"
#STORAGE="/cms/data/store/user/jiafulow/L1MuonTrigger/"
#SRMSTORAGE="srm://srm.ihepa.ufl.edu:8443/srm/v2/server?SFN=/cms/data/store/user/jiafulow/L1MuonTrigger/"

echo "[INFO   ] Creating condor jobs ..."
echo "[INFO   ] arguments: ANALYZER=${ANALYZER} DATASET=${DATASET} DATASETGROUP=${DATASETGROUP} SELECTION=${SELECTION} PERIOD=${PERIOD} NJOBS=${NJOBS}"
echo "[INFO   ] settings : MACHINE=${MACHINE} STORAGE=${STORAGE}"


# ______________________________________________________________________________
# Step 1: prepare directories and files

# Ask for grid certificate
export X509_USER_PROXY=/tmp/x509up_u`id -u`
voms-proxy-info >/dev/null
if [ $? -ne 0 ]; then
  voms-proxy-init -voms cms -valid 192:00
fi

CRABPATH="crab_projects"
CRABSRCPATH="crabSourceFiles"
rm -rf ${CRABPATH}/crab_${DATASET}
#mkdir -p ${CRABPATH}/crab_${DATASET}


# ______________________________________________________________________________
# Step 2: prepare crab.py

cat > crab.py <<EOF
from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

config.General.requestName = '${DATASET}'
config.General.workArea = '${CRABPATH}'
config.General.transferOutputs = True
config.General.transferLogs = True

config.JobType.pluginName = 'Analysis'
#config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'pset.py'
config.JobType.outputFiles = ['histos.root']
#config.JobType.pyCfgParams = []
config.JobType.maxMemoryMB = 3500
#config.JobType.maxJobRuntimeMin = 1800

config.Data.inputDataset = '$(grep '^/' ${CRABSRCPATH}/${DATASET}.txt)'
config.Data.inputDBS = 'global'
#config.Data.outputPrimaryDataset = '${DATASET}'
config.Data.splitting = 'FileBased'
#config.Data.unitsPerJob = 1
njobs = ${NJOBS}
nfiles = $(grep '^/store.*\.root' ${SRCPATH}/${DATASET}.txt | wc -l)
unitsPerJob = (nfiles+njobs-1)/njobs
config.Data.unitsPerJob = unitsPerJob
#config.Data.splitting = 'EventAwareLumiBased'
#config.Data.unitsPerJob = 1000
#config.Data.splitting = 'EventBased'
#config.Data.unitsPerJob = 100000
#njobs = ${NJOBS}
#config.Data.totalUnits = config.Data.unitsPerJob * njobs
storage = '${STORAGE}/${CRABPATH}/${DATASET}'
storage = storage[storage.find('/store'):]
config.Data.outLFNDirBase = storage
config.Data.publication = False
config.Data.outputDatasetTag = 'CRAB3'

#config.Site.storageSite = 'T3_US_FNALLPC'
config.Site.storageSite = 'T2_US_Florida'
EOF


# ______________________________________________________________________________
# Step 3: crab submit

echo "[INFO   ] Submitting CRAB job(s) ..."

crab submit -c crab.py
mv crab.py crab.pyc ${CRABPATH}/crab_${DATASET}/

echo "[INFO   ] ${NJOBS} jobs are submitted."
