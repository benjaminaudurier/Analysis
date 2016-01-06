#! /bin/bash

for line in `ls Find*.root`; do

   time root -b -q runMuMu.C\(\"full\",\"saf\",\"$line\"\) 2>&1 | tee `basename $line.log`
   done