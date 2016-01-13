#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runList.txt)
	
	do
	echo -e "alien:///alice/cern.ch/user/p/ppillot/Sim/LHC15n/muTune2/results/$line/AnalysisResults.root" >> grid.esd.txt
	done < runList.txt

root -b -q CopyFromRemote.C 
	

scod