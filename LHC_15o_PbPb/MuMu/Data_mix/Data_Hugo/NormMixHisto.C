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
TString beamYear  ="mumu.PbPb2015.mix.config";
TString dir       = "/Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Data_mix/Data_Hugo/results";


//_____________________________________________________________________________
void NormMixHisto( const char* bintype = "pt", int debug =0)
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

    AliAnalysisMuMu* analysis(0x0);

    // loop over all directories
    while ( ( sdirs = static_cast<TObjString*>(nextdirs()) ) ) {
        printf("\n");
        printf("\n ----> Looking for results in %s/%s\n", dir.Data(),sdirs->String().Data() );

        if ( gSystem->AccessPathName(gSystem->ExpandPathName(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()))) ) {
            printf("\nError : file '%s/%s/%s' doesn't exists !\n",dir.Data(),sdirs->String().Data(),sfile.Data());
            continue ;
        }

        // main object
        analysis = new AliAnalysisMuMu(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),beamYear.Data());
        if ( !analysis ) {
            printf("Cannot create AliAnalysisMuMu object\n");
            continue ;
        }

        // Clean
        analysis->CleanAllSpectra();

        //_____ Fit
        nextbintype.Reset();
        while ( ( sbintype = static_cast<TObjString*>(nextbintype()) ) ){
            if(sbintype->String().Contains("integrated")){
                analysis->NormMixedMinv(sbintype->String().Data(),"psi","");
            } else {
                analysis->NormMixedMinv(sbintype->String().Data(),"psi","BENJ");
            }
        }
    }

    // Here we merge all restults and fit

    // Delete directory if here
    if ( !gSystem->AccessPathName(Form("%s/mergeResults",dir.Data())) ) gSystem->Exec(Form("rm -rf %s/mergeResults",dir.Data()));
    gSystem->Exec(Form("mkdir %s/mergeResults",dir.Data()));

    AliMergeableCollection* oc      = 0x0;
    AliCounterCollection*   cc      = 0x0;
    AliAnalysisMuMuBinning* binning = 0x0;

    TObjArray* ocCollection = new TObjArray();
    TObjArray* ccCollection = new TObjArray();

    nextdirs.Reset();
    Int_t i = 0;

    // loop over all directories
    while ( ( sdirs = static_cast<TObjString*>(nextdirs()) ) ) {

        printf("Looking for files in %s/%s/%s \n",dir.Data(),sdirs->String().Data(),sfile.Data());
        analysis = new AliAnalysisMuMu(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),beamYear.Data());
        if ( !analysis ) {
            printf("\nCannot create AliAnalysisMuMu object\n");
            continue ;
        }

        // Fill the collections
        if ( analysis->OC()  && i !=0 )  ocCollection->Add(analysis->OC()->Clone());
        else if ( analysis->OC() && i==0 ) oc =static_cast<AliMergeableCollection*>(analysis->OC()->Clone());

        if ( analysis->CC()  && i !=0 )  ccCollection->Add(analysis->CC()->Clone());
        else if ( analysis->CC() && i==0 ) cc =static_cast<AliCounterCollection*>(analysis->CC()->Clone());

        // Assuming same binning for all the files
        if ( analysis->BIN() && i ==0 ) binning =static_cast<AliAnalysisMuMuBinning*>(analysis->BIN()->Clone());

        i++;
    }


    if( oc && ocCollection ) printf("merge %lld files in oc \n",oc->Merge(ocCollection));
    else return;
    // oc->Print();
    oc->SetName("OC");

    if( cc && ccCollection ) printf("merge %lld files in cc \n",cc->Merge(ccCollection));
    else return;
    // cc->Print();
    cc->SetName("CC");

    TFile f(Form("%s/mergeResults/AnalysisResults.root",dir.Data()),"recreate");
    if(oc)      oc->Write();
    if(cc)      cc->Write();
    if(binning) binning->Write();

    f.Close();
}













