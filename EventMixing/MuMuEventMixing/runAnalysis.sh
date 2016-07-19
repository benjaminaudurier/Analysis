#!/bin/bash
vafctl start
nWorkers=1
let "nWorkers -= `pod-info -n`"
echo "requesting $nWorkers additional workers"
vafreq $nWorkers
vafwait 1
root -b -q 'runMuMuFacilities.C("saf3","Find;BasePath=/alice/data/2015/LHC15o/000244918/muon_calo_pass1/AOD175/*;FileName=AliAOD.Muons.root;Mode=cache;")'
vafctl stop
