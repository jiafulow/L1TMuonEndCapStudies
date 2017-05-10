#!/bin/bash

cp RPCInt.py pippo_RPCInt.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI.csc.root'" >> pippo_RPCInt.py
echo "process.simEmtfDigis.RPCEnable = False"                         >> pippo_RPCInt.py
echo "process.simEmtfDigis.GEMEnable = False"                         >> pippo_RPCInt.py
cmsRun pippo_RPCInt.py >/dev/null 2>&1

cp RPCInt.py pippo_RPCInt.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI.rpc.root'" >> pippo_RPCInt.py
echo "process.simEmtfDigis.RPCEnable = True"                          >> pippo_RPCInt.py
echo "process.simEmtfDigis.GEMEnable = False"                         >> pippo_RPCInt.py
cmsRun pippo_RPCInt.py >/dev/null 2>&1

cp RPCInt.py pippo_RPCInt.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI.gem.root'" >> pippo_RPCInt.py
echo "process.simEmtfDigis.RPCEnable = True"                          >> pippo_RPCInt.py
echo "process.simEmtfDigis.GEMEnable = True"                          >> pippo_RPCInt.py
cmsRun pippo_RPCInt.py >/dev/null 2>&1

