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


//_____________________________________________________________________________
void MergeResults()
{
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
        AliAnalysisMuMu analysis(Form("%s/%s/%s",dir.Data(),sdirs->String().Data(),sfile.Data()),sasso.Data(),sasso2.Data(),config.Data());

        // Fill the collections
        if ( analysis.OC()  && i !=0 )  ocCollection->Add(analysis.OC()->Clone());
        else if ( analysis.OC() && i==0 ) oc =static_cast<AliMergeableCollection*>(analysis.OC()->Clone());

        if ( !(i%50) && i!=0 ) printf("merge %lld files in oc \n",oc->Merge(ocCollection));

         if ( analysis.CC()  && i !=0 )  ccCollection->Add(analysis.CC()->Clone());
         else if ( analysis.CC() && i==0 ) cc =static_cast<AliCounterCollection*>(analysis.CC()->Clone());

        if ( !(i%50) && i!=0 ) printf("merge %lld files in cc \n",cc->Merge(ccCollection));

        i++;
    }

    if( oc && ocCollection ) printf("merge %lld files in oc \n",oc->Merge(ocCollection));
    else return;
    // oc->Print();
    oc->SetName("OC");

     if( cc && ccCollection ) printf("merge %lld files in cc \n",cc->Merge(ccCollection));
     else return;
    // // cc->Print();
     cc->SetName("CC");

    TFile f(Form("%s/mergeResults/AnalysisResults.root",dir.Data()),"recreate");
    if(oc)      oc->Write();
     if(cc)      cc->Write();
    // if(binning) binning->Write();

    f.Close();
}













