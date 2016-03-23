#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b -q 'runMuMu.C("test","saf","$DATASETDIR/datasetfull_saf_LHC15n_AOD_data.txt")'
vafctl stop
exit
