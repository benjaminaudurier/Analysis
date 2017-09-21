#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLegend.h>
#include <TROOT.h>
#include <CC_FONLL.h>
#include <RAA.h>
#include <dnchdeta.h>
#include <iostream>
#include <fstream>
using namespace std;

/**
 *   Plot dNch/deta distributions
 */

// Intervals to integrated fit functions
int ny =7;
Double_t y[7]        ={2.5,2.75,3.0,3.25,3.50,3.75,4};

void LoadStyle();
void EvalNchVersusRapidity(TF1 *fitfunction,int nbincent,const char* FitFunctionName);

//______________________________________________
void PlotNchDistributionTeV()
{

	// Correctly init error
	for (int i = 0; i < 6; ++i)
	{
		minsigccbar_5TeV[i]   = sigccbar_5TeV[i] - minsigccbar_5TeV[i];
		maxsigccbar_5TeV[i]   = maxsigccbar_5TeV[i] - sigccbar_5TeV[i];

		minsigccbar_276TeV[i] = sigccbar_276TeV[i] - minsigccbar_276TeV[i];
		maxsigccbar_276TeV[i] = maxsigccbar_276TeV[i] - sigccbar_276TeV[i];
	}

	// Make bins and axes
	Double_t	dnchdeta_axe_5TeV[34];
	Double_t	dnchdy_axe_5TeV[34];
	Double_t	dummy_axe_5TeV[34];

	for (int i = 0; i < 34; i++) {
		dnchdeta_axe_5TeV[i] 	= dnchdeta_5TeV_bin[i] + (dnchdeta_5TeV_bin[i+1]-dnchdeta_5TeV_bin[i])/2. ;
		dnchdy_axe_5TeV[i] 		= dnchdy_5TeV_bin[i]+ (dnchdy_5TeV_bin[i+1]-dnchdy_5TeV_bin[i])/2.;
		dummy_axe_5TeV[i] 		= 0.;
	}

	Double_t	dnchdeta_axe_276TeV[42];
	Double_t	dnchdy_axe_276TeV[42];
	Double_t	dummy_axe_276TeV[42];

	for (int i = 0; i < 42; i++) {
		dnchdeta_axe_276TeV[i] 	= dnchdeta_276TeV_bin[i] + (dnchdeta_276TeV_bin[i+1]-dnchdeta_276TeV_bin[i])/2.;
		dnchdy_axe_276TeV[i] 		= dnchdy_276TeV_bin[i] +(dnchdy_276TeV_bin[i+1]-dnchdy_276TeV_bin[i])/2.;
		dummy_axe_276TeV[i] 		= 0.;
	}


	//---------------------------------------------------
	// Create Graphs
	//---------------------------------------------------


	// dNch/dEta
	TGraphErrors *gNchVsEta_5TeV[10];
	for (int i = 0; i < 10; i++) {
		gNchVsEta_5TeV[i] = new TGraphErrors(34);
		gNchVsEta_5TeV[i]->SetMarkerStyle(20);
		gNchVsEta_5TeV[i]->SetMarkerColor(i+4);
		gNchVsEta_5TeV[i]->SetLineColor(i+4);
		gNchVsEta_5TeV[i]->SetMarkerSize(0.5);
		for (int j = 0; j < 34; j++) {
			gNchVsEta_5TeV[i]->SetPoint(j,dnchdeta_axe_5TeV[j],dnchdeta_5TeV[j][i]);
			gNchVsEta_5TeV[i]->SetPointError(j,dummy_axe_5TeV[j],dnchdeta_5TeV_syst[j][i]);
		}
	}

	TGraphErrors *gNchVsEta_276TeV[4];
	for (int i = 0; i < 4; i++) {
		gNchVsEta_276TeV[i] = new TGraphErrors(42);
		gNchVsEta_276TeV[i]->SetMarkerStyle(20);
		gNchVsEta_276TeV[i]->SetMarkerColor(i+4);
		gNchVsEta_276TeV[i]->SetLineColor(i+4);
		gNchVsEta_276TeV[i]->SetMarkerSize(0.5);
		for (int j = 0; j < 42; j++) {
			gNchVsEta_276TeV[i]->SetPoint(j,dnchdeta_axe_276TeV[j],dnchdeta_276TeV[j][i]);
			gNchVsEta_276TeV[i]->SetPointError(j,dummy_axe_276TeV[j],dnchdeta_276TeV_syst[j][i]);
		}
	}

	// dNch/dy
	TGraphErrors *gNchVsRapidty_5TeV = new TGraphErrors(34,dnchdy_axe_5TeV,dnchdy_5TeV,dummy_axe_5TeV,dnchdy_5TeV_syst);
	gNchVsRapidty_5TeV->SetMarkerStyle(20);
	gNchVsRapidty_5TeV->SetMarkerColor(1);
	gNchVsRapidty_5TeV->SetLineColor(1);
	gNchVsRapidty_5TeV->SetMarkerSize(0.5);

	TGraphErrors *gNchVsRapidty_276TeV = new TGraphErrors(42,dnchdy_axe_276TeV,dnchdy_276TeV,dummy_axe_276TeV,dnchdy_276TeV_syst);
	gNchVsRapidty_276TeV->SetMarkerStyle(20);
	gNchVsRapidty_276TeV->SetMarkerColor(1);
	gNchVsRapidty_276TeV->SetLineColor(1);
	gNchVsRapidty_276TeV->SetMarkerSize(0.5);

	// dNch/dy from dNch/dEta for 2.37 < y < 4.07
	TGraphErrors *gNchVsY_5TeV[10];
	for (int i = 0; i < 10; i++) {
		gNchVsY_5TeV[i] = new TGraphErrors(7);
		gNchVsY_5TeV[i]->SetMarkerStyle(20);
		gNchVsY_5TeV[i]->SetMarkerColor(i+1);
		gNchVsY_5TeV[i]->SetLineColor(i+1);
		if(i==9){
			gNchVsY_5TeV[i]->SetMarkerColor(i+2);
			gNchVsY_5TeV[i]->SetLineColor(i+2);
		}
		gNchVsY_5TeV[i]->SetMarkerSize(0.5);
		for (int j = 0; j < 7; j++) {
			gNchVsY_5TeV[i]->SetPoint(j,dnchdy_axe_5TeV[24+j],dnchdeta_5TeV[24+j][i]);
			gNchVsY_5TeV[i]->SetPointError(j,dummy_axe_5TeV[24+j],dnchdeta_5TeV_syst[24+j][i]);
		}
	}

	TGraphErrors *gNchVsY_276TeV[4];
	for (int i = 0; i < 4; i++) {
		gNchVsY_276TeV[i] = new TGraphErrors(7);
		gNchVsY_276TeV[i]->SetMarkerStyle(20);
		gNchVsY_276TeV[i]->SetMarkerColor(i+1);
		gNchVsY_276TeV[i]->SetLineColor(i+1);
		if(i==9){
			gNchVsY_276TeV[i]->SetMarkerColor(i+2);
			gNchVsY_276TeV[i]->SetLineColor(i+2);
		}
		gNchVsY_276TeV[i]->SetMarkerSize(0.5);
		for (int j = 0; j < 7; j++) {
			gNchVsY_276TeV[i]->SetPoint(j,dnchdy_axe_276TeV[30+j],dnchdeta_276TeV[30+j][i]);
			gNchVsY_276TeV[i]->SetPointError(j,dummy_axe_276TeV[30+j],dnchdeta_276TeV_syst[30+j][i]);
		}
	}

	// dNch/dEta - dNch/dy graph
	TGraphErrors *gRapidtyEtaDiff_5TeV = new TGraphErrors(34);
	gRapidtyEtaDiff_5TeV->SetMarkerStyle(20);
	gRapidtyEtaDiff_5TeV->SetMarkerColor(1);
	gRapidtyEtaDiff_5TeV->SetLineColor(1);
	gRapidtyEtaDiff_5TeV->SetMarkerSize(0.5);
	for (int j = 0; j < 34; j++) {
		double	point 		= dnchdeta_5TeV[j][0]-dnchdy_5TeV[j];
		gRapidtyEtaDiff_5TeV->SetPoint(j,dnchdeta_axe_5TeV[j],point);
		gRapidtyEtaDiff_5TeV->SetPointError(j,dummy_axe_5TeV[j],dnchdy_5TeV_syst[j]);
	}

	// dNch/dEta - dNch/dy graph
	TGraphErrors *gRapidtyEtaDiff_276TeV = new TGraphErrors(42);
	gRapidtyEtaDiff_276TeV->SetMarkerStyle(20);
	gRapidtyEtaDiff_276TeV->SetMarkerColor(1);
	gRapidtyEtaDiff_276TeV->SetLineColor(1);
	gRapidtyEtaDiff_276TeV->SetMarkerSize(0.5);
	for (int j = 0; j < 42; j++) {
		double	point 		= dnchdeta_276TeV[j][0]-dnchdy_276TeV[j];
		gRapidtyEtaDiff_276TeV->SetPoint(j,dnchdeta_axe_276TeV[j],point);
		gRapidtyEtaDiff_276TeV->SetPointError(j,dummy_axe_276TeV[j],dnchdy_276TeV_syst[j]);
	}

	// Fit functions to evaluate dNch/dy
	TF1 *fitfunctiongauss[10];
	for (size_t i = 0; i < 10; i++) {
		fitfunctiongauss[i] = new TF1("f1", "gaus",-5, 5);
		fitfunctiongauss[i]->FixParameter(2,0);
		fitfunctiongauss[i]->SetLineColor(1);
		fitfunctiongauss[i]->SetLineWidth(1);
		fitfunctiongauss[i]->SetLineStyle(1);
	}

	TF1 *fitfunctionpol2[10];
	for (size_t i = 0; i < 10; i++) {
		fitfunctionpol2[i] = new TF1("f1", "pol2",-5, 5);
		fitfunctionpol2[i]->SetLineColor(2);
		fitfunctionpol2[i]->SetLineWidth(1);
		fitfunctionpol2[i]->SetLineStyle(2);
	}

	TF1 *fitfunctionpol4[10];
	for (size_t i = 0; i < 10; i++) {
		fitfunctionpol4[i] = new TF1("f1", "pol4",-5, 5);
		fitfunctionpol4[i]->SetLineColor(6);
		fitfunctionpol4[i]->SetLineWidth(1);
		fitfunctionpol4[i]->SetLineStyle(6);
	}



	//---------------------------------------------------
	// plot
	//---------------------------------------------------

	// Create canvas
	// gStyle->SetOptStat(0);
	// gStyle->SetOptTitle(0);
	// LoadStyle();

	TCanvas *c0 = new TCanvas("c0","c0",20,20,800,800);
	c0->Divide(2,1);
	c0->cd(1);


	// ==== dNch/deta distributions ====

	// Config. Legend
	TLegend*leg0 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg0->SetBorderSize(0);
	leg0->SetFillColor(10);
	leg0->SetFillStyle(1);
	leg0->SetLineStyle(0);
	leg0->SetLineColor(0);
	leg0->SetTextSize(0.03);

	for (size_t i = 0; i < 10; i++) {
		if(i==0) {
			gNchVsEta_5TeV[i]->Draw("AP");
			gNchVsEta_5TeV[i]->GetYaxis()->SetRangeUser(0,2500);
			gNchVsEta_5TeV[i]->GetYaxis()->SetTitle("dNch/d#eta");
			gNchVsEta_5TeV[i]->GetXaxis()->SetTitle("#eta");
			gNchVsEta_5TeV[i]->SetTitle("dNch/d#eta for #sqrt{s_{NN}} = 5.02 TeV");
		} else gNchVsEta_5TeV[i]->Draw("Psame");

		leg0->AddEntry(gNchVsEta_5TeV[i],Form("centrality %.0f-%.0f",Centrality_bin[i],Centrality_bin[i+1]),"PE");
	}
	leg0->Draw("same");


	c0->cd(2);

	// Config. Legend
	TLegend*leg276 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg276->SetBorderSize(0);
	leg276->SetFillColor(10);
	leg276->SetFillStyle(1);
	leg276->SetLineStyle(0);
	leg276->SetLineColor(0);
	leg276->SetTextSize(0.03);

	for (size_t i = 0; i < 4; i++) {
		if(i==0) {
			gNchVsEta_276TeV[i]->Draw("AP");
			gNchVsEta_276TeV[i]->GetYaxis()->SetRangeUser(0,2500);
			gNchVsEta_276TeV[i]->GetYaxis()->SetTitle("dNch/d#eta");
			gNchVsEta_276TeV[i]->GetXaxis()->SetTitle("#eta");
			gNchVsEta_276TeV[i]->SetTitle("dNch/d#eta for #sqrt{s_{NN}} = 2.76 TeV");
		} else gNchVsEta_276TeV[i]->Draw("Psame");

		leg276->AddEntry(gNchVsEta_276TeV[i],Form("centrality %.0f-%.0f",Centrality_bin[i],Centrality_bin[i+1]),"PE");
	}
	leg276->Draw("same");

	// ==== dNch/dy distributions and fit ====

	TCanvas *c1 = new TCanvas("c1","c1",20,20,800,800);
	c1->Divide(2,1);
	c1->cd(1);

	// Config. Legend
	TLegend*leg1 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg1->SetBorderSize(0);
	leg1->SetFillColor(10);
	leg1->SetFillStyle(1);
	leg1->SetLineStyle(0);
	leg1->SetLineColor(0);
	leg1->SetTextSize(0.03);

	TLegend*legfit1 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	legfit1->SetBorderSize(0);
	legfit1->SetFillColor(10);
	legfit1->SetFillStyle(1);
	legfit1->SetLineStyle(0);
	legfit1->SetLineColor(0);
	legfit1->SetTextSize(0.03);

	for (size_t i = 0; i < 10; i++) {
		if(i==0)gNchVsY_5TeV[i]->Draw("AP");
		else 		gNchVsY_5TeV[i]->Draw("Psame");

		gNchVsY_5TeV[i]->GetYaxis()->SetRangeUser(0,2500);
		gNchVsY_5TeV[i]->GetYaxis()->SetTitle("dNch/d#it{y}");
		gNchVsY_5TeV[i]->GetXaxis()->SetTitle("#it{y}");
		gNchVsY_5TeV[i]->SetTitle("dNch/d#it{y} for #sqrt{s_{NN}} = 5.02 TeV");

		gNchVsY_5TeV[i]->Fit(fitfunctiongauss[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctiongauss[i],i,"gauss_5TeV");
		fitfunctiongauss[i]->DrawCopy("same");

		gNchVsY_5TeV[i]->Fit(fitfunctionpol2[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctionpol2[i],i,"pol2_5TeV");
		fitfunctionpol2[i]->DrawCopy("same");

		gNchVsY_5TeV[i]->Fit(fitfunctionpol4[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctionpol4[i],i,"pol4_5TeV");
		fitfunctionpol4[i]->DrawCopy("same");

		leg1->AddEntry(gNchVsY_5TeV[i],Form("centrality %.0f-%.0f",Centrality_bin[i],Centrality_bin[i+1]),"PE");
	}
	legfit1->AddEntry(fitfunctiongauss[1],"gauss","l");
	legfit1->AddEntry(fitfunctionpol2[1],"pol2","l");
	legfit1->AddEntry(fitfunctionpol4[1],"pol4","l");

	leg1->Draw("same");
	legfit1->Draw("same");


	c1->cd(2);

	// Config. Legend
	TLegend*leg1276 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg1276->SetBorderSize(0);
	leg1276->SetFillColor(10);
	leg1276->SetFillStyle(1);
	leg1276->SetLineStyle(0);
	leg1276->SetLineColor(0);
	leg1276->SetTextSize(0.03);

	TLegend*legfit276 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	legfit276->SetBorderSize(0);
	legfit276->SetFillColor(10);
	legfit276->SetFillStyle(1);
	legfit276->SetLineStyle(0);
	legfit276->SetLineColor(0);
	legfit276->SetTextSize(0.03);

	for (size_t i = 0; i < 4; i++) {
		if(i==0)gNchVsY_276TeV[i]->Draw("AP");
		else 		gNchVsY_276TeV[i]->Draw("Psame");

		gNchVsY_276TeV[i]->GetYaxis()->SetRangeUser(0,2500);
		gNchVsY_276TeV[i]->GetYaxis()->SetTitle("dNch/d#it{y}");
		gNchVsY_276TeV[i]->GetXaxis()->SetTitle("#it{y}");
		gNchVsY_276TeV[i]->SetTitle("dNch/d#it{y} for #sqrt{s_{NN}} = 2.76 TeV");

		gNchVsY_276TeV[i]->Fit(fitfunctiongauss[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctiongauss[i],i,"gauss_276TeV");
		fitfunctiongauss[i]->DrawCopy("same");

		gNchVsY_276TeV[i]->Fit(fitfunctionpol2[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctionpol2[i],i,"pol2_276TeV");
		fitfunctionpol2[i]->DrawCopy("same");

		gNchVsY_276TeV[i]->Fit(fitfunctionpol4[i],"0QERS","",2.4,4);
		EvalNchVersusRapidity(fitfunctionpol4[i],i,"pol4_276TeV");
		fitfunctionpol4[i]->DrawCopy("same");

		leg1276->AddEntry(gNchVsY_276TeV[i],Form("centrality %.0f-%.0f",Centrality_bin[i],Centrality_bin[i+1]),"PE");
	}
	legfit276->AddEntry(fitfunctiongauss[1],"gauss","l");
	legfit276->AddEntry(fitfunctionpol2[1],"pol2","l");
	legfit276->AddEntry(fitfunctionpol4[1],"pol4","l");

	leg1276->Draw("same");
	legfit276->Draw("same");


	// ==== dNch/deta - dNch/dy distributions ====


	TCanvas *c3 = new TCanvas("c3","c3",20,20,800,800);
	c3->Divide(2,2);
	c3->cd(1);
	gPad->SetLogy();
	gNchVsRapidty_5TeV->SetTitle(" dNch/dy for #sqrt{s_{NN}} = 5.02 TeV");
	gNchVsRapidty_5TeV->GetYaxis()->SetTitle("dNch/d#it{y}");
	gNchVsRapidty_5TeV->GetXaxis()->SetTitle("#it{y}");
	gNchVsRapidty_5TeV->Draw("AP");
	gNchVsRapidty_5TeV->GetYaxis()->SetRangeUser(0,2500);
	gNchVsRapidty_5TeV->GetXaxis()->SetRangeUser(0,5);
	gNchVsEta_5TeV[0]->Draw("Psame");

	c3->cd(3);
	gRapidtyEtaDiff_5TeV->SetTitle("dNch/deta - dNch/dy ");
	gRapidtyEtaDiff_5TeV->GetYaxis()->SetTitle("Difference");
	gRapidtyEtaDiff_5TeV->GetXaxis()->SetTitle("#it{y}");
	gRapidtyEtaDiff_5TeV->Draw("AP");
	TF1* pol = new TF1("function","pol0",0,5);
	gRapidtyEtaDiff_5TeV->Fit(pol,"ER","",2.5,4);
	gRapidtyEtaDiff_5TeV->GetXaxis()->SetRangeUser(0,5);
	pol->Draw("same");

	c3->cd(2);
	gPad->SetLogy();
	gNchVsRapidty_276TeV->SetTitle(" dNch/dy for #sqrt{s_{NN}} = 2.76 TeV");
	gNchVsRapidty_276TeV->GetYaxis()->SetTitle("dNch/d#it{y}");
	gNchVsRapidty_276TeV->GetXaxis()->SetTitle("#it{y}");
	gNchVsRapidty_276TeV->Draw("AP");
	gNchVsRapidty_276TeV->GetYaxis()->SetRangeUser(0,2500);
	gNchVsRapidty_276TeV->GetXaxis()->SetRangeUser(0,5);
	gNchVsEta_276TeV[0]->Draw("Psame");

	c3->cd(4);
	gRapidtyEtaDiff_276TeV->SetTitle("dNch/deta - dNch/dy ");
	gRapidtyEtaDiff_276TeV->GetYaxis()->SetTitle("Difference");
	gRapidtyEtaDiff_276TeV->GetXaxis()->SetTitle("#it{y}");
	gRapidtyEtaDiff_276TeV->Draw("AP");
	TF1* pol1 = new TF1("function1","pol0",0,5);
	gRapidtyEtaDiff_276TeV->Fit(pol1,"ER","",2.5,4);
	gRapidtyEtaDiff_276TeV->GetXaxis()->SetRangeUser(0,5);
	pol1->Draw("same");

}


//______________________________________________
void EvalNchVersusRapidity(TF1 *fitfunction,int nbincent,const char* FitFunctionName){

	/**
 	* Estimate dNch/dy from fit functions
 	*/

	printf("\n---- Centrality %0.f-%0.f %% ----\n",Centrality_bin[nbincent],Centrality_bin[nbincent+1] );

  ofstream myfile;
	myfile.open(Form("%s/cent_%0.f_%0.f.txt",FitFunctionName,Centrality_bin[nbincent],Centrality_bin[nbincent+1]));

	// Loop over rapidity
	for (int j = 0; j < ny-1; j++) {
		double nch 	= fitfunction->Integral(y[j],y[j+1])/(y[j+1]-y[j]);
		double dnch = fitfunction->IntegralError(y[j],y[j+1])/(y[j+1]-y[j]);

		printf(" --- Nch for function %s and %f < y < %f : %f +/- %f \n",FitFunctionName,y[j],y[j+1],nch,dnch );

		myfile << Form("%.2f-%.2f %f %f \n",y[j],y[j+1],nch,dnch);
	}

	myfile.close();
}

//______________________________________________
void LoadStyle(){
    int font = 42;
  gROOT->SetStyle("Plain");
  // gStyle->SetFrameBorderMode(0);
  // gStyle->SetFrameFillColor(0);
  // gStyle->SetCanvasBorderMode(0);
  // gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(10);
  gStyle->SetCanvasColor(10);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleBorderSize(1);
  gStyle->SetStatColor(10);
  gStyle->SetStatBorderSize(1);
  gStyle->SetLegendBorderSize(1);
  gStyle->SetDrawBorder(0);
  gStyle->SetTextFont(font);
  gStyle->SetStatFont(font);
  gStyle->SetStatFontSize(0.05);
  gStyle->SetStatX(0.97);
  gStyle->SetStatY(0.98);
  gStyle->SetStatH(0.03);
  gStyle->SetStatW(0.3);
  gStyle->SetTickLength(0.02,"y");
  gStyle->SetEndErrorSize(3);
  gStyle->SetLabelSize(0.03,"xyz");
  gStyle->SetLabelFont(font,"xyz");
  gStyle->SetLabelOffset(0.01,"xyz");
  gStyle->SetTitleFont(font,"xyz");
  gStyle->SetTitleOffset(1.,"x");
  gStyle->SetTitleOffset(1.5,"y");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.03,"yz");
  gStyle->SetMarkerSize(1.3);
  gStyle->SetPalette(1,0);
  // gROOT->ForceStyle();
  // gStyle->SetOptStat(0);
  // gStyle->SetOptTitle(0);
  gStyle->SetLineWidth(2);
  gStyle->SetLegendFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(10);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
}
