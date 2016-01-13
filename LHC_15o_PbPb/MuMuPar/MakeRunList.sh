#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runListTrain_LHC15o_AOD.txt)
	
	do
	echo -e "alien:///alice/data/2015/LHC15o/000$line/muon_calo_pass1/PWGDQ/DQ_Dimuons_PbPb/21_20160108-2212/AnalysisResults.root" >> grid.esd.txt
	done < runListTrain_LHC15o_AOD.txt

root -b -q CopyFromRemote.C 
	
