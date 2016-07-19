#!/bin/bash
vafctl start
nWorkers=88
let "nWorkers -= `pod-info -n`"
echo "requesting $nWorkers additional workers"
vafreq $nWorkers
vafwait 88
root -b -q 'runGenTuner.C("saf3","datasetSaf3.txt")'
vafctl stop
