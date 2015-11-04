#! /bin/bash

rm grid.esd.txt

rm -dr alice

for line in $(cat runList.txt)
	
do
echo -e "alien:///alice/cern.ch/user/p/ppillot/Sim/LHC15g/muTuneCMUU7/GenTuner/results/$line/AnalysisResults.root" >> grid.esd.txt
done < runList.txt
	
root -b -q CopyFromRemote.C 

#alien:///alice/cern.ch/user/j/jmartinb/Efficiency/EffData/Pass1/TrackingEff64runLHC13f/Output/000196433/AnalysisResults.root