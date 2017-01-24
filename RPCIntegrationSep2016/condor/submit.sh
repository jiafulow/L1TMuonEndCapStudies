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

rm -rf ${JOBPATH}/
mkdir -p ${JOBPATH}/
mkdir ${JOBPATH}/job/ ${JOBPATH}/res/
gfal-rm -r -v -t 600 ${SRMSTORAGE}/${JOBPATH}/ >/dev/null
gfal-mkdir -p ${SRMSTORAGE}/${JOBPATH}/
ln -s ${STORAGE}/${JOBPATH}/ ${JOBPATH}/out


# ______________________________________________________________________________
# Step 2: prepare blt.sh

cat > blt.sh <<EOF
#!/bin/bash

echo "Job submitted on host \`hostname\` on \`date\`"
echo "System \`uname -a\`"

JOBID=\$5
NJOBS=${NJOBS}
MACHINE=${MACHINE}
TARBALLPATH=${SRMSTORAGE}/${BLT}/
STORAGEPATH=${SRMSTORAGE}/${JOBPATH}/
export SCRAM_ARCH=${SCRAM_ARCH}
export CMSSW_VERSION=${CMSSW_VERSION}

echo ">>> arguments: \$@"
#echo ">>> environment variables:"; printenv

echo ">>> JOBID=\$JOBID"
echo ">>> NJOBS=\$NJOBS"
echo ">>> MACHINE=\$MACHINE"
echo ">>> SCRAM_ARCH=\$SCRAM_ARCH"
echo ">>> CMSSW_VERSION=\$CMSSW_VERSION"
echo ">>> TARBALLPATH=\$TARBALLPATH"
echo ">>> STORAGEPATH=\$STORAGEPATH"

echo ">>> X509_USER_PROXY=\$X509_USER_PROXY"
echo ">>> OSG_SITE_NAME=\$OSG_SITE_NAME"
echo ">>> OSG_HOSTNAME=\$OSG_HOSTNAME"
echo ">>> GLIDEIN_Gatekeeper=\$GLIDEIN_Gatekeeper"
echo ">>> GLIDEIN_CMSSite=\$GLIDEIN_CMSSite"
echo ">>> Dashboard_taskid=\$Dashboard_taskid Dashboard_monitorid=\$Dashboard_monitorid Dashboard_syncid=\$Dashboard_syncid"

if [ \$MACHINE == "ufl.edu" ] && [ ! -z \$OSG_WN_TMP ]; then
    REMOTE_INITIAL_DIR=\$(pwd)
    if [ \$REMOTE_INITIAL_DIR != \$OSG_WN_TMP ]; then
        cp -pr * \$OSG_WN_TMP
        cd \$OSG_WN_TMP
    fi
    echo "*** On ufl.edu machine, cd from \$REMOTE_INITIAL_DIR to \$OSG_WN_TMP"
fi

RUNTIME_AREA=\$(pwd)
echo ">>> RUNTIME_AREA=\$RUNTIME_AREA"

# Source cmsset_default.sh
if [ -f /cvmfs/cms.cern.ch/cmsset_default.sh ]; then
    export VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
    . \$VO_CMS_SW_DIR/cmsset_default.sh
    EXIT_STATUS=\$?
elif [ -f "\$VO_CMS_SW_DIR"/cmsset_default.sh ]; then
    . \$VO_CMS_SW_DIR/cmsset_default.sh
    EXIT_STATUS=\$?
elif [ -f "\$OSG_APP"/cmssoft/cms/cmsset_default.sh ]; then
    . \$OSG_APP/cmssoft/cms/cmsset_default.sh
    EXIT_STATUS=\$?
fi
if [ \$EXIT_STATUS -ne 0 ]; then echo "cmsset_default.sh exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

# Setup CMSSW environment
scramv1 project CMSSW \$CMSSW_VERSION 2>&1
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "scram project exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi
cd \$CMSSW_VERSION
eval \`scramv1 runtime -sh\` 2>&1
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "scram runtime exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

SOFTWARE_DIR=\$(pwd)
echo ">>> SOFTWARE_DIR=\$SOFTWARE_DIR"

# Copy using lcg-cp or gfal-copy?
if command -v gfal-copy >/dev/null 2>&1; then
    export BLTCP="gfal-copy -f -p -v -t 2400 -T 2400 "
    [[ -f /tmp/null ]] || touch /tmp/null
    gfal-copy -f file:////tmp/null file:////dev/null >/dev/null
    EXIT_STATUS=\$?
else
    EXIT_STATUS=1
fi
if [ \$EXIT_STATUS -ne 0 ]; then
    if command -v lcg-cp >/dev/null 2>&1; then
        export BLTCP="lcg-cp -v -b -D srmv2 --connect-timeout 2400 "
        [[ -f /tmp/null ]] || touch /tmp/null
        lcg-cp file:////tmp/null file:////dev/null >/dev/null
        EXIT_STATUS=\$?
    else
        EXIT_STATUS=1
    fi
fi
echo ">>> BLTCP=\$BLTCP"
if [ \$EXIT_STATUS -ne 0 ]; then echo "getting copy command exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

# Download and extract tarball
\$BLTCP \$TARBALLPATH/default.tgz file:///\$RUNTIME_AREA/default.tgz >/dev/null
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "transfer tarball exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi
tar xzf \$RUNTIME_AREA/default.tgz
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "unpack tarball exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

# Compile
scramv1 b -j4

# Return to working directory
cd \$RUNTIME_AREA
echo ">>> Listing RUNTIME_AREA"
ls -Al \$RUNTIME_AREA
echo ">>> Listing SOFTWARE_DIR"
ls -Al \$SOFTWARE_DIR

# Run the analyzer
#echo ">>> ${ANALYZER} \$@"
#${ANALYZER} \$@
echo ">>> cmsRun pset_imp.py"
cp \$SOFTWARE_DIR/pset.py \$SOFTWARE_DIR/pset_imp.py .
echo "tweak_pset(\$NJOBS,\$JOBID)" >> pset_imp.py
cmsRun pset_imp.py 2>&1
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "analyzer exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

echo ">>> Listing RUNTIME_AREA"
ls -Al \$RUNTIME_AREA

# Transfer output file
ROOTFILE=\$(ls *.root)
\$BLTCP file:///\$RUNTIME_AREA/"\$ROOTFILE" \$STORAGEPATH/"\$ROOTFILE" >/dev/null
EXIT_STATUS=\$?
if [ \$EXIT_STATUS -ne 0 ]; then echo "transfer output exited with status=\$EXIT_STATUS"; exit \$EXIT_STATUS; fi

# Done
rm input.txt default.tgz *.root pset.py pset_imp.py
echo "Exit status=\$EXIT_STATUS"
echo "Job finished on host \`hostname\` on \`date\`"
EOF


# ______________________________________________________________________________
# Step 3: prepare blt.jdl

# Print some warnings
if [ ${MACHINE} == "ufl.edu" ]; then
    echo -e "[WARNING] \033[93mMake sure to uncomment 'Universe = grid' and 'grid_resource = ...', comment out 'Universe = vanilla'.\033[0m"
elif [ ${MACHINE} == "uscms.org" ]; then
    echo -e "[WARNING] \033[93mMake sure to check whether the modified version of 'condor_submit' is still valid.\033[0m"
fi

cat > blt.jdl <<EOF
Universe                = vanilla
#Universe                = grid
#grid_resource           = condor cms.rc.ufl.edu cms.rc.ufl.edu:9619
#grid_resource           = condor red-gw1.unl.edu red-gw1.unl.edu:9619
Notification            = Never
Executable              = job/blt.sh
Transfer_Input_Files    = job/input.txt
Output                  = res/job_\$(Cluster)_\$(Process).stdout
Error                   = res/job_\$(Cluster)_\$(Process).stderr
Log                     = res/job_\$(Cluster)_\$(Process).out
Requirements            = (OpSys == "LINUX") && (Arch != "DUMMY")
request_disk            = 1000000
request_memory          = 2500
use_x509userproxy       = TRUE
x509userproxy           = \$ENV(X509_USER_PROXY)
should_transfer_files   = YES
when_to_transfer_output = ON_EXIT
#on_exit_remove          = (ExitBySignal == False) && (ExitCode == 0 || NumJobStarts>3)
EOF

for JOBID in `seq 1 ${NJOBS}`; do
cat >> blt.jdl <<EOF
Arguments               = ${DATASET} ${DATASETGROUP} ${SELECTION} ${PERIOD} ${JOBID}
Queue 1
EOF
done

# ______________________________________________________________________________
# Step 4: upload default.tgz, make input.txt

echo "[INFO   ] Uploading tarball ..."

if [ ! -f default.tgz ]; then
  echo -e "[ERROR  ] \033[91mCannot find tarball: default.tgz\033[0m"
  exit 1
fi

gfal-copy -f -p -v -t 2400 -T 2400 file:///$PWD/default.tgz ${SRMSTORAGE}/${BLT}/default.tgz >/dev/null

if [ ! -f ${SRCPATH}/${DATASET}.txt ]; then
  echo -e "[ERROR  ] \033[91mCannot find source file: ${SRCPATH}/${DATASET}.txt\033[0m"
  exit 1
fi

cp ${SRCPATH}/${DATASET}.txt input.txt

# ______________________________________________________________________________
# Step 5: condor_submit

echo "[INFO   ] Submitting job(s) ..."

mv input.txt blt.sh blt.jdl ${JOBPATH}/job/

if [ ${MACHINE} == "uscms.org" ]; then
  cd ${JOBPATH}/ && ~/condor_submit job/blt.jdl
else
  cd ${JOBPATH}/ && condor_submit job/blt.jdl
fi

echo "[INFO   ] ${NJOBS} jobs are submitted."
