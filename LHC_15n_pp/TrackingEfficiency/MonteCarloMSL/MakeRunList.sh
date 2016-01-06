#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runList.txt)
	
	do
	echo -e "alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15n/TrackingEfficiency/MonteCarlo/singleMuon/tuned/WiththnSparse/results/$line/AnalysisResults.root" >> grid.esd.txt
	done < runList.txt

root -b -q CopyFromRemote.C 
	

scod