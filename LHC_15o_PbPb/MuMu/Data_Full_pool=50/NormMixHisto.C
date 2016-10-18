//
//  Fit.c
//
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro to fit Minv Spectra and draw J/psi distribution vs pt and y

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

TString sfile     ="AnalysisResults.root";
TString sasso     ="";
TString sasso2    ="";
TString config    = "mumu.PbPb2015.full.config";
TString dir       = "/Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Data_Full_pool=50/results";

Bool_t acceffcorr = kFALSE;
Double_t Mmin = 2.0;
Double_t Mmax = 5.0;

//_____________________________________________________________________________
void NormMixHisto( const char* bintype = "pt,y",const char* flavour="BENJ", int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    //General conf.
    TObjArray* bintypeArray= TString(bintype).Tokenize(",");
    TIter nextbintype(bintypeArray);
    TObjString* sbintype;

    //Check directories
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(dir.Data()))) {
        printf("Error : Dir '%s' doesn't exists !!!",dir.Data());
        return ;
    }

    // Get what's inside
    TString dirsStr  = gSystem->GetFromPipe(Form("ls %s",dir.Data()));
    TObjArray* dirs  = dirsStr.Tokenize("\n");

    TIter nextdirs(dirs);
    TObjString* sdirs;
    nextdirs.Reset();

    int i=0;
    // loop over all directories
    while ( ( sdirs = static_cast<TObjString*>(nextdirs()) ) ) {
        printf("\n");
        printf("\n ----> Looking for results in %s/%s\n", dir.Data(),sdirs->String().Data() );
         ++i;
         if ( i > dirs->GetEntries()/2 ) continue;
         // if ( i <= 50) continue;

        if ( gSystem->AccessPathName(gSystem->ExpandPathName(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()))) ) {
            printf("\n Error : file '%s/%s/%s' doesn't exists !\n",dir.Data(),sdirs->String().Data(),sfile.Data());
            continue ;
        }

        // main object
        AliAnalysisMuMu analysis(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),config.Data());
	    if(!analysis.OC()) continue;

        // Clean
        analysis.CleanAllSpectra();

        //_____ Fit
        nextbintype.Reset();
        while ( ( sbintype = static_cast<TObjString*>(nextbintype()) ) ){
            if(sbintype->String().Contains("integrated")){
                analysis.NormMixedMinv("integrated","psi","",acceffcorr,Mmin,Mmax);
            } else {
                analysis.NormMixedMinv(sbintype->String().Data(),"psi",flavour);
            }
        }

    }
}


