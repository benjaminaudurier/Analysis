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
TString beamYear  ="mumu.PbPb2015.config";
TString dir       = "/Users/audurier/Documents/Analysis/EventMixing/MuMuEventMixingOfficial/results";


//_____________________________________________________________________________
void FitMacroLoop( const char* bintype = "pt", const char* option = "", int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);
    AliAnalysisMuMu* analysis =0x0;

    // Set all the fit options
    Bool_t rawcount     = kFALSE;
    Bool_t clean        = kFALSE;
    Bool_t print        = kFALSE;
    Bool_t mergeResults = kFALSE;
    TString fitMethod;
    
    if(TString(option).Contains("rawcount"))    rawcount        =kTRUE;
    if(TString(option).Contains("clean"))       clean           =kTRUE;
    if(TString(option).Contains("print"))       print           =kTRUE;
    if(TString(option).Contains("merge"))      { mergeResults    =kTRUE; clean           =kTRUE;} // Spectra are not mergeable
    if(TString(option).Contains("addmix"))      fitMethod ="addmix" ;
    if(TString(option).Contains("mixonly"))     fitMethod ="mixonly" ;

    //General conf.
    TObjArray* bintypeArray= TString(bintype).Tokenize(",");
    TIter nextbintype(bintypeArray);
    TObjString* sbintype;
    

    //Check directories
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(dir.Data()))) {
        printf("Error [CreateListOfManagersFromDir] : Dir '%s' doesn't exists !!!",dir.Data());
        return ;
    }
    
    // Get what's inside
    TString dirsStr = gSystem->GetFromPipe(Form("ls %s",dir.Data()));
    TObjArray* dirs  = dirsStr.Tokenize("\n");
        
    TIter nextdirs(dirs);
    TObjString* sdirs;

    // loop over all directories
    while ( ( sdirs = static_cast<TObjString*>(nextdirs()) ) ) {
        
        // main object
        analysis = new AliAnalysisMuMu(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),beamYear.Data());
        if ( !analysis ) {
            printf("Cannot create AliAnalysisMuMu object");
            continue ;
        }

        // Clean   
        if(clean) analysis->CleanAllSpectra();

        //_____ Fit 
        nextbintype.Reset();
        while ( ( sbintype = static_cast<TObjString*>(nextbintype()) ) ){
            if(sbintype->String().Contains("integrated")){

                if( !fitMethod.IsNull() ) analysis->NormarliseMixedMinv(sbintype->String().Data(),"psi","");
                if( !mergeResults) analysis->FitJpsi(sbintype->String().Data(),"","",fitMethod.Data());
            } else {
                if( !fitMethod.IsNull() ) analysis->NormarliseMixedMinv(sbintype->String().Data(),"psi","BENJ");
                if( !mergeResults) analysis->FitJpsi(sbintype->String().Data(),"BENJ","",fitMethod.Data());
            }
        }

        // Print the number of Jpsi exctracted
        if(print && !mergeResults && strcmp(bintype,"pt"))         analysis->PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
        if(print && !mergeResults && strcmp(bintype,"y"))          analysis->PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
        if(print && !mergeResults && strcmp(bintype,"integrated")) analysis->PrintNofParticle("PSI","NofJPsi","INTEGRATED",kFALSE);
        if(print && !mergeResults && strcmp(bintype,"yvspt"))      analysis->PrintNofParticle("PSI","NofJPsi","YVSPT",kFALSE);

        if(rawcount){
            analysis->ComputeDimuonRawCount(2.8,3.4); 
            analysis->ComputeDimuonRawCount(2.1,2.8); 
        }
    }

    // Here we merge all restults and fit
    if(!mergeResults) return;
    // Delete directory if here
    // if ( !gSystem->AccessPathName(Form("%s/mergeResults",dir.Data())) ) gSystem->Exec(Form("rm -rf %s/mergeResults",dir.Data()));
    gSystem->Exec(Form("mkdir %s/mergeResults",dir.Data()));
 
    AliMergeableCollection* oc      = 0x0;
    AliCounterCollection*   cc      = 0x0;
    AliAnalysisMuMuBinning* binning = 0x0;

    TObjArray* ocCollection       = new TObjArray();
    TObjArray* ccCollection       = new TObjArray();

    analysis =0x0;
    nextdirs.Reset();
    Int_t i = 0;

    // loop over all directories
    while ( ( sdirs = static_cast<TObjString*>(nextdirs()) ) ) {

        printf("Looking for files in %s/%s/%s \n",dir.Data(),sdirs->String().Data(),sfile.Data());
        analysis = new AliAnalysisMuMu(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),beamYear.Data());
        if ( !analysis ) {
            printf("Cannot create AliAnalysisMuMu object");
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
    
    // printf("--- output ----\n");
    // if(oc)oc->Print();
    // printf("---\n");
    // if(cc)cc->Print();
    // printf("---\n");
    // if(binning) binning->Print();
    // printf("binning = %p\n", binning);

    // printf("---\n");
    // printf("---\n");

    // ocCollection->Print();
    // printf("---\n");
    // ccCollection->Print();
    // printf("---\n");

    if( oc && ocCollection ) printf("merge %lld files in oc \n",oc->Merge(ocCollection));
    else return;
    // oc->Print();
    oc->SetName("OC");

    if( cc && ccCollection ) printf("merge %lld files in cc \n",cc->Merge(ccCollection));
    else return;
    // cc->Print();
    cc->SetName("CC");
    
    TFile f(Form("%s/mergeResults/AnalysisResults.root",dir.Data()),"recreate");
    if(oc) oc->Write();
    if(cc) cc->Write();
    if(binning) binning->Write();
    
    f.Close();

    // finally fit the last files
    analysis = new AliAnalysisMuMu(Form("%s/mergeResults/%s",dir.Data(),sfile.Data()),sasso.Data(),sasso2.Data(),beamYear.Data());
    if ( !analysis ) {
        printf("Cannot create AliAnalysisMuMu object");
        return ;
    }

    // Clean   
    if(clean) analysis->CleanAllSpectra();

    // //_____ Fit 
    // nextbintype.Reset();
    // while ( ( sbintype = static_cast<TObjString*>(nextbintype()) ) ){
    //     if(sbintype->String().Contains("integrated")){
    //         analysis->FitJpsi(sbintype->String().Data(),"","",fitMethod.Data());
    //     } else {
    //         analysis->FitJpsi(sbintype->String().Data(),"BENJ","",fitMethod.Data());
    //     }
    // }
}













