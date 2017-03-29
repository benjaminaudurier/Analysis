

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
#include <TF1.h>
#include <TBox.h>


void LoadStyle();
Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey);


//______________________________________________
void ComparisonPP2DBINWithAnalysisNote(){

  LoadStyle();

//------------------------------------------------------
// pp values @ 5TeV
//------------------------------------------------------

// --- A.N ---

// functions are already dpt/dy normalized

// const int npt=11;
// Double_t pt[npt]       ={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11.};
// Double_t ept[npt]      ={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,1.};
// Double_t ept_syst[npt] ={0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3};
//
// Double_t cs[npt]       ={0.6509,1.1731,0.9033,0.5192,0.2304,0.1268,0.0621,0.0291,0.0144,0.0111,0.0034};
// Double_t estat_cs[npt] ={ 0.0238,0.0311,0.0273,0.0194,0.0126,0.0076,0.0051,0.0033,0.0024,0.0021,0.0007};
// Double_t esyst_cs[npt] ={ 0.033,0.052,0.036,0.021,0.014,0.005,0.0022,0.0009,0.0007,0.0011,0.00009};
// Double_t etotglobal_cs[npt];
// Double_t etotfull[npt];



// --- 2D binning ---

// functions are already dpt/dy normalized

const int npt=8;

Double_t pt[npt]       ={0.5,1.5,2.5,3.5,4.5,5.5,7.0,10.0};
Double_t epsilonpt[npt]={0.5+0.2,1.5+0.2,2.5+0.2,3.5+0.2,4.5+0.2,5.5+0.2,7.0+0.2,10.0+0.2};
// Double_t ept[npt]      ={0.5,0.5,0.5,0.5,0.5,0.5,1.0,2.0};
Double_t ept[npt]      ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
Double_t ept_syst[npt] ={0.3,0.3,0.3,0.3,0.3,0.3,0.6,0.9};

Double_t cs[npt]       ={ 0.6509,1.1731,0.9033,0.5192,0.2304,0.1268,0.0456,0.00807};
Double_t estat_cs[npt] ={ 0.0238,0.0311,0.0273,0.0194,0.0126,0.0076,0.0030,0.0008};
Double_t esyst_cs[npt] ={ 0.0330,0.0520,0.0360,0.0210,0.0140,0.0050,0.0024,0.0003};
Double_t etotglobal_cs[npt];
Double_t etotfull[npt];


// -- Compute corr. and total error ---

for(int i=0;i<npt;i++){
  // global error
  etotglobal_cs[i]=(2.16/100.)*cs[i];
  // global2 + stat2 + syst2
  etotfull[i]=TMath::Sqrt( esyst_cs[i]*esyst_cs[i]+estat_cs[i]*estat_cs[i]);
}


// Normalize to dy and bin width

Double_t deltay=1.;
// Double_t deltapt[npt]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.}; //CHECK
Double_t deltapt[npt]={1.,1.,1.,1.,1.,1.,1.,1.}; //CHECK

for(int i=0;i<npt;i++){

  // cs[i]=cs[i]/deltay/deltapt[i];
  // estat_cs[i]=estat_cs[i]/deltay/deltapt[i];
  // esyst_cs[i]=esyst_cs[i]/deltay/deltapt[i];
  // etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltapt[i];

  printf("i=%d cs= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  cs[i],
  estat_cs[i],
  estat_cs[i]/cs[i]*100.,
  esyst_cs[i],
  esyst_cs[i]/cs[i]*100.,
  etotglobal_cs[i]/cs[i]*100.);
}
// Data points and error stat.
TGraphErrors *gData = new TGraphErrors(npt,pt,cs,ept,estat_cs);
gData->SetMarkerStyle(20);
gData->SetMarkerColor(2);
gData->SetLineColor(2);
gData->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_syst = new TGraphErrors(npt,pt,cs,ept_syst,esyst_cs);
gData_syst->SetMarkerStyle(20);
gData_syst->SetMarkerColor(2);
gData_syst->SetMarkerSize(0.5);
gData_syst->SetMarkerSize(0.5);
gData_syst->SetFillStyle(0);
gData_syst->SetLineColor(2);


//---------------------------------
// 2D bin values
//---------------------------------


//  --- 3.25 < y < 4 ---

Double_t cs_2DBIN1[npt]       ={ 0.5561,1.0520,0.7466,0.4223,0.1746,0.1082,0.0280,0.0067};
Double_t estat_cs_2DBIN1[npt] ={ 0.0292,0.0396,0.0354,0.0252,0.0144,0.0095,0.0038,0.0010};
Double_t esyst_cs_2DBIN1[npt] ={0.0297,0.0521,0.0364,0.0217,0.0101,0.0061,0.0017,0.0004};
Double_t etotglobal_cs_2DBIN1[npt];
Double_t etotfull_cs_2DBIN1[npt];
for(int i=0;i<npt;i++){
  // global error
  etotglobal_cs_2DBIN1[i]=(2.16/100.)*cs_2DBIN1[i];
  // global2 + stat2 + syst2
  etotfull_cs_2DBIN1[i]=TMath::Sqrt( esyst_cs_2DBIN1[i]*esyst_cs_2DBIN1[i]+estat_cs_2DBIN1[i]*estat_cs_2DBIN1[i]);
}

for(int i=0;i<npt;i++){

  // cs[i]=cs[i]/deltay/deltapt[i];
  // estat_cs[i]=estat_cs[i]/deltay/deltapt[i];
  // esyst_cs[i]=esyst_cs[i]/deltay/deltapt[i];
  // etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltapt[i];

  // printf("i=%d cs_2DBIN1= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  // cs_2DBIN1[i],
  // estat_cs_2DBIN1[i],
  // estat_cs_2DBIN1[i]/cs_2DBIN1[i]*100.,
  // esyst_cs_2DBIN1[i],
  // esyst_cs_2DBIN1[i]/cs_2DBIN1[i]*100.,
  // etotglobal_cs_2DBIN1[i]/cs_2DBIN1[i]*100.);
}

// Data points and error stat.
TGraphErrors *gData_2DBIN1 = new TGraphErrors(npt,pt,cs_2DBIN1,ept,estat_cs_2DBIN1);
gData_2DBIN1->SetMarkerStyle(20);
gData_2DBIN1->SetMarkerColor(3);
gData_2DBIN1->SetLineColor(3);
gData_2DBIN1->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_2DBIN1_syst = new TGraphErrors(npt,pt,cs_2DBIN1,ept_syst,esyst_cs_2DBIN1);
gData_2DBIN1_syst->SetMarkerStyle(20);
gData_2DBIN1_syst->SetMarkerColor(3);
gData_2DBIN1_syst->SetMarkerSize(0.5);
gData_2DBIN1_syst->SetMarkerSize(0.5);
gData_2DBIN1_syst->SetFillStyle(0);
gData_2DBIN1_syst->SetLineColor(2);


//  --- 2.5 < y < 3.25 ---

Double_t cs_2DBIN2[npt]       ={ 0.7506,1.2930,1.0717,0.6259,0.2874,0.1472,0.0634,0.0089};
Double_t estat_cs_2DBIN2[npt] ={ 0.0447,0.0494,0.0445,0.0301,0.0196,0.0117,0.0052,0.0013};
Double_t esyst_cs_2DBIN2[npt] ={ 0.0461,0.0674,0.0522,0.0353,0.0154,0.0071,0.0029,0.0004};
Double_t etotglobal_cs_2DBIN2[npt];
Double_t etotfull_cs_2DBIN2[npt];
for(int i=0;i<npt;i++){
  // global error
  etotglobal_cs_2DBIN2[i]=(2.16/100.)*cs_2DBIN2[i];
  // global2 + stat2 + syst2
  etotfull_cs_2DBIN2[i]=TMath::Sqrt( esyst_cs_2DBIN2[i]*esyst_cs_2DBIN2[i]+estat_cs_2DBIN2[i]*estat_cs_2DBIN2[i]);
}

for(int i=0;i<npt;i++){

  // cs[i]=cs[i]/deltay/deltapt[i];
  // estat_cs[i]=estat_cs[i]/deltay/deltapt[i];
  // esyst_cs[i]=esyst_cs[i]/deltay/deltapt[i];
  // etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltapt[i];

  printf("i=%d cs_2DBIN2= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  cs_2DBIN2[i],
  estat_cs_2DBIN2[i],
  estat_cs_2DBIN2[i]/cs_2DBIN2[i]*100.,
  esyst_cs_2DBIN2[i],
  esyst_cs_2DBIN2[i]/cs_2DBIN2[i]*100.,
  etotglobal_cs_2DBIN2[i]/cs_2DBIN2[i]*100.);
}

// Data points and error stat.
TGraphErrors *gData_2DBIN2 = new TGraphErrors(npt,pt,cs_2DBIN2,ept,estat_cs_2DBIN2);
gData_2DBIN2->SetMarkerStyle(20);
gData_2DBIN2->SetMarkerColor(4);
gData_2DBIN2->SetLineColor(4);
gData_2DBIN2->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_2DBIN2_syst = new TGraphErrors(npt,pt,cs_2DBIN2,ept_syst,esyst_cs_2DBIN2);
gData_2DBIN1_syst->SetMarkerStyle(20);
gData_2DBIN1_syst->SetMarkerColor(4);
gData_2DBIN1_syst->SetMarkerSize(0.5);
gData_2DBIN1_syst->SetMarkerSize(0.5);
gData_2DBIN1_syst->SetFillStyle(0);
gData_2DBIN1_syst->SetLineColor(2);


// --- sum ---

Double_t cs_sum[npt];
Double_t estat_cs_sum[npt];
Double_t esyst_cs_sum[npt];
Double_t etotglobal_cs_sum[npt];
Double_t etotfull_cs_sum[npt];
for (int i = 0; i < npt; ++i)
{
  cs_sum[i] = (cs_2DBIN1[i]+cs_2DBIN2[i])*0.75/1.5;
  estat_cs_sum[i] = TMath::Sqrt(estat_cs_2DBIN1[i]*estat_cs_2DBIN1[i]+estat_cs_2DBIN2[i]*estat_cs_2DBIN2[i])*0.75/1.5;
  esyst_cs_sum[i] = TMath::Sqrt(esyst_cs_2DBIN1[i]*esyst_cs_2DBIN1[i]+esyst_cs_2DBIN2[i]*esyst_cs_2DBIN2[i])*0.75/1.5;
  etotglobal_cs_sum[i]=(2.16/100.)*cs_sum[i];
  etotfull_cs_sum[i]=TMath::Sqrt( esyst_cs_sum[i]*esyst_cs_sum[i]+estat_cs_sum[i]*estat_cs_sum[i]);

}

// Data points and error stat.
TGraphErrors *gData_sum = new TGraphErrors(npt,epsilonpt,cs_sum,ept,estat_cs_sum);
gData_sum->SetMarkerStyle(20);
gData_sum->SetMarkerColor(4);
gData_sum->SetLineColor(4);
gData_sum->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_sum_syst = new TGraphErrors(npt,epsilonpt,cs_sum,ept_syst,esyst_cs_sum);
gData_sum_syst->SetMarkerStyle(20);
gData_sum_syst->SetMarkerColor(4);
gData_sum_syst->SetMarkerSize(0.5);
gData_sum_syst->SetMarkerSize(0.5);
gData_sum_syst->SetFillStyle(0);
gData_sum_syst->SetLineColor(4);


// --- Division ---

Double_t ptdiv[npt+1]           ={0.,1.,2.,3.,4.,5.,6.,8.0,12.0};

Double_t cs_div[npt];
Double_t estat_cs_div[npt];
Double_t esyst_cs_div[npt];
Double_t etotglobal_cs_div[npt];
Double_t etotfull_cs_div[npt];
TH1F* h_2DBIN1 = new TH1F("2DBIN1","2DBIN1",npt,ptdiv);
TH1F* h_2DBIN2 = new TH1F("2DBIN2","2DBIN2",npt,ptdiv);
for (int i = 0; i < npt; ++i)
{

  h_2DBIN1->SetBinContent(i+1,cs_2DBIN1[i]);
  h_2DBIN1->SetBinError(i+1,etotfull_cs_2DBIN1[i] );

  h_2DBIN2->SetBinContent(i+1,cs_2DBIN2[i]);
  h_2DBIN2->SetBinError(i+1,etotfull_cs_2DBIN2[i]);


}
h_2DBIN1->SetMarkerStyle(20);
h_2DBIN1->SetMarkerColor(4);
h_2DBIN1->SetMarkerSize(0.5);
h_2DBIN1->SetMarkerSize(0.5);
h_2DBIN1->SetFillStyle(0);
h_2DBIN1->SetLineColor(4);

h_2DBIN2->SetMarkerStyle(20);
h_2DBIN2->SetMarkerColor(3);
h_2DBIN2->SetMarkerSize(0.5);
h_2DBIN2->SetMarkerSize(0.5);
h_2DBIN2->SetFillStyle(0);
h_2DBIN2->SetLineColor(3);


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
TH2D *hnull = new TH2D("hnull","hnull",100,0.,12.,100,0.001,2.5);
hnull->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
hnull->GetYaxis()->SetTitle("d#sigma/dy/d#it{p}_{T} (#mub/GeV/c)");
hnull->GetYaxis()->SetTitleOffset(0.7);
hnull->Draw();
gData->Draw("PEsame");
gData_syst->Draw("E2Psame");
gData_sum->Draw("PEsame");
gData_sum_syst->Draw("E2Psame");


// Config. Legend
TLegend*leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gData_sum,"sum of the 2 rapidity ranges ","PE");
leg0->AddEntry(gData_sum_syst,"syst.uncert.","f");
leg0->AddEntry(gData,"pp data @ 5TeV ","PE");
leg0->AddEntry(gData_syst,"syst.uncert.","f");
leg0->Draw();

// //----------------------------------------------
// // Difference
// //----------------------------------------------

Double_t Ratio_Data_2D[npt];
Double_t estatRatio_Data_2D[npt];
Double_t esysRatio_Data_2D[npt];

// Compute Ratio points and Errors
for(int i=0;i<npt;i++){

    // Points
    Ratio_Data_2D[i]    = cs[i] - cs_sum[i];
    //Error (full)
    estatRatio_Data_2D[i] = estat_cs[i];
    esysRatio_Data_2D[i]  = esyst_cs[i];
}

  TGraphErrors *gRatio_Data_2D = new TGraphErrors(npt,pt,Ratio_Data_2D,0,estatRatio_Data_2D);
  gRatio_Data_2D->SetMarkerStyle(20);
  gRatio_Data_2D->SetMarkerColor(kRed);
  gRatio_Data_2D->SetMarkerSize(0.5);

  TGraphErrors *gRatioSys_Data_2D = new TGraphErrors(npt,pt,Ratio_Data_2D,ept_syst,esysRatio_Data_2D);
  gRatioSys_Data_2D->SetMarkerStyle(20);
  gRatioSys_Data_2D->SetMarkerColor(kRed);
  gRatioSys_Data_2D->SetMarkerSize(0.5);
  gRatioSys_Data_2D->SetMarkerSize(0.5);
  gRatioSys_Data_2D->SetFillStyle(0);
  gRatioSys_Data_2D->SetLineColor(2);

  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,0.,12.,100,-0.1,0.1);
  hnull2->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
  hnull2->GetYaxis()->SetTitle("Difference");
  hnull2->GetYaxis()->SetTitleOffset(0.7);

  hnull2->Draw();
  gRatio_Data_2D->Draw("P");
  gRatioSys_Data_2D->Draw("E2Psame");
  TLine*l = new TLine(0.,0.,12.,0.);
  l->SetLineColor(8);
  l->SetLineWidth(2);
  l->Draw();

  // Config. Legend
  TLegend*leg = new TLegend(0.2772713,0.6872277,0.5569771,0.8642429,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.06);
  leg->SetLineColor(0);
  leg->SetLineStyle(0);
  leg->SetLineWidth(2);
  leg->SetFillColor(10);
  leg->SetFillStyle(1);
  TLegendEntry* entry=leg->AddEntry("","stat. uncert. and syst. from A.N","P");
  entry->SetLineColor(1);
  entry->SetLineStyle(1);
  entry->SetLineWidth(1);
  entry->SetMarkerColor(1);
  entry->SetMarkerStyle(21);
  entry->SetMarkerSize(1);
  entry->SetTextFont(42);
  // entry=leg->AddEntry("","syst. uncert. from A.N","PE");
  // entry->SetLineColor(1);
  // entry->SetLineStyle(1);
  // entry->SetLineWidth(1);
  // entry->SetMarkerColor(1);
  // entry->SetMarkerStyle(21);
  // entry->SetMarkerSize(1);
  // entry->SetTextFont(42);
  leg->Draw();


  Double_t glob = TMath::Sqrt(2.16*2.16 + 2.16*2.16)/100; ;
  TBox *box_glob = new TBox(7.8,1.-glob,8.,1+glob);
  box_glob->SetFillColor(kRed);
  box_glob->Draw("same");


  // --- Difference of the two ---

// Create canvas
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
TCanvas *c2 = new TCanvas("c2","c2",20,20,800,800);
c2->Divide(1,2);

c2->cd(1);
gPad->SetLogy(1);

h_2DBIN1->GetYaxis()->SetTitle("d#sigma/dy/d#it{p}_{T} (#mub/GeV/c)");
 TLegend*leg3 = new TLegend(0.2772713,0.6872277,0.5569771,0.8642429,NULL,"brNDC");
 leg3->AddEntry(h_2DBIN1," 3.25 < y < 4 (stat. + uncorr)","PE");
 leg3->AddEntry(h_2DBIN2," 2.5 < y < 3.25 (stat. + uncorr)","PE");
 leg3->SetBorderSize(0);
 leg3->SetTextSize(0.06);
 leg3->SetLineColor(0);
 leg3->SetLineStyle(0);
 leg3->SetLineWidth(2);
 leg3->SetFillColor(10);
 leg3->SetFillStyle(1);

  TF1 *f2DBIN1 = new TF1("f2DBIN1","x/((1+(x/[0])**2)**[1])",0,12);
  f2DBIN1->SetParameters(1,2);
  f2DBIN1->SetLineColor(4);
  f2DBIN1->SetLineWidth(0.5);
  TF1 *f2DBIN2 = new TF1("f2DBIN2","x/((1+(x/[0])**2)**[1])",0,12);
  f2DBIN2->SetParameters(1,2);
  f2DBIN2->SetLineColor(3);
  f2DBIN2->SetLineWidth(0.5);


  h_2DBIN1->Fit(f2DBIN1,"SERLI");
  h_2DBIN2->Fit(f2DBIN2,"SERLI");

 h_2DBIN1->DrawCopy();
 h_2DBIN2->DrawCopy("same");
 f2DBIN1->DrawCopy("same");
 f2DBIN2->DrawCopy("same");
 leg3->Draw("same");


c2->cd(2);

h_2DBIN1->Divide(h_2DBIN2);
h_2DBIN1->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
h_2DBIN1->GetYaxis()->SetTitle("#frac{d#sigma/d#it{p}_{T}(3.25 < y < 4)}{d#sigma/d#it{p}_{T}(2.5 < y < 3.25)}");
h_2DBIN1->GetYaxis()->SetRangeUser(0,1.2);
h_2DBIN1->GetYaxis()->SetTitleOffset(0.7);
h_2DBIN1->Draw();

TF1 *fratio = new TF1("fratio","(x/((1+(x/[0])**2)**[1]))/(x/((1+(x/[2])**2)**[3]))",pt[0],pt[npt-1]);
fratio->SetParameters(f2DBIN1->GetParameter(0),f2DBIN1->GetParameter(1),f2DBIN2->GetParameter(0),f2DBIN2->GetParameter(1));;
fratio->SetLineColor(2);

fratio->DrawCopy("same");
// f2DBIN1->DrawCopy("same");



// h_2DBIN1->Draw("");

}

//______________________________________________
Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey){
  Double_t error = TMath::Power(ex/x,2)+TMath::Power(ey/y,2);
  return TMath::Sqrt(error);
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