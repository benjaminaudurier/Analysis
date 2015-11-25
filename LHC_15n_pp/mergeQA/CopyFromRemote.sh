#! /bin/bash

rm grid.QA.txt

# rm -dr alice

for line in $(cat grid.QA.txt)
	
do
echo -e "alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/singleMuon/CMSL7-B-NOPF-MUFAST/QA/$line/Merged.QA.Data.root" >> grid.QA.txt
done < grid.QA.txt
	
root -b -q CopyFromRemote.C 

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root