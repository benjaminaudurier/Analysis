#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b <<EOF
.x runMuMu.C("full","saf","$DATASETDIR/datasetfull_saf_LHC15g_AOD.txt")
.q
EOF
vafctl stop
exit
