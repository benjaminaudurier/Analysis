#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliAnalysisMuMuResult.h>
#include <AliAnalysisMuMuBinning.h>
#include <AliMergeableCollection.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TLegend.h>
#include <TFitResultPtr.h>
#include <AliCounterCollection.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>
#include <TEfficiency.h>
#include <TFile.h>
#include <iostream>

void PrintTriggerRatio(const char * filename)
{	
	// Get counter collection
	AliAnalysisMuMu* a = new AliAnalysisMuMu (filename,"","","mumu.pp2015.fnorm.config");
	if(!a){
		cout << "cannot open file " << filename << endl;
		return;
	}

	// Get histo for CINT7&0MSL
	TH1* hmsl = a->CC()->Draw("run","centrality:PP/trigger:CINT7-B-NOPF-MUFAST&0MSL/event:VDM");
	if(!hmsl){
		cout << "cannot find histo at centrality:PP/trigger:CINT7-B-NOPF-MUFAST&0MSL/event:VDM " << endl;
		return;
	}
	hmsl->Sumw2();

	// Get histo for CINT7&0TVX
	TH1* htvx = a->CC()->Draw("run","centrality:PP/trigger:CINT7-B-NOPF-MUFAST&0TVX/event:VDM");
	if(!htvx){
		cout << "cannot find histo at centrality:PP/trigger:CINT7-B-NOPF-MUFAST&0TVX/event:VDM " << endl;
		return;
	}
	htvx->Sumw2();

	// Get histo for CINT7
	TH1* hint = a->CC()->Draw("run","centrality:PP/trigger:CINT7-B-NOPF-MUFAST/event:VDM");
	if(!hint){
		cout << "cannot find histo at centrality:PP/trigger:CINT7-B-NOPF-MUFAST/event:VDM " << endl;
		return;
	}
	hint->Sumw2();

	//Get clone
	if(! hmsl->Divide(hmsl,hint,1,1,"B")){
		cout << "cannot divide hmsl with hint " << endl;
		return;
	}
	// new TCanvas;
	// hmsl->DrawCopy("e");

	//Get clone
	if(! htvx->Divide(htvx,hint,1,1,"B")){
		cout << "cannot divide htvx with hint " << endl;
		return;
	}
	// new TCanvas;
	// htvx->Draw("e");

	hmsl->Divide(htvx);
	new TCanvas;
	hmsl->Draw("e");

	// New Histo to plot purposes
	TH1* hproj = new TH1F(Form("%s_projected",hmsl->GetName()),Form("%s_projected",hmsl->GetTitle()),80,0.01,0.05);
		
	//Fill with Fnorm run by run divided by squared of the error
	for ( Int_t ipoint = 0; ipoint < hmsl->GetEntries(); ++ipoint ){
		Double_t y  = hmsl->GetBinContent(ipoint+1);
		Double_t e2 = (hmsl->GetBinError(ipoint+1)* hmsl->GetBinError(ipoint+1));		
		hproj->Fill(y,1./e2);			
	}
	// new TCanvas;
	// hproj->Draw();

	return;

}