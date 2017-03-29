

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
void ComparisonPbPb2DBINWithAnalysisNote(){

  LoadStyle();

//------------------------------------------------------
// PbPb values @ 5TeV
//------------------------------------------------------

const int npt=12;
Double_t pt[npt]       ={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11.};
Double_t ept[npt]      ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
Double_t ept_syst[npt] ={0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.6};

Double_t epsilonpt[npt]={0.5+0.2,1.5+0.2,2.5+0.2,3.5+0.2,4.5+0.2,5.5+0.2,6.5+0.2,7.5+0.2,8.5+0.2,9.5+0.2,11.+0.2};


const int nptvar=8;
Double_t pt_var[nptvar]       ={0.5,1.5,2.5,3.5,4.5,5.5,7.0,10.0};
Double_t ept_var[nptvar]      ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
Double_t ept_syst_var[nptvar] ={0.3,0.3,0.3,0.3,0.3,0.3,0.6,0.9};


//______________________________________________
// --- Results versus pt in 0-20% the old A.N ---
//______________________________________________

Double_t NofJPsi_old[npt] = {36836,55865,39081,21576,11227,6299,4672,3157,1527,998,550,542};
Double_t estat_old[npt]    = {1292,1256,932,540,365,242,167,129,105,71,56,45};
Double_t esyst_old[npt]    = {760,1022,769,451,210,77,78,69,28,28,15,17};

Double_t NofJPsi_old_var[nptvar]  = {0,0,0,0,0,0,0,0};
Double_t estat_old_var[nptvar]    = {0,0,0,0,0,0,0,0};
Double_t esyst_old_var[nptvar]    = {0,0,0,0,0,0,0,0};


// Compute the new bins
for (int i = 0; i < npt; ++i)
{
  if(i<6)
  {
    NofJPsi_old_var[i] = NofJPsi_old[i];
    estat_old_var[i]   = estat_old[i];
    esyst_old_var[i]   = esyst_old[i];
  }

  if( i == 7 || i == 8) {
    NofJPsi_old_var[6] +=  NofJPsi_old[i];
    estat_old_var[6]   +=  estat_old[i]*estat_old[i];
    esyst_old_var[6]   +=  esyst_old[i]*esyst_old[i];
  }

  if( i == 9 || i == 10 || i == 11 ) {
      NofJPsi_old_var[7] +=  NofJPsi_old[i];
      estat_old_var[7]   +=  estat_old[i]*estat_old[i];
      esyst_old_var[7]   +=  esyst_old[i]*esyst_old[i];
    }
}

printf(" NofJPsi_old_var[6] before = %f\n", NofJPsi_old_var[6]);
NofJPsi_old_var[6] = NofJPsi_old_var[6];
estat_old_var[6]   = TMath::Sqrt(estat_old_var[6]);
esyst_old_var[6]   = TMath::Sqrt(esyst_old_var[6]);

printf(" NofJPsi_old_var[7] before = %f\n", NofJPsi_old_var[7]);
NofJPsi_old_var[7] = NofJPsi_old_var[7];
estat_old_var[7]   = TMath::Sqrt(estat_old_var[7]);
esyst_old_var[7]   = TMath::Sqrt(esyst_old_var[7]);

// Data points and error stat.
TGraphErrors *gData_old = new TGraphErrors(nptvar,pt_var,NofJPsi_old_var,ept_var,estat_old_var);
gData_old->SetMarkerStyle(20);
gData_old->SetMarkerColor(2);
gData_old->SetLineColor(2);
gData_old->SetMarkerSize(0.5);

// Data points and error stat.
TGraphErrors *gData_old_syst = new TGraphErrors(nptvar,pt_var,NofJPsi_old_var,ept_var,esyst_old_var);
gData_old_syst->SetMarkerStyle(20);
gData_old_syst->SetMarkerColor(2);
gData_old_syst->SetLineColor(2);
gData_old_syst->SetMarkerSize(0.5);
gData_old_syst->SetFillStyle(0);


//______________________________________________
// --- Results versus pt in the new A.N ---
//______________________________________________

// 2.5<y<3.25
Double_t NofJPsi_2D1[nptvar]  = {19735,31760,21995,13016,6953,3824,2842,1402};
Double_t estat_2D1[nptvar]    = {1005,999,675,422,339,184,124,82};
Double_t esyst_2D1[nptvar]    = {473,485,429,452,227,112,67,21};

// 3.25<y<4
Double_t NofJPsi_2D2[nptvar]  = {17322,25289,17526,8972,4634,2594,1946,729};
Double_t estat_2D2[nptvar]    = {734,757,545,371,215,151,94,63};
Double_t esyst_2D2[nptvar]    = {598,688,559,155,77,75,22,17};

// --- Compute sum ---

Double_t NofJPsi_sum[nptvar] = {0,0,0,0,0,0,0,0};
Double_t estat_sum[nptvar]   = {0,0,0,0,0,0,0,0};
Double_t esyst_sum[nptvar]   = {0,0,0,0,0,0,0,0};

for (int i = 0; i < nptvar; ++i)
{
  NofJPsi_sum[i]= (NofJPsi_2D1[i] + NofJPsi_2D2[i]);
  estat_sum[i]  = TMath::Sqrt(estat_2D1[i]*estat_2D1[i] + estat_2D2[i]*estat_2D2[i]);
  estat_sum[i]  = TMath::Sqrt(esyst_2D1[i]*esyst_2D1[i] + esyst_2D2[i]*esyst_2D2[i]);
}


// Data points and error stat.
TGraphErrors *gData_sum = new TGraphErrors(nptvar,pt_var,NofJPsi_sum,ept_var,estat_sum);
gData_sum->SetMarkerStyle(20);
gData_sum->SetMarkerColor(4);
gData_sum->SetLineColor(4);
gData_sum->SetMarkerSize(0.5);

// Data points and error stat.
TGraphErrors *gData_sum_syst = new TGraphErrors(nptvar,pt_var,NofJPsi_sum,ept_var,esyst_sum);
gData_sum_syst->SetMarkerStyle(20);
gData_sum_syst->SetMarkerColor(4);
gData_sum_syst->SetLineColor(4);
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
TH2D *hnull = new TH2D("hnull","hnull",100,0.,12.,100,0,120000);
hnull->GetXaxis()->SetTitle("#it{p}_{T}");
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

Double_t Diff_Data_cent[nptvar];
Double_t estatDiff_Data_cent[nptvar];
Double_t esysDiff_Data_cent[nptvar];

// Compute Diff points and Errors
for(int i=0;i<nptvar;i++){

    // Points
    Diff_Data_cent[i]    = NofJPsi_old_var[i] - NofJPsi_sum[i];
    printf("Diff_Data_cent[%d] = %f\n",i,Diff_Data_cent[i] );
    //Error (full)
    estatDiff_Data_cent[i] = estat_old_var[i];
    printf("estatDiff_Data_cent[%d] = %f\n",i,estatDiff_Data_cent[i] );
    esysDiff_Data_cent[i]  = esyst_old_var[i];
    printf("esysDiff_Data_cent[%d] = %f\n",i,esysDiff_Data_cent[i] );

}

  TGraphErrors *gDiff_Data_cent = new TGraphErrors(nptvar,pt_var,Diff_Data_cent,0,estatDiff_Data_cent);
  gDiff_Data_cent->SetMarkerStyle(20);
  gDiff_Data_cent->SetMarkerColor(kRed);
  gDiff_Data_cent->SetMarkerSize(0.5);

  TGraphErrors *gDiffSys_Data_cent = new TGraphErrors(nptvar,pt_var,Diff_Data_cent,ept_syst_var,esysDiff_Data_cent);
  gDiffSys_Data_cent->SetMarkerStyle(20);
  gDiffSys_Data_cent->SetMarkerColor(kRed);
  gDiffSys_Data_cent->SetMarkerSize(0.5);
  gDiffSys_Data_cent->SetMarkerSize(0.5);
  gDiffSys_Data_cent->SetFillStyle(0);
  gDiffSys_Data_cent->SetLineColor(1);

  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,0.,12.,100,-5000,5000);
  hnull2->GetXaxis()->SetTitle("#it{p}_{T}");
  hnull2->GetYaxis()->SetTitle("Difference");
  hnull2->GetYaxis()->SetTitleOffset(0.7);

  hnull2->Draw();
  gDiff_Data_cent->Draw("P");
  gDiffSys_Data_cent->Draw("E2Psame");

  TLine*l = new TLine(0.,0.,12.,0.);
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
  TLegendEntry* entry=leg->AddEntry("","stat. uncert. and syst. from previous A.N","P");
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