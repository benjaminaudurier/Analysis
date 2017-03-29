

#include <TROOT.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TMath.h>
#include <TLegendEntry.h>
#include <TStyle.h>
#include <TBox.h>


void LoadStyle();

//______________________________________________
void ComparisonNofJPsiPbPbCentralityVsAnalysisNote()
{

LoadStyle();

// Npart
const int ncent=9;
Double_t NPart_data[ncent]    ={356.0,260.1,185.8,128.5,94.7,52.4,29.77,15.27,7.49};
Double_t eNPart_data[ncent]   ={356.0+10,260.1+10,185.8+10,128.5+10,94.7+10,52.4+10,29.77+10,15.27+10,7.49+10};
Double_t ErrNPart_data[ncent] ={8.,8.,8.,8.,8.,8.,8.,8.,8.};

const int ncentvar=7;
Double_t NPart_var_data[ncentvar]    ={356.0,260.1,185.8,128.5,94.7,52.4,17.86};
Double_t eNPart_var_data[ncentvar]   ={356.0+10,260.1+10,185.8+10,128.5+10,94.7+10,52.4+10,13+10};
Double_t ErrNPart_var_data[ncentvar] ={8.,8.,8.,8.,8.,8.,8.};


//______________________________________________
// --- Results integrated in pt versus cent in the old A.N ---
//______________________________________________

Double_t NofJPsi_old[ncent] = {108070,69808,45007,24876,15010,7895,4112,2042,932};
Double_t estat_old[ncent]   = {1801,1168,822,491,321,190,105,66,37};
Double_t esyst_old[ncent]   = {1996,1161,728,630,281,147,71,34,15};
Double_t accxeff_old[ncent] = {0.13239,0.13690,0.13892,0.14088,0.14264,0.14233,0.14308,0.14363,0.14315};

Double_t NofJPsi_old_var[ncentvar] = {0,0,0,0,0,0,0};
Double_t estat_old_var[ncentvar]   = {0,0,0,0,0,0,0};
Double_t esyst_old_var[ncentvar]   = {0,0,0,0,0,0,0};
Double_t acceff[ncentvar]          = {0,0,0,0,0,0,0};


// Compute the new bins
for (int i = 0; i < ncent; ++i)
{
	if(i<ncentvar-1)
	{
		NofJPsi_old_var[i] = NofJPsi_old[i]/*/accxeff_old[i]*/;
		estat_old_var[i]   = estat_old[i];
		esyst_old_var[i]   = esyst_old[i];
	} else {
		NofJPsi_old_var[6] +=  NofJPsi_old[i]/*/accxeff_old[i]*/;
		estat_old_var[6]   +=  estat_old[i]*estat_old[i];
		esyst_old_var[6]   +=  esyst_old[i]*esyst_old[i];
	}
}

estat_old_var[6] = TMath::Sqrt(estat_old_var[6]);
esyst_old_var[6] = TMath::Sqrt(esyst_old_var[6]);

// Data points and error stat.
TGraphErrors *gData_old = new TGraphErrors(ncentvar,NPart_var_data,NofJPsi_old_var,ErrNPart_data,estat_old_var);
gData_old->SetMarkerStyle(20);
gData_old->SetMarkerColor(2);
gData_old->SetLineColor(2);
gData_old->SetMarkerSize(0.5);

// Data points and error stat.
TGraphErrors *gData_old_syst = new TGraphErrors(ncentvar,NPart_var_data,NofJPsi_old_var,ErrNPart_data,esyst_old_var);
gData_old_syst->SetMarkerStyle(20);
gData_old_syst->SetMarkerColor(2);
gData_old_syst->SetLineColor(2);
gData_old_syst->SetMarkerSize(0.5);
gData_old_syst->SetFillStyle(0);


//______________________________________________
// --- Results  versus cent in the new A.N ---
//______________________________________________
//

Double_t NofJPsi_0_2[ncentvar]  = {58251,35778,22109,11055,6729,3655,3561};
Double_t estat_0_2[ncentvar]    = {1590,1150,725,422,274,162,107};
Double_t esyst_0_2[ncentvar]    = { 1323,650,373,286,177,88,107};
Double_t acceff_0_2[ncentvar]   = {0.1197,0.1239,0.1252,0.1272,0.1275,0.1282,0.1298};

Double_t NofJPsi_2_5[ncentvar]  = {44038,29240,19456,11021,6549,3448,2839};
Double_t estat_2_5[ncentvar]    = {911,673,440,282,178,104,79};
Double_t esyst_2_5[ncentvar]    = {1298,688,567,268,185,77,44};
Double_t acceff_2_5[ncentvar]   = {0.1327,0.1389,0.1417,0.1456,0.1471,0.1465,0.1480};

Double_t NofJPsi_5_8[ncentvar]  = {6265,4872,3444,2422,1446,820,639};
Double_t estat_5_8[ncentvar]    = {242,197,139,90,56,41,33};
Double_t esyst_5_8[ncentvar]    = {153,142,42,71,32,16,20};
Double_t acceff_5_8[ncentvar]   = {0.2581,0.2675,0.2727,0.2771,0.2799,0.2812,0.2822};

Double_t NofJPsi_8_12[ncentvar] = {1201,909,683,448,310,172,131};
Double_t estat_8_12[ncentvar]   = {88,67,45,39,24,17,14};
Double_t esyst_8_12[ncentvar]   = {17,26,10,10,10,2,3};
Double_t acceff_8_12[ncentvar]  = {0.3789,0.3846,0.3892,0.4050,0.3991,0.4025,0.4084};


// --- Compute sum ---

Double_t NofJPsi_sum[ncentvar] = {0,0,0,0,0,0,0};
Double_t estat_sum[ncentvar]   = {0,0,0,0,0,0,0};
Double_t esyst_sum[ncentvar]   = {0,0,0,0,0,0,0};

for (int i = 0; i < ncentvar; ++i)
{
	NofJPsi_sum[i]= NofJPsi_0_2[i]/*/acceff_0_2[i]*/ + NofJPsi_2_5[i]/*/acceff_2_5[i]*/ + NofJPsi_5_8[i]/*/acceff_5_8[i]*/ + NofJPsi_8_12[i]/*/acceff_8_12[i]*/;
	estat_sum[i]  = TMath::Sqrt(estat_0_2[i]*estat_0_2[i] + estat_2_5[i]*estat_2_5[i] + estat_5_8[i]*estat_5_8[i] + estat_8_12[i]*estat_8_12[i]);
	estat_sum[i]  = TMath::Sqrt(esyst_0_2[i]*esyst_0_2[i] + esyst_2_5[i]*esyst_2_5[i] + esyst_5_8[i]*esyst_5_8[i] + esyst_8_12[i]*esyst_8_12[i]);
}


// Data points and error stat.
TGraphErrors *gData_sum = new TGraphErrors(ncentvar,NPart_var_data,NofJPsi_sum,ErrNPart_data,estat_sum);
gData_sum->SetMarkerStyle(20);
gData_sum->SetMarkerColor(3);
gData_sum->SetLineColor(3);
gData_sum->SetMarkerSize(0.5);

// Data points and error stat.
TGraphErrors *gData_sum_syst = new TGraphErrors(ncentvar,NPart_var_data,NofJPsi_sum,ErrNPart_data,esyst_sum);
gData_sum_syst->SetMarkerStyle(20);
gData_sum_syst->SetMarkerColor(3);
gData_sum_syst->SetLineColor(3);
gData_sum_syst->SetMarkerSize(0.5);
gData_sum_syst->SetFillStyle(0);


//---------------------------------------------------
// plot
//---------------------------------------------------

// Create canvas
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
TCanvas *c = new TCanvas("c","c",20,20,800,800);
c->Divide(1,2);

// Plot Data and interpolated CC
c->cd(1);
// gPad->SetLogy(1);
TH2D *hnull = new TH2D("hnull","hnull",100,0.,450.,100,0,120000);
hnull->GetXaxis()->SetTitle("Npart");
hnull->GetYaxis()->SetTitle("Number of J/#psi ");
hnull->GetYaxis()->SetTitleOffset(0.7);
hnull->Draw();
gData_old->Draw("PEsame");
gData_old_syst->Draw("E2Psame");
gData_sum->Draw("PEsame");
gData_sum_syst->Draw("E2Psame");


// Config. Legend
TLegend*leg0 = new TLegend(0.12,0.55,.4,.8,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gData_sum," New  Measurements ","PE");
leg0->AddEntry(gData_sum_syst,"syst. uncert. ","f");
leg0->AddEntry(gData_old,"PbPb A.N @ 5TeV","PE");
leg0->AddEntry(gData_old_syst,"syst. uncert. ","f");
leg0->Draw();


//----------------------------------------------
// Difference
//----------------------------------------------

Double_t Diff_Data_cent[ncentvar];
Double_t estatDiff_Data_cent[ncentvar];
Double_t esysDiff_Data_cent[ncentvar];

// Compute Diff points and Errors
for(int i=0;i<ncentvar;i++){

    // Points
    Diff_Data_cent[i]    = NofJPsi_old_var[i] - NofJPsi_sum[i];
    printf("Diff_Data_cent[%d] = %f\n",i,Diff_Data_cent[i] );
    //Error (full)
    estatDiff_Data_cent[i] = estat_old_var[i];
    printf("estatDiff_Data_cent[%d] = %f\n",i,estatDiff_Data_cent[i] );
    esysDiff_Data_cent[i]  = esyst_old_var[i];
    printf("esysDiff_Data_cent[%d] = %f\n",i,esysDiff_Data_cent[i] );

}

  TGraphErrors *gDiff_Data_cent = new TGraphErrors(ncentvar,NPart_var_data,Diff_Data_cent,0,estatDiff_Data_cent);
  gDiff_Data_cent->SetMarkerStyle(20);
  gDiff_Data_cent->SetMarkerColor(kRed);
  gDiff_Data_cent->SetMarkerSize(0.5);

  TGraphErrors *gDiffSys_Data_cent = new TGraphErrors(ncentvar,NPart_var_data,Diff_Data_cent,ErrNPart_var_data,esysDiff_Data_cent);
  gDiffSys_Data_cent->SetMarkerStyle(20);
  gDiffSys_Data_cent->SetMarkerColor(kRed);
  gDiffSys_Data_cent->SetMarkerSize(0.5);
  gDiffSys_Data_cent->SetMarkerSize(0.5);
  gDiffSys_Data_cent->SetFillStyle(0);
  gDiffSys_Data_cent->SetLineColor(1);

  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,0.,450.,100,-5000,1000);
  hnull2->GetXaxis()->SetTitle("Npart");
  hnull2->GetYaxis()->SetTitle("Difference");
  hnull2->GetYaxis()->SetTitleOffset(0.7);

  hnull2->Draw();
  gDiff_Data_cent->Draw("P");
  gDiffSys_Data_cent->Draw("E2Psame");

  TLine*l = new TLine(0.,0.,450.,0.);
  l->SetLineColor(8);
  l->SetLineWidth(2);
  l->Draw("same");

  // Config. Legend
  TLegend*leg = new TLegend(0.1,0.2,0.4,0.4,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.06);
  leg->SetLineColor(0);
  leg->SetLineStyle(0);
  leg->SetLineWidth(2);
  leg->SetFillColor(10);
  leg->SetFillStyle(1);
  TLegendEntry* entry=leg->AddEntry("","stat. uncert. and syst. from old A.N","P");
  entry->SetLineColor(1);
  entry->SetLineStyle(1);
  entry->SetLineWidth(1);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  leg->Draw("same");

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