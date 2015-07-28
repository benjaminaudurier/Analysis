#! /bin/bash

for line in $(cat runlist.txt)
do
echo -e "Find;FileName=AliAOD.Muons.root;BasePath=/alice/data/2011/LHC11h/000$line/ESDs/pass2_muon/AOD119" >> datasetfull.txt
done < runlist.txt