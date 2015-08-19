#! /bin/bash

for line in $(cat runlist_15-07-2015-ESD.txt)
	
	do
	echo -e "alien:///alice/cern.ch/user/b/baudurie/Data/LHC15g/Eff/Data/results/000$line/AnalysisResults.root" >> grid.esd.txt
	done < runlist_15-07-2015-ESD.txt
	

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root