#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Riostream.h"
#include "TString.h"
#include "TFile.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TMath.h"

#include "AliLog.h"
#include "AliMergeableCollection.h"
#endif

void getSystematics ( Bool_t useFit, TString filename = "AnalysisResults.root")
{

  // Double_t ptBins[] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,12.};
  // Double_t ptBins[] = {0.,1.,2.,3.,4.,5.,6.,8.};
  Double_t ptBins[] = {0.,12.};
  // Double_t ptBins[] = {8.,12.};
//  Double_t rapidityBins[] = {-4.,-3.75,-3.5,-3.25,-3.,-2.75,-2.5};
  Double_t rapidityBins[] = {-4.,-3.5,-3.0,-2.5};
 // Double_t rapidityBins[] = {-4.,-2.5};
//  Double_t ptBins[] = {0.,12.};

  Int_t nRapidityBins = sizeof(rapidityBins)/sizeof(rapidityBins[0]) - 1;
  Int_t nPtBins = sizeof(ptBins)/sizeof(ptBins[0]) - 1;

  gSystem->ExpandPathName(filename);
  TFile* file = TFile::Open(filename.Data());
  if ( ! file ) {
    printf("Error: cannot open %s\n",filename.Data());
    return;
  }

  AliMergeableCollection* mcol = static_cast<AliMergeableCollection*>(file->FindObjectAny("WeightMTRResponseOut"));
  if ( ! mcol ) {
    printf("Error: cannot find mergeable collection in %s\n",filename.Data());
    file->ls();
    return;
  }


  TString histoName = "weightedHistoPerEta";
  if ( useFit ) histoName += "_fit";
  TH2* histos[2] = {0x0,0x0};
  TH2* rebinHistos[2] = {0x0,0x0};
  for ( Int_t imc=0; imc<2; imc++ ) {
    TString identifier = ( imc == 1 ) ? "MCRESP" : "DATARESP";
    histos[imc] = static_cast<TH2*>(mcol->GetObject(identifier.Data(),histoName.Data()));
    if ( ! histos[imc] ) return;
    rebinHistos[imc] = new TH2D(Form("rebin_%s",identifier.Data()),"",nPtBins,ptBins,nRapidityBins,rapidityBins);
    rebinHistos[imc]->Sumw2();
    for ( Int_t iybin=1; iybin<=histos[imc]->GetYaxis()->GetNbins(); iybin++ ) {
      Double_t ycenter = histos[imc]->GetYaxis()->GetBinCenter(iybin);
      for ( Int_t ixbin=1; ixbin<=histos[imc]->GetXaxis()->GetNbins(); ixbin++ ) {
        Double_t xcenter = histos[imc]->GetXaxis()->GetBinCenter(ixbin);
        Double_t binContent = histos[imc]->GetBinContent(ixbin,iybin);
        Double_t binError = histos[imc]->GetBinError(ixbin,iybin);
//        Int_t fillBinY = rebinHistos[imc]->GetYaxis()->FindBin(ycenter);
          if( ptBins[nPtBins] < xcenter || xcenter < ptBins[0] || rapidityBins[nRapidityBins] < ycenter || xcenter < rapidityBins[0] ) continue;

        Int_t fillBin = rebinHistos[imc]->FindBin(xcenter,ycenter);
        Double_t currContent = rebinHistos[imc]->GetBinContent(fillBin);
        Double_t currErr = rebinHistos[imc]->GetBinError(fillBin);
        Double_t newContent = binContent+currContent;
        Double_t newError = TMath::Sqrt(currErr*currErr + binError*binError);
        rebinHistos[imc]->SetBinContent(fillBin,newContent);
        rebinHistos[imc]->SetBinError(fillBin,newError);
      }
    }
//    printf("Rebin histo %g\n",rebinHistos[imc]->GetBinContent(1,1));
//    printf("Rebin histo err %g\n",rebinHistos[imc]->GetBinError(1,1));
  }

//  histos[0]->ProjectionX()->Draw();
//  histos[1]->ProjectionX()->Draw("same");
//  return;
  TCanvas* can = new TCanvas();
  can->cd();
  rebinHistos[1]->Add(rebinHistos[0],-1.);
  rebinHistos[1]->Divide(rebinHistos[0]);
  rebinHistos[1]->Draw("etext");
}
