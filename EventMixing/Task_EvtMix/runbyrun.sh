#!/bin/bash

source /Users/lardeux/.bash_profile


export Study=SL
export Out=Outputs/$Study/TaskMixingRunByRun

mkdir Outputs/$Study
mkdir Outputs/$Study/TaskMixingRunByRun


# Task Mixing
export Filename=ListDataSet_134runs_AOD119_runbyrun.txt

for ds in $(cat $Filename | xargs);
do
  root -l << EOF
  gEnv->SetValue("XSec.GSI.DelegProxy", "2")
  .x RunALICE.C("proof","full","aod","",1e10,0,"AddAMEventMixingTest","$ALICE_ROOT","",kTRUE,"$ds")
  cin>>k
  .q
EOF
  mv Output.root $Out/MixOutput${ds:(-6)}.root
  mv MixInfo.root $Out/MixInfo${ds:(-6)}.root
  echo MixOutput${ds:(-6)}.root >> RunOutputAll.txt
done


# Merge outputs
cp mergeGridFiles.C $Out
cp RunOutputAll.txt $Out

cd $Out

aliroot -l << EOF
  .x mergeGridFiles.C("../MixOutput_MergeNoNorme.root", "RunOutputAll.txt", kTRUE)
  .q
EOF


echo ""
echo "######################################################"
echo "#                 Script is done !                   #"
echo "######################################################"