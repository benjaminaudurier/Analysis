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

// --- MuMu config. ---
const char* sfile="AnalysisResults.root";
const char* sasso="../../Simulation/full/AnalysisResults.JPSI.root";
const char* sasso2="";
const char* beamYear="mumu.pp2015.full.AccEff.config";

const char* spectraName= "PSI-PT-INT_PUTCUT-AccEffCorr-MeanPtVsMinvUS" ;

// --- Parameters  ----

// TString param          =  "c,c',sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF";
// TString param          =  "sJPsi,mJPsi,NofJPsi,FitChi2PerNDF";
TString param          ="mJPsi,MeanPtJPsi,MeanPtPsiP";
// TString param          =  "";

//_____________________________________________________________________________
void PrintFitParameterMacro(int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    TString subresults = "";

    // --- subresults for minv fit ----

    // subresults = "CB2VWG_1.7_4.8_SP1.1,CB2VWG_2.0_4.4_SP1.1,";
    // subresults += "CB2POL1POL2_1.7_4.8_SP1.1,CB2POL1POL2_2.0_4.4_SP1.1,";
    // subresults += "CB2VWG_1.7_4.8_Weight=2.0_SP1.1,CB2VWG_2.0_4.4_Weight=2.0_SP1.1,";
    // subresults += "CB2POL1POL2_1.7_4.8_Weight=2.0_SP1.1,CB2POL1POL2_2.0_4.4_Weight=2.0_SP1.1,";
    // subresults += "NA60NEWVWG_1.7_4.8_SP1.1,NA60NEWVWG_2.0_4.4_SP1.1,";
    // subresults += "NA60NEWPOL1POL2_1.7_4.8_SP1.1,NA60NEWPOL1POL2_2.0_4.4_SP1.1,";

    // --- subresults for mpt fit ----

    subresults += "CB2VWG_BKGMPTPOL2_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.0_5.0_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.0_5.0_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.2_4.8_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2_2.2_4.8_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.0_5.0_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.0_5.0_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.2_4.8_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2VWG_BKGMPTPOL2EXP_2.2_4.8_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2EXP_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2EXP_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2EXP_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWVWG_BKGMPTPOL2EXP_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.0_5.0_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.0_5.0_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.2_4.8_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2_2.2_4.8_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.0_5.0_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.0_5.0_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.2_4.8_Weight=2.0(Sig:2.0_4.8_Weight=2.0_SP1.1),";
    subresults += "CB2POL1POL2_BKGMPTPOL2EXP_2.2_4.8_Weight=2.0(Sig:2.2_4.4_Weight=2.0_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2_2.2_4.8(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2_2.2_4.8(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2EXP_2.0_5.0(Sig:2.0_4.8_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2EXP_2.0_5.0(Sig:2.2_4.4_SP1.1),";
    subresults += "NA60NEWPOL1POL2_BKGMPTPOL2EXP_2.2_4.8(Sig:2.0_4.8_SP1.1),";

    TObjArray* sparam = TString(param).Tokenize(",");

    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    //_____ Draw
    TIter nextParam(sparam);
    TObjString* sParam;
    while ((sParam=static_cast<TObjString*>(nextParam())))
        analysis.PrintFitParam(sParam->String().Data(),spectraName,subresults.Data());
}

