#!/bin/sh

# usage:
# $1 = mode
# $2 = dataset

if [ $# != 2 ]; then
    echo "usage: \$1=\"local\" or \"saf3\"; \$2=dataset"
    exit 1
fi

runonsaf3="no";

if [ "$1" = "saf3" ] && [ `hostname` = "nansafmaster3.in2p3.fr" ]; then

    vafctl start
    nWorkers=88
    let "nWorkers -= `pod-info -n`"
    echo "requesting $nWorkers additional workers"
    vafreq $nWorkers
    vafwait 88
    runonsaf3="yes"

fi

for (( i=0 ; i < 16 ; i++ )); do

    fileName="AnalysisResults_Corr_map_${i}.root"

    echo "processing AnalysisResults_Corr_map_$i ..."

    if [ -e $fileName ]; then
        continue
    fi

    if [ "$runonsaf3" = "yes" ]; then
        root -q -b runMuMuFacilities.C\(\"$1\",\"$2\",kFALSE,$i\) 2>&1 | tee run_${i}.log
    else
        root -q -l runMuMuFacilities.C\(\"$1\",\"$2\",kFALSE,$i\) 2>&1 | tee run_${i}.log
    fi

    mv AnalysisResults.root $fileName

done

if [ "$runonsaf3" = "yes" ]; then

    vafctl stop

fi