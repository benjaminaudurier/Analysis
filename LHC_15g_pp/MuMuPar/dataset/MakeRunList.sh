#! /bin/bash

for line in $(cat runlist_15-07-2015.txt)
do
echo -e "Find;FileName=AliAOD.Muons.root;BasePath=/alice/data/2015/LHC15g/000$line/ESDs/pass2_muon/AOD119" >> datasetfull.txt
done < runlist_15-07-2015.txt