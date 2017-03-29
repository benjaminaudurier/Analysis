//
//  PrintJPsiYield.c
//
//
//  Created by Benjamin Audurier on 02/12/16.
//
//

// Macro to Print the Jpsi yield

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <THnSparse.h>
#include <AliAnalysisMuMu.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>

#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <AliAnalysisMuMuBinning.h>



const char* sfile     ="AnalysisResults.root";
const char* sasso     ="";
const char* sasso2    ="";
const char* config    = "../mumu.PbPb2015.full.config";

void PrintJPsiYield()
{
 	AliLog::SetGlobalDebugLevel(debug);

    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,config);

    // Get Y dist
    THnSparse* Y_dist_cent   = static_cast<THnSparse*>(a.OC()->GetObject("/PSMUL/CMUL7-B-NOPF-MUFAST/V0M_00.00_10.00/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA/Y")->Clone());
    THnSparse* Y_dist_periph = static_cast<THnSparse*>(a.OC()->GetObject("/PSMUL/CMUL7-B-NOPF-MUFAST/V0M_80.00_90.00/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA/Y")->Clone());
    if(!Y_dist_cent || !Y_dist_periph ) return;

    Int_t binmin = Y_dist->GetAxis(1)->FindBin(0);
	Int_t binmax = Y_dist->GetAxis(1)->FindBin(12);
	Double_t nmul = a.OC()->GetSum("event:PSMUL/trigger:CMUL7-B-NOPF-MUFAST/centrality:V0M_00.00_10.00,V0M_10.00_20.00,V0M_10.00_20.00,V0M_20.00_30.00,V0M_20.00_30.00,V0M_30.00_40.00,V0M_30.00_40.00,V0M_40.00_50.00,V0M_40.00_50.00,V0M_50.00_60.00,V0M_50.00_60.00,V0M_60.00_70.00,V0M_60.00_70.00,V0M_70.00_80.00,V0M_70.00_80.00,V0M_80.00_90.00");

	// Select the good range
	Y_dist_cent->GetAxis(1)->SetRange(binmin,binmax);
	Y_dist_periph->GetAxis(1)->SetRange(binmin,binmax);

    // AccxEff
    Double_t AccxEff_cent   = {0.03939,0.14392,0.20493,0.20580,0.15404,0.05824};
    Double_t AccxEff_periph = {0.03939,0.14392,0.20493,0.20580,0.15404,0.05824};


}