#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runList.txt)
	
	do
	echo -e "alien:///alice/data/2015/LHC15o/000$line/muon_calo_pass1/PWGDQ/DQ_Dimuons_PbPb_ESD/3_20160112-2134/AnalysisResults.root" >> grid.esd.txt
	done < runlist.txt

root -b -q CopyFromRemote.C 
	

scod