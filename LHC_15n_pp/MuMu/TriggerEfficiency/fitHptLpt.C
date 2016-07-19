#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TList.h"
#include "TVirtualPad.h"
#include "TPaveStats.h"
#endif

TH1* GetHistoInCanvas(TVirtualPad*, TString );
Double_t FitFuncErfFixed ( Double_t*, Double_t* );
Double_t FitFuncErf ( Double_t*, Double_t* );
Double_t FitFuncInt ( Double_t*, Double_t* );

//_______________________________________________________________________
void fitHptLpt ( TString filename, TString histoName = "" )
{

  TFile* file = TFile::Open(filename.Data());
  if ( ! file ) {
    printf("Fatal: cannot find %s\n", filename.Data());
    return;
  }
  
  if ( histoName.IsNull() ) {
    TString currName = filename;
    while ( currName.Contains("/") ) currName.Remove(0,currName.Index("/")+1);
    currName.Remove(currName.Index(".root"));
    histoName = currName + ":";
    currName.Remove(currName.Index("Can"));
    histoName += currName;
  }

  Bool_t isInCanvas = kFALSE;
  TString canName = "";
  if ( histoName.Contains(":") ) {
    isInCanvas = kTRUE;
    TObjArray* auxNames = histoName.Tokenize(":");
    histoName = auxNames->At(1)->GetName();
    canName = auxNames->At(0)->GetName();
    delete auxNames;
  }
  TH1* histo = 0x0;
  if ( isInCanvas ) {
    TVirtualPad* pad = dynamic_cast<TVirtualPad*>(file->FindObjectAny(canName.Data()));
    histo = GetHistoInCanvas(pad, histoName);
    }
  else {
    histo = dynamic_cast<TH1*>(file->FindObjectAny(histoName.Data()));  
  }
  if ( ! histo ) {
    printf("Fatal: histo %s not found in file %s\n", histoName.Data(), filename.Data());
    return;
  }

  TCanvas* can = new TCanvas("can","can",200,10,600,600);
  can->cd();
//  TF1* fitFunc = new TF1("fitFunc",FitFuncErf,0.,10.,9);
//  fitFunc->SetParameters(0.5, 1., 0.3, 0.1, 1.2, 0.1, 0.2, 0.1, 0.35);
//  fitFunc->SetParLimits(0,0.,1.);
//  fitFunc->SetParLimits(3,-1.,1.);
//  TF1* fitFunc = new TF1("fitFunc",FitFuncInt,0.4,10.,5);
//  fitFunc->SetParameters(0.5, 1., 0.3, 0.1, 0.2);
//    fitFunc->SetParNames("Norm", "p_{T} cut", "#sigma", "Offset", "Norm (LE)", "p_{T} cut (LE)", "#sigma (LE)", "Offset (LE)", "p_{T} change");
  TF1* fitFunc = new TF1("fitFunc",FitFuncErfFixed,0.,10.,7);
  fitFunc->SetParameters(0.5, 1., 0.3, 1., 0.2, 0.1, 0.35);


  //histo->GetListOfFunctions()->Clear();
  histo->Fit(fitFunc,"R");
  histo->Draw("e");
}

//_______________________________________________________________________
TH1* GetHistoInCanvas(TVirtualPad* pad, TString histoName)
{
  if ( ! pad ) return 0x0;
  TList* list = pad->GetListOfPrimitives();
  TH1* histo = (TH1*)list->FindObject(histoName.Data());
  if ( histo ) return histo;
  
  for ( Int_t ientry=0; ientry<list->GetEntries(); ientry++ ) {
    TVirtualPad* subPad = dynamic_cast<TVirtualPad*>(list->At(ientry));
    if ( subPad ) {
      histo = GetHistoInCanvas(subPad, histoName);
      if ( histo ) break;
    }
  }
  
  return histo;
}

//______________________________________________
Double_t FitFuncErfFixed ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  Double_t currX = TMath::Max(xx,par[6]);
  Double_t sqrtTwo = TMath::Sqrt(2.);
  Double_t yVal = 1.+par[0]*(TMath::Erf((currX-par[1])/par[2]/sqrtTwo)-1.);
  if ( xx < par[6] ) yVal += par[3]*(TMath::Erf((-xx-par[4])/par[5]/sqrtTwo) - TMath::Erf((-par[6]-par[4])/par[5]/sqrtTwo));

  return yVal;
}


//______________________________________________
Double_t FitFuncErf ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  Double_t yVal = 0.;
  if ( xx > par[8] )
    yVal = par[3]+par[0]*(1.+TMath::Erf((xx-par[1])/par[2]/TMath::Sqrt(2.)));
  else
    yVal = par[7]+par[4]*(1.+TMath::Erf((-xx-par[5])/par[6]/TMath::Sqrt(2.)));
  
  return yVal;
}

//______________________________________________
Double_t FitFuncInt ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  
  Double_t yVal = 0.;
  Double_t sigma = par[2] + par[4] * ( xx - par[1] ) / par[2];
  yVal = par[3]+par[0]*(1.+TMath::Erf((xx-par[1])/sigma/TMath::Sqrt(2.)));  
  return yVal;
}
