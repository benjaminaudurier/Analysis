#!/bin/bash
vafctl start
nWorkers=88
let "nWorkers -= `pod-info -n`"
echo "requesting $nWorkers additional workers"
vafreq $nWorkers
vafwait 88
root -b -q 'runGenTuner.C("saf3","Find;FileName=AliAOD.Muons.root;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tune3/CMUL7-B-NOPF-MUFAST/results/*"")'
vafctl stop
