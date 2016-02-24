#! /bin/bash

rm grid.QA.txt

# rm -dr alice

for line in $(cat run-list.txt)
	
do
echo -e "alien:///alice/data/2015/LHC15n/000$line/muon_calo_pass1/QAresults.root" >> grid.QA.txt
done < run-list.txt
	
root -b -q CopyFromRemote.C 

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root
##alien:///alice/data/2015/LHC15n/000244340/QAresults.root