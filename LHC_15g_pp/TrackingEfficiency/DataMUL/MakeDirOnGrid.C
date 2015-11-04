
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
  		
        // gSystem->Exec(Form("alien_mkdir /alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/ResultatPhilippe/Data/results/000%s",line));
        TFile::Cp(Form("/Users/audurier/Documents/Analysis/LHC_15g_pp/mergeQA/alice/cern.ch/user/p/ppillot/Data/LHC15g/muon_calo_pass1/Eff/results/000%s/AnalysisResults.root",line),Form("alien:///alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/ResultatPhilippe/Data/results/000%s/AnalysisResults.root",line));

  	}
}

