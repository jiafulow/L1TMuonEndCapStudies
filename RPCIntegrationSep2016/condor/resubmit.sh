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

echo "[INFO   ] Checking condor outputs ..."
echo "[INFO   ] arguments: ANALYZER=${ANALYZER} DATASET=${DATASET} DATASETGROUP=${DATASETGROUP} SELECTION=${SELECTION} PERIOD=${PERIOD} NJOBS=${NJOBS}"
echo "[INFO   ] settings : MACHINE=${MACHINE} STORAGE=${STORAGE}"


# ______________________________________________________________________________
# Step 1: make a copy of blt.sh -> blt_resub.sh

# Ask for grid certificate
export X509_USER_PROXY=/tmp/x509up_u`id -u`
voms-proxy-info >/dev/null
if [ $? -ne 0 ]; then
  voms-proxy-init -voms cms -valid 192:00
fi

cp ${JOBPATH}/job/blt.jdl blt_resub.jdl

sed -i '/\(Arguments\|Queue\)/d' blt_resub.jdl
NJOBS_resub=0
for JOBID in `seq 1 ${NJOBS}`; do
if ! ls ${JOBPATH}/out/*_${JOBID}.root 1> /dev/null 2>&1; then
#echo "Output from job ${JOBID} does not exists."
cat >> blt_resub.jdl <<EOF
Arguments               = ${DATASET} ${DATASETGROUP} ${SELECTION} ${PERIOD} ${JOBID}
Queue 1
EOF
let NJOBS_resub+=1
fi
done

# ______________________________________________________________________________
# Step 2: condor_submit

echo "[INFO   ] Resubmitting job(s) ..."

mv blt_resub.jdl ${JOBPATH}/job/

if [ ${MACHINE} == "uscms.org" ]; then
  cd ${JOBPATH}/ && ~/condor_submit job/blt_resub.jdl
else
  cd ${JOBPATH}/ && condor_submit job/blt_resub.jdl
fi

echo "[INFO   ] ${NJOBS_resub} jobs are submitted."
