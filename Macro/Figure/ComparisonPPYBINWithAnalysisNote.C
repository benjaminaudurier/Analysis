

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
void ComparisonPPYBINWithAnalysisNote(){

  LoadStyle();

//------------------------------------------------------
// pp values @ 5TeV
//------------------------------------------------------

// --- A.N ---

// functions are already dy/dy normalized

// const int ny          =6;
// Double_t y[ny]        ={2.625,2.875,3.125,3.375,3.625,3.875};
// Double_t ey[ny]       ={0.,0.,0.,0.,0.,0.};
// Double_t ey_syst[ny]  ={0.03,0.03,0.03,0.03,0.03,0.03};
// Double_t cs[ny]       ={4.594,4.346,3.989,3.643,3.097,2.519};
// Double_t estat_cs[ny] ={0.264,0.136,0.112,0.104,0.107,0.165};
// Double_t esyst_cs[ny] ={0.293,0.190,0.155,0.132,0.109,0.133};



// --- new y binning ---

// functions are already dy/dy normalized

const int ny=3;

Double_t y[ny]       ={2.75,3.25,3.75};
Double_t epsilony[ny]={2.75+0.05,3.25+0.05,3.75+0.05};
// Double_t ey[ny]      ={0.5,0.5,0.5,0.5,0.5,0.5,1.0,2.0};
Double_t ey[ny]      ={0.1,0.1,0.1};
Double_t ey_syst[ny] ={0.1,0.1,0.1};

Double_t cs[ny]       ={ 4.4705,3.816,2.808};
Double_t estat_cs[ny] ={ 0.149,0.0764,0.0983};
Double_t esyst_cs[ny] ={ 0.1746,0.1018,0.0860};
Double_t etotglobal_cs[ny];
Double_t etotfull[ny];


// -- Compute corr. and total error ---

for(int i=0;i<ny;i++){
  // global error
  etotglobal_cs[i]=(2.16/100.)*cs[i];
  // global2 + stat2 + syst2
  etotfull[i]=TMath::Sqrt( esyst_cs[i]*esyst_cs[i]+estat_cs[i]*estat_cs[i]);
}


// Normalize to dy and bin width

Double_t deltapt=1.;
// Double_t deltay[ny]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.}; //CHECK
Double_t deltay[ny]={1.,1.,1.}; //CHECK

for(int i=0;i<ny;i++){

  // cs[i]=cs[i]/deltay/deltay[i];
  // estat_cs[i]=estat_cs[i]/deltay/deltay[i];
  // esyst_cs[i]=esyst_cs[i]/deltay/deltay[i];
  // etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltay[i];

  printf("i=%d cs= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  cs[i],
  estat_cs[i],
  estat_cs[i]/cs[i]*100.,
  esyst_cs[i],
  esyst_cs[i]/cs[i]*100.,
  etotglobal_cs[i]/cs[i]*100.);
}
// Data points and error stat.
TGraphErrors *gData = new TGraphErrors(ny,y,cs,ey,estat_cs);
gData->SetMarkerStyle(20);
gData->SetMarkerColor(2);
gData->SetLineColor(2);
gData->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_syst = new TGraphErrors(ny,y,cs,ey_syst,esyst_cs);
gData_syst->SetMarkerStyle(20);
gData_syst->SetMarkerColor(2);
gData_syst->SetMarkerSize(0.5);
gData_syst->SetFillStyle(0);
// gData_syst->SetFillColor(0);
gData_syst->SetLineColor(2);


//---------------------------------
// new y values
//---------------------------------


Double_t cs_PTNEW[ny]       ={4.5346,3.8238,2.8266};
Double_t estat_cs_PTNEW[ny] ={0.1247,0.0787,0.0914};
Double_t esyst_cs_PTNEW[ny] ={0.2437,0.1469,0.1488};
Double_t etotglobal_cs_PTNEW[ny];
Double_t etotfull_cs_PTNEW[ny];
for(int i=0;i<ny;i++){
  // global error
  etotglobal_cs_PTNEW[i]=(2.16/100.)*cs_PTNEW[i];
  // global2 + stat2 + syst2
  etotfull_cs_PTNEW[i]=TMath::Sqrt( esyst_cs_PTNEW[i]*esyst_cs_PTNEW[i]+estat_cs_PTNEW[i]*estat_cs_PTNEW[i]);
}

for(int i=0;i<ny;i++){

  // cs[i]=cs[i]/deltay/deltay[i];
  // estat_cs[i]=estat_cs[i]/deltay/deltay[i];
  // esyst_cs[i]=esyst_cs[i]/deltay/deltay[i];
  // etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltay[i];

  printf("i=%d cs_PTNEW= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  cs_PTNEW[i],
  estat_cs_PTNEW[i],
  estat_cs_PTNEW[i]/cs_PTNEW[i]*100.,
  esyst_cs_PTNEW[i],
  esyst_cs_PTNEW[i]/cs_PTNEW[i]*100.,
  etotglobal_cs_PTNEW[i]/cs_PTNEW[i]*100.);
}

// Data points and error stat.
TGraphErrors *gData_PTNEW = new TGraphErrors(ny,epsilony,cs_PTNEW,ey,estat_cs_PTNEW);
gData_PTNEW->SetMarkerStyle(20);
gData_PTNEW->SetMarkerColor(1);
gData_PTNEW->SetLineColor(1);
gData_PTNEW->SetMarkerSize(0.5);

// Data points and error syst.
TGraphErrors *gData_PTNEW_syst = new TGraphErrors(ny,epsilony,cs_PTNEW,ey_syst,esyst_cs_PTNEW);
gData_PTNEW_syst->SetMarkerStyle(20);
gData_PTNEW_syst->SetMarkerColor(1);
gData_PTNEW_syst->SetMarkerSize(0.5);
gData_PTNEW_syst->SetFillStyle(0);
// gData_PTNEW_syst->SetFillColor(0);
gData_PTNEW_syst->SetLineColor(1);


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
TH2D *hnull = new TH2D("hnull","hnull",100,2.5,4,100,0.,5.5);
hnull->GetXaxis()->SetTitle("#it{y}");
hnull->GetYaxis()->SetTitle("d#sigma/dy (#mub/GeV/c)");
hnull->GetYaxis()->SetTitleOffset(0.7);
hnull->Draw();
gData->Draw("PEsame");
gData_syst->Draw("E2Psame");
gData_PTNEW->Draw("PEsame");
gData_PTNEW_syst->Draw("E2Psame");


// Config. Legend
TLegend*leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gData_PTNEW,"new y bins ","PE");
leg0->AddEntry(gData_PTNEW_syst,"syst. uncert. ","f");
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
    Diff_Data_NEWPT[i]    = cs[i] - cs_PTNEW[i];
    //Error (full)
    estatDiff_Data_NEWPT[i] = estat_cs_PTNEW[i];
    esysDiff_Data_NEWPT[i]  = esyst_cs_PTNEW[i];
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
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,2.5,4.,100,-1,1);
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
