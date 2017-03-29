

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
Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey);


//______________________________________________
void ComparisonNofJPsiYBINWithAnalysisNote(){

  LoadStyle();

//------------------------------------------------------
// pp values @ 5TeV
//------------------------------------------------------


// --- new y binning ---

// Number of JPsi are already dy/dy normalized, this is the sum of the results from the old A.N (with multiple bins)

const int ny=3;

Double_t y[ny]       ={2.75,3.25,3.75};
Double_t epsilony[ny]={2.75+0.05,3.25+0.05,3.75+0.05};
// Double_t ey[ny]      ={0.5,0.5,0.5,0.5,0.5,0.5,1.0,2.0};
Double_t ey[ny]      ={0.1,0.1,0.1};
Double_t ey_syst[ny] ={0.1,0.1,0.1};

Double_t NofJPsi[ny]       ={2452,4443,1783};
Double_t estat_NofJPsi[ny] ={68,89,55};
Double_t esyst_NofJPsi[ny] ={73,60,39};
Double_t etotglobal_NofJPsi[ny];
Double_t etotfull[ny];


// -- Compute corr. and total error ---

for(int i=0;i<ny;i++){
  // global error
  etotglobal_NofJPsi[i]=(2.16/100.)*NofJPsi[i];
  // global2 + stat2 + syst2
  etotfull[i]=TMath::Sqrt( esyst_NofJPsi[i]*esyst_NofJPsi[i]+estat_NofJPsi[i]*estat_NofJPsi[i]);
}


// Normalize to dy and bin width

Double_t deltapt=1.;
// Double_t deltay[ny]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.}; //CHECK
Double_t deltay[ny]={1.,1.,1.}; //CHECK

for(int i=0;i<ny;i++){

  // NofJPsi[i]=NofJPsi[i]/deltay/deltay[i];
  // estat_NofJPsi[i]=estat_NofJPsi[i]/deltay/deltay[i];
  // esyst_NofJPsi[i]=esyst_NofJPsi[i]/deltay/deltay[i];
  // etotglobal_NofJPsi[i]=etotglobal_NofJPsi[i]/deltay/deltay[i];

  printf("i=%d NofJPsi= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  NofJPsi[i],
  estat_NofJPsi[i],
  estat_NofJPsi[i]/NofJPsi[i]*100.,
  esyst_NofJPsi[i],
  esyst_NofJPsi[i]/NofJPsi[i]*100.,
  etotglobal_NofJPsi[i]/NofJPsi[i]*100.);
}
// Data points and error stat.
TGraphErrors *gData = new TGraphErrors(ny,y,NofJPsi,ey,estat_NofJPsi);
gData->SetMarkerStyle(20);
gData->SetMarkerColor(2);
gData->SetLineColor(2);
gData->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_syst = new TGraphErrors(ny,y,NofJPsi,ey_syst,esyst_NofJPsi);
gData_syst->SetMarkerStyle(20);
gData_syst->SetMarkerColor(2);
gData_syst->SetMarkerSize(0.5);
gData_syst->SetFillStyle(0);
// gData_syst->SetFillColor(0);
gData_syst->SetLineColor(2);


//---------------------------------
// new y values
//---------------------------------


Double_t NofJPsi_YNEW[ny]       ={2467,4444,1778};
Double_t estat_NofJPsi_YNEW[ny] ={68,91,58};
Double_t esyst_NofJPsi_YNEW[ny] ={92,138,67};
Double_t etotglobal_NofJPsi_YNEW[ny];
Double_t etotfull_NofJPsi_YNEW[ny];
for(int i=0;i<ny;i++){
  // global error
  etotglobal_NofJPsi_YNEW[i]=(2.16/100.)*NofJPsi_YNEW[i];
  // global2 + stat2 + syst2
  etotfull_NofJPsi_YNEW[i]=TMath::Sqrt( esyst_NofJPsi_YNEW[i]*esyst_NofJPsi_YNEW[i]+estat_NofJPsi_YNEW[i]*estat_NofJPsi_YNEW[i]);
}

for(int i=0;i<ny;i++){

  // NofJPsi[i]=NofJPsi[i]/deltay/deltay[i];
  // estat_NofJPsi[i]=estat_NofJPsi[i]/deltay/deltay[i];
  // esyst_NofJPsi[i]=esyst_NofJPsi[i]/deltay/deltay[i];
  // etotglobal_NofJPsi[i]=etotglobal_NofJPsi[i]/deltay/deltay[i];

  printf("i=%d NofJPsi_YNEW= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  NofJPsi_YNEW[i],
  estat_NofJPsi_YNEW[i],
  estat_NofJPsi_YNEW[i]/NofJPsi_YNEW[i]*100.,
  esyst_NofJPsi_YNEW[i],
  esyst_NofJPsi_YNEW[i]/NofJPsi_YNEW[i]*100.,
  etotglobal_NofJPsi_YNEW[i]/NofJPsi_YNEW[i]*100.);
}

// Data points and error stat.
TGraphErrors *gData_YNEW = new TGraphErrors(ny,epsilony,NofJPsi_YNEW,ey,estat_NofJPsi_YNEW);
gData_YNEW->SetMarkerStyle(20);
gData_YNEW->SetMarkerColor(1);
gData_YNEW->SetLineColor(1);
gData_YNEW->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_YNEW_syst = new TGraphErrors(ny,epsilony,NofJPsi_YNEW,ey_syst,esyst_NofJPsi_YNEW);
gData_YNEW_syst->SetMarkerStyle(20);
gData_YNEW_syst->SetMarkerColor(1);
gData_YNEW_syst->SetMarkerSize(0.5);
gData_YNEW_syst->SetFillStyle(0);
// gData_YNEW_syst->SetFillColor(0);
gData_YNEW_syst->SetLineColor(1);


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
TH2D *hnull = new TH2D("hnull","hnull",100,2.5,4.,100,0,6000);
hnull->GetXaxis()->SetTitle("#it{y}");
hnull->GetYaxis()->SetTitle("Number of J/#psi ");
hnull->GetYaxis()->SetTitleOffset(0.7);
hnull->Draw();
gData->Draw("PEsame");
gData_syst->Draw("E2Psame");
gData_YNEW->Draw("PEsame");
gData_YNEW_syst->Draw("E2Psame");

// Config. Legend
TLegend*leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gData_YNEW,"new y bins ","PE");
leg0->AddEntry(gData_YNEW_syst,"syst. uncert. ","f");
leg0->AddEntry(gData,"pp data @ 5TeV","PE");
leg0->AddEntry(gData_syst,"syst. uncert. ","f");
leg0->Draw();

// //----------------------------------------------
// // Difference
// //----------------------------------------------

Double_t Diff_Data_NEWPT[ny];
Double_t estatDiff_Data_NEWPT[ny];
Double_t esysDiff_Data_NEWPT[ny];

// Compute Diff points and Errors
for(int i=0;i<ny;i++){

    // Points
    Diff_Data_NEWPT[i]    = NofJPsi[i] - NofJPsi_YNEW[i];
    //Error (full)
    estatDiff_Data_NEWPT[i] = estat_NofJPsi_YNEW[i];
    esysDiff_Data_NEWPT[i]  = esyst_NofJPsi_YNEW[i];
}

  TGraphErrors *gDiff_Data_NEWPT = new TGraphErrors(ny,y,Diff_Data_NEWPT,0,estatDiff_Data_NEWPT);
  gDiff_Data_NEWPT->SetMarkerStyle(20);
  gDiff_Data_NEWPT->SetMarkerColor(kRed);
  gDiff_Data_NEWPT->SetMarkerSize(0.5);

  TGraphErrors *gDiffSys_Data_NEWPT = new TGraphErrors(ny,y,Diff_Data_NEWPT,ey_syst,esysDiff_Data_NEWPT);
  gDiffSys_Data_NEWPT->SetMarkerStyle(20);
  gDiffSys_Data_NEWPT->SetMarkerColor(kRed);
  gDiffSys_Data_NEWPT->SetMarkerSize(0.5);
  gDiffSys_Data_NEWPT->SetMarkerSize(0.5);
  gDiffSys_Data_NEWPT->SetFillStyle(0);
  gDiffSys_Data_NEWPT->SetLineColor(1);

  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,2.5,4,100,-200,200);
  hnull2->GetXaxis()->SetTitle("#it{y}");
  hnull2->GetYaxis()->SetTitle("Difference");
  hnull2->GetYaxis()->SetTitleOffset(0.7);

  hnull2->Draw();
  gDiff_Data_NEWPT->Draw("P");
  gDiffSys_Data_NEWPT->Draw("E2Psame");
  TLine*l = new TLine(2.5,0.,4.,0.);
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
  TLegendEntry* entry=leg->AddEntry("","stat. uncert. and syst. from new measurements","P");
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
