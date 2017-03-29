//
//  CompareMuonDistributions.C
//  aliphysics-dev
//
//  Created by philippe pillot on 14/08/2015.
//  Copyright (c) 2015 Philippe Pillot. All rights reserved.
//

#include <TH1.h>
#include <THnSparse.h>
#include <TAxis.h>
#include <TString.h>
#include <Riostream.h>
#include <TFile.h>
#include <TList.h>
#include <TCanvas.h>
#include <THashList.h>
#include <TParameter.h>
#include <TLegend.h>

// kinematics range for histogram projection (pT, y, phi, charge)
const Float_t kineRange[4][2] = {{1., 999.}, {-999., 999.}, {-999., 999.}, {-999., 999.}};

// in case the ouput containers of the efficiency task have an extension in their name (like in the train)
TString extension[2] = {"_1"};

const Int_t nHist = 3;
TString sRes[nHist] = {"fHistPt", "fHistY", "fHistPhi"};
THashList *runWeights = 0x0;

void LoadRunWeights(TString fileName);
void AddHisto(TString sfile[2], TH1 *hRes[nHist][3], Double_t weight);
void AddHistoProj(TString sfile[2], TH1 *hProj[4][3], Double_t weight);
void SetKineRange(THnSparse& hKine);
void MatchNumberOfBin(TH1 *h1,TH1 *h2 );
TH1* MatchRange(TH1 *h1,TH1 *h2 );

//______________________________________________________________________________
void CompareMuonDistributionsV2(TString dir1, TString dir2, TString fileNameWeights = "")
{
  /// compare reconstructed muon distributions used for efficiency measurements
  /// 
  
  // load run weights
  if (!fileNameWeights.IsNull()) {
    LoadRunWeights(fileNameWeights);
    if (!runWeights) return;
  }
  
  TH1          *hRes[nHist][3];//Pointer for raw results
  for (Int_t i = 0; i < nHist; i++) for (Int_t j = 0; j < 3; j++) hRes[i][j] = 0x0;
  TH1          *hProj[4][3]; // Pointer for projected ThnSparse
  for (Int_t i = 0; i < 4; i++) for (Int_t j = 0; j < 3; j++) hProj[i][j] = 0x0; 
  
  // get results
  if (runWeights) {
    TIter next(runWeights);
    TParameter<Double_t> *weight = 0x0;
    while ((weight = static_cast<TParameter<Double_t>*>(next()))) {
      TString sfile[2];
      sfile[0] = Form("%s/runs/%s/AnalysisResults.root", dir1.Data(), weight->GetName());
      sfile[1] = Form("%s/runs/%s/AnalysisResults.root", dir2.Data(), weight->GetName());
      AddHisto(sfile, hRes, weight->GetVal());
      AddHistoProj(sfile, hProj, weight->GetVal());
    }
  } else {
    TString sfile[2];
    sfile[0] = Form("%s/AnalysisResults.root", dir1.Data());
    sfile[1] = Form("%s/AnalysisResults.root", dir2.Data());
    AddHisto(sfile, hRes, 1.);
    AddHistoProj(sfile, hProj, 1.);
  }
   
  //Match range for pt graph
  
  TH1* h1 = static_cast<TH1*>(hRes[0][0]->Clone());
  hRes[0][0] = MatchRange(h1,hProj[0][0]);
  
  new TCanvas;
  hRes[0][0]->DrawClone();
  new TCanvas;
  hProj[0][0]->DrawClone();

  delete h1;
  
  return;

  // Set the same number of bin and compute ratios
  for (Int_t i = 0; i < nHist && hRes[i][0] && hRes[i][1]; i++) { 
    hRes[i][2] = static_cast<TH1*>(hProj[i][0]->Clone(Form("%sOver1",hProj[i][0]->GetName())));
    hRes[i][2]->Divide(hRes[i][1]);
  }
  
  // display results at the reconstruction level
  TLegend *lRes = new TLegend(0.5,0.55,0.85,0.75);
  TCanvas *cRec = new TCanvas("cRec", "cRec", 1200, 800);
  cRec->Divide(nHist,2);
  for (Int_t i = 0; i < nHist; i++) {
    cRec->cd(i+1);
    if (i == 0) gPad->SetLogy();
    for (Int_t j = 0; j < 2 && hRes[i][j]; j++) 
    {
      if (j==0) 
      {
        if (i == 0) lRes->AddEntry(hProj[i][j],Form("dir%d",j+1),"l");
        hProj[i][j]->SetLineColor(2*j+2);
        hProj[i][j]->Draw("");
      }
      else
      {
        if (i == 0) lRes->AddEntry(hRes[i][j],Form("dir%d",j+1),"l");
        hRes[i][j]->SetLineColor(2*j+2);
        hRes[i][j]->Draw("same");
      } 
    }
    cRec->cd(i+nHist+1);
    if (hRes[i][2]) {
      hRes[i][2]->Draw();
      hRes[i][2]->SetMinimum(0.5);
      hRes[i][2]->SetMaximum(1.5);
    }
  }

}

//______________________________________________________________________________
void AddHisto(TString sfile[2], TH1 *hRes[nHist][3], Double_t weight)
{
  /// get or add histograms with given weight
  
  // get results
  for (Int_t j = 0; j < 2; j++) {
    TFile *file = TFile::Open(sfile[j].Data(),"READ");
    if (!file || !file->IsOpen()) {
      printf("cannot open file\n");
      return;
    }
    if (file && file->IsOpen()) {
      TList *list =0x0;
      if(j==0)list = static_cast<TList*>(file->FindObjectAny(Form("ExtraHistos%s",extension[j].Data())));
      else list = static_cast<TList*>(file->FindObjectAny("ExtraHistos"));
      if (!list) {
        printf("cannot find histograms\n");
        return;
      }
      for (Int_t i = 0; i < nHist; i++) {
        if (!hRes[i][j]) {
          hRes[i][j] = static_cast<TH1*>(list->FindObject(sRes[i].Data())->Clone(Form("%s%d",sRes[i].Data(),j+1)));
          if (hRes[i][j]) {
            hRes[i][j]->SetDirectory(0);
            hRes[i][j]->Sumw2();
            Double_t nEntries = static_cast<Double_t>(hRes[i][j]->GetEntries());
            if (nEntries > 0.) hRes[i][j]->Scale(weight/nEntries);
          }
        } else {
          TH1* h = static_cast<TH1*>(list->FindObject(sRes[i].Data()));
          Double_t nEntries = static_cast<Double_t>(h->GetEntries());
          hRes[i][j]->Add(h, weight/nEntries);
        }
      }
    }
    file->Close();
  }
  
}
//______________________________________________________________________________
void AddHistoProj(TString sfile[2], TH1 *hProj[4][3], Double_t weight)
{
  /// get or add histograms with given weight
  
  // get results
  for (Int_t j = 0; j < 1; j++) {
    TFile *file = TFile::Open(sfile[j].Data(),"READ");
    if (!file || !file->IsOpen()) {
      printf("cannot open file\n");
      return;
    }
    if (file && file->IsOpen()) {
      TList *list = static_cast<TList*>(file->FindObjectAny(Form("ExtraHistos%s",extension[j].Data())));
      if (!list) {
        printf("cannot find histograms\n");
        return;
      }
      THnSparse *hKine = static_cast<THnSparse*>(list->FindObject("hKine"));
      if (!hKine) return;
      SetKineRange(*hKine);
      for (Int_t i = 0; i < 4; i++) {
        if (!hProj[i][j]) {
          hProj[i][j] = hKine->Projection(i+1,"eo");
          if (hProj[i][j]) {
            hProj[i][j]->SetDirectory(0);
            Double_t nEntries = static_cast<Double_t>(hProj[i][j]->GetEntries());
            if (nEntries > 0.) hProj[i][j]->Scale(weight/nEntries);
          }
        } else {
          TH1* h = hKine->Projection(i+1,"eo");
          Double_t nEntries = static_cast<Double_t>(hProj[i][j]->GetEntries());
          hProj[i][j]->Add(h, weight/nEntries);
          delete h;
        }
      }
    }
    file->Close();
  }
  
}

//______________________________________________________________________________
void SetKineRange(THnSparse& hKine)
{
  /// Sets the kinematics range for histogram projection
  /// If the range exceeds the minimum (maximum) it includes the underflow (overflow)
  /// in the integration (same behaviour as if the range is not set)
  
  for (Int_t i = 0; i < 4; i++) {
    TAxis *a = hKine.GetAxis(i+1);
    Int_t lowBin = a->FindBin(kineRange[i][0]);
    Int_t upBin = a->FindBin(kineRange[i][1]);
    a->SetRange(lowBin, upBin);
  }
  
}

//______________________________________________________________________________
void LoadRunWeights(TString fileName)
{
  /// Set the number of interested events per run
  /// (used to weight the acc*eff correction integrated
  /// over run for any pt/y/centrality bins)
  
  if (runWeights) return;
  
  ifstream inFile(fileName.Data());
  if (!inFile.is_open()) {
    printf("cannot open file %s\n", fileName.Data());
    return;
  }
  
  runWeights = new THashList(1000);
  runWeights->SetOwner();
  
  TString line;
  while (! inFile.eof() ) {
    
    line.ReadLine(inFile,kTRUE);
    if(line.IsNull()) continue;
    
    TObjArray *param = line.Tokenize(" ");
    if (param->GetEntries() != 2) {
      printf("bad input line %s", line.Data());
      continue;
    }
    
    Int_t run = ((TObjString*)param->UncheckedAt(0))->String().Atoi();
    if (run < 0) {
      printf("invalid run number: %d", run);
      continue;
    }
    
    Float_t weight = ((TObjString*)param->UncheckedAt(1))->String().Atof();
    if (weight <= 0.) {
      printf("invalid weight: %g", weight);
      continue;
    }
    
    runWeights->Add(new TParameter<Double_t>(Form("%d",run), weight));
    
    delete param;
  }
  
  inFile.close();
  
}

//______________________________________________________________________________
void MatchNumberOfBin(TH1 *h1,TH1 *h2 )
{
    Int_t nBinRes  = 0;
    Int_t nBinProj = 0;
    Int_t rebin = 0;
    h1->SetAxisRange(h2->GetXaxis()->GetXmin(),h2->GetXaxis()->GetXmax());
    if(h1)nBinRes  = h1->GetXaxis()->GetNbins();
    if(h2)nBinProj = h2->GetXaxis()->GetNbins();
    if (nBinRes<nBinProj) return;
    if(nBinProj    !=0.) rebin = nBinRes/nBinProj;
    h1->Rebin(rebin);
}

//______________________________________________________________________________
TH1* MatchRange(TH1 *h1,TH1 *h2 )
{
    // Replace h1 by a new one with the same range as h2
    
    //Create new Histo
    TH1D* h1new = static_cast<TH1D*>(h2->Clone(Form("%s_new",h1->GetName())));
    h1new->SetTitle(Form("%s_new",h1->GetName()));
    h1new->Reset();


    MatchNumberOfBin(h1,h1new);
    printf("h1new->GetXaxis()->GetNbins() = %d\n",h1new->GetXaxis()->GetNbins() );
    printf("h1->GetXaxis()->GetNbins() = %d\n",h1->GetXaxis()->GetNbins() );
    new TCanvas;
    h1->DrawClone();

    //Fill
    for (int i = 0; i < h1new->GetXaxis()->GetNbins()+1; i++)
    {
      Double_t x ;
      Double_t dx;
   
      x  = h1->GetBinContent(i);
      dx = h1->GetBinError(i);
    

      printf("bin = %d\n",i );
      printf("x = %f\n",x );
      printf("dx = %f\n",dx );
      
      h1new->SetBinContent(i,x); 
      h1new->SetBinError(i,dx); 
    }
    return h1new;
}



