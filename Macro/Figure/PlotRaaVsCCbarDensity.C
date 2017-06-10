#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TAxis.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TLegend.h>
#include <TROOT.h>

/**
 *   Plot The rapidity Raas versus ccbar density
 */


// Prediction Cacciari
//
// # Job started on: Tue May 30 10:29:09 CEST 2017 ..
// # FONLL heavy quark hadroproduction cross section, calculated on Tue May 30 10:29:21 CEST 2017
// # FONLL version and perturbative order: ## FONLL v1.3.2 fonll [ds/dpt^2dy (pb/GeV^2)]
// # quark = charm
// # final state = quark
// # ebeam1 = 2515, ebeam2 = 2515
// # PDF set = CTEQ6.6
// # ptmin = 0
// # ptmax = 12
// # ymin  = -4
// # ymax  = -2.5
// # Uncertainties from scales, masses, PDFs combined quadratically
// # cross section is ds/dy (pb)
// # y       central      min       max       min_sc     max_sc     min_mass   max_mass     min_pdf    max_pdf
//   -2.6250 3.7480e+08 1.2795e+08 8.8503e+08 1.4840e+08 8.7260e+08 2.9770e+08 4.6860e+08 3.1370e+08 4.3590e+08
//   -2.8750 3.5610e+08 1.2478e+08 8.4650e+08 1.4440e+08 8.3470e+08 2.8140e+08 4.4730e+08 3.0030e+08 4.1190e+08
//   -3.1250 3.3530e+08 1.2068e+08 8.0303e+08 1.3940e+08 7.9190e+08 2.6350e+08 4.2340e+08 2.8500e+08 3.8560e+08
//   -3.3750 3.1240e+08 1.1551e+08 7.5448e+08 1.3330e+08 7.4400e+08 2.4390e+08 3.9700e+08 2.6770e+08 3.5710e+08
//   -3.6250 2.8760e+08 1.0912e+08 7.0098e+08 1.2590e+08 6.9120e+08 2.2300e+08 3.6800e+08 2.4840e+08 3.2670e+08
//   -3.8750 2.6100e+08 1.0165e+08 6.4268e+08 1.1740e+08 6.3360e+08 2.0070e+08 3.3660e+08 2.2730e+08 2.9470e+08


// # Job started on: Tue May 30 10:25:02 CEST 2017 ..
// # FONLL heavy quark hadroproduction cross section, calculated on Tue May 30 10:25:13 CEST 2017
// # FONLL version and perturbative order: ## FONLL v1.3.2 fonll [ds/dpt^2dy (pb/GeV^2)]
// # quark = charm
// # final state = quark
// # ebeam1 = 1375, ebeam2 = 1375
// # PDF set = CTEQ6.6
// # ptmin = 0
// # ptmax = 8
// # ymin  = -4
// # ymax  = -2.5
// # Uncertainties from scales, masses, PDFs combined quadratically
// # cross section is ds/dy (pb)
// # y       central      min       max       min_sc     max_sc     min_mass   max_mass     min_pdf    max_pdf
//   -2.6250 2.4640e+08 9.7746e+07 5.9712e+08 1.1420e+08 5.8780e+08 1.9010e+08 3.1710e+08 2.0830e+08 2.8450e+08
//   -2.8750 2.2980e+08 9.3516e+07 5.6136e+08 1.0920e+08 5.5260e+08 1.7620e+08 2.9750e+08 1.9580e+08 2.6370e+08
//   -3.1250 2.1150e+08 8.8369e+07 5.2151e+08 1.0320e+08 5.1330e+08 1.6100e+08 2.7580e+08 1.8180e+08 2.4130e+08
//   -3.3750 1.9180e+08 8.2140e+07 4.7763e+08 9.6060e+07 4.7000e+08 1.4480e+08 2.5220e+08 1.6630e+08 2.1740e+08
//   -3.6250 1.7110e+08 7.4944e+07 4.3040e+08 8.7980e+07 4.2340e+08 1.2790e+08 2.2690e+08 1.4940e+08 1.9270e+08
//   -3.8750 1.4950e+08 6.6976e+07 3.8038e+08 7.8980e+07 3.7400e+08 1.1060e+08 2.0030e+08 1.3150e+08 1.6750e+08

// # Job started on: Tue May 30 11:04:28 CEST 2017 .
// # FONLL heavy quark hadroproduction cross section, calculated on Tue May 30 11:04:29 CEST 2017
// # FONLL version and perturbative order: ## FONLL v1.3.2 fonll [ds/dpt^2dy (pb/GeV^2)]
// # quark = charm
// # final state = quark
// # ebeam1 = 2515, ebeam2 = 2515
// # PDF set = CTEQ6.6
// # ptmin = 0
// # ptmax = 12
// # ymin  = -0.8
// # ymax  = 0.8
// # Uncertainties from scales, masses combined quadratically
// # cross section is ds/dy (pb)
// # y       central      min       max       min_sc     max_sc     min_mass   max_mass
//   -0.8000 4.5290e+08 1.4573e+08 1.0332e+09 1.5810e+08 1.0240e+09 3.6660e+08 5.5570e+08


// # Job started on: Fri Jun  2 14:11:03 CEST 2017 .
// # FONLL heavy quark hadroproduction cross section, calculated on Fri Jun  2 14:11:03 CEST 2017
// # FONLL version and perturbative order: ## FONLL v1.3.2 fonll [ds/dpt^2dy (pb/GeV^2)]
// # quark = charm
// # final state = quark
// # ebeam1 = 1375, ebeam2 = 1375
// # PDF set = CTEQ6.6
// # ptmin = 0
// # ptmax = 8
// # ymin  = 0
// # ymax  = 0
// # Uncertainties from scales, masses combined quadratically
// # cross section is ds/dy (pb)
// # y       central      min       max       min_sc     max_sc     min_mass   max_mass
//    0.0000 3.2370e+08 1.1754e+08 7.5051e+08 1.2880e+08 7.4260e+08 2.5650e+08 4.0550e+08


void LoadStyle();

void PlotRaaVsCCbarDensity()
{

	const int npoints=6;
	Double_t dummy_uncert[npoints] = {0.5e+07,0.5e+07,0.5e+07,0.5e+07,0.5e+07,0.5e+07};


	// ################## ALICE  ##################

	// 							               -2.5---------5.02 TeV---------4-
	Double_t Raa_5TeV[npoints] 				= {0.721,0.673,0.651,0.617,0.614,0.582};
	Double_t Raa_stat_5TeV[npoints] 		= {0.028,0.013,0.011,0.011,0.011,0.018};
	Double_t Raa_syst_5TeV[npoints] 		= {0.086,0.055,0.048,0.044,0.046,0.060};


	Double_t sigccbar_5TeV[npoints]   		= {3.7480e+08,3.5610e+08,3.3530e+08,3.1240e+08,2.8760e+08,2.6100e+08};
	Double_t minsigccbar_5TeV[npoints]		= {1.2795e+08,1.2478e+08,1.2068e+08,1.1551e+08,1.0912e+08,1.0165e+08};
	Double_t maxsigccbar_5TeV[npoints]		= {8.8503e+08,8.4650e+08,8.0303e+08,7.5448e+08,7.0098e+08,6.4268e+08};

	//                                         -----0.8----
	//                                         Warning : 0-10%
	Double_t Raa_5TeV_mid[1] 				= {0.89};
	Double_t Raa_stat_5TeV_mid[1] 			= {0.08};
	Double_t Raa_syst_5TeV_mid[1] 			= {0.17};


	//                                         -2.5---------2.76 TeV---------4-
	Double_t Raa_276TeV[npoints] 			= {0.631,0.747,0.632,0.566,0.467,0.395};
	Double_t Raa_stat_276TeV[npoints] 		= {0.087,0.068,0.048,0.044,0.041,0.050};
	Double_t Raa_syst_276TeV[npoints] 		= {0.088,0.097,0.094,0.088,0.070,0.050};

	Double_t sigccbar_276TeV[npoints]   	= {2.4640e+08,2.2980e+08,2.1150e+08,1.9180e+08,1.7110e+08,1.4950e+08};
	Double_t minsigccbar_276TeV[npoints]	= {9.7746e+07,9.3516e+07,8.8369e+07,8.2140e+07,7.4944e+07,6.6976e+07};
	Double_t maxsigccbar_276TeV[npoints]	= {5.9712e+08,5.6136e+08,5.2151e+08,4.7763e+08,4.3040e+08,3.8038e+08};

	//                                         -----y < |0.8|----
	Double_t Raa_276TeV_mid[1] 				= {0.72};
	Double_t Raa_stat_276TeV_mid[1] 		= {0.06};
	Double_t Raa_syst_276TeV_mid[1] 		= {0.10};

	Double_t sigccbar_276TeV_mid[1]   	    = {3.2370e+08};



	// Correctly init error
	for (int i = 0; i < npoints; ++i)
	{
		minsigccbar_5TeV[i] = sigccbar_5TeV[i] - minsigccbar_5TeV[i];
		maxsigccbar_5TeV[i] = maxsigccbar_5TeV[i] - sigccbar_5TeV[i];

		minsigccbar_276TeV[i] = sigccbar_276TeV[i] - minsigccbar_276TeV[i];
		maxsigccbar_276TeV[i] = maxsigccbar_276TeV[i] - sigccbar_276TeV[i];
	}

	// Data points and error stat.
	TGraphAsymmErrors *gData_5_stat = new TGraphAsymmErrors(npoints,sigccbar_5TeV,Raa_5TeV,dummy_uncert,dummy_uncert,Raa_stat_5TeV,Raa_stat_5TeV);
	gData_5_stat->SetMarkerStyle(20);
	gData_5_stat->SetMarkerColor(2);
	gData_5_stat->SetLineColor(2);
	gData_5_stat->SetMarkerSize(0.5);


	// Data points and error stat.
	TGraphAsymmErrors *gData_5_syst = new TGraphAsymmErrors(npoints,sigccbar_5TeV,Raa_5TeV,dummy_uncert,dummy_uncert,Raa_syst_5TeV,Raa_syst_5TeV);
	gData_5_syst->SetMarkerStyle(20);
	gData_5_syst->SetMarkerColor(2);
	gData_5_syst->SetLineColor(2);
	gData_5_syst->SetMarkerSize(0.5);
	gData_5_syst->SetFillStyle(0);

	TGraphAsymmErrors *gData_276_stat = new TGraphAsymmErrors(npoints,sigccbar_276TeV,Raa_276TeV,dummy_uncert,dummy_uncert,Raa_stat_276TeV,Raa_stat_276TeV);
	gData_276_stat->SetMarkerStyle(20);
	gData_276_stat->SetMarkerColor(4);
	gData_276_stat->SetLineColor(4);
	gData_276_stat->SetMarkerSize(0.5);

	// Data points and error stat.
	TGraphAsymmErrors *gData_276_syst = new TGraphAsymmErrors(npoints,sigccbar_276TeV,Raa_276TeV,dummy_uncert,dummy_uncert,Raa_syst_276TeV,Raa_syst_276TeV);
	gData_276_syst->SetMarkerStyle(20);
	gData_276_syst->SetMarkerColor(4);
	gData_276_syst->SetLineColor(4);
	gData_276_syst->SetMarkerSize(0.5);
	gData_276_syst->SetFillStyle(0);

	TGraphAsymmErrors *gData_276_mid_stat = new TGraphAsymmErrors(1,sigccbar_276TeV_mid,Raa_276TeV_mid,dummy_uncert,dummy_uncert,Raa_stat_276TeV_mid,Raa_stat_276TeV_mid);
	gData_276_mid_stat->SetMarkerStyle(20);
	gData_276_mid_stat->SetMarkerColor(6);
	gData_276_mid_stat->SetLineColor(6);
	gData_276_mid_stat->SetMarkerSize(0.5);

	// Data points and error stat.
	TGraphAsymmErrors *gData_276_mid_syst = new TGraphAsymmErrors(1,sigccbar_276TeV_mid,Raa_276TeV_mid,dummy_uncert,dummy_uncert,Raa_syst_276TeV_mid,Raa_syst_276TeV_mid);
	gData_276_mid_syst->SetMarkerStyle(20);
	gData_276_mid_syst->SetMarkerColor(6);
	gData_276_mid_syst->SetLineColor(6);
	gData_276_mid_syst->SetMarkerSize(0.5);
	gData_276_mid_syst->SetFillStyle(0);


	// ################## PHENIX  ##################

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
	leg0->AddEntry(gData_276_mid_stat," PbPb@2.76TeV 0-90% #it{p}_{T} < 8 GeV/#it{c}; y < |0.8| ","PE");
	leg0->AddEntry(gData_276_mid_syst,"syst. uncert. for R_{AA}","f");



	//---------------------------------------------------
	// plot
	//---------------------------------------------------

	// Create canvas
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
	TCanvas *c = new TCanvas("c","c",20,20,800,800);
	c->SetGrid();
	LoadStyle();

	// TH1D *hRAA = new TH1D("hRAA","",10,0.,6e+08,100,0.,1.5);
	TH1D *hRAA = new TH1D("hRAA","",10,0.,6e+08);
	hRAA->GetXaxis()->SetTitle("d#sigma_{c#bar{c}}/d#it{y} (pb)");
	hRAA->GetYaxis()->SetTitle("R_{AA}");
	hRAA->GetYaxis()->SetRangeUser(0,1.1);
	hRAA->GetXaxis()->SetRangeUser(0,4e+08);

		// gData_5_stat->GetYaxis()->SetTitle("R_{AA}");
	// gData_5_stat->GetXaxis()->SetTitle("d#sigma_{c#bar{c}}/d#it{y} (pb)");

	hRAA->Draw("");
	gData_5_stat->Draw("PEsame");
	gData_5_syst->Draw("E2Psame");
	gData_276_stat->Draw("PEsame");
	gData_276_syst->Draw("E2Psame");
	gData_276_mid_stat->Draw("PEsame");
	gData_276_mid_syst->Draw("E2Psame");
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