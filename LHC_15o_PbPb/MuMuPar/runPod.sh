#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b -q 'runMuMu.C("full","saf","$DATASETDIR/AOD.ds.fc.137.AliAOD.root")'
vafctl stop
exit
