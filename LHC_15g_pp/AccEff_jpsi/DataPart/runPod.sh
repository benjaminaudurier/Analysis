#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b <<EOF
.x runMuMu.C("full","saf","$DATASETDIR/datasetlight_saf_AOD_lhc15g.txt"")
.q
EOF
vafctl stop
exit
