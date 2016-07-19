#if !defined(__CINT__) || defined(__MAKECINT__)
#include <AliLog.h>
#include "TSystem.h"
#include <Riostream.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include "TLegend.h"
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream> 
#endif


char                * wfile="AnalysisResults.JPSI.weightDATA.root";
char                * reffile="AnalysisResults.JPSI.weightReference.root";
char                * beamYear="mumu.pp2015.simu.nomatchlow.config";
TString spectraPath    = "/ALL/ANY/PP/pALLPAIRYPAIRPTIN0.0-12.0RABSETAPDCA";

//_______________________________________________________________________
void CompareTriggerEfficiency( TString what ="pt", int debug =0  )
{
	//Debut level
	AliLog::SetGlobalDebugLevel(debug);

	 // main object
    AliAnalysisMuMu* aw = new AliAnalysisMuMu(wfile,"","",beamYear);
    if ( !aw ) {
		printf("Fatal: cannot find %s\n", wfile);
		return;
	}
	AliAnalysisMuMu* aref = new AliAnalysisMuMu(reffile,"","",beamYear);
    if ( ! aref ) {
		printf("Fatal: cannot find %s\n", reffile);
		return;
	}

    // Fit the weighted histograms
	aw->Jpsi(what.Data(),"BENJ",kFALSE,kFALSE);
	aref->Jpsi(what.Data(),"BENJ",kFALSE,kFALSE);
	// return;


	

	//Get weighted Spectra
	AliAnalysisMuMuSpectra                 * weightedspectra = 0x0;
	if(what.Contains("pt")) weightedspectra              = static_cast<AliAnalysisMuMuSpectra*>(aw->OC()->GetObject(Form("%s/PSI-PT",spectraPath.Data())));
	else if(what.Contains("y")) weightedspectra          = static_cast<AliAnalysisMuMuSpectra*>(aw->OC()->GetObject(Form("%s/PSI-Y",spectraPath.Data())));
	else if(what.Contains("integrated")) weightedspectra = static_cast<AliAnalysisMuMuSpectra*>(aw->OC()->GetObject(Form("%s/PSI-INTEGRATED",spectraPath.Data())));
    
    if(!weightedspectra){
        cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
        return;
    }

    //Get reference Spectra
	AliAnalysisMuMuSpectra            * mcspectra = 0x0;
	if(what.Contains("pt")) mcspectra = static_cast<AliAnalysisMuMuSpectra*>(aref->OC()->GetObject(Form("%s/PSI-PT",spectraPath.Data())));
	if(what.Contains("y")) mcspectra  = static_cast<AliAnalysisMuMuSpectra*>(aref->OC()->GetObject(Form("%s/PSI-Y",spectraPath.Data())));
	if(what.Contains("y")) mcspectra  = static_cast<AliAnalysisMuMuSpectra*>(aref->OC()->GetObject(Form("%s/PSI-INTEGRATED",spectraPath.Data())));
    if(!mcspectra){
        cout << Form("Cannot find reference spectra with name %s",spectraPath.Data()) <<endl;
        return;
    }

    //Get histos
    TH1* weightedHisto=0x0;
    TH1* refHisto=0x0;

    weightedHisto = static_cast<TH1*>(weightedspectra->Plot("NofJPsi","",kTRUE)->Clone());
	 if(!weightedHisto){
	    cout << Form("Cannot find weighted histo ") <<endl;
	    return;
	}
    
    refHisto = static_cast<TH1*>(mcspectra->Plot("NofJPsi","",kTRUE)->Clone());
    if(!refHisto){
	    cout << Form("Cannot find weighted histo ") <<endl;
	    return;
	}

	TCanvas*c = new TCanvas();
	c->Divide(1,2);
	c->cd(1);
	
	refHisto->SetMarkerColor(kRed);
	refHisto->SetLineColor(kRed);
	
	refHisto->SetMarkerColor(kBlue);
	refHisto->SetLineColor(kBlue);
	
	// refHisto->SetMarkerSize(1.5);
	// weightedHisto->SetMarkerSize(1.5);

	refHisto->SetMarkerStyle(8);
	weightedHisto->SetMarkerStyle(22);

	TLegend * leg = new TLegend(0.4,0.7,0.70,0.9);
    leg->SetHeader(Form("Weighted NofJPsi for pp@5TeV"));
    leg->AddEntry(weightedHisto,"lpt/allpt Data ","pel");
    leg->AddEntry(refHisto,"lpt/allpt MC","pel");
	
	refHisto->DrawCopy();
	weightedHisto->Draw("same");
	leg->Draw("same");

	c->cd(2);

	refHisto->Divide(weightedHisto);
	refHisto->GetYaxis()->SetTitle("Ratio MC_lpt/allpt / weightedMC_lpt/allpt ");
	refHisto->Draw();

	for (int i = 0; i < refHisto->GetNbinsX(); ++i)
	{	
		double efficiency = 1.-refHisto->GetBinContent(i+1);
		printf("Syst. uncertainty for bin %d = %f\n",i+1,efficiency);
	}

}