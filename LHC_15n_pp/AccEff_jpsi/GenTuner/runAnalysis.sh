#!/bin/bash
vafctl start
nWorkers=88
let "nWorkers -= `pod-info -n`"
echo "requesting $nWorkers additional workers"
vafreq $nWorkers
vafwait 88
root -b -q 'runGenTunerLoop.C("saf3","datasetSaf3.txt",3)'
vafctl stop
