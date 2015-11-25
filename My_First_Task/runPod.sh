#!/bin/bash
nWorkers=${1-88}
vafctl start
vafreq $nWorkers
vafwait $nWorkers
export TASKDIR="$HOME/taskDir"
cd $TASKDIR
root -b <<EOF
.x runLocal.C("full","saf")
.q
EOF
vafctl stop
exit
