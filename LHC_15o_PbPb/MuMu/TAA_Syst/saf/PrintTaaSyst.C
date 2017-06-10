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


void PrintTaaSyst()
{
	const int ncent=3;

	Double_t cent[ncent]             ={10,30,65};
	Double_t ecent[ncent]            ={5,10,25};
	
	Double_t V0Mplus05[ncent]        ={173203,67563,29847};
	Double_t estat_V0Mplus05[ncent]  ={2080,893,502};
	
	Double_t V0Mminus05[ncent]       ={ 174319,67310,29048};
	Double_t estat_V0Mminus05[ncent] ={2056,988,496};


// Graph
// Data points and error stat.
TGraphErrors *gData_V0Mplus05 = new TGraphErrors(ncent,cent,V0Mplus05,ecent,estat_V0Mplus05);
gData_V0Mplus05->SetMarkerStyle(20);
gData_V0Mplus05->SetMarkerColor(1);
gData_V0Mplus05->SetLineColor(1);
gData_V0Mplus05->SetMarkerSize(0.5);

TGraphErrors *gData_V0Mminus05 = new TGraphErrors(ncent,cent,V0Mminus05,ecent,estat_V0Mminus05);
gData_V0Mminus05->SetMarkerStyle(20);
gData_V0Mminus05->SetMarkerColor(1);
gData_V0Mminus05->SetLineColor(1);
gData_V0Mminus05->SetMarkerSize(0.5);




//---------------------------------------------------
// plot
//---------------------------------------------------

// Create canvas
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
TCanvas *c = new TCanvas("c","c",20,20,800,800);
// c->Divide(1,2);

// Plot Data and interpolated CC
// c->cd(1);
// gPad->SetLogy(1);
// TH2D *hnull = new TH2D("hnull","hnull",9,0.,90.,100,0.,200000);
// hnull->GetXaxis()->SetTitle("Centrality");
// hnull->GetYaxis()->SetTitle("Number of J/#psi");
// hnull->GetYaxis()->SetTitleOffset(0.7);
// hnull->Draw();
// gData_V0Mplus05->Draw("PEsame");
// gData_V0Mminus05->Draw("PEsame");


// Config. Legend
// TLegend*leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
// leg0->SetBorderSize(0);
// leg0->SetFillColor(10);
// leg0->SetFillStyle(1);
// leg0->SetLineStyle(0);
// leg0->SetLineColor(0);
// leg0->SetTextSize(0.06);
// leg0->AddEntry(gData_V0Mplus05," V0Mplus05","PE");
// leg0->AddEntry(gData_V0Mminus05,"V0Mplus05","PE");
// leg0->Draw();

// //----------------------------------------------
// // Ratio
// //----------------------------------------------

Double_t Ratio_Data_V0M[ncent];
Double_t estatRatio_Data_V0M[ncent];

// Compute Ratio points and Errors
for(int i=0;i<ncent;i++){

    // Points
    Ratio_Data_V0M[i]    =V0Mplus05[i]/V0Mminus05[i];
    printf("ratio : %f \n",Ratio_Data_V0M[i] );
    //Error (full)
    estatRatio_Data_V0M[i] = Ratio_Data_V0M[i]*estat_V0Mplus05[i]/V0Mplus05[i] ;
}

  TGraphErrors *gRatio_Data_V0M = new TGraphErrors(ncent,cent,Ratio_Data_V0M,ecent,estatRatio_Data_V0M);
  gRatio_Data_V0M->SetMarkerStyle(20);
  gRatio_Data_V0M->SetMarkerColor(kRed);
  gRatio_Data_V0M->SetMarkerSize(0.5);

  // c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",9,0.,90.,100,0.95,1.05);
  hnull2->GetXaxis()->SetTitle("centrality");
  hnull2->GetYaxis()->SetTitle("Ratio");
  hnull2->GetYaxis()->SetTitleOffset(0.7);

  hnull2->Draw();
  gRatio_Data_V0M->Draw("P");
  // gRatioSys_Data_V0M->Draw("E2Psame");
  TLine*l = new TLine(0.,1.,90.,1.);
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
  TLegendEntry* entry=leg->AddEntry("","V0Mplus05/V0Mminus05/2 ","P");
  // entry->SetLineColor(1);
  // entry->SetLineStyle(1);
  // entry->SetLineWidth(1);
  // entry->SetMarkerColor(1);
  // entry->SetMarkerStyle(21);
  // entry->SetMarkerSize(1);
  // entry->SetTextFont(42);
  // entry=leg->AddEntry("","syst. uncert. from A.N","PE");
  // entry->SetLineColor(1);
  // entry->SetLineStyle(1);
  // entry->SetLineWidth(1);
  // entry->SetMarkerColor(1);
  // entry->SetMarkerStyle(21);
  // entry->SetMarkerSize(1);
  // entry->SetTextFont(42);
  leg->Draw();

}
