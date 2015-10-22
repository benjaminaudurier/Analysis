#! /bin/bash

rm grid.QA.txt

# rm -dr alice

for line in $(cat runList.txt)
	
do
echo -e "alien:///alice/cern.ch/user/p/ppillot/Data/LHC15g/muon_calo_pass1/QA/results/000$line/Merged.QA.Data.root" >> grid.QA.txt
done < runlist_15-07-2015-ESD_main.txt
	
root -b -q CopyFromRemote.C 

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root