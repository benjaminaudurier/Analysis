#!/bin/bash

condorpid=$1

for archive in $(ls /tmp/pod-log-baudurie/$condorpid/proof_log.*.tgz)
do
	tar -zvxf $archive --wildcards --no-anchored '*.log'
done

for log in $(find var -name *.log)
do
	cp $log .
done

rm -rf var
