#include <TH1.h>
#include <TGraphErrors.h>
#include <TString.h>
#include <TFile.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TCanvas.h>
#include <Riostream.h>
#include <TLegend.h>
#include <AliAnalysisMuMu.h>

std::set<int> RunNumbers(AliAnalysisMuMu* a);
TH1* ScaleFNorm(AliAnalysisMuMu* a1, TH1* h1, TString trigger,TString event, Double_t VdMCC, Double_t VdMCCErr );
// TH1* GetGraphAsHisto(AliAnalysisMuMu* a1, TGraphErrors* h1 );
// void GetValueAndErrorFromGraph(TGraphErrors* graph,Int_t runNumber,Double_t& value,Double_t& error);


void FnormOutPut(
	const char * filename1 ="../AnalysisResults_grid_CINT7-B-NOPF-MUFAST.root" ,
	const char * filename2 ="../AnalysisResults_grid_CINT7-B-NOPF-MUFAST_MUPBS.root",
	const char * filename3 ="../AnalysisResults_grid_CINT7-B-NOPF-MUFAST0TVX.root ",
	const char * filename4 ="../AnalysisResults_grid_CINT7-B-NOPF-MUFAST0TVX_MUPBS.root")
{
	// Offline
	TH1*FnormOffline2PUPS1 =0x0;
	TH1*FnormOffline2PUPS3 =0x0;
	TGraphErrors*FnormOffline2PUPSGraph =0x0;
	
	//Scaler
	TH1*FnormScalersPUPS1 =0x0;
	TH1*FnormScalersPUPS2 =0x0;
	TH1*FnormScalersPUPS3 =0x0;
	TH1*FnormScalersPUPS4 =0x0;
	
	//Purity for CINT7
	TH1*CINTPurity1 =0x0;
	TH1*CINTPurity2 =0x0;
	TH1*CINTPurity3 =0x0;
	TH1*CINTPurity4 =0x0;
	
	//Get files
	AliAnalysisMuMu ana1(filename1,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana2(filename2,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana3(filename3,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana4(filename4,"","","mumu.pp2015.config");

	//Get Histo
	FnormScalersPUPS1 = static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	FnormScalersPUPS2 = static_cast<TH1*>(ana2.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	FnormScalersPUPS3 = static_cast<TH1*>(ana3.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	FnormScalersPUPS4 = static_cast<TH1*>(ana4.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	
	if(!FnormScalersPUPS1 || !FnormScalersPUPS2 || !FnormScalersPUPS3 ||!FnormScalersPUPS4) {
		printf("cannot get all the FNorm histo\n");
		return;
	}

	// new TCanvas;
	// FnormScalersPUPS1->DrawCopy();
	// FnormScalersPUPS2->DrawCopy("same");
	// FnormScalersPUPS3->DrawCopy("same");
	// FnormScalersPUPS4->DrawCopy("same");

	TH1* CMULCC1 = ScaleFNorm(&ana1,FnormScalersPUPS1,"CINT7-B-NOPF-MUFAST","VDM",52.62,0.08);
	TH1* CMULCC2 = ScaleFNorm(&ana2,FnormScalersPUPS2,"CINT7-B-NOPF-MUFAST","PSMUSPB",52.62,0.08);
	TH1* CMULCC3 = ScaleFNorm(&ana3,FnormScalersPUPS3,"CINT7-B-NOPF-MUFAST&0TVX","VDM",22.29,0.02);
	TH1* CMULCC4 = ScaleFNorm(&ana4,FnormScalersPUPS4,"CINT7-B-NOPF-MUFAST&0TVX","PSMUSPB",22.29,0.02);


	CINTPurity1 =static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	CINTPurity2 =static_cast<TH1*>(ana2.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	CINTPurity3 =static_cast<TH1*>(ana3.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	CINTPurity4 =static_cast<TH1*>(ana4.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
    
    if(!CINTPurity1 || !CINTPurity2 || !CINTPurity3 ||!CINTPurity4) {
		printf("cannot get all the purity histo\n");
		return;
	}
	// new TCanvas;
	// CINTPurity1->DrawCopy();
	// CINTPurity2->DrawCopy("same");
	// CINTPurity3->DrawCopy("same");
	// CINTPurity4->DrawCopy("same");

	TCanvas* c2 = new TCanvas();
	c2->Divide(1,2,0,0);

	c2->cd(1);
	
	CINTPurity1->SetMarkerColor(2);
	CINTPurity1->SetMarkerSize(0.9);
	CINTPurity1->SetMarkerStyle(8);
	
	CINTPurity2->SetMarkerColor(3);
	CINTPurity2->SetMarkerSize(0.9);
	CINTPurity2->SetMarkerStyle(8);

	CINTPurity3->SetMarkerColor(4);
	CINTPurity3->SetMarkerSize(0.9);
	CINTPurity3->SetMarkerStyle(8);

	CINTPurity4->SetMarkerColor(6);
	CINTPurity4->SetMarkerSize(0.9);
	CINTPurity4->SetMarkerStyle(8);

	CINTPurity1->SetTitle("MB Purity");
	CINTPurity2->GetYaxis()->SetRangeUser(0.9,1.1);


	CINTPurity1->DrawCopy();
	CINTPurity2->DrawCopy("same");
	CINTPurity3->DrawCopy("same");
	CINTPurity4->DrawCopy("same");

	TLegend * leg1 = new TLegend(0.4,0.2,0.90,0.4);
	leg1->SetHeader("MB Purity Factor");
	leg1->AddEntry(CINTPurity1,"V0-VDM","pe");
	leg1->AddEntry(CINTPurity2,"V0-PS","pe");
	leg1->AddEntry(CINTPurity3,"T0-VDM","pe");
	leg1->AddEntry(CINTPurity4,"T0-PS","pe");
	leg1->Draw("same");

	c2->cd(2);

	CINTPurity2->SetTitle("Ratio");
	CINTPurity2->Divide(CINTPurity1);
	CINTPurity4->Divide(CINTPurity3);

	TLegend * leg2 = new TLegend(0.4,0.2,0.90,0.4);
	leg2->SetHeader("Ratio");
	leg2->AddEntry(CINTPurity2,"V0-PS/V0-VDM","pe");
	leg2->AddEntry(CINTPurity4,"T0-PS/T0-VDM","pe");

	CINTPurity2->GetYaxis()->SetRangeUser(0.95,1.1);
	CINTPurity2->DrawCopy();
	CINTPurity4->DrawCopy("same");
	leg2->Draw("same");




	TCanvas* c3 = new TCanvas();
	c3->Divide(1,2,0,0);

	c3->cd(1);
	
	CMULCC1->SetMarkerColor(2);
	CMULCC1->SetMarkerSize(0.9);
	CMULCC1->SetMarkerStyle(8);
	
	CMULCC2->SetMarkerColor(3);
	CMULCC2->SetMarkerSize(0.9);
	CMULCC2->SetMarkerStyle(8);

	CMULCC3->SetMarkerColor(4);
	CMULCC3->SetMarkerSize(0.9);
	CMULCC3->SetMarkerStyle(8);

	CMULCC4->SetMarkerColor(6);
	CMULCC4->SetMarkerSize(0.9);
	CMULCC4->SetMarkerStyle(8);

	CMULCC1->SetTitle("MUL Cross Section");
	CMULCC1->GetYaxis()->SetTitle("#sigma (mb)");
	CMULCC1->GetYaxis()->SetRangeUser(0.0104,0.0124);

	CMULCC1->Draw();
	CMULCC2->DrawCopy("same");
	CMULCC3->DrawCopy("same");
	CMULCC4->DrawCopy("same");

	TLegend * leg3 = new TLegend(0.4,0.2,0.90,0.4);
	leg3->SetHeader("MUL Cross-section");
	leg3->AddEntry(CMULCC1,"V0-VDM","pe");
	leg3->AddEntry(CMULCC2,"V0-PS","pe");
	leg3->AddEntry(CMULCC3,"T0-VDM","pe");
	leg3->AddEntry(CMULCC4,"T0-PS","pe");
	leg3->Draw("same");

	c3->cd(2);
	CMULCC1->SetTitle("Ratio");

	CMULCC2->Divide(CMULCC1);
	CMULCC3->Divide(CMULCC1);
	CMULCC4->Divide(CMULCC1);

	TLegend * leg4 = new TLegend(0.4,0.2,0.90,0.4);
	leg4->SetHeader("Ratio");

	leg4->AddEntry(CMULCC2,"V0-PS/V0-VDM","pe");
	leg4->AddEntry(CMULCC3,"T0-VDM/V0-VDM","pe");
	leg4->AddEntry(CMULCC4,"T0-PS/V0-VDM","pe");

	CMULCC2->GetYaxis()->SetRangeUser(0.95,1.04);
	CMULCC2->DrawCopy();
	CMULCC3->DrawCopy("same");
	CMULCC4->DrawCopy("same");
	leg4->Draw("same");
}

//______________________________________________
std::set<int> RunNumbers(AliAnalysisMuMu* a)
{
  // Extract the run numbers from our counter collection

  std::set<int> runset;

  TString sruns = a->CC()->GetKeyWords("run");
  TObjArray* runs = sruns.Tokenize(",");

  TIter next(runs);
  TObjString* s;

  while ( ( s = static_cast<TObjString*>(next())) )
  {
    runset.insert(s->String().Atoi());
  }
  delete runs;

  return runset;
}

//______________________________________________
TH1* ScaleFNorm(AliAnalysisMuMu* a1, TH1* h1, TString trigger,TString event, Double_t VdMCC, Double_t VdMCCErr )
{
	std::set<int> runs= RunNumbers(a1);

	TH1*h  = ((TH1*)h1->Clone("MUL Cross-section"));
	h->Reset();

	int i= 1;
	for ( std::set<int>::const_iterator it = runs.begin(); it != runs.end(); ++it )
	{
		Double_t CMULCC = h1->GetBinContent(i);
		Double_t CMULCCErr = h1->GetBinError(i);
		
		CMULCCErr = CMULCCErr*CMULCCErr/CMULCC/CMULCC + VdMCCErr*VdMCCErr/VdMCC/VdMCC;
		CMULCC    = VdMCC/CMULCC;
		CMULCCErr = CMULCC*TMath::Sqrt(CMULCCErr);

		h->SetBinContent(i,CMULCC);
		h->SetBinError(i,CMULCCErr);
		i++;
	}
	return h;
}

