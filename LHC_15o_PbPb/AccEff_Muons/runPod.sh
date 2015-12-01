#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b <<EOF
.x runGenTuner.C("full","saf","Find;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/simsinglemuon/pp/CMUL7-B-NOPF-MUON/228936/*;FileName=AliAOD.Muons.root;Mode=cache;","MC")
.q
EOF
vafctl stop
exit
