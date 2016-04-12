#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b 'runMuMu.C("proof","saf","ds.fc.137.AliAOD.AOD.root")'
vafctl stop
exit
