#! /bin/bash

for line in $(cat runlist_15-07-2015-ESD.txt)
	do
	for i in `seq 1 4`
		do
		echo -e "alien:///alice/cern.ch/user/b/baudurie/Data/LHC15g/Eff/MonteCarlo/results/$line/00$i/AnalysisResults.root" >> grid.esd.txt
		done < runlist_15-07-2015-ESD.txt
	done

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root