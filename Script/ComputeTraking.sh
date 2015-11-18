#! /bin/bash
##
##Script to compute Efficiency at the Data and MC level and do the comparison

echo 
echo "What trigger do you use (MUL or MSL)?"
read trigg

case  "$trigg" in
	MSL)
		echo "trigger=$trigg , using my dataset"
		export datapath="/Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/DataMSL/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/Data/results"
		export mcpath="/Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/MonteCarloMSL/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/MonteCarlo/singleMuon/results"
		;; 
	MUL)
		echo "trigger=$trigg , using philippe dataset"
		export datapath="/Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/DataMUL/alice/cern.ch/user/p/ppillot/Data/LHC15g/muon_calo_pass1/Eff/results"
		export mcpath="/Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/MonteCarloMUL/alice/cern.ch/user/p/ppillot/Sim/LHC15g/muTuneCMUU7/GenTuner/results"
		;; 
	*) 
		echo "trigger not defined"
		exit 1
		;;
esac


echo "What ptMin for files ?"
read ptMin1
echo "ptMin = $ptMin1" 

#Efficiency for Data
echo 
echo "Compute Data Efficiency? (y/n)"
read computedata
if [ "$computedata" = "y" ]  ; then
	echo "Computing efficieny for data ..."
	echo
	cd /Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/Data${trigg}
	pwd
	# grep 'Double_t ptMin = 0.' MuonTrackingEfficiency.C | sed -n "s/Double_t ptMin = 0./Double_t ptMin = ${ptMin1}/p"
	root -b -q 'MuonTrackingEfficiency.C++("runList.txt","weightfileCMUL.txt","AnalysisResults.root","efficiency_new.root","'${datapath}'")'
	# grep 'Double_t ptMin = ${ptMin1}' MuonTrackingEfficiency.C | sed -n "s/ptMin = ${ptMin1}/ptMin = 0./"
	cd —
fi

#Efficiency for MC
echo 
echo "Compute Mc Efficiency? (y/n)"
read computeMc
if [ "$computeMc" = "y" ] ; then
	echo "Computing efficieny for MonteCarlo ..."
	echo
	cd /Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/MonteCarlo${trigg}
	# grep 'Double_t ptMin = 0.' MuonTrackingEfficiency.C | sed -n "s/Double_t ptMin = 0./Double_t ptMin = ${ptMin1}/p"
	root -b -q  'MuonTrackingEfficiency.C++("runList.txt","weightfileCMUL.txt","AnalysisResults.root","efficiency_new.root","'${mcpath}'")'
	cd —
fi


#Eff comparison
echo 
echo "Compute Data/Mc? (y/n)"
read DataOverMc
if [ "$DataOverMc" = "y" ] ; then
	echo "Computing the ratio ..."
	echo
	cd /Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency
	root -b -q 'ComparisonDataMC.C++("Data'${trigg}'/efficiency_new.root","MonteCarlo'${trigg}'/efficiency_new.root")'
	cp EffComparison.root Resultats/EffComparison${trigg}Pt=4.root
	echo
	cd -
fi


exit 0