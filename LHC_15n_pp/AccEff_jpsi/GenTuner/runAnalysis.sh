#!/bin/bash
vafctl start
nWorkers=88
let "nWorkers -= `pod-info -n`"
echo "requesting $nWorkers additional workers"
vafreq $nWorkers
vafwait 88
root -b  'runGenTuner.C ("saf3","Find;BasePath=/alice/cern.ch/user/j/jcastill/pp15wrk/JpsiMC_LHC15n/resultsJpsiFamily/244340/*;FileName=AliAOD.Muons.root;Mode=cache;") '
vafctl stop
