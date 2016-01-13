#include <TH1.h>
#include <TString.h>
#include <TFile.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TCanvas.h>
#include <Riostream.h>
#include <TLegend.h>
#include <AliAnalysisMuMu.h>
		


void FnormOutPut(const char * filename)
{
	TH1*FnormOffline1PUPS =0x0;
	TH1*FnormOffline2PUPS =0x0;
	TH1*FnormScalers =0x0;
	TH1*FnormScalersPUPS =0x0;
	TH1*f4 =0x0;

	// TFile * f = TFile::Open(filename,"READ");
	// if(!f){
	// 	printf("Error: cannot open file \n");
	// 	return;
	// }

	// AliMergeableCollection * oc = static_cast<AliMergeableCollection*>(f->FindObjectAny("OC"));
	// AliCounterCollection * cc = static_cast<AliCounterCollection*>(f->FindObjectAny("CC"));

	// // return;
	// if(!oc) {
	// 	printf("Error: cannot find OC\n");
	// 	return;
	// }
	// if(!cc) {
	// 	printf("Error: cannot find CC\n");
	// 	return;
	// }

	// get files
	AliAnalysisMuMu ana(filename,"","","PbPb2015");


	//Compute purity
	Double_t CINTPurity = ana.CC()->GetSum("trigger:CINT7-B-NOPF-MUFAST/event:PSALL/centrality:V0M_00.00_90.00")/ana.CC()->GetSum("trigger:CINT7-B-NOPF-MUFAST/event:ALL/centrality:V0M");
	Double_t CINTPurityMUL = ana.CC()->GetSum("trigger:CMUL7-B-NOPF-MUFAST/event:PSALL/centrality:V0M_00.00_90.00")/ana.CC()->GetSum("trigger:CMUL7-B-NOPF-MUFAST/event:ALL/centrality:V0M");

	// ana.CC()->Print("centrality/event","trigger:CINT7-B-NOPF-MUFAST");
 //    ana.CC()->Print("centrality/event","trigger:CINT7-B-NOPF-MUFAST&0MUL");
    // ana.CC()->Print("centrality/event","trigger:CMUL7-B-NOPF-MUFAST");
    // return;

	printf("purity factor MB = %f\n", CINTPurity);
	printf("purity factor MUL = %f\n", CINTPurityMUL);

	//Get Graph
	FnormOffline1PUPS = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/FnormOffline1PUPS_AsHisto")->Clone());
	FnormOffline2PUPS = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/FnormOffline2PUPS_AsHisto")->Clone());
	FnormScalers = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalers_AsHisto")->Clone());
	FnormScalersPUPS = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalers_AsHisto")->Clone());


	//Get purity graphs
	TH1* CorrectionPUPSMB = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/CorrectionPUPSMB_AsHisto")->Clone());
	// TH1* CorrectionPSMUL = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/CorrectionPSMUL_AsHisto")->Clone());
	TH1* CorrectionPSMB = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/CorrectionPSMB_AsHisto")->Clone());

	//Correct scalers fnorm histo
	FnormScalersPUPS->Scale(CINTPurity);
	// FnormScalersPUPS->Divide(CorrectionPSMUL);
	FnormScalersPUPS->Multiply(CorrectionPUPSMB);

	// for (int i=0;i<h4->GetN();i++) h4->GetY()[i] *= CINTPurity * PUPSfactor->GetY()[i];
	//Change Names
	FnormScalersPUPS->SetName("FnormScalersPUPS_AsHisto");
	FnormScalersPUPS->SetTitle("Computed using OCDB scalers and corrected by PU and purity factor");

	//______Projection histograms
	// New Histo to plot purposes
	TH1F* h1 =new TH1F(Form("%s_projected",FnormOffline1PUPS->GetName()),Form("%s_projected",FnormOffline1PUPS->GetTitle()),30,10.,15.);
	TH1F* h2 =new TH1F(Form("%s_projected",FnormOffline2PUPS->GetName()),Form("%s_projected",FnormOffline2PUPS->GetTitle()),30,10.,15.);
	TH1F* h3 =new TH1F(Form("%s_projected",FnormScalers->GetName()),Form("%s_projected",FnormScalers->GetTitle()),30,10.,15.);
	TH1F* h4 =new TH1F(Form("%s_projected",FnormScalers->GetName()),Form("%s_projected",FnormScalers->GetTitle()),30,10.,15.);// Total histo
		
	//Fill with Fnorm run by run divided by squared of the error
	for ( Int_t ipoint = 0; ipoint < FnormOffline1PUPS->GetEntries(); ++ipoint ){
		Double_t y  = FnormOffline1PUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormOffline1PUPS->GetBinError(ipoint+1)* FnormOffline1PUPS->GetBinError(ipoint+1));		
		h1->Fill(y,1./e2);			
	}

	for ( Int_t ipoint = 0; ipoint < FnormOffline2PUPS->GetEntries(); ++ipoint ){
		Double_t y  = FnormOffline2PUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormOffline2PUPS->GetBinError(ipoint+1)* FnormOffline2PUPS->GetBinError(ipoint+1));		
		h2->Fill(y,1./e2);			
	}

	for ( Int_t ipoint = 0; ipoint < 137; ++ipoint ){
		Double_t y  = FnormScalersPUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormScalersPUPS->GetBinError(ipoint+1)* FnormScalersPUPS->GetBinError(ipoint+1));		
		h3->Fill(y,1./e2);			
	}

	for ( Int_t ipoint = 0; ipoint < 137; ++ipoint ){
		Double_t y  = FnormScalersPUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormScalersPUPS->GetBinError(ipoint+1)* FnormScalersPUPS->GetBinError(ipoint+1));

		Double_t y2  = FnormOffline2PUPS->GetBinContent(ipoint+1);
		Double_t e3 = (FnormOffline2PUPS->GetBinError(ipoint+1)* FnormOffline2PUPS->GetBinError(ipoint+1));

		Double_t y3  = FnormOffline1PUPS->GetBinContent(ipoint+1);
		Double_t e4 = (FnormOffline1PUPS->GetBinError(ipoint+1)* FnormOffline1PUPS->GetBinError(ipoint+1));
		
		h4->Fill(y,1./e2);			
		h4->Fill(y2,1./e3);			
		h4->Fill(y3,1./e4);			
	}
	//Scale
	h1->Scale(1./h1->Integral());
	h2->Scale(1./h2->Integral());
	h3->Scale(1./h3->Integral());
	h4->Scale(1./h4->Integral());

	//______________Plot
	h1->SetLineColor(2);
	h2->SetLineColor(3);
	h3->SetLineColor(4);
	h4->SetLineColor(1);

	h1->SetLineStyle(9);
	h2->SetLineStyle(9);
	h3->SetLineStyle(9);
	h4->SetLineStyle(1);
	
	h4->SetLineWidth(2);
	

	new TCanvas;
	TLegend * leg = new TLegend(0.4,0.2,0.90,0.4);
	leg->SetHeader("Distribution of FNorm for different methods");
	leg->AddEntry(h1,"FNormOffline1","pe");
	leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h1->GetMean(),h1->GetRMS()),"");
	leg->AddEntry(h2,"FNormOffline2","pe");
	leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h2->GetMean(),h2->GetRMS()),"");
	leg->AddEntry(h3,"Scalers method corrected","pe");
	leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h3->GetMean(),h3->GetRMS()),"");
	leg->AddEntry(h4,"Sum","pe");
	leg->AddEntry((TObject*)0,Form("mean = %.3f; RMS =  %.3f",h4->GetMean(),h4->GetRMS()),"");

	h4->Draw();
	h1->DrawCopy("Same");
	h2->DrawCopy("same");
	h3->DrawCopy("same");
	leg->Draw("same");

	
	// ana.OC()->Remove("/FNORM/Scaler/GRAPHS/FnormScalersPUPS");
	// ana.OC()->Remove("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto");
	// ana.OC()->Adopt("/FNORM/Scaler/GRAPHS",FnormScalers);
	// ana.OC()->Adopt("/FNORM/Scaler/GRAPHS",h4);

	TCanvas* c2 = new TCanvas();
	TLegend * leg2 = new TLegend(0.4,0.2,0.90,0.4);
	leg2->SetHeader("FNorm calculated with Offline method");
	leg2->AddEntry(FnormScalersPUPS,"W/o purity factor correction","pe");
	leg2->AddEntry(FnormScalers,"W/ purity factor correction","pe");
	FnormScalers->SetMarkerColor(3);
	FnormScalers->SetMarkerStyle(8);
	FnormScalers->SetMarkerSize(0.5);
	FnormScalers->GetYaxis()->SetRangeUser(2,18);
	FnormScalers->SetTitle("FNorm Scarler");
	FnormScalersPUPS->SetMarkerColor(4);
	FnormScalersPUPS->SetMarkerSize(0.5);
	FnormScalersPUPS->SetMarkerStyle(8);
	
	FnormScalers->DrawCopy("");
	FnormScalersPUPS->DrawCopy("same");
	leg2->Draw("Same");

	TCanvas* c3 = new TCanvas();
	c3->Divide(1,2,0,0);
	
	c3->cd(1);
	TLegend * leg3 = new TLegend(0.4,0.2,0.90,0.4);
	leg3->SetHeader("FNorm");
	leg3->AddEntry(FnormOffline1PUPS,"FNormOffline1 = CINT7/CINT7&0MUL","pe");
	leg3->AddEntry(FnormOffline2PUPS,"FNormOffline2 = CMSL/CMSL&0MUL x CINT7/CINT7&0MSL","pe");
	leg3->AddEntry(FnormScalersPUPS,"Scaler Fnorm with purity and pile-up correction","pe");
	FnormOffline1PUPS->SetMarkerColor(2);
	FnormOffline1PUPS->SetMarkerSize(0.55);
	FnormOffline1PUPS->SetMarkerStyle(8);
	FnormOffline2PUPS->SetMarkerColor(3);
	FnormOffline2PUPS->SetMarkerSize(0.55);
	FnormOffline2PUPS->SetMarkerStyle(8);
	FnormScalersPUPS->SetMarkerColor(4);
	FnormScalersPUPS->SetMarkerSize(0.55);
	FnormScalersPUPS->SetMarkerStyle(8);
	FnormOffline1PUPS->SetTitle("FNorm");
	FnormOffline1PUPS->GetYaxis()->SetRangeUser(8,15);
	FnormOffline1PUPS->DrawCopy();
	FnormOffline2PUPS->DrawCopy("same");
	FnormScalersPUPS->DrawCopy("same");
	leg3->Draw("same");
	
	c3->cd(2);
	FnormOffline1PUPS->SetTitle("Ratio");
	TH1* FnormOffline1PUPSclone = static_cast<TH1*>(FnormOffline1PUPS->Clone());
	TLegend * leg4 = new TLegend(0.4,0.2,0.90,0.4);
	leg4->SetHeader("FNorm");
	leg4->AddEntry(FnormOffline1PUPS,"FNormOffline1/FNormOffline2","pe");
	leg4->AddEntry(FnormOffline1PUPSclone,"FNormOffline1/Scaler","pe");
	leg4->AddEntry(FnormOffline2PUPS,"FNormOffline2/Scaler","pe");
	FnormOffline1PUPS->Divide(FnormOffline2PUPS);
	FnormOffline1PUPS->GetYaxis()->SetRangeUser(0,1.5);
	FnormOffline1PUPS->DrawCopy();
	FnormOffline1PUPSclone->Divide(FnormScalersPUPS);
	FnormOffline1PUPSclone->SetMarkerColor(4);
	FnormOffline1PUPSclone->SetMarkerSize(0.55);
	FnormOffline1PUPSclone->SetMarkerStyle(8);
	FnormOffline1PUPSclone->DrawCopy("same");
	FnormOffline2PUPS->Divide(FnormScalersPUPS);
	FnormOffline2PUPS->DrawCopy("same");
	leg4->Draw("same");

	return;

}