#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
using namespace std; 


void PrintDistributionMacro()
{
	TFile* f1 = TFile::Open("pol2/AnalysisResults.root","READ");
	if(!f1) printf("Cannot open pol2 results\n"); 

	TFile* f2 = TFile::Open("pol4/AnalysisResults.root","READ");
	if(!f2) printf("Cannot open pol4 results\n"); 

	TFile* f3 = TFile::Open("gauss/AnalysisResults.root","READ");
	if(!f3) printf("Cannot open gauss results\n"); 

	

	TF1* poly2 = (TF1*)f1->FindObjectAny("fYFuncNew")->Clone("poly2");
	if(!poly2) printf("Cannot find poly2 function\n"); 
	poly2->SetLineColor(2);
	poly2->Print();

	TF1* poly4 = (TF1*)f2->FindObjectAny("fYFuncNew")->Clone("poly4");
	if(!poly4) printf("Cannot find poly4 function\n"); 
	poly4->SetLineColor(3);
	poly4->Print();

	TF1* gauss = (TF1*)f3->FindObjectAny("fYFuncNew")->Clone("gauss");
	if(!gauss) printf("Cannot find gauss function\n"); 
	gauss->SetLineColor(4);
	gauss->Print();

	TCanvas* c = new TCanvas();
	c->Divide(1,2,0.,0.);
	c->cd(1);

	TLegend * leg1 = new TLegend(0.52,0.1,0.89,0.38);
	leg1->SetHeader(" Jpsi Tuner functions");
	leg1->AddEntry(poly2,"poly2","l");
	leg1->AddEntry(poly4,"poly4","l");
	leg1->AddEntry(gauss,"gauss","l");

	poly2->SetTitle("Results for LHC15o");
	poly2->DrawCopy();
	poly4->DrawCopy("same");
	gauss->DrawCopy("same");
	leg1->Draw("same");
	
	c->cd(2);

	 TF1* Rat1 = new TF1("Rat1","poly2/poly4");
	 Rat1->SetRange(-4,-2.5);
	 Rat1->SetLineColor(2);
	 TF1* Rat2 = new TF1("Rat1","poly2/gauss");
	 Rat2->SetRange(-4,-2.5);
	 Rat2->SetLineColor(3);
	 TF1* Rat3 = new TF1("Rat1","poly4/gauss");
	 Rat3->SetRange(-4,-2.5);
	 Rat3->SetLineColor(4);



	TLegend * leg2 = new TLegend(0.52,0.1,0.89,0.38);
	leg2->SetHeader(" Jpsi Tuner functions ratio");
	leg2->AddEntry(Rat1,"poly2/pol4","l");
	leg2->AddEntry(Rat2,"poly2/gauss","l");
	leg2->AddEntry(Rat3,"poly4/gauss","l");

	Rat1->DrawCopy();
	Rat2->DrawCopy("same");
	Rat3->DrawCopy("same");
	leg2->Draw("same");

}




