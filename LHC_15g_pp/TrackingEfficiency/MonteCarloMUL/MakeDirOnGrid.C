
#include "Riostream.h"
#include "TUrl.h"
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TGrid.h"


void MakeDirOnGrid(const char* txtfile="runList.txt")
{
	char line[1024];
  	ifstream in(gSystem->ExpandPathName(txtfile));
  	TGrid::Connect("alien://");
    if (!gGrid) 
    {
      cout << "Cannot get alien connection" << endl;
      return;          
    }

  	while ( in.getline(line,1024,'\n') )
  	{
  		
        gSystem->Exec(Form("alien_mkdir /alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/ResultatPhilippe/MonteCarlo/results/%s",line));
        TFile::Cp(Form("/Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency/MonteCarloMUL/alice/cern.ch/user/p/ppillot/Sim/LHC15g/muTuneCMUU7/GenTuner/results/%s/AnalysisResults.root",line),Form("alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/ResultatPhilippe/MonteCarlo/results/%s/AnalysisResults.root",line));

  	}
}

