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


void FnormOutPut(
	const char * filename1 ="AnalysisResults_CINT7-B-NOPF-MUFAST_safnewMUSPB.root" ,
	const char * filename2 ="AnalysisResults_CINT7-B-NOPF-MUFAST_safnewVDM.root",
	const char * filename3 ="AnalysisResults_CINT7-B-NOPF-MUFAST0TVX_safnewVDM.root ")
{
	// Offline
	TH1*FnormOffline2PUPSCINT =0x0;

	//Scaler
	TH1*FnormScalersPUPSCINT =0x0;
	TH1*FnormScalersPUPSCOTVX =0x0;

	//Purity for CINT7
	TH1*CINTPurityCINT7PS =0x0;
	TH1*CINTPurityCINT7 =0x0;
	TH1*CINTPurityC0TVXVDM =0x0;
	TH1*CINTPurity4 =0x0;

	//P.U for CINT7
	TH1*CINTPU1 =0x0;
	TH1*CINTPU2 =0x0;
	TH1*CINTPU3 =0x0;

	//Get files
	AliAnalysisMuMu ana1(filename1,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana3(filename3,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana2(filename2,"","","mumu.pp2015.config");
	// AliAnalysisMuMu ana4(filename4,"","","mumu.pp2015.config");

	//Get FNorm Histo
	FnormOffline2PUPSCINT = static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Offline/GRAPHS/FnormOffline2PUPS_AsHisto")->Clone());
	FnormScalersPUPSCOTVX = static_cast<TH1*>(ana3.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	FnormScalersPUPSCINT  = static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	// FnormScalersPUPS4  = static_cast<TH1*>(ana4.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());

	//Get Purity histo
	CINTPurityCINT7PS     =static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	CINTPurityC0TVXVDM    =static_cast<TH1*>(ana3.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	CINTPurityCINT7       =static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());
	// CINTPurity4        =static_cast<TH1*>(ana4.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMB_PP_AsHisto")->Clone());

	//Get P.U histo
	CINTPU1               =static_cast<TH1*>(ana1.OC()->GetObject("/FNORM/Scaler/GRAPHS/CorrectionPUPSMB_AsHisto")->Clone());
	CINTPU3               =static_cast<TH1*>(ana3.OC()->GetObject("/FNORM/Scaler/GRAPHS/CorrectionPUPSMB_AsHisto")->Clone());


	// Compute MUL Cross-section
	TH1* CMULCC1 = ScaleFNorm(&ana1,FnormOffline2PUPSCINT,"CINT7-B-NOPF-MUFAST","VDM",51.17,1.08);
	TH1* CMULCC2 = ScaleFNorm(&ana1,FnormScalersPUPSCINT,"CINT7-B-NOPF-MUFAST","VDM",51.17,1.08);
	TH1* CMULCC3 = ScaleFNorm(&ana3,FnormScalersPUPSCOTVX,"CINT7-B-NOPF-MUFAST&0TVX","VDM",21.55,0.45255);



	if(!FnormOffline2PUPSCINT /*|| !FnormScalersPUPSCINT */|| !FnormScalersPUPSCOTVX /*||!FnormScalersPUPS4*/) {
		printf("cannot get all the FNorm histo\n");
		return;
	}

	if(!CINTPurityCINT7PS || !CINTPurityCINT7 || !CINTPurityC0TVXVDM /*||!CINTPurity4*/) {
		printf("cannot get all the FNorm histo\n");
		return;


		printf("cannot get all the FNorm histo\n");
		return;
	}

	 if(!CMULCC1 /*|| !CMULCC2 */|| !CMULCC3/* ||!CMULCC4*/) {
		printf("cannot get all the purity histo\n");
		return;
	}
	new TCanvas;
    CINTPurityCINT7PS->DrawCopy("");

    // ---- purity plot ----
	// TCanvas* c1 = new TCanvas();
	// c1->Divide(1,2,0,0);

	// c1->cd(1);

	// CINTPurityCINT7PS->SetMarkerColor(2);
	// CINTPurityCINT7PS->SetMarkerSize(1);
	// CINTPurityCINT7PS->SetMarkerStyle(8);

	// CINTPurityCINT7->SetMarkerColor(3);
	// CINTPurityCINT7->SetMarkerSize(1);
	// CINTPurityCINT7->SetMarkerStyle(8);

	// // CINTPurityC0TVXVDM->SetMarkerColor(4);
	// // CINTPurityC0TVXVDM->SetMarkerSize(1);
	// // CINTPurityC0TVXVDM->SetMarkerStyle(8);

	// // CINTPurity4->SetMarkerColor(6);
	// // CINTPurity4->SetMarkerSize(1);
	// // CINTPurity4->SetMarkerStyle(8);

	// CINTPurityCINT7PS->SetTitle("CINT7 Purity Factor");
	// CINTPurityCINT7PS->GetYaxis()->SetRangeUser(0.,1.1);

	// CINTPurityCINT7PS->DrawCopy("");
	// CINTPurityCINT7->DrawCopy("same");
	// // CINTPurityC0TVXVDM->DrawCopy("same");
	// // CINTPurity4->DrawCopy("same");

	// TLegend * leg1 = new TLegend(0.4,0.2,0.90,0.4);
	// leg1->SetHeader("CINT7 Purity Factor");
	// leg1->AddEntry(CINTPurityCINT7PS,"CINT7-PS","pe");
	// leg1->AddEntry(CINTPurityCINT7,"CINT7-VDM","pe");
	// leg1->Draw("same");

	// c1->cd(2);

	// CINTPurityCINT7PS->SetTitle("Ratio");
	// CINTPurityCINT7PS->Divide(CINTPurityCINT7);

	// TLegend * leg2 = new TLegend(0.4,0.2,0.90,0.4);
	// leg2->SetHeader("Ratio");
	// leg2->AddEntry(CINTPurityCINT7PS,"CINT7-PS/CINT7-VDM","pe");

	// CINTPurityCINT7PS->GetYaxis()->SetRangeUser(0.95,1.1);
	// CINTPurityCINT7PS->Draw("");
	// // CINTPurityCINT7->DrawCopy("same");
	// leg2->Draw("same");



    // ---- P.U plot ----
	TCanvas* c2 = new TCanvas();
	// c2->Divide(1,2,0,0);

	// c2->cd(1);

	CINTPU1->SetLineColor(2);
	CINTPU3->SetLineColor(4);
	// CINTPU2->SetLineColor(3);

	CINTPU1->SetTitle("Purity Corrected Pile-up Factor");
	CINTPU1->GetYaxis()->SetTitle("P.U");
	// CINTPU1->GetYaxis()->SetTitleSize(0.09);
	// CINTPU2->GetYaxis()->SetRangeUser(0.9,1.1);

	CINTPU1->Draw("");
	// CINTPU2->DrawCopy("same");
	CINTPU3->DrawCopy("same");

	TLegend * leg3 = new TLegend(0.4,0.2,0.90,0.4);
	leg3->SetHeader("Pile-up Factor");
	leg3->AddEntry(CINTPU1,"CINT7","pe");
	leg3->AddEntry(CINTPU3,"C0TVX","pe");
	// leg3->AddEntry(CINTPU2,"V0-PS","pe");
	leg3->Draw("same");

	// c1->cd(2);

	// CINTPU1->SetTitle("Ratio");
	// CINTPU1->Divide(CINTPU3);
	// // CINTPU2->Divide(CINTPU1);

	// TLegend * leg4 = new TLegend(0.4,0.2,0.90,0.4);
	// leg4->SetHeader("Ratio");
	// leg4->AddEntry(CINTPU1,"V0-VDM/T0-VDM","pe");

	// CINTPU1->GetYaxis()->SetRangeUser(0.95,1.1);
	// CINTPU1->Draw();
	// // CINTPU2->DrawCopy("same");
	// leg4->Draw("same");






	// ---- Cross section plot ----
	TCanvas* c3 = new TCanvas();
	c3->Divide(1,2,0,0);

	c3->cd(1);

	CMULCC1->SetMarkerColor(2);
	CMULCC1->SetMarkerSize(1);
	CMULCC1->SetMarkerStyle(8);

	// CMULCC2->SetMarkerColor(3);
	// CMULCC2->SetMarkerSize(1);
	// CMULCC2->SetMarkerStyle(8);

	CMULCC3->SetMarkerColor(4);
	CMULCC3->SetMarkerSize(1);
	CMULCC3->SetMarkerStyle(8);

	// CMULCC4->SetMarkerColor(6);
	// CMULCC4->SetMarkerSize(1);
	// CMULCC4->SetMarkerStyle(8);

	CMULCC1->SetTitle("MUL Cross Section");
	CMULCC1->GetYaxis()->SetTitle("#sigma (mb)");
	CMULCC1->GetYaxis()->SetRangeUser(0.004,0.018);

	CMULCC1->DrawCopy("");
	// CMULCC2->DrawCopy("same");
	CMULCC3->DrawCopy("same");
	// CMULCC4->DrawCopy("same");

	TLegend * leg5 = new TLegend(0.4,0.2,0.90,0.4);
	leg5->SetHeader("MUL Cross-section #sigma_{MUL} = #sigma_{VDM}/FNorm");
	leg5->AddEntry(CMULCC1,"F_{norm}Off2 (CINT7)","pe");
	// leg5->AddEntry(CMULCC2,"F_{norm}Scal (CINT7)","pe");
	leg5->AddEntry(CMULCC3,"F_{norm}Scal (COTVX)","pe");
	// leg5->AddEntry(CMULCC4,"T0-PS","pe");
	leg5->Draw("same");

	c3->cd(2);

	CMULCC1->SetTitle("Ratio");
	CMULCC1->GetYaxis()->SetRangeUser(0.,1.4);

	CMULCC1->Divide(CMULCC3);
	// CMULCC2->Divide(CMULCC3);
	// CMULCC4->Divide(CMULCC1);

	TLegend * leg6 = new TLegend(0.4,0.2,0.90,0.4);
	leg6->SetHeader("Ratio");

	leg6->AddEntry(CMULCC1,"CINT7(Offline)/C0TVX","pe");
	// leg6->AddEntry(CMULCC2,"CINT7(Online)/C0TVX","pe");
	// leg6->AddEntry(CMULCC4,"T0-PS/CINT7","pe");

	// CMULCC2->GetYaxis()->SetRangeUser(0.4,2.0);
	CMULCC1->Draw("");
	// CMULCC2->Draw("same");
	// CMULCC4->DrawCopy("same");
	leg6->Draw("same");





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
	// Compute the MUL cross-section from an FNorm histo and a VDM reference.
	std::set<int> runs= RunNumbers(a1);

	//Create histo
	TH1*h  = ((TH1*)h1->Clone("MUL Cross-section"));
	h->Reset();

	Double_t sum          =0.;
	Double_t sumerr2      =0.;
	Double_t sumFnorm     =0.;
	Double_t sumFnormerr2 =0.;

	int i= 1;
	// Run by run loop
	for ( std::set<int>::const_iterator it = runs.begin(); it != runs.end(); ++it ){

		Int_t runNumber    = *it;

		//Getting FNorm
		Double_t FNorm     = h1->GetBinContent(i);
		//Getting FNorm stat. error
		Double_t FNormErr  = h1->GetBinError(i);
		//Getting MUL
		Double_t nMUL      = a1->CC()->GetSum(Form("run:%d/trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL",runNumber));
		// MUL Cross-section Error
		Double_t CMULCCErr = FNormErr/FNorm ;
		// MUL Cross-section
		Double_t CMULCC    = VdMCC/FNorm;
		 if(runNumber == 244411) {
		 	cout << "______________________________________________" << endl;
		 	printf("Fnorm for run 244411  and trigger %s = %f +/- %f \n",trigger.Data(),FNorm,FNormErr );
		 	printf("MUL for run 244411 = %f \n",nMUL );
		 	printf("CMUL for run 244411 = %f +/- %f \n",CMULCC,CMULCCErr );
		 	cout << "______________________________________________" << endl;
		 }
		sum                = sum +nMUL/CMULCC;
		sumerr2            = sumerr2 + nMUL*nMUL*FNormErr*FNormErr/VdMCC/VdMCC;

		sumFnorm           = sumFnorm + FNorm*nMUL;
		sumFnormerr2       = sumFnormerr2 + nMUL*nMUL*FNormErr*FNormErr;

		// printf("----\n");
		// printf(" -----  sum for run %d                     = %.0f  \n",runNumber,sum );
		// printf(" -----  sumerr2  for run %d                = %.0f  \n",runNumber,sumerr2 );
		// printf(" -----  Relative error on FNorm for run %d = %f %% \n",runNumber,100*CMULCCErr );
		// printf(" -----  Absolute error on FNorm for run %d = %f  \n",runNumber,FNormErr );
		// printf("----\n");

		h->SetBinContent(i,CMULCC);
		h->SetBinError(i,CMULCC*CMULCCErr);

		i++;
	}
	Double_t nMULTot = a1->CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL"));
	printf("nMULTot = %f\n",nMULTot );

	Double_t Lumi      = sumFnorm/VdMCC;
	Double_t sigmaLumi = TMath::Sqrt(sumFnormerr2)/VdMCC;

	printf("Sum of Fnorm             : %f  \n",sumFnorm);
	printf("Luminosity for trigger %s: %f +/- %f nb^-1 ( %f percent ) +/-  %f percent  \n",trigger.Data(),Lumi/1000000,sigmaLumi/1000000,100*sigmaLumi/Lumi,100*VdMCCErr/VdMCC);
	printf("FNorm for %s             : %.5f +/- %0.5f ( %f percent ) \n",h1->GetName(), sumFnorm/nMULTot, TMath::Sqrt(sumFnormerr2)/nMULTot, 100*TMath::Sqrt(sumFnormerr2)/sumFnorm );
	printf("______________________\n");
	// cout << a1->CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL"))<< endl;

	return h;
}
