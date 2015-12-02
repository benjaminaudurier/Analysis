#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runList.txt)
	
	do
	echo -e "alien:///alice/data/2015/LHC15n/000$line/muon_calo_pass1/PWGDQ/DQ_Dimuons_pp_ESD/1_20151130-1155/AnalysisResults.root" >> grid.esd.txt
	done < runList.txt

root -b -q CopyFromRemote.C 
	

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root