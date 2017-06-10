//
//  PileUpNtrCorrDistribution.c
//  
//
//  Created by Javier Martin Blanco on 29/06/15.
//
//

#include "PileUpNtrCorrDistribution.h"
#include <Riostream.h>
#include <TFile.h>
#include <TArrayD.h>
#include <TObjString.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGraphErrors.h>


//____________________________
void SetPUConfig(Bool_t contProb, Bool_t puEvtON, Bool_t puJpsiON, Bool_t mainMaxMult)
{
  
  // Set the configuration mode of pile-up, and if the jpsi yield distribution will be the binned one or a fit.
  // This serves to study the impact on the distributions of the different pile-up effects
  // i.e. The configuration which reproduces the full data-taking mode is "puEvtON = kTRUE, puJpsiON = kTRUE, mainMaxMult = kTRUE"
  // i.e. The configuration "puEvtON = kFALSE, puJpsiON = kFALSE, mainMaxMult = kTRUE" serves to study the effect of having several
  // collisions per bunch crossing, but not mergin them.
  
  fcontProb = contProb;
  fPUEvON = puEvtON;
  fpuJpsiON = puJpsiON;
  fmainMaxMult = mainMaxMult;
  
  TString isPuEvt(fPUEvON ? "ON" : "OFF");
  std::cout << "+++ Event merging is " << isPuEvt.Data() << std::endl;
  std::cout << std::endl;
  
  TString ismainMax(fmainMaxMult ? "as the one with the maximum NtrCorr" : "randomly among the collisions");
  std::cout << "+++ The main collision is chosen " << ismainMax.Data() << " in the event" << std::endl;
  std::cout << std::endl;
  
  TString isPuJpsi(fpuJpsiON ? "every collision" : "the main collision");
  std::cout << "+++ Taking Jpsis coming from " << isPuJpsi.Data() << " in the event" << std::endl;
  std::cout << std::endl;
  
}

//____________________________
Bool_t LoadDistrHistos(const char* fileY, const char* fileNtr, const char* fileZv)
{
  // Load the histograms which serve as input distributions for the yield vs NtrCorr, the NtrCorr distribution and the SPD z-vertex distribution
  
  //_____ Input Yield
  TFile* fY = new TFile(fileY,"read");
  if ( !fY)
  {
    std::cout << Form("File %s not found",fileY) << std::endl;
    return kFALSE;
  }
  
  TH1F* hY = static_cast<TH1F*>(fY->FindObjectAny("YieldVsNtrCorr"));
  if ( !hY )
  {
    std::cout << Form("Yields histo not found in %s",fileY) << std::endl;
    return kFALSE;
  }
  
  fNofJpsi = static_cast<TH1*>(hY->Clone("NofJpsiPileUp")); // Clone to store the MC number of Jpsi
  fNofJpsi->SetDirectory(0);
  fNofJpsi->SetLineColor(1);
  
  fYieldVsNtrCorr = static_cast<TH1*>(hY->Clone("JpsiYieldPileUp")); //Clone to store the MC yield
  fYieldVsNtrCorr->SetDirectory(0);
  fYieldVsNtrCorr->SetLineColor(1);
  fY->Close();
  
  
  //______ Input tracklets distribution
  TFile* fN = new TFile(fileNtr,"read");
  if ( !fN )
  {
    std::cout << Form("File %s not found",fileNtr) << std::endl;
    return kFALSE;
  }
  
  TH1F* h = static_cast<TH1F*>(fN->FindObjectAny("Nch"));
  if ( !h )
  {
    std::cout << Form("Tracklets histo not found in %s",fileNtr) << std::endl;
    return kFALSE;
  }
  
  fNtrCorr = static_cast<TH1*>(h->Clone("NtrCorr"));
  fNtrCorr->SetDirectory(0);
  fNtrCorr->SetLineColor(1);
  
  fN->Close();
  
  
  //______ Input Zv distribution
  TFile* fZv = new TFile(fileZv,"read");
  if ( !fZv )
  {
    std::cout << Form("File %s not found",fileZv) << std::endl;
    return kFALSE;
  }
  
  TH1F* hZ = static_cast<TH1F*>(fZv->FindObjectAny("SPDZvertex"));
  if ( !hZ )
  {
    std::cout << Form("Zvertex distribution histo not found in %s",fileZv) << std::endl;
    return kFALSE;
  }
  fZVtx = static_cast<TH1*>(hZ->Clone("ZvertexDistr"));
  fZVtx->SetDirectory(0);
  
  fZv->Close();
  
  
  //============== Fit of the yield to get a continuous probability
  TString isCont(fcontProb ? " continuous " : " binned ");
  std::cout << "+++ Using a" << isCont.Data() << "yield distribution as reference " << std::endl;
  std::cout << std::endl;
  
  
  fgYield= new TGraphErrors();
  
  for (Int_t k = 1 ; k <= fYieldVsNtrCorr->GetNbinsX() ; k++)
  {
    Double_t xmin = fYieldVsNtrCorr->GetXaxis()->GetBinLowEdge(k);
    Double_t xmax = fYieldVsNtrCorr->GetXaxis()->GetBinUpEdge(k);
    
    Int_t binMin = fNtrCorr->FindBin(xmin);
    Int_t binMax = fNtrCorr->FindBin(xmax - 1.E-6);
    
    Double_t meanX1(0.);
    for ( Int_t i = binMin ; i <= binMax ; i++ )
    {
      meanX1 += fNtrCorr->GetBinContent(i)*TMath::Nint(fNtrCorr->GetBinCenter(i));
    }
    
    Double_t Integral1Bin = fNtrCorr->Integral(binMin,binMax);
    
    meanX1 /= Integral1Bin;
    
    fgYield->SetPoint(k-1,meanX1,fYieldVsNtrCorr->GetBinContent(k));
    fgYield->SetPointError(k-1,meanX1*0.04,fYieldVsNtrCorr->GetBinError(k));
    
  }
  
  
  if ( fcontProb )
  {
    std::cout << "      + Fitting yield distribution ... " << std::endl;
    std::cout << std::endl;
    
    //  Yield = new TF1("fYieldL","[0]+[1]*x",1.,140.);
    //  ffYield = new TF1("fYieldP2","[0]+[1]*x+[2]*x*x",0.,140.);
    //  ffYield = new TF1("fYieldP3","[0]+[1]*x+[2]*x*x+[3]*x*x*x",0.,140.);
    //    ffYield= new TF1("fYieldP2","[0]+[1]*x^(1./2.)",0.,140.);
    ffYield= new TF1("fYieldP2","[0]+[1]*x^(1./3.)+[2]*x^(1./5.)",0.,140.);
    ffYield->SetLineColor(1);
    
    fgYield->Fit(ffYield,"NRO");
  }
  //==============
  
  return kTRUE;
}

//____________________________
void CreateStoringObjects()
{
  // Create the arrays to contain the collisions and bin by bin pile-up information. Also creates the legends for the canvases.
  
  //______ Axis with the bin info
  const TArrayD* binArray = fNofJpsi->GetXaxis()->GetXbins(); //Just get the axis to make the plots for the different MU's
  fsize = binArray->GetSize();
  faxis = new Double_t[fsize];
  for ( Int_t k = 0 ; k < fsize ; k++ )
  {
    faxis[k] = binArray->At(k);
  }
  
  //______ Arrays for the bin by bin pile-up info (number of events, number of events merged...)
  fnEventsBin = new TArrayI(fsize-1); // Number of events in a bin
  fnEventsBinPU = new TArrayI(fsize-1); // Number of pile-up events in a bin
  fnEventsBinwJpsi = new TArrayI(fsize-1); // Number of events with a Jpsi in a bin
  fnEventsBinPUwJpsi = new TArrayI(fsize-1); // Number of events with a Jpsi not corresponding to the bin where it has been placed
  
  
  //______ Random for the generation (The same for all the quantities)
  frand = new TRandom3(0);
  
  
  //______ Arrays to contain the histograms
  fhNArray = new TObjArray(); //Array to contain Ntr histos distributions
  fhNArray->SetOwner();
  
  fhNBinsArray = new TObjArray();//Array to contain Ntr histos distributions in bins
  fhNBinsArray->SetOwner();
  
  fhMeanXArray = new TObjArray();//Array to contain Ntr histos distributions in bins
  fhMeanXArray->SetOwner();
  
  fhNBinsRatioArray = new TObjArray();//Array to contain ratios of Ntr histos distributions in bins
  fhNBinsRatioArray->SetOwner();
  
  fhMeanXRatioArray = new TObjArray();//Array to contain ratios of Ntr histos distributions in bins
  fhMeanXRatioArray->SetOwner();
  
  fhYArray = new TObjArray();//Array to contain NJpsi histos in bins
  fhYArray->SetOwner();
  
  fhYieldArray = new TObjArray();//Array to contain yield histos in bins
  fhYieldArray->SetOwner();
  
  
  //______ Lengends for the final canvases
  flegendNtr = new TLegend (0.6646929,0.5487637,0.9578544,0.9072525);//(0.8, 0.8, 0.95, 0.95); //Legend for the Ntr distributions
  flegendNtr->SetTextSize(0.04);
  
  flegendNtr->AddEntry(fNtrCorr,"N_{tr}^{corr} Ref", "l");
  
  flegendNofJpsi = new TLegend (0.6336207,0.397463,0.9770115,0.756871);//(0.8, 0.8, 0.95, 0.95); //Legend for the number of Jpsi
  flegendNofJpsi->SetTextSize(0.04);
  
  flegendYield = new TLegend (0.566092,0.1310782,0.9741379,0.4862579);//(0.8, 0.8, 0.95, 0.95); //Legend for the yields
  flegendYield->SetTextSize(0.04);
}

//____________________________
void DrawHistos()
{
  // Draw the output histograms
  // FIXME: Clean this part
  
  Float_t fracOfHeight = 0.3; //Some definitions for the canvases
  Float_t rightMargin = 0.03;
  
  TH1F* hNtr(0x0);
  TH1F* hYP(0x0);
  TH1F* hYieldP(0x0);
  
  //======================== Plot NofJpsis distributions ========================
  //_____________ Plot NofJpsis and its ratio
  TCanvas* cYield = new TCanvas("NJpsiOverNMBVsMU"); //Canvas for the Yield //FIXME: The histo part could be removed, now is replaced by graphs
  
  cYield->Divide(1,2,0,0);
  
  cYield->cd(1);
  
  gPad->SetPad(0., fracOfHeight, 0.99, 0.99);
  gPad->SetTopMargin(0.03);
  gPad->SetRightMargin(rightMargin);
  
  flegendYield->AddEntry(fYieldVsNtrCorr, "Ref. J/#psi yield", "l");
  fYieldVsNtrCorr->GetXaxis()->SetRangeUser(0,130);
  fYieldVsNtrCorr->SetTitle("J/#psi  yield vs N_{tr}^{corr} pile-up comparison");
  fYieldVsNtrCorr->SetStats(0);
  fYieldVsNtrCorr->GetYaxis()->SetLabelSize(0.06);
  fYieldVsNtrCorr->GetYaxis()->SetTitleSize(0.06);
  fYieldVsNtrCorr->GetYaxis()->SetTitleOffset(0.79);
  //  fYieldVsNtrCorr->Draw(); // Original input yield draw
  flegendYield->Draw("same");
  
  cYield->cd(2);
  gPad->SetPad(0., 0., 0.99, fracOfHeight);
  gPad->SetRightMargin(rightMargin);
  gPad->SetBottomMargin(0.08/fracOfHeight);
  gPad->SetGridy();
  
  TH1* hratio(0x0); //Pointer to contain ratios
  TGraphErrors* gYieldPU(0x0); //Pointer to contain yield graphs
  TGraphErrors* gYieldRatio(0x0); //Pointer to contain yield ratio
  
  TIter nextH(fhNArray);
  TIter nextYield(fhYieldArray);
  Int_t col(1); //colour index
  while ( (hNtr = static_cast<TH1F*>(nextH())) && (hYieldP = static_cast<TH1F*>(nextYield())) ) //Compute ratio of the  MC yields
  {
    hratio = new TH1F(Form("RatioYieldVsNtr_%d",col),"Ratio yields vs Ntr",fsize-1,faxis);
    gYieldPU = new TGraphErrors();
    gYieldPU->SetLineColor(col+1);
    
    gYieldRatio = new TGraphErrors();
    gYieldRatio->SetLineColor(col+1);
    
    for ( Int_t k = 1 ; k <= hYieldP->GetNbinsX() ; k++ )
    {
      Double_t xmin = hYieldP->GetXaxis()->GetBinLowEdge(k);
      Double_t xmax = hYieldP->GetXaxis()->GetBinUpEdge(k);
      
      Int_t binMin = hNtr->FindBin(xmin);
      Int_t binMax = hNtr->FindBin(xmax - 1.E-6);
      
      Double_t nEvBin = hNtr->Integral(binMin,binMax);
      Double_t yield = hYieldP->GetBinContent(k)/(nEvBin*fYieldEnhancement);
      Double_t yieldError = yield*TMath::Sqrt( TMath::Power(TMath::Sqrt(hYieldP->GetBinContent(k))/(nEvBin),2.) +
                                              TMath::Power(TMath::Sqrt(nEvBin)/nEvBin,2.));
      
      hYieldP->SetBinContent(k,yield);
      hYieldP->SetBinError(k,yieldError);
      
      Double_t meanX1(0.),meanY(0.);
      for ( Int_t i = binMin ; i <= binMax ; i++ )
      {
        meanX1 += hNtr->GetBinContent(i)*TMath::Nint(hNtr->GetBinCenter(i));
        if (fcontProb) meanY += hNtr->GetBinContent(i)*ffYield->Eval(hNtr->GetBinCenter(i));
      }
      
      meanX1 /= nEvBin;
      meanY /= nEvBin;
      
      gYieldPU->SetPoint(k-1,meanX1,yield);
      gYieldPU->SetPointError(k-1,meanX1*0.04,yieldError);
      
      
      Double_t ratio = yield/fYieldVsNtrCorr->GetBinContent(k);
      Double_t ratioError = TMath::Sqrt( TMath::Power(yieldError/fYieldVsNtrCorr->GetBinContent(k),2.) +
                                        TMath::Power(yield*fYieldVsNtrCorr->GetBinError(k)/TMath::Power(fYieldVsNtrCorr->GetBinContent(k),2.),2.) );
      
      if (fcontProb)
      {
        ratio = yield/meanY;
        ratioError = TMath::Sqrt( TMath::Power(yieldError/yield,2.));
      }
      
      hratio->SetBinContent(k,ratio);
      hratio->SetBinError(k,ratioError);
      
      gYieldRatio->SetPoint(k-1,meanX1,ratio);
      gYieldRatio->SetPointError(k-1,meanX1*0.04,ratioError);
      
    }
    
    cYield->cd(1);
    hYieldP->SetStats(0);
    //    hYieldP->Draw("same");
    if (col == 1) gYieldPU->Draw("pa");
    else gYieldPU->Draw("psame");
    
    cYield->cd(2);
    hratio->SetStats(0);
    hratio->SetTitle("Ratio Ref/PU");
    hratio->GetXaxis()->SetTitle("N_{tr}^{corr}");
    hratio->GetXaxis()->SetLabelSize(0.14);
    hratio->GetXaxis()->SetTitleSize(0.12);
    hratio->GetXaxis()->SetTitleOffset(0.75);
    hratio->GetYaxis()->SetTitle("Ratio Ref/PU");
    hratio->GetYaxis()->SetLabelSize(0.10);
    hratio->GetYaxis()->SetTitleSize(0.10);
    hratio->GetYaxis()->SetTitleOffset(0.4);
    hratio->GetXaxis()->SetRangeUser(0,130);
    hratio->SetLineColor(col+1);
    //    hratio->Draw("same");
    
    if (col == 1) gYieldRatio->Draw("pa");
    else gYieldRatio->Draw("psame");
    
    col++;
  }
  cYield->cd(1);
  
  fgYield->Draw("psame");
  if (fcontProb) ffYield->Draw("same");
  
  cYield->Update();
  //__________________________________
  
  //=============================================================================
  
  
  
  //======================== Plot Tracklets distributions ========================
  
  //____Plot of integral of Ntr distributions in bins
  nextH.Reset();
  col = 1;
  TH1* h1clone = static_cast<TH1*>(fNtrCorr->Clone()); //Clone of the original Nof traclets (not really necesary, just to reuse this piece of code)
  TH1* h2clone(0x0);
  TGraph* g1(0x0);
  TH1* hNratioclone(0x0);
  TH1* hN1clone(0x0);
  Bool_t hN1Already(kFALSE);
  while ( (h2clone = static_cast<TH1F*>(nextH())) )
  {
    hNratioclone = new TH1F(Form("NtrDistribBinsRatio_%d",col),"N_{tr}^{corr} distributions",fsize-1,faxis); //static_cast<TH1*>(fNtrCorr->Clone(Form("NtrDistribBinsRatio_%d",col)));
    hNratioclone->SetStats(0);
    hNratioclone->SetTitle("Ratio Ref/PU");
    hNratioclone->GetXaxis()->SetTitle("N_{tr}^{corr}");
    hNratioclone->GetXaxis()->SetLabelSize(0.14);
    hNratioclone->GetXaxis()->SetTitleSize(0.12);
    hNratioclone->GetXaxis()->SetTitleOffset(0.75);
    hNratioclone->GetYaxis()->SetTitle("Ratio Ref/PU");
    hNratioclone->GetYaxis()->SetLabelSize(0.10);
    hNratioclone->GetYaxis()->SetTitleSize(0.10);
    hNratioclone->GetYaxis()->SetTitleOffset(0.4);
    hNratioclone->SetDirectory(0);
    hNratioclone->SetLineColor(1);
    hNratioclone->SetMarkerColor(1);
    hNratioclone->SetMarkerStyle(20);
    
    if ( !g1 )
    {
      g1 = new TGraph();
      g1->SetMarkerColor(1);
      g1->SetMarkerStyle(20);
    }
    TGraph* gratio = new TGraph();
    gratio->SetMarkerColor(col+1);
    gratio->SetMarkerStyle(21);
    
    TGraph* g2 = new TGraph();
    g2->SetMarkerColor(col+1);
    g2->SetMarkerStyle(21);
    
    if ( !hN1clone ) hN1clone = new TH1F(Form("Ntr1_%d",col),"N_{tr}^{corr} distribution",fsize-1,faxis); //To store the original normalized tracklets distribution
    TH1* hN2clone = new TH1F(Form("Ntr_%d",col),"N_{tr}^{corr} distribution",fsize-1,faxis);
    
    Double_t Integral1 = h1clone->Integral();
    Double_t Integral2 = h2clone->Integral();
    for (Int_t k = 1 ; k <= hN1clone->GetNbinsX() ; k++)
    {
      Double_t xmin = hN1clone->GetXaxis()->GetBinLowEdge(k);
      Double_t xmax = hN1clone->GetXaxis()->GetBinUpEdge(k);
      
      Int_t binMin = h1clone->FindBin(xmin);
      Int_t binMax = h1clone->FindBin(xmax - 1.E-6);
      
      //      std::cout << "Bink = " << k << " ; xmin = " << xmin << " ; binMin = " << binMin << " ; xmax = " << xmax << " ; BinMax = " << binMax << std::endl;
      
      Double_t meanX1(0.),meanX2(0.);
      for ( Int_t i = binMin ; i <= binMax ; i++ )
      {
        meanX1 += h1clone->GetBinContent(i)*TMath::Nint(h1clone->GetBinCenter(i));
        meanX2 += h2clone->GetBinContent(i)*TMath::Nint(h2clone->GetBinCenter(i));
      }
      
      Double_t Integral1Bin = h1clone->Integral(binMin,binMax);
      Double_t Integral2Bin = h2clone->Integral(binMin,binMax);
      
      meanX1 /= Integral1Bin;
      meanX2 /= Integral2Bin;
      
      Double_t value1 = Integral1Bin/(Integral1*(binMax - binMin + 1));
      Double_t error1 = TMath::Sqrt( TMath::Power( TMath::Sqrt(Integral1Bin)/(Integral1*(binMax - binMin + 1)) ,2.) + TMath::Power( TMath::Sqrt(Integral1)*Integral1Bin/(TMath::Power(Integral1,2.)*(binMax - binMin + 1)) ,2.) );
      
      Double_t value2 = Integral2Bin/(Integral2*(binMax - binMin + 1));
      Double_t error2 = TMath::Sqrt( TMath::Power( TMath::Sqrt(Integral2Bin)/(Integral2*(binMax - binMin + 1)) ,2.) + TMath::Power( TMath::Sqrt(Integral2)*Integral2Bin/(TMath::Power(Integral2,2.)*(binMax - binMin + 1)) ,2.) );
      
      if ( !hN1Already ) g1->SetPoint(k-1,meanX1,1.);
      g2->SetPoint(k-1,meanX2,1.+col*0.1);
      gratio->SetPoint(k-1,meanX1,meanX1/meanX2);
      
      Double_t ratio = value1/value2;
      Double_t ratioError = TMath::Sqrt( TMath::Power( error1/value2 ,2.) + TMath::Power( error2*value1/TMath::Power(value2,2.) ,2.) );
      
      if ( !hN1Already )
      {
        hN1clone->SetBinContent(k,value1);
        hN1clone->SetBinError(k,error1);
      }
      
      hN2clone->SetBinContent(k,value2);
      hN2clone->SetBinError(k,error2);
      
      hNratioclone->SetBinContent(k,ratio);
      hNratioclone->SetBinError(k,ratioError);
      
    }
    hN2clone->SetLineColor(col+1);
    hN2clone->SetMarkerColor(col+1);
    hN2clone->SetStats(0);
    
    if ( !hN1Already )
    {
      hN1clone->SetLineColor(1);
      hN1clone->SetMarkerColor(1);
      hN1clone->SetStats(0);
    }
    
    hNratioclone->SetLineColor(col+1);
    hNratioclone->SetMarkerColor(col+1);
    hNratioclone->SetStats(0);
    
    col++;
    
    if ( !hN1Already )
    {
      fhNBinsArray->Add(hN1clone);
      hN1Already = kTRUE;
      
      fhMeanXArray->Add(g1);
    }
    
    fhNBinsArray->Add(hN2clone);
    fhNBinsRatioArray->Add(hNratioclone);
    
    fhMeanXArray->Add(g2);
    fhMeanXRatioArray->Add(gratio);
  }
  
  TCanvas* cNtrBins = new TCanvas("TrackletsDistributionsVsMUBins");
  
  cNtrBins->Divide(1,2,0,0);
  
  cNtrBins->cd(1)->SetLogy();
  
  gPad->SetPad(0., fracOfHeight, 0.99, 0.99);
  gPad->SetTopMargin(0.03);
  gPad->SetRightMargin(rightMargin);
  
  TH1* hD = static_cast<TH1*>(fhNBinsArray->At(0));
  hD->Draw();
  
  TIter next(fhNBinsArray);
  while ( (hD = static_cast<TH1*>(next())) )
  {
    hD->Draw("same");
  }
  flegendNtr->Draw("same");
  
  cNtrBins->cd(2);
  gPad->SetPad(0., 0., 0.99, fracOfHeight);
  gPad->SetRightMargin(rightMargin);
  gPad->SetBottomMargin(0.08/fracOfHeight);
  gPad->SetGridy();
  
  TIter nextR(fhNBinsRatioArray);
  while ( (hD = static_cast<TH1*>(nextR())) )
  {
    hD->Draw("same");
  }
  
  cNtrBins->Update();
  //__________________________________
  
  
  //________Plot of mean NtrCorr in each bin
  TCanvas* cNtrMeanXBins = new TCanvas("MeanTrackletsInBinVsMUBins");
  
  cNtrMeanXBins->Divide(1,2,0,0);
  
  cNtrMeanXBins->cd(1);
  
  gPad->SetPad(0., fracOfHeight, 0.99, 0.99);
  gPad->SetTopMargin(0.03);
  gPad->SetRightMargin(rightMargin);
  
  TGraph* hM = static_cast<TGraph*>(fhMeanXArray->At(0));
  hM->SetTitle("Mean N_{tr}^{corr} in multiplictiy bins");
  hM->Draw("ap");
  
  
  TIter nextM(fhMeanXArray);
  while ( (hM = static_cast<TGraph*>(nextM())) )
  {
    hM->Draw("psame");
  }
  flegendNtr->Draw("same");
  
  cNtrMeanXBins->cd(2);
  gPad->SetPad(0., 0., 0.99, fracOfHeight);
  gPad->SetRightMargin(rightMargin);
  gPad->SetBottomMargin(0.08/fracOfHeight);
  gPad->SetGridy();
  
  TGraph* hMR = static_cast<TGraph*>(fhMeanXRatioArray->At(0));
  hMR->SetTitle("Ratio Ref/PU");
  hMR->GetXaxis()->SetTitle("N_{tr}^{corr}");
  hMR->GetXaxis()->SetLabelSize(0.14);
  hMR->GetXaxis()->SetTitleSize(0.12);
  hMR->GetXaxis()->SetTitleOffset(0.75);
  hMR->GetYaxis()->SetTitle("Ratio Ref/PU");
  hMR->GetYaxis()->SetLabelSize(0.10);
  hMR->GetYaxis()->SetTitleSize(0.10);
  hMR->GetYaxis()->SetTitleOffset(0.4);
  
  hMR->Draw();
  
  
  hMR->Draw("ap");
  TIter nextMR(fhMeanXRatioArray);
  while ( (hMR = static_cast<TGraph*>(nextMR())) )
  {
    hMR->Draw("psame");
  }
  
  cNtrMeanXBins->Update();
  //__________________________________
  
  
  //________Plot of continuous NtrCorr distributions
  TCanvas* c1 = new TCanvas("TrackletsDistributionsVsMU");
  
  c1->Divide(1,2,0,0);
  
  c1->cd(1)->SetLogy();
  
  gPad->SetPad(0., fracOfHeight, 0.99, 0.99);
  gPad->SetTopMargin(0.03);
  gPad->SetRightMargin(rightMargin);
  
  Double_t Integral = fNtrCorr->Integral();
  std::cout << "Nof events data = " << Integral << std::endl;
  for (Int_t i = 1 ; i <= fNtrCorr->GetNbinsX() ; i++) //Normalize the original input Nof tracklets
  {
    Double_t value = fNtrCorr->GetBinContent(i);
    Double_t error = TMath::Sqrt( TMath::Power(TMath::Sqrt(value)/Integral,2.) + TMath::Power(value*TMath::Sqrt(Integral)/TMath::Power(Integral,2.),2.) );
    
    fNtrCorr->SetBinContent(i,value/Integral);
    fNtrCorr->SetBinError(i,error);
  }
  
  fNtrCorr->SetStats(0);
  fNtrCorr->GetYaxis()->SetTitle("N_{events}/N_{events}^{tot}");
  fNtrCorr->GetYaxis()->SetLabelSize(0.06);
  fNtrCorr->GetYaxis()->SetTitleSize(0.06);
  fNtrCorr->GetYaxis()->SetTitleOffset(0.79);
  fNtrCorr->GetXaxis()->SetRangeUser(0,130);
  fNtrCorr->Draw();
  flegendNtr->Draw("same");
  
  c1->cd(2);
  gPad->SetPad(0., 0., 0.99, fracOfHeight);
  gPad->SetRightMargin(rightMargin);
  gPad->SetBottomMargin(0.08/fracOfHeight);
  gPad->SetGridy();
  
  nextH.Reset();
  col = 1;
  while ( (hNtr = static_cast<TH1F*>(nextH())) )
  {
    hratio = new TH1F(Form("RatioNtr_%d",col),"N_{tr}^{corr} Ratio",fNtrCorr->GetNbinsX(),0,fNtrCorr->GetXaxis()->GetXmax());
    
    Integral = hNtr->Integral();
    for (Int_t i = 1 ; i <= fNtrCorr->GetNbinsX() ; i++)
    {
      Double_t value = hNtr->GetBinContent(i);
      Double_t error = TMath::Sqrt( TMath::Power(TMath::Sqrt(value)/Integral,2.) + TMath::Power(value*TMath::Sqrt(Integral)/TMath::Power(Integral,2.),2.) );
      
      hNtr->SetBinContent(i,value/Integral);
      hNtr->SetBinError(i,error);
      
      Double_t ratio,ratioError;
      if ( value != 0 && fNtrCorr->GetBinContent(i) != 0 )
      {
        ratio = fNtrCorr->GetBinContent(i)/(value/Integral);
        ratioError = TMath::Sqrt( TMath::Power(error*fNtrCorr->GetBinContent(i)/TMath::Power((value/Integral),2.),2.) +
                                 TMath::Power(fNtrCorr->GetBinError(i)/(value/Integral),2.) );
      }
      else
      {
        ratio = 0.;
        ratioError = 0.;
      }
      
      hratio->SetBinContent(i,ratio);
      hratio->SetBinError(i,ratioError);
    }
    
    c1->cd(1);
    hNtr->SetStats(0);
    hNtr->Draw("same");
    c1->cd(2);
    hratio->SetStats(0);
    hratio->GetXaxis()->SetTitle("N_{tr}^{corr}");
    hratio->GetXaxis()->SetLabelSize(0.14);
    hratio->GetXaxis()->SetTitleSize(0.12);
    hratio->GetXaxis()->SetTitleOffset(0.75);
    hratio->GetYaxis()->SetTitle("Ratio Ref/PU");
    hratio->GetYaxis()->SetLabelSize(0.10);
    hratio->GetYaxis()->SetTitleSize(0.10);
    hratio->GetYaxis()->SetTitleOffset(0.4);
    hratio->GetXaxis()->SetRangeUser(0,130);
    hratio->SetLineColor(col+1);
    hratio->Draw("same");
    col++;
  }
  //__________________________________
  
  //=============================================================================
  
  
  
  //======================== Plot Jpsi yield distributions ========================
  
  //____Plot Jpsi yield distributions
  TCanvas* c = new TCanvas("JPsiDistributionsVsMU");
  
  TIter nextY(fhYArray);
  Bool_t first(kTRUE);
  while ( (hYP = static_cast<TH1F*>(nextY())) )
  {
    hYP->SetStats(0);
    if ( first )
    {
      Integral = hYP->Integral();
      for (Int_t i = 1 ; i <= fNtrCorr->GetNbinsX() ; i++)
      {
        Double_t value = hYP->GetBinContent(i);
        Double_t error = TMath::Sqrt( TMath::Power(TMath::Sqrt(value)/Integral,2.) + TMath::Power(value*TMath::Sqrt(Integral)/TMath::Power(Integral,2.),2.) );
        
        hYP->SetBinContent(i,value/Integral);
        hYP->SetBinError(i,error);
      }
      
      hYP->Draw();
      first = kFALSE;
    }
    else
    {
      Integral = hYP->Integral();
      for (Int_t i = 1 ; i <= fNtrCorr->GetNbinsX() ; i++)
      {
        Double_t value = hYP->GetBinContent(i);
        Double_t error = TMath::Sqrt( TMath::Power(TMath::Sqrt(value)/Integral,2.) + TMath::Power(value*TMath::Sqrt(Integral)/TMath::Power(Integral,2.),2.) );
        
        hYP->SetBinContent(i,value/Integral);
        hYP->SetBinError(i,error);
      }
      hYP->Draw("same");
    }
  }
  
  flegendNofJpsi->Draw("same");
  //__________________________________
  
  //=============================================================================
  
  
  TFile* ff = new TFile("PileUpResults.root","update");
  
  c1->Write(0x0, TObject::kOverwrite);
  c->Write(0x0, TObject::kOverwrite);
  cYield->Write(0x0, TObject::kOverwrite);
  cNtrBins->Write(0x0, TObject::kOverwrite);
  cNtrMeanXBins->Write(0x0, TObject::kOverwrite);
  
  ff->Close();
  
  delete ff;
  delete c1;
  delete c;
  delete cYield;
  delete cNtrBins;
  delete cNtrMeanXBins;
}

//____________________________
void SetEvent(TArrayD& zVtxsEventArray,TArrayI& nTrsEventArray,TArrayI& nJpsiEventArray)
{
  // The event collisions properties are defined randomly here according to the input distributions: number of tracklets, vertex and nof Jpsi for each collision in the event
  
  Double_t yield(0.),spdZv(-99.),nTrCollision(0);
  Int_t nJp(0),bin(0);
  for ( Int_t n = 0 ; n < fnCollisions ; n++ ) //Collisions loop
  {
    
    spdZv = fZVtx->GetRandom(); //SPD z vertex generation of the nth collision
    zVtxsEventArray.AddAt(spdZv,n);
    
    nTrCollision = TMath::Nint(fNtrCorr->GetRandom());
    
    nTrsEventArray.AddAt(nTrCollision,n);
    
    bin = fNofJpsi->FindBin(nTrCollision);
    
    if ( !fcontProb ) yield = fNofJpsi->GetBinContent(bin);
    else yield = ffYield->Eval(nTrCollision);
    
    nJp = frand->Poisson(yield*fYieldEnhancement);
    
    nJpsiEventArray.AddAt(nJp,n);
    
  } //End collisions loop
  
}

//____________________________
void PileUpMerging(UInt_t& puMask, UInt_t& puMaskR, TArrayD& zVtxsEventArray, TArrayI& nTrsEventArray, TArrayI& nJpsiEventArray)
{
  // Merge the collisions which vertex are closer than 1cm
  
  Bool_t alreadyJpsiPU(kFALSE);
  if ( fnCollisions > 1 && fPUEvON ) // This algorithm will merge piled up collisions
  {
    for ( Int_t i = 0 ; i < fnCollisions - 1 ; i++ )
    {
      for ( Int_t j = i + 1 ; j < fnCollisions ; j++ )
      {
        if ( TMath::Abs(zVtxsEventArray.At(i) - zVtxsEventArray.At(j)) < 1. ) //True Pile up condition (we cannot resolve the collision)
        {
          puMask += 1<<j; // Set the bit of the j collision to 1 to know that contains a pile-up (Note that even events with 0 tracklets can be pile-up, since they have tracklets out of the eta range )
          
          puMaskR += 1<<i; // Set the bit of the i collision to 1 to know that this one cannot be a main collision, since its tracklets have been added to the collision in j and then set to 0.
          
          Double_t nTrDiff = nTrsEventArray.At(i) - nTrsEventArray.At(j);
          
          if ( nTrDiff > 0. ) zVtxsEventArray.AddAt(zVtxsEventArray.At(i),j); // We set the Z vertex of the j collision to the one of the biggest NofTracklets (This could be inproved by setting the weighted mean)
          
          nTrsEventArray.AddAt(nTrsEventArray.At(i) + nTrsEventArray.At(j),j); // // We add the Nof tracklets in i to the collision in j
          nTrsEventArray.AddAt(0.,i); //We set it to 0 to don't have the i collision into account later in the main vertex determination (cause we add it up to j)
          
          if ( nJpsiEventArray.At(i) != 0 ) //
          {
            if ( (nJpsiEventArray.At(j) != 0) && !alreadyJpsiPU ) // To count just once per event when we have more than 1 Jpsi in a merged collision
            {
              fnEvJpsiPileUp = fnEvJpsiPileUp + 1./TMath::Power(fYieldEnhancement,2.) ; //FIXME:Not clear how to renormalize by the yieldEnhancement factor
              alreadyJpsiPU = kTRUE;
            }
            nJpsiEventArray.AddAt(nJpsiEventArray.At(i) + nJpsiEventArray.At(j),j); // We add the Nof Jpsi in i to the collision in j
            nJpsiEventArray.AddAt(0.,i); // We set the Nof Jpsi of i to 0 (cause we add it up to j)
          }
          
          break; //to get out of the j loop (cause we have merged the i and j collision so we want to go to the i+1 collision)
        }
      }
    }
  } // End of merging algorithm
  
}

//____________________________
void SetMainCol(UInt_t& mainColMask, UInt_t puMaskR, Int_t& posMax, Double_t& spdZvMain, Int_t& nTrEvent, TArrayD* zVtxsEventArray,TArrayI* nTrsEventArray)
{
  // Decide which is the main collision according to the configuration mode
  
  Int_t ntr(0);
  nTrEvent = -1; // Like this a collision with 0 tracklets can be main
  if ( fmainMaxMult )
  {
    for ( Int_t m = 0 ; m < fnCollisions ; m++ )
    {
      ntr = nTrsEventArray->At(m);
      if ( ntr > nTrEvent ) // We try to find the position in the array with the biggest Nof tracklets
      {
        posMax = m;
        nTrEvent = ntr;
      }
    }
    
    mainColMask = 1<<posMax;
  }
  else
  {
    mainColMask = puMaskR;
    while ( mainColMask & puMaskR ) // To avoid positions merged to another collision
    {
      posMax = TMath::Nint(frand->Uniform(-0.5,fnCollisions-0.5));
      std::cout << fnCollisions << std::endl;
      
      mainColMask = 1<<posMax;
    }
    
    nTrEvent = nTrsEventArray->At(posMax);
  }
  
  spdZvMain = zVtxsEventArray->At(posMax); // we set the main vertex
}

//____________________________
void SetNofJpsi(Int_t& nJpsiEvent, Int_t posMax, TArrayI* nJpsiEventArray)
{
  nJpsiEvent = nJpsiEventArray->At(posMax); // At first the NofJpsi is set as the Jpsi coming from the main collision
  
  Bool_t alreadyJpsiFromSec(kFALSE);
  if ( fpuJpsiON )
  {
    for ( Int_t j = 0 ; j < fnCollisions ; j++ )
    {
      if ( j == posMax ) continue;
      
      if ( (nJpsiEventArray->At(j) != 0) && !alreadyJpsiFromSec )
      {
        fnMBWitSecondaryJpsi = fnMBWitSecondaryJpsi + 1./(fYieldEnhancement); // Counts events where at least one of the Jpsi does not come from the main vertex. //FIXME:Not clear how to renormalize by the fYieldEnhancement factor
        alreadyJpsiFromSec = kTRUE;
        if ( nJpsiEventArray->At(posMax) == 0 && fYieldEnhancement == 1.) fnMBWithJpsiFromSecColl++; // Counts events where at least one of the Jpsi does not come from the main vertex and there is no Jpsi coming from the main vertex. Only computed when enhancement factor = 1 since it cannot be renormalized. P = (1-P_{Jpsi}^{main col})*P_{Jpsi}^{sec col} //FIXME:Not clear how to renormalize by the yieldEnhancement factor
      }
      nJpsiEvent += nJpsiEventArray->At(j); // We add the Jpsi from the non piled up events (if any) we have not enough resolution to resolve the exact Jpsi vertex
      
    }
  }
  
  if ( nJpsiEvent != 0) fnMBEventsWithJpsi = fnMBEventsWithJpsi + 1./(fYieldEnhancement); // P = P_{Jpsi}^{col1} + P_{Jpsi}^{col2} + ... ?? //FIXME:Not clear how to renormalize by the yieldEnhancement factor
  
  if ( nJpsiEvent > 1 )
  {
    if ( alreadyJpsiFromSec ) fnMBEventsWithMore1Jpsi = fnMBEventsWithMore1Jpsi + 1./fYieldEnhancement;
    else fnMBEventsWithMore1Jpsi = fnMBEventsWithMore1Jpsi + 1./TMath::Power(fYieldEnhancement,2.); // P = P_{N1 Jpsi}^{col1}*P_{N2 Jpsi}^{col2}*... ?? //FIXME:Not clear how to renormalize by the yieldEnhancement factor
  }
}

//____________________________
void GetPUFraction(Bool_t colPU, Int_t nTrEvent, Double_t nJpsiEvent)
{
  // Determine the fraction of vertex-merged pile up events in multiplicity bins
  
  for ( Int_t k = 0 ; k < fsize - 1 ; k++ )
  {
    Double_t low = faxis[k];
    Double_t high = faxis[k+1];
    
    if( (nTrEvent >= low) && (nTrEvent < high) )
    {
      fnEventsBin->AddAt(fnEventsBin->At(k) + 1,k);
      
      if ( nJpsiEvent != 0. ) fnEventsBinwJpsi->AddAt(fnEventsBinwJpsi->At(k) + 1,k);
      
      if ( colPU )
      {
        fnEventsBinPU->AddAt(fnEventsBinPU->At(k) + 1,k);
        if ( nJpsiEvent != 0. ) fnEventsBinPUwJpsi->AddAt(fnEventsBinPUwJpsi->At(k) + 1,k);
      }
    }
  }
  
}

//____________________________
void PrintValues()
{
  // Printout of the found pile-up fractions, nof jpsis... values
  
  Double_t ratioPU = fnEventsPileUp*100. / fnMBEvents;
  Double_t ratioMore1Col = fnMBEventsMore1Col*100. / fnMBEvents;
  Double_t ratioEvWithJpsi = fnMBEventsWithJpsi*100. / (fnMBEvents);
  Double_t ratioJpsiPU = fnEvJpsiPileUp*100. / fnMBEventsWithJpsi;
  Double_t ratioJpsiSec = fnMBWitSecondaryJpsi*100. / fnMBEventsWithJpsi;
  Double_t ratioJpsiSecColl = fnMBWithJpsiFromSecColl*100. / fnMBEventsWithJpsi;
  Double_t ratioWithMore1Jpsi = fnMBEventsWithMore1Jpsi*100./fnMBEventsWithJpsi;
  
  
  TString strnMBWithJpsiFromSecColl(fYieldEnhancement == 1. ? Form("%f",fnMBWithJpsiFromSecColl) : " not computed ");
  TString strratioJpsiSecColl(fYieldEnhancement == 1. ? Form("%f",ratioJpsiSecColl) : " not computed ");
  
  
  std::cout << std::endl;
  
  std::cout << "     =====> NofMBEvents = " << fnMBEvents << " ; NofPileUpEvents = " << fnEventsPileUp << " (" << ratioPU << "% NofMBEvents)" << " ; nMBEventsMore1Col = " << fnMBEventsMore1Col << " (" << ratioMore1Col << "% NofMBEvents)" << " ; NofJpsi = " << fnofTotalJpsi << std::endl;
  
  std::cout << std::endl;
  std::cout << "//_____only reliable when enhancement factor = 1 !! _____\\ " << std::endl;
  
  std::cout << "     =====> nMBEventsWithJpsi = " << fnMBEventsWithJpsi <<  " (" << ratioEvWithJpsi << "% NofMBEvents)" << " ; nMBEventsWithMore1Jpsi = " << fnMBEventsWithMore1Jpsi << " (" << ratioWithMore1Jpsi << "% nMBEventsWithJpsi)" << " ; " <<  std::endl;
  
  std::cout << "            nEvJpsiPileUp = " << fnEvJpsiPileUp << " (" << ratioJpsiPU << "% nMBEventsWithJpsi)" << " ; fnMBWitSecondaryJpsi = " << fnMBWitSecondaryJpsi  << " (" << ratioJpsiSec << "% nMBEventsWithJpsi)" << " ; fnMBWithJpsiFromSecColl = " << strnMBWithJpsiFromSecColl.Data()  << " (" << strratioJpsiSecColl.Data() << "% nMBEventsWithJpsi)" << std::endl;
  
  std::cout << "//______________________________________________________\\ " << std::endl;
  std::cout << std::endl;
  
  std::cout << "=====> PU fraction (N_PU / N_MB):" << std::endl;
  for ( Int_t k = 0 ; k < fsize - 1 ; k++ )
  {
    Double_t nEB = fnEventsBin->At(k);
    Double_t nEBPU = fnEventsBinPU->At(k);
    Double_t ratioEB = nEBPU*100./nEB;
    
    std::cout << "       bin " << k+1 << " = " << ratioEB << "%" << " ( NofEvents = " << nEB << ")" << std::endl;
  }
  
  std::cout << std::endl;
  std::cout << "=====> Jpsi PU fraction (N_PU w Jpsi / N_MB w Jpsi): (only reliable when enhancement factor = 1 !!)" << std::endl;
  for ( Int_t k = 0 ; k < fsize - 1 ; k++ )
  {
    Double_t nEBPUJpsi = fnEventsBinPUwJpsi->At(k)/fYieldEnhancement;
    Double_t nEBJpsi= fnEventsBinwJpsi->At(k)/fYieldEnhancement;
    Double_t ratioEBJspsi = nEBPUJpsi*100./nEBJpsi;
    
    std::cout << "       bin " << k+1 << " = " << ratioEBJspsi << "%" << " ( NofJpsi = " << nEBJpsi << ")" << std::endl;
  }
}

//____________________________
void PileUpNtrDistribution(Float_t nBC, const char* muList,Double_t yieldEnhancement,
                           Bool_t contProb, Bool_t puEvtON, Bool_t puJpsiON, Bool_t mainMaxMult,
                           const char* fileY,const char* fileNtr,const char* fileZv)
{
  // This macro is a toy MC which simulates the effect of pile-up in the Jpsi yield vs charged particle multiplicity analysis.
  // The starting point is a reference yield vs number of corrected tracklets as free from pile-up as possible. We use the measured one cause we estimate that the pile-up efect would be small. We need also a corrected tracklets distribution and a z-vertex distribution to assign a multiplicity and a vertex position to each collision.
  
  // ++++Parameters:
  // - nBC: number of Bunch crossings
  // - muList: List with the mu factors to use to generate the collisions per bunch crossing
  // - yieldEnhancement: enhancement factor for the yield (at the end every quantity is divided by it to get the correct probabilities)
  // - contProb: To choose between binned yield as prob distribution or fitting the yield and use the function
  // - puEvtON: To shich on/off the event pile-up
  // - puJpsiON: To shich on/off the Jpsi pile-up (if kTRUE all the Jpsi are counted in the event no matter if they come from the main vertex or not)
  // - mainMaxMult: To change the option for choosing the main collision (if kTRUE main is the collision with the max Nof tracklets. If kFALSE main is chosen randomly from the collisions in the event, in this case the result is like having the same mu factor as the input NtrCorr distribution i.e. 1 collision per event)
  // - fileY: File with the reference yield vs number of corrected tracklets
  // - fileNtr: File with reference number of corrected tracklets distribution. The histogram bins MUST be centered on the NtrCorr value (i.e. 1,2,3...)
  // - fileZv: File with z-vertex distribution
  
  std::cout << std::endl;
  std::cout << "---------------------------------//---------------------------------" << std::endl;
  
  fYieldEnhancement = yieldEnhancement;
  
  std::cout << "+++ Yield enhanced by a factor " << fYieldEnhancement << std::endl;
  std::cout << std::endl;
  
  
  
  //____________ Configure pile-up mode, load the files with the input distributions and create the objects to store collisions
  
  SetPUConfig(contProb,puEvtON,puJpsiON,mainMaxMult);
  
  if ( !LoadDistrHistos(fileY,fileNtr,fileZv) ) return;
  
  CreateStoringObjects();
  
  //___________
  
  
  std::cout << "---------------------------------//---------------------------------" << std::endl;
  std::cout << std::endl;
  
  
  //_______Body of simulation__________
  //___________________________________
  
  TString smuList(muList);
  TObjArray* muArray = smuList.Tokenize(",");
  TObjString* sMu;
  TIter nextMU(muArray);
  
  TH1F* hNtr(0x0);
  TH1F* hYP(0x0);
  TH1F* hYieldP(0x0);
  
  Int_t iColor(1); //Index for histo color
  while ( (sMu = static_cast<TObjString*>(nextMU())) ) //MU's loop
  {
    Double_t mu = sMu->String().Atof();
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "//=====================" << " MU factor = " << mu << " " << "===========================\\ " << std::endl;
    
    
    //____________________ Create the histos to store the resutls for the current mu factor
    hNtr = new TH1F(Form("NtrCorrPileUpMU%s",sMu->String().Data()),"Number of corrected tracklets distribution as a function of #mu factor;N_{tr}^{corr};Events",
                    fNtrCorr->GetXaxis()->GetNbins(),fNtrCorr->GetXaxis()->GetXmin(),fNtrCorr->GetXaxis()->GetXmax());
    
    hYP = new TH1F(Form("NofJpsiMU%s",sMu->String().Data()),"Number of JPsi as a function of #mu factor;N_{tr}^{corr};N^{J/#psi}",fsize-1,faxis);
    
    hYieldP = new TH1F(Form("JpsiYieldMU%s",sMu->String().Data()),"JPsi yield as a function of #mu factor;N_{tr}^{corr};N^{J/#psi}",fsize-1,faxis);
    //____________________
    
    
    //____________________ Definitions for the collision properties
    Int_t nTrEvent(0),nJpsiEvent(0);
    
    Double_t spdZvMain(0.);
    
    Double_t nEffEvents = nBC/(mu/0.02); // To have the same number of MB events for every MU value
    //____________________
    
    for ( Int_t nE = 0 ; nE < nEffEvents ; nE++ )//Events loop
    {
      
      //____ Reset event properties
      fnCollisions = 0;
      nTrEvent = 0.;
      nJpsiEvent = 0;
      //____
      
      
      //____ Generate collisions for the bunch crossing
      fnCollisions = frand->Poisson(mu); // Number of collisions in this event
      if ( fnCollisions == 0 ) continue; // If the bunch crossing have no collisions we skip it
      else if ( fnCollisions > 1 ) fnMBEventsMore1Col++; // It count as a pile up event (more than 1 collision)
      
      fnMBEvents++; // If there are collisions in the bunch crossing then is a MB event
      //____
      
      
      //____________ Definition of arrays to contain the number of tracklets, z vertex position and Nof Jpsis of each collision
      TArrayI* nTrsEventArray = new TArrayI(fnCollisions);
      TArrayD* zVtxsEventArray = new TArrayD(fnCollisions);
      TArrayI* nJpsiEventArray = new TArrayI(fnCollisions);
      //____________
      
      
      //      std::cout << " ========= NEW EVENT ==========" << std::endl;
      
      
      //____________ Set event collision's properties
      SetEvent(*zVtxsEventArray,*nTrsEventArray,*nJpsiEventArray);
      //____________
      
      
      //____________ Search for merged-vertex pile-up collisions
      UInt_t puMask(0); // Mask to know what positions of the collision array contains a pile-up collision
      UInt_t puMaskR(0); // Mask to know what positions of the collision array have been merged to another collision
      
      PileUpMerging(puMask,puMaskR,*zVtxsEventArray,*nTrsEventArray,*nJpsiEventArray);
      //____________
      
      
      //____________ Decide which is main collision and set main Zvertex and NtrCorr of the event
      UInt_t mainColMask(0);
      Int_t posMax(0);
      
      SetMainCol(mainColMask,puMaskR,posMax,spdZvMain,nTrEvent,zVtxsEventArray,nTrsEventArray);
      //____________
      ;
      
      //____________ Skip events not passing the vertex cut
      if ( TMath::Abs(spdZvMain) > 10. )
      {
        delete nTrsEventArray;
        delete zVtxsEventArray;
        delete nJpsiEventArray;
        continue; // We select only events with |MainZSPD| < 10 cm
      }
      //____________
      
      
      //____________ Check if the event is a vertex-merged event pile-up
      Bool_t colPU(kFALSE);
      if ( mainColMask & puMask ) // The main collision is a pile-up one so the event is tagged as pile-up
      {
        colPU = kTRUE;
        
        fnEventsPileUp++; // The event counts as a vertex-merged event pile-up
      }
      //____________
      
      
      //____________ Event NofJpsi determination
      SetNofJpsi(nJpsiEvent,posMax,nJpsiEventArray);
      
      fnofTotalJpsi += nJpsiEvent/(fYieldEnhancement); // P = P_{Jpsi}^{col1} + P_{Jpsi}^{col2} + ... ?? //FIXME:Not clear how to renormalize by the yieldEnhancement factor
      //____________
      
      
      //_______ Get the fraction of PU events in the bins
      GetPUFraction(colPU,nTrEvent,nJpsiEvent);
      //_______
      
      
      //_______ Set the event properties in the histos
      hNtr->Fill(nTrEvent); // Fill the NofTracklets distribution
      hYP->Fill(nTrEvent,nJpsiEvent); //Fill the Nof Jpsi in the Ntr bin
      hYieldP->Fill(nTrEvent,nJpsiEvent);
      //_______
      
      
      //_______ Delete the created arrays to contain event properties
      delete nTrsEventArray;
      delete zVtxsEventArray;
      delete nJpsiEventArray;
      //_______
      
      
    } //End of events loop
    
    
    //_______ Print out the found values
    PrintValues();
    //_______
    
    
    //_______ Set the values of the event NtrCorr, Jpsis and Yield in the output histos
    hNtr->SetLineColor(1 + iColor);
    flegendNtr->AddEntry(hNtr, Form("N_{tr}^{corr} for MU = %1.3f",mu), "l");
    fhNArray->Add(hNtr);
    
    hYP->SetLineColor(1 + iColor);
    flegendNofJpsi->AddEntry(hYP, Form("N^{J/#psi} for MU = %1.3f",mu), "l");
    fhYArray->Add(hYP);
    
    hYieldP->SetLineColor(1 + iColor);
    flegendYield->AddEntry(hYieldP, Form("J/#psi yield for MU = %1.3f",mu), "l");
    fhYieldArray->Add(hYieldP);
    //_______
    
    
    iColor++;
  } //End of MU's loop
  
  delete muArray;
  //_______End of body of simulation___
  //___________________________________
  
  
  
  //______Plot the output histos
  DrawHistos();
  //_______
  
  delete fhNArray;
  delete fhYArray;
  delete fhYieldArray;
  delete frand;
  delete[] faxis;
}


