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
char                   * sfile="AnalysisResults.root";
char                   * sasso="";
char                   * sasso2="";
char                   * beamYear="mumu.pp2015.config";

TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSMUL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETA";
TString scentrality    ="PP";

// TString param          =  "c,c',sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF";
TString param          =  "sJPsi,mJPsi,NofJPsi,FitChi2PerNDF";
// TString param          =  "";





void PrintDist(TObjString* swhat,Bool_t yield,AliAnalysisMuMu &ana);
// void PrintCrossSection(AliAnalysisMuMuSpectra *spec,AliAnalysisMuMu &an,char*what);

//_____________________________________________________________________________
void PrintFitMacro(char         * what ="PT",const char * printWhat = "distribution",int debug =0)
{.

    TString subresults = "";
    subresults = "CB2VWG_1.7_4.8_SP1.1,CB2VWG_2.0_4.4_SP1.1,";
    subresults += "CB2POL1POL2_1.7_4.8_SP1.1,CB2POL1POL2_2.0_4.4_SP1.1,";
    subresults += "CB2VWG_1.7_4.8_Weight=2.0_SP1.1,CB2VWG_2.0_4.4_Weight=2.0_SP1.1,";
    subresults += "CB2POL1POL2_1.7_4.8_Weight=2.0_SP1.1,CB2POL1POL2_2.0_4.4_Weight=2.0_SP1.1,";
    subresults += "NA60NEWVWG_1.7_4.8_SP1.1,NA60NEWVWG_2.0_4.4_SP1.1,";
    subresults += "NA60NEWPOL1POL2_1.7_4.8_SP1.1,NA60NEWPOL1POL2_2.0_4.4_SP1.1,";


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
        analysis.DrawFitResults("PSI",swhat->String().Data(),subresults.Data());
        analysis.PrintNofParticle("PSI","NofJPsi",swhat->String(),kFALSE);
        TIter nextParam(sparam);
        TObjString* sParam;
         if(swhat->String().Contains("PT") || swhat->String().Contains("Y")) 
            while ((sParam=static_cast<TObjString*>(nextParam()))) 
                analysis.PrintFitParam("PSI",sParam->String().Data(),swhat->String().Data(),subresults.Data(),"",kFALSE);

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

    while ( ( scent = static_cast<TObjString*>(nextCent()) ) ){
        //________Get spectra
        TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scent->String().Data(),spairCut.Data(),"PSI",swhat->String().Data());
        printf("--- Centrality = %s\n", scent->String().Data());

        AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(ana.OC()->GetObject(spectraPath.Data()));
        if(!spectra) {
            cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
            return;
        }
        // if(swhat->String().Contains("PT") || swhat->String().Contains("Y"))
        // {
        //    new TCanvas;
        //    spectra->Plot("NofJPsi","",kTRUE)->DrawCopy("");
        //     if(swhat->String().Contains("PT")) ana.ComputePPCrossSection(swhat->String().Data(),"PSI","CorrNofJPsi","externFile_PT.txt","externFile_CENT.txt");
        //     else if (swhat->String().Contains("Y")) ana.ComputePPCrossSection(swhat->String().Data(),"PSI","CorrNofJPsi","externFile_Y.txt","externFile_CENT.txt");
        // }
        // else ana.ComputePPCrossSection(swhat->String().Data(),"PSI","CorrNofJPsi","externFile_PT.txt","externFile_CENT.txt");
    }
}
