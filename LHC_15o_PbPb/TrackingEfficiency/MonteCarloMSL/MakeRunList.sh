#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runlist.txt)
	
	do
	echo -e "alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15o/TrackingEfficiency/MonteCarlo/singleMuon/tuned/WiththnSparse/results/$line/AnalysisResults.root" >> grid.esd.txt
	done < runlist.txt

root -b -q CopyFromRemote.C 
	