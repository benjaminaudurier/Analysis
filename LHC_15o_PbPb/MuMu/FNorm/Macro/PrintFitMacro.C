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
char           * sfile="../Data_0-12/AnalysisResults.root";
// char                   * sfile="../Simulation/AnalysisResults.JPSI.root";
char                   * sasso="../Simulation/AnalysisResults.JPSI.root";
char                   * sasso2="";
char                   * beamYear="mumu.PbPb2015.config";
// char                   * beamYear="mumu.PbPb2015.simu.config";

TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSALL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETAPDCA";
TString scentrality    ="V0M_00.00_90.00";

TString param          =  "sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF,Significance3s";

void PrintDist(TObjString* swhat,AliAnalysisMuMu &ana);

//_____________________________________________________________________________
void PrintFitMacro(char         * what ="PT",const char * printWhat = "distribution",int debug =0)
{    

    TString subresults = "PSINA60NEW,PSICB2";

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
        analysis.DrawFitResults("PSI",swhat->String().Data(),"");
        analysis.PrintNofParticle("PSI","NofJPsi",swhat->String(),kFALSE);
        TIter nextParam(sparam);
        TObjString* sParam;
        // /*if(swhat->String().Contains("PT") || swhat->String().Contains("Y")) */while ((sParam=static_cast<TObjString*>(nextParam()))) analysis.PrintFitParam("PSI",sParam->String().Data(),swhat->String().Data(),subresults.Data(),"",kFALSE);

        if (Raa) {
            if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else continue;
        }

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


