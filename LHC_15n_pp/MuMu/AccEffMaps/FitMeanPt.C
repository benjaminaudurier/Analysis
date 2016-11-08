//
//  FitMeanPt.C
//
//
//  Created by Benjamin Audurier on 05/11/16.
//
//

// Macro to fit MeanPt in loop

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>

#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <AliAnalysisMuMuBinning.h>


// --- Config for the mumu object ---
TString sasso     ="/Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/Simulation/full/AnalysisResults.JPSI.root";
TString sasso2    ="";
TString config    = "mumu.pp2015.meanpt.config";

// --- Directory where the results are
TString dir       = "/Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/AccEffMaps/results";


//_____________________________________________________________________________
void FitMeanPt( const char* bintype = "integrated", const char* flavour = "", const char* histoType = "minv", int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    // Check directories
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(dir.Data()))) {
        printf("Error : Dir '%s' doesn't exists !!!",dir.Data());
        return ;
    }

    // Get what's inside
    TString file  = gSystem->GetFromPipe(Form("ls %s",dir.Data()));
    TObjArray* files  = file.Tokenize("\n");

    TIter nextfiles(files);
    TObjString* sfile;
    nextfiles.Reset();

    int i=0;
    

    // The loop where we fit
    while ( ( sfile = static_cast<TObjString*>(nextfiles()) ) ) {
        printf("\n");
        printf("\n ----> Looking for results in %s/%s\n", dir.Data(),sfile->String().Data() );

        // main object
        AliAnalysisMuMu analysis(Form("%s/%s",dir.Data(),sfile->String().Data()),sasso.Data(),sasso2.Data(),config.Data());
	    if(!analysis.OC()) continue;
        analysis.FitJpsi(bintype,flavour,"",histoType);

        // Clean
        // analysis.CleanAllSpectra();
    }
}













