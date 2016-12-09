#!/bin/bash

cp RPCInt_sep.py pippo_RPCInt_sep.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI_sep.orig.root'"  >> pippo_RPCInt_sep.py
cmsRun pippo_RPCInt_sep.py >/dev/null 2>&1

cp RPCInt_sep.py pippo_RPCInt_sep.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI_sep.coord.root'" >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.CoordLUTDir    = 'ph_lut_v2'"       >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.FixME11Edges   = True"              >> pippo_RPCInt_sep.py
cmsRun pippo_RPCInt_sep.py >/dev/null 2>&1

cp RPCInt_sep.py pippo_RPCInt_sep.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI_sep.zone.root'"  >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.ZoneBoundaries = [0,36,54,96,127]"  >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.UseNewZones    = True"              >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.CoordLUTDir    = 'ph_lut_v2'"       >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.FixME11Edges   = True"              >> pippo_RPCInt_sep.py
cmsRun pippo_RPCInt_sep.py >/dev/null 2>&1

cp RPCInt_sep.py pippo_RPCInt_sep.py
echo "process.RAWSIMoutput.fileName = 'l1NtupleMC_RAW2DIGI_sep.rpc.root'"   >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.RPCEnable                   = True"              >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.ZoneBoundaries = [0,36,54,96,127]"  >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.UseNewZones    = True"              >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.CoordLUTDir    = 'ph_lut_v2'"       >> pippo_RPCInt_sep.py
echo "process.simEmtfDigis.spPCParams16.FixME11Edges   = True"              >> pippo_RPCInt_sep.py
cmsRun pippo_RPCInt_sep.py >/dev/null 2>&1

