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
#include <AliAnalysisMuMuResult.h>
#include <AliAnalysisMuMuBinning.h>
#include <AliMergeableCollection.h>
#include <TF1.h>
#include <TLegend.h>
#include <TFitResultPtr.h>
#include <AliCounterCollection.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream>

//Some strings and constants
char                   * sfile="../AnalysisResults_grid.root";
// char                   * sasso="";
char                   * sasso="../../AccEff_jpsi/MCPart/AnalysisResults.JPSI.root";
char                   * sasso2="";
char                   * beamYear="mumu.pp2015.config";

TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSMUL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETAPDCA";
TString scentrality    ="PP";

// TString param          =  "c,c',sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF";
TString param          =  "sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF";

// TString subresults     =  "CB2POL1POL2_1.6_5.0_SP1.2,CB2POL1POL2_1.8_4.7_SP1.2";
// TString subresults     =  "CB2POL1POL2_2.2_4.5_SP1.2,CB2POL1POL2_2.0_5.0_SP1.2";
// TString subresults     =  "CB2VWG_1.6_5.0_SP1.2,CB2VWG_1.8_4.7_SP1.2";
TString subresults     = "CB2VWG_1.7_4.7_SP1.2,CB2VWG_1.8_4.5_SP1.2,CB2POL1POL2_1.7_4.7_SP1.2,CB2POL1POL2_1.8_4.5_SP1.2,NA60NEWVWG_1.7_4.7_SP1.2,NA60NEWVWG_1.8_4.5_SP1.2,NA60NEWPOL1POL2_1.7_4.7_SP1.2,NA60NEWPOL1POL2_1.8_4.5_SP1.2";


void PrintDist(TObjString* swhat,Bool_t yield,AliAnalysisMuMu &ana);
// void PrintCrossSection(AliAnalysisMuMuSpectra *spec,AliAnalysisMuMu &an,char*what);

//_____________________________________________________________________________
void PrintFitMacro(char         * what ="PT",const char * printWhat = "distribution",int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    Bool_t PrintDistribution = kFALSE;
    Bool_t Raa               = kFALSE;
    Bool_t print             = kFALSE;
    Bool_t yield             = kFALSE;

    TObjArray* sprint = TString(printWhat).Tokenize(",");
    TObjArray* sparam = TString(param).Tokenize(",");

    //Set bool
    if(sprint->FindObject("raa")) Raa                        =kTRUE;
    if(sprint->FindObject("distribution")) PrintDistribution =kTRUE;
    if(sprint->FindObject("save")) print                     =kTRUE;
    if(sprint->FindObject("yield")) yield                    =kTRUE;

    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    //_____ Draw
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    {
        analysis.DrawFitResults("PSI",swhat->String().Data(),"histo",print);
        analysis.PrintNofParticle("PSI","NofJPsi",swhat->String(),kFALSE);
        TIter nextParam(sparam);
        TObjString* sParam;
         if(swhat->String().Contains("PT") || swhat->String().Contains("Y")) while ((sParam=static_cast<TObjString*>(nextParam()))) analysis.PrintFitParam("PSI",sParam->String().Data(),swhat->String().Data(),subresults.Data(),"",kFALSE);

        if (Raa) {
            if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else continue;
        }

        PrintDist(swhat,kTRUE,analysis);
    }

    return ;
}

//___________________________________________
void PrintDist(TObjString* swhat,Bool_t yield,AliAnalysisMuMu &ana)
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
        if(!spectra) {
            cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
            return;
        }
        if(swhat->String().Contains("PT") || swhat->String().Contains("Y"))
        {
           new TCanvas;
           spectra->Plot("NofJPsi","",kTRUE)->DrawCopy("");
        }
       ana.ComputePPCrossSection(swhat->String().Data());
    }
}
