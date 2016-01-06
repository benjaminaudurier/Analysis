#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b -q 'runMuMu.C("full","saf","/Users/audurier/Documents/Analysis/Dataset/datasetfull_saf_LHC15o_AOD.txt")'
vafctl stop
exit
