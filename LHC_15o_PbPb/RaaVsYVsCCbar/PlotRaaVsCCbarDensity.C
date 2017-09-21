#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TROOT.h>
#include "CC_FONLL.h"
#include "RAA.h"
#include "dnchdeta.h"

/**
 *   Plot The rapidity Raas versus ccbar density
 */


// values for 0-30 %
double Taa_5TeV 	=  6.22;
double dTaa_5TeV 	=  0.2;

double Taa_276TeV 	=  6.31;
double dTaa_276TeV 	=  0.21;

void LoadStyle();
void NormalizeCrossSection(int nbin, Double_t * x_axis_5TeV, Double_t * x_axis_5TeV_syst,Double_t * x_axis_276TeV, Double_t * x_axis_276TeV_syst, Bool_t print );
void PlotCCBarCrossSection();

void PlotRaaVsCCbarDensity(Bool_t sigmaccbar_only = kTRUE, Bool_t print = kTRUE)
{

	const int npoints=6; // Number of Raa points for each colliding energy
	Double_t dummy_uncert[npoints] = {0.5e+07,0.5e+07,0.5e+07,0.5e+07,0.5e+07,0.5e+07};

	Double_t x_axis_5TeV[npoints];
	for (size_t i = 0; i < npoints; i++) x_axis_5TeV[i] = sigccbar_5TeV[i];

	Double_t x_axis_5TeV_syst[npoints];
	for (size_t i = 0; i < npoints; i++) x_axis_5TeV_syst[i] = 0.5e+07;

	Double_t x_axis_276TeV[npoints];
	for (size_t i = 0; i < npoints; i++) x_axis_276TeV[i] = sigccbar_276TeV[i];

	Double_t x_axis_276TeV_syst[npoints];
	for (size_t i = 0; i < npoints; i++) x_axis_276TeV_syst[i] = 0.5e+07;

	PlotCCBarCrossSection();
	return;


	// // Correctly init error
	// for (int i = 0; i < npoints; ++i)
	// {
	// 	minsigccbar_5TeV[i]   = sigccbar_5TeV[i] - minsigccbar_5TeV[i];
	// 	maxsigccbar_5TeV[i]   = maxsigccbar_5TeV[i] - sigccbar_5TeV[i];
	//
	// 	minsigccbar_276TeV[i] = sigccbar_276TeV[i] - minsigccbar_276TeV[i];
	// 	maxsigccbar_276TeV[i] = maxsigccbar_276TeV[i] - sigccbar_276TeV[i];
	// }

	if (!sigmaccbar_only) NormalizeCrossSection(npoints,x_axis_5TeV,x_axis_5TeV_syst,x_axis_276TeV,x_axis_276TeV_syst,print);

	// Data points and error stat.
	TGraphAsymmErrors *gData_5_stat = new TGraphAsymmErrors(npoints,x_axis_5TeV,Raa_5TeV,x_axis_5TeV_syst,x_axis_5TeV_syst,Raa_stat_5TeV,Raa_stat_5TeV);
	gData_5_stat->SetMarkerStyle(20);
	gData_5_stat->SetMarkerColor(2);
	gData_5_stat->SetLineColor(2);
	gData_5_stat->SetMarkerSize(0.5);

	// Data points and error stat.
	TGraphAsymmErrors *gData_5_syst = new TGraphAsymmErrors(npoints,x_axis_5TeV,Raa_5TeV,x_axis_5TeV_syst,x_axis_5TeV_syst,Raa_syst_5TeV,Raa_syst_5TeV);
	gData_5_syst->SetMarkerStyle(20);
	gData_5_syst->SetMarkerColor(2);
	gData_5_syst->SetLineColor(2);
	gData_5_syst->SetMarkerSize(0.5);
	gData_5_syst->SetFillStyle(0);

	TGraphAsymmErrors *gData_276_stat = new TGraphAsymmErrors(npoints,x_axis_276TeV,Raa_276TeV,x_axis_276TeV_syst,x_axis_276TeV_syst,Raa_stat_276TeV,Raa_stat_276TeV);
	gData_276_stat->SetMarkerStyle(20);
	gData_276_stat->SetMarkerColor(4);
	gData_276_stat->SetLineColor(4);
	gData_276_stat->SetMarkerSize(0.5);

	// Data points and error stat.
	TGraphAsymmErrors *gData_276_syst = new TGraphAsymmErrors(npoints,x_axis_276TeV,Raa_276TeV,x_axis_276TeV_syst,x_axis_276TeV_syst,Raa_syst_276TeV,Raa_syst_276TeV);
	gData_276_syst->SetMarkerStyle(20);
	gData_276_syst->SetMarkerColor(4);
	gData_276_syst->SetLineColor(4);
	gData_276_syst->SetMarkerSize(0.5);
	gData_276_syst->SetFillStyle(0);

	TGraphAsymmErrors *gData_276_mid_stat = new TGraphAsymmErrors(1,sigccbar_276TeV_mid,Raa_276TeV_mid,x_axis_276TeV_syst,x_axis_276TeV_syst,Raa_stat_276TeV_mid,Raa_stat_276TeV_mid);
	gData_276_mid_stat->SetMarkerStyle(20);
	gData_276_mid_stat->SetMarkerColor(6);
	gData_276_mid_stat->SetLineColor(6);
	gData_276_mid_stat->SetMarkerSize(0.5);

	// Data points and error stat.
	TGraphAsymmErrors *gData_276_mid_syst = new TGraphAsymmErrors(1,sigccbar_276TeV_mid,Raa_276TeV_mid,x_axis_276TeV_syst,x_axis_276TeV_syst,Raa_syst_276TeV_mid,Raa_syst_276TeV_mid);
	gData_276_mid_syst->SetMarkerStyle(20);
	gData_276_mid_syst->SetMarkerColor(6);
	gData_276_mid_syst->SetLineColor(6);
	gData_276_mid_syst->SetMarkerSize(0.5);
	gData_276_mid_syst->SetFillStyle(0);

	// Config. Legend
	TLegend*leg0 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg0->SetBorderSize(0);
	leg0->SetFillColor(10);
	leg0->SetFillStyle(1);
	leg0->SetLineStyle(0);
	leg0->SetLineColor(0);
	leg0->SetTextSize(0.03);
	leg0->AddEntry(gData_5_stat," PbPb@5.02TeV 0-90% #it{p}_{T} < 12 GeV/#it{c} ","PE");
	leg0->AddEntry(gData_5_syst,"syst. uncert. for R_{AA} (global 3.9%) ","f");
	leg0->AddEntry(gData_276_stat," PbPb@2.76TeV 0-90% #it{p}_{T} < 8 GeV/#it{c} ","PE");
	leg0->AddEntry(gData_276_syst,"syst. uncert. for R_{AA} (global 8%)","f");
	// leg0->AddEntry(gData_276_mid_stat," PbPb@2.76TeV 0-90% #it{p}_{T} < 8 GeV/#it{c}; y < |0.8| ","PE");
	// leg0->AddEntry(gData_276_mid_syst,"syst. uncert. for R_{AA}","f");



	//---------------------------------------------------
	// plot
	//---------------------------------------------------

	// Create canvas
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TCanvas *c = new TCanvas("c","c",20,20,800,800);
	c->SetGrid();
	// LoadStyle();

	// TH2D *hRAA = new TH2D("hRAA","",10,0.,6e+08,100,0.,1.5);
	TH1D *hRAA =  sigmaccbar_only ? new TH1D("hRAA","",10,0.,6e+08) : new TH1D("hRAA","",10,10000.,100000) ;
	if(sigmaccbar_only)hRAA->GetXaxis()->SetTitle("d#sigma_{c#bar{c}}/d#it{y} (pb)");
	else
		{
			hRAA->GetXaxis()->SetTitle("#frac{d#sigma_{c#bar{c}}}{d#it{y}}/<nch>_{0-30%} (pb)");
			hRAA->GetXaxis()->SetTitle("#frac{d#sigma_{c#bar{c}}}{d#it{y}}/<nch>_{0-30%} (pb)");
	}
	hRAA->GetYaxis()->SetTitle("R_{AA}");
	hRAA->GetYaxis()->SetRangeUser(0,1.1);
	// hRAA->GetXaxis()->SetRangeUser(0,4e+08);
	if(sigmaccbar_only)hRAA->GetXaxis()->SetRangeUser(0,4e+08);
	else hRAA->GetXaxis()->SetRangeUser(40000,80000);

		// gData_5_stat->GetYaxis()->SetTitle("R_{AA}");
	// gData_5_stat->GetXaxis()->SetTitle("d#sigma_{c#bar{c}}/d#it{y} (pb)");
	//

	hRAA->Draw("");
	gData_5_stat->Draw("PEsame");
	gData_5_syst->Draw("E2same");
	gData_276_stat->Draw("PEsame");
	gData_276_syst->Draw("E2Psame");
	// gData_276_mid_stat->Draw("PEsame");
	// gData_276_mid_syst->Draw("E2Psame");
	leg0->Draw("same");


}

//______________________________________________
void NormalizeCrossSection(int nbin, Double_t * x_axis_5TeV, Double_t * x_axis_5TeV_syst,Double_t * x_axis_276TeV, Double_t * x_axis_276TeV_syst,Bool_t print )
{
	/**
	 * Change the x axis varaible for ccbar cross-section to cc_bar * <Taa> / <nch>
	 */

	for (int i = 0; i < nbin; i++) {
		x_axis_5TeV[i]  			= x_axis_5TeV[i]/dnchdy_030_502TeV[i] ;
		if(print) printf("sigma_ccbar_5TeV = %f / dnch/dy = %f -> point = %.f\n", sigccbar_5TeV[i],dnchdy_030_502TeV[i],  x_axis_5TeV[i] );
		x_axis_5TeV_syst[i]  	= x_axis_5TeV[i]*dnchdy_030_502TeV_syst[i]/dnchdy_030_502TeV[i];
		// if(print) printf("systsigma_ccbar_5TeV = %f / dnch/dy = %f -> point = %.f\n\n", sigccbar_5TeV[i],dnchdy_030_502TeV_syst[i],  x_axis_5TeV_syst[i] );

		x_axis_276TeV[i]			= x_axis_276TeV[i]/dnchdy_030_276TeV[i] ;
		if(print) printf("sigma_ccbar_276 = %f / dnch/dy = %f -> point = %.f\n", sigccbar_276TeV[i],dnchdy_030_276TeV[i],  x_axis_276TeV[i] );
		x_axis_276TeV_syst[i]	= x_axis_276TeV[i]*dnchdy_030_276TeV_syst[i]/dnchdy_030_276TeV[i] ;
		// if(print) printf("systsigma_ccbar_276 = %f / dnch/dy = %f -> point = %.f\n\n\n", x_axis_276TeV[i],dnchdy_030_276TeV_syst[i],  x_axis_276TeV_syst[i] );
	}
}

//______________________________________________
void PlotCCBarCrossSection()
{
	LoadStyle();

	Double_t rapidity_axes[6]  = {2.6250,2.8750,3.1250,3.3750,3.6250,3.8750};
	Double_t drapidity_axes[6] = {0.125,0.125,0.125,0.125,0.125,0.125};

	// Data points and error stat.
	TGraphAsymmErrors *gSigmaCCbar_502TeV = new TGraphAsymmErrors(6,rapidity_axes,sigccbar_5TeV,drapidity_axes,drapidity_axes,minsigccbar_5TeV,maxsigccbar_5TeV);
	gSigmaCCbar_502TeV->SetMarkerStyle(20);
	gSigmaCCbar_502TeV->SetMarkerColor(4);
	gSigmaCCbar_502TeV->SetLineColor(4);
	gSigmaCCbar_502TeV->SetMarkerSize(0.5);
	gSigmaCCbar_502TeV->SetLineColor(4);
	gSigmaCCbar_502TeV->SetFillColor(4);

	TGraphAsymmErrors *gSigmaCCbar_276TeV = new TGraphAsymmErrors(6,rapidity_axes,sigccbar_276TeV,drapidity_axes,drapidity_axes,minsigccbar_276TeV,maxsigccbar_276TeV);
	gSigmaCCbar_276TeV->SetMarkerStyle(20);
	gSigmaCCbar_276TeV->SetMarkerColor(2);
	gSigmaCCbar_276TeV->SetLineColor(2);
	gSigmaCCbar_276TeV->SetMarkerSize(0.5);
	gSigmaCCbar_276TeV->SetLineColor(2);
	gSigmaCCbar_276TeV->SetFillColor(2);

	TH1D *hRAA = new TH1D("hCC502","",10,2.5,4);

	// Config. Legend
	TLegend*leg0 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
	leg0->SetBorderSize(0);
	leg0->SetFillColor(10);
	leg0->SetFillStyle(1);
	leg0->SetLineStyle(0);
	leg0->SetLineColor(0);
	leg0->SetTextSize(0.03);
	leg0->AddEntry(gSigmaCCbar_502TeV," FONLL@5.03 TeV, #it{p}_{T} < 12 GeV/#it{c} ","PE");
	leg0->AddEntry(gSigmaCCbar_276TeV," FONLL@2.75 TeV, #it{p}_{T} < 8 GeV/#it{c} ","PE");


	TCanvas* c1 = new TCanvas();
	gPad->SetLogy(1);
	hRAA->GetYaxis()->SetRangeUser(1e+07,100e+08);
	hRAA->GetYaxis()->SetTitleSize(0.04);
	hRAA->GetYaxis()->SetTitleOffset(1.2);
	hRAA->GetXaxis()->SetTitle("#it{y}");
	hRAA->GetYaxis()->SetTitle("d#sigma_{c#bar{c}}/d#it{y} (pb)");
	hRAA->Draw("");

	gSigmaCCbar_502TeV->Draw("PEsame");
	gSigmaCCbar_276TeV->Draw("PEsame");
	leg0->Draw("same");


}


//______________________________________________
void LoadStyle(){
    int font = 42;
  gROOT->SetStyle("Plain");
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
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
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelFont(font,"xyz");
  gStyle->SetLabelOffset(0.01,"xyz");
  gStyle->SetTitleFont(font,"xyz");
  gStyle->SetTitleOffset(1.,"x");
  gStyle->SetTitleOffset(0.5,"y");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.06,"yz");
  gStyle->SetMarkerSize(1.3);
  gStyle->SetPalette(1,0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetLineWidth(2);
  gStyle->SetLegendFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(10);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
}
