//
//  PrintFitMacro.c
//
//
//  Created by Benjamin Audurier on 09/06/15.
//
//

// Macro who prints fit results

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream>

//Some strings and constants
char                   * sfile="AnalysisResults.JPSI.root";
char                   * sasso="";
char                   * sasso2="";
char                   * beamYear="mumu.pp2015.config";

TString striggerDimuon ="ANY";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="ALL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-15.0RABSMATCHLOWETAPDCA";
TString scentrality    ="PP";

TString param          =  "sJPsi,mJPsi,NofJPsi,FitChi2PerNDF";

TString subresults     ="";
// TString subresults     ="PSICB2,PSINA60NEW";


void PrintDist(TObjString* swhat,AliAnalysisMuMu &ana);

//_____________________________________________________________________________
void PrintFitMacro(char         * what ="PT",const char * printWhat = "distribution",int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    Bool_t PrintDistribution = kFALSE;
    Bool_t Raa               = kFALSE;
    Bool_t print             = kFALSE;


    TObjArray* sprint = TString(printWhat).Tokenize(",");
    TObjArray* sparam = TString(param).Tokenize(",");


    //Set bool
    if(sprint->FindObject("raa")) Raa                        =kTRUE;
    if(sprint->FindObject("distribution")) PrintDistribution =kTRUE;
    if(sprint->FindObject("save")) print                     =kTRUE;


    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    //_____ Draw
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    {
        analysis.DrawFitResults("PSI",swhat->String().Data(),subresults.Data(),kFALSE);
        analysis.PrintNofParticle("PSI","NofJPsi",swhat->String(),kFALSE);
        TIter nextParam(sparam);
        TObjString* sParam;
        if(swhat->String().Contains("PT") || swhat->String().Contains("Y")) while ((sParam=static_cast<TObjString*>(nextParam()))) analysis.PrintFitParam("PSI",sParam->String().Data(),swhat->String().Data(),subresults.Data(),"",kFALSE);


        if (PrintDistribution) PrintDist(swhat,analysis);
    }

    return ;
}

//___________________________________________
void PrintDist(TObjString* swhat,AliAnalysisMuMu &ana)
{


    TObjArray* whatCent= TString(scentrality.Data()).Tokenize(",");
    TIter nextCent(whatCent);
    TObjString* scent;

    while ( ( scent = static_cast<TObjString*>(nextCent()) ) )
    {
        //________Get spectra
        TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scent->String().Data(),spairCut.Data(),"PSI",swhat->String().Data());
        printf("--- Centrality = %s\n", scent->String().Data());

        AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(ana.OC()->GetObject(spectraPath.Data()));
        if(!spectra)
        {
            cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
            return;
        }
        new TCanvas;
        spectra->Plot("NofJPsi","",kTRUE)->DrawCopy("");

    }


}

