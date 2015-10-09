#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runlist_15-07-2015-ESD.txt)
	
	do
	echo -e "alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/Data/new/results/000$line/AnalysisResults.root" >> grid.esd.txt
	done < runlist_15-07-2015-ESD.txt

root -b -q CopyFromRemote.C 
	

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root