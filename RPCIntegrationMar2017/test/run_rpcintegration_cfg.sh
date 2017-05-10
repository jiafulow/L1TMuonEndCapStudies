#!/bin/bash

run() {
cp rpcintegration_cfg.py pippo_rpcintegration_cfg.py
python modify_pset.py pippo_rpcintegration_cfg.py l1NtupleMC_RAW2DIGI.${pippo}.root  histos.${pippo}.root  -1
cmsRun pippo_rpcintegration_cfg.py >/dev/null 2>&1
#python drawer.py --infile histos.${pippo}.root  --outdir figures_${pippo}
}

pippi="csc rpc gem"
for pippo in ${pippi}; do
  echo "running ${pippo} ..."
  run;
done
