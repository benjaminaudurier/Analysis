/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include <Riostream.h>

// ROOT includes
#include <TMath.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>
#include <TArrayD.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>
#include <TObjArray.h>
#include <TLegend.h>

// STEER includes
#include "AliLog.h"
#include "AliAODEvent.h"
#include "AliMCEvent.h"
#include "AliAODMCHeader.h"
#include "AliAODMCParticle.h"
#include "AliAODTrack.h"
#include "AliAODDimuon.h"

// ANALYSIS includes
#include "AliAnalysisTaskSE.h"
#include "AliAnalysisManager.h"
#include "AliMergeableCollection.h"
#include "AliAnalysisMuMuSpectra.h"

#include "AliAnalysisTaskGenTunerJpsi.h"

ClassImp(AliAnalysisTaskGenTunerJpsi)

//________________________________________________________________________
AliAnalysisTaskGenTunerJpsi::AliAnalysisTaskGenTunerJpsi() :
AliAnalysisTaskSE(),
fList(0x0),
fCentMin(-FLT_MAX),
fCentMax(FLT_MAX),
fMuonTrackCuts(0x0),
fPtCut(-1.),
fWeight(kFALSE),
fPtNofBin(0),
fYNofBin(0),
fPtBin(0),
fYBin(0),
fPtFunc(0x0),
fPtFuncMC(0x0),
fPtFix(0x0),
fPtFuncNew(0x0),
fPtFixNew(0x0),
fYFunc(0x0),
fYFuncMC(0x0),
fYFix(0x0),
fYFuncNew(0x0),
fYFixNew(0x0),
fcRes(0x0),
fcRat(0x0),
fPtCopyFunc(0x0),
fPtCopyFuncNew(0x0),
fYCopyFunc(0x0),
fYCopyFuncNew(0x0)
{
  /// Dummy constructor
}

//________________________________________________________________________
AliAnalysisTaskGenTunerJpsi::AliAnalysisTaskGenTunerJpsi(const char *name) :
AliAnalysisTaskSE(name),
fList(0x0),
fCentMin(-FLT_MAX),
fCentMax(FLT_MAX),
fMuonTrackCuts(0x0),
fPtCut(-1.),
fWeight(kFALSE),
fPtNofBin(0),
fYNofBin(0),
fPtBin(0),                 
fPtFunc(0x0),
fPtFuncMC(0x0),
fPtFix(0x0),
fPtFuncNew(0x0),
fPtFixNew(0x0),
fYBin(0),                  
fYFunc(0x0),
fYFuncMC(0x0),
fYFix(0x0),
fYFuncNew(0x0),
fYFixNew(0x0),
fcRes(0x0),
fcRat(0x0),
fPtCopyFunc(0x0),
fPtCopyFuncNew(0x0),
fYCopyFunc(0x0),
fYCopyFuncNew(0x0),
fHptRef(0x0),
fHyRef(0x0)
{
  /// Constructor
  
  // Output slot #1 writes into a TObjArray container
  DefineOutput(1,TObjArray::Class());
  
}

//________________________________________________________________________
AliAnalysisTaskGenTunerJpsi::~AliAnalysisTaskGenTunerJpsi()
{
  /// Destructor
  
  if (!AliAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fList;
  delete fMuonTrackCuts;
  delete fPtFunc;
  delete fPtFuncMC;
  delete fPtFix;
  delete fPtFuncNew;
  delete fPtFixNew;
  delete fYFunc;
  delete fYFuncMC;
  delete fYFix;
  delete fYFuncNew;
  delete fYFixNew;
  delete fcRes;
  delete fcRat;
  delete fPtCopyFunc;
  delete fPtCopyFuncNew;
  delete fYCopyFunc;
  delete fYCopyFuncNew;
  delete fHptRef;
  delete fHyRef;
  delete[] fPtBin;                 
  delete[] fYBin;                 
}

//___________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::UserCreateOutputObjects()
{
  /// Create histograms
  
  // initialize histos
  fList = new TObjArray(2000);
  fList->SetOwner();
  
Int_t nofptbin = fPtNofBin-1;
Int_t nofybin = fYNofBin-1;

if (nofptbin==0 || nofybin==0 )
{
  AliError("I don't have the number of bin ...");
  return;
}

// if(!fPtBin || !fYBin) return;

 // const Double_t* ptbin[fPtNofBin] ={0.,1,2.,3.,4.,5.,6.,8.};
 // const Double_t* ybin[fYNofBin] ={-4,-3.75,-3.5,-3.25,-3.0,-2.75,-2.5};

  TH1* hPtGen = new TH1D("hPtGen","generated p_{T} distribution;p_{T} (GeV/c);dN/dp_{T}",nofptbin,fPtBin);
  hPtGen->Sumw2();// Create structure to store sum of squares of weights
  fList->AddAtAndExpand(hPtGen, kPtGen);// Double array size if reach the end
  TH1* hPtRec = new TH1D("hPtRec","reconstructed p_{T} distribution;p_{T} (GeV/c);dN/dp_{T}",nofptbin,fPtBin);
  hPtRec->Sumw2();// Create structure to store sum of squares of weights
  fList->AddAtAndExpand(hPtRec, kPtRec);// Double array size if reach the end
  
  TH1* hYGen = new TH1D("hYGen","generated y distribution;y;dN/dy",nofybin,fYBin);
  hYGen->Sumw2();
  fList->AddAtAndExpand(hYGen, kYGen);
  TH1* hYRec = new TH1D("hYRec","reconstructed y distribution;y;dN/dy",nofybin,fYBin);
  hYRec->Sumw2();
  fList->AddAtAndExpand(hYRec, kYRec);

  if(fHptRef)fHptRef->Sumw2();// Set histo error
  if(fHyRef)fHyRef->Sumw2();// set histo error
  
  // recreate the functions for weighting particles because they are lost when streamed to a file (proof and grid mode)
  if (fWeight && fPtFunc && fPtFuncNew && fYFunc && fYFuncNew) 
  {
    fPtCopyFunc = new TF1("fPtCopyFunc", Pt, fPtFunc->GetXmin(), fPtFunc->GetXmax(), fgkNPtParam);
    fPtCopyFunc->SetParameters(fPtFunc->GetParameters());
    fPtCopyFuncNew = new TF1("fPtCopyFuncNew", Pt, fPtFuncNew->GetXmin(), fPtFuncNew->GetXmax(), fgkNPtParam);
    fPtCopyFuncNew->SetParameters(fPtFuncNew->GetParameters());
    fYCopyFunc = new TF1("fYCopyFunc", Y, fYFunc->GetXmin(), fYFunc->GetXmax(), fgkNYParam);
    fYCopyFunc->SetParameters(fYFunc->GetParameters());
    fYCopyFuncNew = new TF1("fYCopyFuncNew", Y, fYFuncNew->GetXmin(), fYFuncNew->GetXmax(), fgkNYParam);
    fYCopyFuncNew->SetParameters(fYFuncNew->GetParameters());
  } 
  else 
  {
    // or disable the weighting
    fWeight = kFALSE;
  }

  // Post data at least once per task to ensure data synchronisation (required for merging)
  PostData(1, fList);
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::NotifyRun()
{
  /// Prepare processing of new run: load corresponding OADB objects...
  
  // get the trackCuts for this run
  if (!fMuonTrackCuts) AliFatal("You must specify the requested selections (AliMuonTrackCut obj is missing)");
  fMuonTrackCuts->SetRun(fInputHandler);
  
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::UserExec(Option_t *)
{
  /// process event
  
  // get AOD event
  AliAODEvent* aod = dynamic_cast<AliAODEvent*>(InputEvent());
  if ( !aod ) return;
  
  // select the centrality range
  Float_t centrality = aod->GetCentrality()->GetCentralityPercentileUnchecked("V0M");
  if (centrality <= fCentMin || centrality > fCentMax) return;
  
  TArrayD weight; 

  if (!MCEvent()) return; 

  weight.Set(MCEvent()->GetNumberOfTracks());

  //__________Generated tracks part
  for (Int_t i = 0; i < MCEvent()->GetNumberOfTracks(); i++) 
  { 
    //Set size
    

    // get MC particle track
    AliAODMCParticle *mctrack = static_cast<AliAODMCParticle*>(MCEvent()->GetTrack(i));

    // get PID
    Int_t mctrackPID = mctrack->PdgCode();
    
    if (mctrack->IsPrimary())// Gen. tracks
    {
      // Only J/psi
      if(mctrackPID!=443) continue; 
      
      Double_t y  = mctrack->Y();
      Double_t pT = mctrack->Pt();

      if( pT>7 || pT<0 || y < -4 || y>-2.5 ) continue; 

      //________compute weight
      if (fWeight) 
      {
        weight[i] = (fPtCopyFuncNew->Eval(pT) / fPtCopyFunc->Eval(pT) )*( fYCopyFuncNew->Eval(y) / fYCopyFunc->Eval(y));// Compute Weight
        if (weight[i] < 0.) 
        {
          AliError(Form("negative weight: y = %g, pT = %g: w = %g", y, pT, weight[i]));
          weight[i] = 0.;
        }
      } 
      else weight[i] = 1.;
      //________
      Double_t w = weight[i];
      // if (pT>6)
      // {
      //   cout << "Pt               : " << pT << endl;
      //   cout << "Y                : " << y << endl;
      //   // // cout << "w          : " << w << endl;
      //   // cout << "fPtCopyFuncNew: " << fPtCopyFuncNew->Eval(pT) << endl;
      //   if(!fPtCopyFunc)cout << "fPtCopyFunc is nul  " << endl;
      //   if(!fPtCopyFuncNew)cout << "fPtCopyFuncNew is nul  " << endl;
      //   if(!fYCopyFuncNew)cout << "fYCopyFuncNew is nul  " << endl;
      //   if(!fYCopyFunc)cout << "fYCopyFunc is nul  " << endl;
      //   // cout << "fYCopyFuncNew : " << fYCopyFuncNew->Eval(y) << endl;
      //   // cout << "fYCopyFunc    : " << fYCopyFunc->Eval(y) << endl;
      //   // cout << "" << endl;
      // }

      
    ((TH1*)fList->UncheckedAt(kPtGen))->Fill(pT, w); // fill PtGenHisto
    ((TH1*)fList->UncheckedAt(kYGen))->Fill(y, w); // fill YGenHisto
    // ((TH1*)fList->UncheckedAt(kPhiGen))->Fill(mctrack->Phi()*TMath::RadToDeg(), weight);// fill PhiGenHisto
    }
  }
  //__________

  //__________Rec. Part
  for (Int_t i = 0; i < aod->GetNumberOfTracks(); i++)
  {
    AliAODTrack *track1 = static_cast<AliAODTrack*>(aod->GetTrack(i)); // track 1
    
    Int_t mcLabel1 = track1->GetLabel();
    if (!fMuonTrackCuts->IsSelected(track1) || (MCEvent() && mcLabel1 < 0)) continue;
    //    Physic selection                      track associated with mc

    AliAODMCParticle *mctrack = static_cast<AliAODMCParticle*>(MCEvent()->GetTrack(mcLabel1)); // MC info associated with track1
    if((mctrack->PdgCode()!=13 && mctrack->PdgCode()!=-13)) continue; // Cut on muons

    //Check mother
    Int_t mother = mctrack->GetMother(); // mother label
    if(mother<0) continue;
    AliAODMCParticle *mcmothertrack = static_cast<AliAODMCParticle*>(MCEvent()->GetTrack(mother)); // MC mother trac
    if(mcmothertrack->PdgCode()!=443 ) continue; // Check if mother is a J/psi

      // Get 4-Vector  
      TLorentzVector vec(track1->Px(),track1->Py(),track1->Pz(),track1->E());
      // cout << "vec.M() = " <<vec.M()<< endl;
    
    for (Int_t j = i+1; j < aod->GetNumberOfTracks(); j++)// Loop over second paricles 
    {
      AliAODTrack *track2 = static_cast<AliAODTrack*>(aod->GetTrack(j)); // track 2
    
      Int_t mcLabel2 = track2->GetLabel();
      if (!fMuonTrackCuts->IsSelected(track2) || (MCEvent() && mcLabel2 < 0) || (track2->Charge()*track1->Charge()>=0)   ) continue;
      //    Physic selection                      track associated with mc part.    opposite sign  
      
      // MC info associated with track2    
      AliAODMCParticle *mctrack2 = static_cast<AliAODMCParticle*>(MCEvent()->GetTrack(mcLabel2)); 
      
      if((mctrack2->PdgCode()!=13 && mctrack2->PdgCode()!=-13)  ) continue; 
      //                  Cut muons                               

      //Check mother
      Int_t mother2 = mctrack2->GetMother(); // mother label
      if(mother2<0) continue;
      AliAODMCParticle *mcmothertrack2 = static_cast<AliAODMCParticle*>(MCEvent()->GetTrack(mother2)); // MC mother trac
      if(mcmothertrack2->PdgCode()!=443 ) continue; // Check if mother is a J/psi

      // Get 4-Vector
      TLorentzVector vec2(track2->Px(),track2->Py(),track2->Pz(),track2->E());

      // Add the two 4-vectors.
      TLorentzVector Dimu = vec2+vec;

      // Cut on paire
      if( Dimu.Pt()>7 || Dimu.Pt()<0 || Dimu.Rapidity() < -4 || Dimu.Rapidity()>-2.5 ) continue; 
      
      //________compute weight
      if (fWeight) 
      {
        weight[i] = fPtCopyFuncNew->Eval(Dimu.Pt()) / fPtCopyFunc->Eval(Dimu.Pt()) * fYCopyFuncNew->Eval(Dimu.Rapidity()) / fYCopyFunc->Eval(Dimu.Rapidity());// Compute Weight
        if (weight[i] < 0.) 
        {
          AliError(Form("negative weight: Dimu.Rapidity() = %g, Dimu.Pt() = %g: w = %g", Dimu.Rapidity(), Dimu.Pt(), weight[i]));
          weight[i] = 0.;
        }
      } 
      else weight[i] = 1.;
      //________
      Double_t w = weight[i];

      // cout << "Dimu.Pt() = " <<Dimu.Pt()<< endl; 
      // cout << "Diff = " << Dimu.Rapidity() -y << endl; 
      // cout << "vec2.Y() = " <<vec2.Y()<< endl; 
      ((TH1*)fList->UncheckedAt(kPtRec))->Fill(Dimu.Pt(),w);
      ((TH1*)fList->UncheckedAt(kYRec))->Fill(Dimu.Rapidity(),w);
    } 
  } 
  //__________

   
  // Post final data. It will be written to a file with option "RECREATE"

  PostData(1, fList);
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::Terminate(Option_t *)
{
  /// post-processing
  
  //__________get current results
  Int_t hIndex[4] = {kPtGen, kYGen, /*kPhiGen,*/ kPtRec, kYRec/*, kPhiRec*/};
  fList = static_cast<TObjArray*>(GetOutputData(1));
  TH1 *h[4]; // List to store histo
  for (Int_t i = 0; i < 4; i++) 
  {
    h[i] = static_cast<TH1*>(fList->UncheckedAt(hIndex[i])->Clone());
    h[i]->SetDirectory(0);
    h[i]->Scale(1,"width");// Normalize
  }
  //  new TCanvas;
  // h[0]->Draw("");
  // new TCanvas;
  // h[1]->Draw("");
  // new TCanvas;
  // h[2]->Draw("");
  // new TCanvas;
  // h[3]->Draw("");
  // return; 

  //__________get the fit ranges
  Double_t fitRangeMC[2][2];
  fitRangeMC[0][0] = GetFitLowEdge(*(h[0]));
  fitRangeMC[0][1] = 999.;
  fitRangeMC[1][0] = GetFitLowEdge(*(h[1]));
  fitRangeMC[1][1] = GetFitUpEdge(*(h[1]));
  Double_t fitRange[2][2];
  fitRange[0][0] = 0.;
  fitRange[0][1] = 7.;
  fitRange[1][0] = -3.98; // not -4. because to the influence of the eta cut
  fitRange[1][1] = -2.5;
  //__________
  
  //__________compute acc*eff corrections if it is simulated data
  TH1 *hAccEff[2] = {0x0,       0x0/*,      0x0*/};
  //                 AccEffPt   AccEffY   AccEffPhi

  for (Int_t i = 0; i < 2 && h[i]->GetEntries() > 0; i++) 
  {
    hAccEff[i] = ComputeAccEff(*(h[i]), *(h[i+2]), Form("%sOverGen",h[i+2]->GetName()), "Acc#{times}Eff");
    hAccEff[i]->SetTitle("Acc#{times}Eff");
  }
  //__________
  
  //__________get reference data if provided
  TH1 *hRef[4] = {0x0,            0x0,             0x0,          0x0};
  //              ptAccEffCorr.   yAccEffCorr.     RefPtHisto,   RefYHisto, 

  // J/psi vs PT histo
  if(fHptRef) hRef[2] = static_cast<TH1*>(fHptRef->Clone());
  // J/psi vs Y histo
  if(fHyRef) hRef[3]  = static_cast<TH1*>(fHyRef->Clone());
  //__________
  

  //__________compute corrected data
  for (Int_t i = 0; i < 2 && hRef[i+2] && hAccEff[i]; i++) 
  {
    hRef[i] = static_cast<TH1*>(hRef[i+2]->Clone());
    
    if(!hRef[i])
    {
      cout << "Can't clone ref. histo " << i << endl;
      continue;
    }
    hRef[i]->SetTitle("corrected data");
    if(!hRef[i]->Divide(hAccEff[i]))
    {
      cout << "Can't divide " << i << endl;
      continue;
    }
  }
  //__________
  

  //__________normalize histograms
  Bool_t normalized = kFALSE;
  for (Int_t i = 0; i < 2 && hRef[i]; i++) 
  {
    Double_t integral = h[i]->Integral(h[i]->FindBin(fitRange[i][0]), h[i]->FindBin(fitRange[i][1]), "width");
    Double_t norm = (integral != 0.) ? 1./integral : 1.;
    h[i]->Scale(norm);
    h[i+2]->Scale(norm);
    integral = hRef[i]->Integral(hRef[i]->FindBin(fitRange[i][0]), hRef[i]->FindBin(fitRange[i][1]), "width");
    norm = (integral != 0.) ? 1./integral : 1.;
    hRef[i]->Scale(norm);
    hRef[i+2]->Scale(norm);
    normalized = kTRUE;
  }
  //_________
   
  //__________compute dataCorr/MC ratios
  TH1 *hRat[4] = {0x0,                  0x0,                 0x0,              0x0            };
  //              ptAccEffCorr./ptGen   yAccEffCorr./yGen    RefPtHisto/RecPt  RefYHisto/RecY 
  for (Int_t i = 0; i < 4 && hRef[i]; i++) 
  {
    hRat[i] = static_cast<TH1*>(hRef[i]->Clone());
    hRat[i]->SetTitle("data / MC");
    hRat[i]->Divide(h[i]);
    if(!hRat[i]) cout << Form("Cannot divide histo %d",i) << endl;
  }
  //__________

  //__________prepare fitting functions
  if (hAccEff[0]) // Pt AccEff histo
  {
    cout << "prepare fitting functions for Pt AccEff histo... " << endl;

    if (fPtFunc) 
    { 
      fPtFunc->SetRange(fitRange[0][0], fitRange[0][1]);
      if (fWeight && fPtFuncNew) fPtFunc->SetParameters(fPtFuncNew->GetParameters());// Change old fit param. with new fit param.
      NormFunc(fPtFunc, fitRange[0][0], fitRange[0][1]);
    }
    if (fPtFuncMC) 
    { 
      fPtFuncMC->SetRange(fitRangeMC[0][0], fitRangeMC[0][1]);
      if (fWeight && fPtFuncNew) fPtFuncMC->SetParameters(fPtFuncNew->GetParameters());// Change old fit param. with new fit param.
      NormFunc(fPtFuncMC, fitRange[0][0], fitRange[0][1]);
    }
    if (hRef[0] && fPtFuncNew) 
    {
      fPtFuncNew->SetRange(fitRange[0][0], fitRange[0][1]);
      NormFunc(fPtFuncNew, fitRange[0][0], fitRange[0][1]);
    }
    if (!normalized) 
    {
      Double_t integral = h[0]->Integral(h[0]->FindBin(fitRange[0][0]), h[0]->FindBin(fitRange[0][1]), "width");
      if (fPtFunc) fPtFunc->SetParameter(0, fPtFunc->GetParameter(0)*integral);
      if (fPtFuncMC) fPtFuncMC->SetParameter(0, fPtFuncMC->GetParameter(0)*integral);
      if (fPtFuncNew) 
      {
      	integral = hRef[0]->Integral(hRef[0]->FindBin(fitRange[0][0]), hRef[0]->FindBin(fitRange[0][1]), "width");
      	fPtFuncNew->SetParameter(0, fPtFuncNew->GetParameter(0)*integral);
      }
    }
  }
  
  if (hAccEff[1]) // Y AccEff histo
  {
    cout << "prepare fitting functions for Y AccEff histo..." << endl;

    if (fYFunc) 
    { 
      // cout <<""<< endl;
      // cout << "fYFunc ok" << endl;
      fYFunc->SetRange(fitRange[1][0], fitRange[1][1]);
      if (fWeight && fYFuncNew) fYFunc->SetParameters(fYFuncNew->GetParameters());// Change old fit param. with new fit param.
      NormFunc(fYFunc, fitRange[1][0], fitRange[1][1]);
    }
    if (fYFuncMC) 
    { 
      // cout <<""<< endl;
      // cout << "fYFuncMC ok" << endl;
      fYFuncMC->SetRange(fitRangeMC[1][0], fitRangeMC[1][1]);
      if (fWeight && fYFuncNew) fYFuncMC->SetParameters(fYFuncNew->GetParameters());// Change old fit param. with new fit param.
      NormFunc(fYFuncMC, fitRange[1][0], fitRange[1][1]);
    }
    if (hRef[1] && fYFuncNew) 
    { 
      // cout <<""<< endl;
      // cout << "hRef[1] && fYFuncNew ok" << endl;
      fYFuncNew->SetRange(fitRange[1][0], fitRange[1][1]);
      NormFunc(fYFuncNew, fitRange[1][0], fitRange[1][1]);
    }
    if (!normalized) 
    { 
      // cout <<""<< endl;
      // cout << "!normalized ok" << endl;
      Double_t integral = h[1]->Integral(h[1]->FindBin(fitRange[1][0]), h[1]->FindBin(fitRange[1][1]), "width");
      if (fYFunc) fYFunc->SetParameter(0, fYFunc->GetParameter(0)*integral);
      if (fYFuncMC) fYFuncMC->SetParameter(0, fYFuncMC->GetParameter(0)*integral);// Change old fit param. with new fit param.
      if (fYFuncNew) 
      {
      	integral = hRef[1]->Integral(hRef[1]->FindBin(fitRange[1][0]), hRef[1]->FindBin(fitRange[1][1]), "width");
      	fYFuncNew->SetParameter(0, fYFuncNew->GetParameter(0)*integral);// Change old fit param. with new fit param.
      }
    }
  }
  //__________
  
  //__________plot results
  fcRes = new TCanvas("cRes", "results", 900, 600);
  fcRes->Divide(2,2);

//------Pad 1
  fcRes->cd(1);
  gPad->SetLogy();
  TLegend*leg = new TLegend(0.48,0.7,0.70,0.9);

  if (hAccEff[0]) 
  {

    if (fPtFuncMC) 
    {
      fPtFuncMC->SetLineColor(3);// Green
      fPtFuncMC->SetLineWidth(3);
      h[0]->Fit(fPtFuncMC, "IWLMR", "e0sames");// Gen. pt histo.
      leg->AddEntry(fPtFuncMC,"MC fit function","l");
    } 
    else h[0]->Draw("");
    leg->AddEntry(h[0],"MC Generated tracks ","lep");
    
    if (fPtFunc) 
    {
      fPtFunc->SetLineColor(4); // Blue
      h[0]->Fit(fPtFunc, "IWLMR+");
      leg->AddEntry(fPtFunc,"Old Data fit function","l");
    }
  }
  
  if (hRef[0]) // AccEff corrected Data pt histo. 
  {
    hRef[0]->SetLineColor(2); // Red
    if (fPtFuncNew) 
    {
      fPtFuncNew->SetLineColor(2);// Red
      hRef[0]->Fit(fPtFuncNew, "IWLMR", "sames");
      leg->AddEntry(fPtFuncNew,"New Data Fit function","l");
    } 
    else hRef[0]->Draw("sames");
    leg->AddEntry(hRef[0],"AccEff corrected Data","lep");
  }
  leg->Draw("same");

//------Pad 2
  fcRes->cd(2);
  TLegend*leg2 = new TLegend(0.48,0.7,0.70,0.9);

  if (hAccEff[1]) 
  {
    if (fYFuncMC) 
    {
      fYFuncMC->SetLineColor(3);// Green
      fYFuncMC->SetLineWidth(3);
      h[1]->Fit(fYFuncMC, "IWLMR", "sames");//  Gen. y histo.
      leg2->AddEntry(fYFuncMC,"MC fit function","l");
    } 
    else h[1]->Draw("");
    leg2->AddEntry(h[1],"MC Generated tracks ","lep");
    
    if (fYFunc) 
    {
      fYFunc->SetLineColor(4);// Blue
      h[1]->Fit(fYFunc, "IWLMR+");
      leg2->AddEntry(fYFunc,"Old Data fit function","l");
    }
  }
  if (hRef[1])  // AccEff corrected Data pt histo. 
  {
    hRef[1]->SetLineColor(2);// Red
    if (fYFuncNew) 
    {
      fYFuncNew->SetLineColor(2); // Red
      hRef[1]->Fit(fYFuncNew, "IWLMR", "sames");
      leg2->AddEntry(fYFuncNew,"New Data Fit function","l");
    } 
    else hRef[1]->Draw("sames");
    leg2->AddEntry(hRef[1],"AccEff corrected Data","lep");
  }
  leg2->Draw("same");

//------Pad 3 and 4
  for (Int_t i = 2; i < 4; i++) 
  {
    fcRes->cd(i+1);
    TLegend*legend = new TLegend(0.48,0.7,0.70,0.9);

    if (i == 2) gPad->SetLogy();
    h[i]->Draw(""); // AccEff corrected Rec.  histo.
    legend->AddEntry(h[i],"Rec. MC Track","lep");
    
    if (hRef[i]) 
    {
      hRef[i]->SetLineColor(2);// Red
      hRef[i]->Draw("sames");// data histo.
      legend->AddEntry(hRef[i],"Data Track","lep");
    }
    legend->Draw("same");
  }
  //__________
  
  //__________normalize functions to their integral in the range used in MC
  if (hAccEff[0] && fPtFunc) 
  {
    fPtFunc->SetRange(fitRangeMC[0][0], fitRangeMC[0][1]);
    NormFunc(fPtFunc, fitRangeMC[0][0], fitRangeMC[0][1]);
  }
 
  if (hAccEff[0] && fPtFuncMC) 
  {
    NormFunc(fPtFuncMC, fitRangeMC[0][0], fitRangeMC[0][1]);
  }
 
  if (hRef[0] && fPtFuncNew) 
  {
    fPtFuncNew->SetRange(fitRangeMC[0][0], fitRangeMC[0][1]);
    NormFunc(fPtFuncNew, fitRangeMC[0][0], fitRangeMC[0][1]);
  }
  
  if (hAccEff[1] && fYFunc) 
  {
    fYFunc->SetRange(fitRangeMC[1][0], fitRangeMC[1][1]);
    NormFunc(fYFunc, fitRangeMC[1][0], fitRangeMC[1][1]);
  }
  
  if (hAccEff[1] && fYFuncMC)
  {
    NormFunc(fYFuncMC, fitRangeMC[1][0], fitRangeMC[1][1]);
  }
  
  if (hRef[1] && fYFuncNew) 
  {
    fYFuncNew->SetRange(fitRangeMC[1][0], fitRangeMC[1][1]);
    NormFunc(fYFuncNew, fitRangeMC[1][0], fitRangeMC[1][1]);
  }
  //__________
  
  //__________prepare data/MC function ratios
  TF1 *ptRat = 0x0;
  if (hRat[0] && fPtFunc && fPtFuncNew) 
  {
    ptRat = new TF1("ptRat", PtRat, fitRangeMC[0][0], hRat[0]->GetXaxis()->GetXmax(), 2*fgkNPtParam);
    Double_t p[2*fgkNPtParam];
    fPtFuncNew->GetParameters(p);
    fPtFunc->GetParameters(&(p[fgkNPtParam]));
    ptRat->SetParameters(p);
  }
  TF1 *yRat = 0x0;
  
  if (hRat[1] && fYFunc && fYFuncNew) 
  {
    yRat = new TF1("yRat", YRat, fitRangeMC[1][0], fitRangeMC[1][1], 2*fgkNYParam);
    Double_t p[2*fgkNYParam];
    fYFuncNew->GetParameters(p);
    fYFunc->GetParameters(&(p[fgkNYParam]));
    yRat->SetParameters(p);
  }
  //__________
  
  //__________plot ratios
  fcRat = new TCanvas("cRat", "ratios", 900, 600);
  fcRat->Divide(2,2);
  
  for (Int_t i = 0; i < 4 && hRat[i]; i++) 
  {
    fcRat->cd(i+1);
    TLegend*leg= new TLegend(0.48,0.8,0.70,0.9);
    hRat[i]->Draw("e0");
    if (i == 0 && ptRat) 
    {
      ptRat->Draw("same");
      leg->AddEntry(ptRat,"MC-Data fit function","l");
      leg->AddEntry(hRat[i],"Data Dist. / MC Gen. ","leg"); 
    }
    else if (i == 1 && yRat)
    {
      yRat->Draw("same");
      leg->AddEntry(ptRat,"MC-Data fit function","l");
      leg->AddEntry(hRat[i],"Data Dist. / MC Gen. ","leg");
    }
    leg->AddEntry(hRat[i],"Data/Rec","leg"); 
    
    leg->Draw("same");
  }
  // __________
  
  //__________print fitting ranges
  if (fPtFuncMC && fYFuncMC) 
  {
    printf("\npT fitting range MC = [%g, %g]\n", fitRangeMC[0][0], fitRangeMC[0][1]);
    printf("y fitting range MC = [%g, %g]\n\n", fitRangeMC[1][0], fitRangeMC[1][1]);
  }
  
  if (fPtFunc && fYFunc) 
  {
    printf("pT fitting range = [%g, %g]\n", fitRange[0][0], fitRange[0][1]);
    printf("y fitting range = [%g, %g]\n\n", fitRange[1][0], fitRange[1][1]);
  }
  //__________
  
  //__________print parameters
  Double_t *param = GetOldPtParamMC();
  
  if (param) 
  {
    printf("Double_t oldPtParamMC[%d] = {", fgkNPtParam);
    for (Int_t i = 0; i < fgkNPtParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n", param[fgkNPtParam-1]);
  }
  param = GetOldYParamMC();
  
  if (param) 
  {
    printf("Double_t oldYParamMC[%d] = {", fgkNYParam);
    for (Int_t i = 0; i < fgkNYParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n\n", param[fgkNYParam-1]);
  }
  param = GetOldPtParam();
  
  if (param) 
  {
    printf("Double_t oldPtParam[%d] = {", fgkNPtParam);
    for (Int_t i = 0; i < fgkNPtParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n", param[fgkNPtParam-1]);
  }
  param = GetOldYParam();
  
  if (param) 
  {
    printf("Double_t oldYParam[%d] = {", fgkNYParam);
    for (Int_t i = 0; i < fgkNYParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n\n", param[fgkNYParam-1]);
  }
  param = GetNewPtParam();
  
  if (param) 
  {
    printf("Double_t newPtParam[%d] = {", fgkNPtParam);
    for (Int_t i = 0; i < fgkNPtParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n", param[fgkNPtParam-1]);
  }
  param = GetNewYParam();
  
  if (param) 
  {
    printf("Double_t newYParam[%d] = {", fgkNYParam);
    for (Int_t i = 0; i < fgkNYParam-1; i++) printf("%g, ", param[i]);
    printf("%g};\n\n", param[fgkNYParam-1]);
  }
  //__________
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::SetPtBin(Int_t nofbin,Double_t* bin)
{
  fPtNofBin = nofbin ;

  if(nofbin==0)
  {

    fPtBin = 0x0;
    return;
  } 

  fPtBin = new Double_t[fPtNofBin];

  for (int i = 0; i < fPtNofBin; ++i)
  {
    fPtBin[i]=bin[i];
    // cout << "fPtBin " << i << "=" << fPtBin[i] << endl;
  }
  return;
} 

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::SetYBin(Int_t nofbin,Double_t* bin)
{
  fYNofBin = nofbin;

  if(nofbin==0)
  {
    fYBin = 0x0;
    return;
  } 

  fYBin = new Double_t[fYNofBin];

  for (int i = 0; i < fYNofBin; ++i)
  {
    fYBin[i]=bin[i];
    // cout << "fYBin " << i << "=" << fYBin[i] << endl;
  }
  return;
} 


//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::SetPtParam(const Double_t *pOld, const Bool_t *fixOld, const Double_t *pNew,
					 const Bool_t *fixNew, Double_t min, Double_t max)
{
  /// create the function(s) to fit the generated pT distribution with the given parameters
  /// if the new parameters are given, the new function is used to weight the tracks

  // prepare the old fitting functions
  if (!fPtFunc) fPtFunc = new TF1("fPtFunc", Pt, min, max, fgkNPtParam);
  else fPtFunc->SetRange(min, max);
  if (!fPtFuncMC) fPtFuncMC = new TF1("fPtFuncMC", Pt, min, max, fgkNPtParam);
  else fPtFuncMC->SetRange(min, max);
  
  // set the current parameters
  if (pOld) 
  {
    fPtFunc->SetParameters(pOld);
    fPtFuncMC->SetParameters(pOld);
  }
  
  // fix some of them if required
  if (fixOld) 
  {
    if (!fPtFix) fPtFix = new Bool_t[fgkNPtParam];
    for (Int_t i = 0; i < fgkNPtParam; i++) 
    {
      fPtFix[i] = fixOld[i];
      if (fPtFix[i]) 
      {
      	fPtFunc->FixParameter(i, fPtFunc->GetParameter(i));
      	fPtFuncMC->FixParameter(i, fPtFuncMC->GetParameter(i));
      }
    }
  }
  
  // normalize the functions
  NormFunc(fPtFunc, min, max);
  NormFunc(fPtFuncMC, min, max);
  
  if (pNew) 
  {
    
    // prepare the new fitting function if needed
    if (!fPtFuncNew) fPtFuncNew = new TF1("fPtFuncNew", Pt, min, max, fgkNPtParam);
    else fPtFuncNew->SetRange(min, max);
    
    // set the current parameters
    fPtFuncNew->SetParameters(pNew);
    
    // fix some of them if required
    if (fixNew) 
    {
      if (!fPtFixNew) fPtFixNew = new Bool_t[fgkNPtParam];
      for (Int_t i = 0; i < fgkNPtParam; i++) 
      {
      	fPtFixNew[i] = fixNew[i];
      	if (fPtFixNew[i]) fPtFuncNew->FixParameter(i, fPtFuncNew->GetParameter(i));
      }
    }
    
    // normalize the function
    NormFunc(fPtFuncNew, min, max);
    
  } 
  else 
  {
    
    // or make sure the new function does not exist if not required
    delete fPtFuncNew;
    fPtFuncNew = 0x0;
    
  }
  
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::SetYParam(const Double_t *pOld, const Bool_t *fixOld, const Double_t *pNew,
					const Bool_t *fixNew, Double_t min, Double_t max)
{
  /// create the function(s) to fit the generated y distribution with the given parameters
  /// if the new parameters are given, the new function is used to weight the tracks
  
  // prepare the old fitting functions
  if (!fYFunc) fYFunc = new TF1("fYFunc", Y, min, max, fgkNYParam);
  else fYFunc->SetRange(min, max);
  if (!fYFuncMC) fYFuncMC = new TF1("fYFuncMC", Y, min, max, fgkNYParam);
  else fYFuncMC->SetRange(min, max);
  
  // set the current parameters
  if (pOld) 
  {
    fYFunc->SetParameters(pOld);
    fYFuncMC->SetParameters(pOld);
  }
  
  // fix some of them if required
  if (fixOld) 
  {
    if (!fYFix) fYFix = new Bool_t[fgkNYParam];
    for (Int_t i = 0; i < fgkNYParam; i++) 
    {
      fYFix[i] = fixOld[i];
      if (fYFix[i]) 
      {
      	fYFunc->FixParameter(i, fYFunc->GetParameter(i));
      	fYFuncMC->FixParameter(i, fYFuncMC->GetParameter(i));
      }
    }
  }
  
  // normalize the function
  NormFunc(fYFunc, min, max);
  NormFunc(fYFuncMC, min, max);
  
  if (pNew) 
  {
    
    // prepare the new fitting function if needed
    if (!fYFuncNew) fYFuncNew = new TF1("fYFuncNew", Y, min, max, fgkNYParam);
    else fYFuncNew->SetRange(min, max);
    
    // set the current parameters
    fYFuncNew->SetParameters(pNew);
    
    // fix some of them if required
    if (fixNew) 
    {
      if (!fYFixNew) fYFixNew = new Bool_t[fgkNYParam];
      for (Int_t i = 0; i < fgkNYParam; i++) 
      {
      	fYFixNew[i] = fixNew[i];
      	if (fYFixNew[i]) fYFuncNew->FixParameter(i, fYFuncNew->GetParameter(i));
      }
    }
    
    // normalize the function
    NormFunc(fYFuncNew, min, max);
    
  } 
  else 
  {
    
    // or make sure the new function does not exist if not required
    delete fYFuncNew;
    fYFuncNew = 0x0;
    
  }
  
}

//________________________________________________________________________
TH1* AliAnalysisTaskGenTunerJpsi::ComputeAccEff(TH1 &hGen, TH1 &hRec, const Char_t *name, const Char_t *title)
{
  /// Compute acc*eff and binomial errors by hand, i.e. not using TGraphAsymmErrors
  /// Result is identical to divide histograms with option "B", except here error is forced > 1/gen
  
  Int_t nbins = hGen.GetNbinsX();
  TH1* hAcc = static_cast<TH1*>(hGen.Clone());
  hAcc->SetName(name);
  hAcc->SetTitle(title);
  for (Int_t i = 1; i <nbins+1; i++)
  { 
    Double_t accEff = 0.;
    Double_t accEffErr = 0.;
    Double_t gen = hGen.GetBinContent(i);
    // cout << "gen for bin " << i <<" = " << gen << endl;

    if (gen > 0.) 
    {
      Double_t rec = hRec.GetBinContent(i);
      // cout << "rec for bin " << i <<" = " << rec << endl;

      Double_t genErr = hGen.GetBinError(i);
      // cout << "genErr for bin " << i <<" = " << genErr << endl;

      Double_t recErr = hRec.GetBinError(i);
      // cout << "recErr for bin " << i <<" = " << recErr << endl;


      accEff = rec/gen;
      //Double_t accEffErr2 = ((1.-2.*accEff)*recErr*recErr + accEff*accEff*genErr*genErr)/(gen*gen);
      //accEffErr = TMath::Max(accEff*genErr/gen, TMath::Sqrt(TMath::Abs(accEffErr2)));
      accEffErr = TMath::Max(recErr/gen, accEff*genErr/gen);
      // cout << "AccEff for bin " << i <<" = " << accEff << "+/-" << accEffErr << endl;
    }
    hAcc->SetBinContent(i, accEff);
    hAcc->SetBinError(i, accEffErr);
  }
  
  return hAcc;
}

//________________________________________________________________________
Double_t AliAnalysisTaskGenTunerJpsi::Pt(const Double_t *x, const Double_t *p)
{
  /// generated pT fit function
  
  Double_t pT = *x;
  Double_t arg = 0;
  
  arg = p[0]*pT / TMath::Power( 1 + p[1]*pT*pT, p[2]);
  
  return arg;
}

//________________________________________________________________________
Double_t AliAnalysisTaskGenTunerJpsi::Y(const Double_t *x, const Double_t *p)
{
  /// generated y fit function
  Double_t y = *x;
  Double_t arg1 = 0;
    
  arg1 = p[0] * ( 1 + p[1]*y );
  
  
  return arg1;
}

//________________________________________________________________________

Double_t AliAnalysisTaskGenTunerJpsi::PtRat(const Double_t *x, const Double_t *p)
{
  /// generated pT fit function ratio
  const Double_t *p1 = &(p[0]);
  const Double_t *p2 = &(p[fgkNPtParam]);
  return Pt(x,p1) / Pt(x,p2);
}

//________________________________________________________________________
Double_t AliAnalysisTaskGenTunerJpsi::YRat(const Double_t *x, const Double_t *p)
{
  /// generated y fit function ratio
  const Double_t *p1 = &(p[0]);
  const Double_t *p2 = &(p[fgkNYParam]);
  return Y(x,p1) / Y(x,p2);
}

//________________________________________________________________________
Double_t AliAnalysisTaskGenTunerJpsi::GetFitLowEdge(TH1 &h)
{
  /// adjust the lower edge of the fit range according to the content of the histogram
  Int_t binAbove0 = h.FindFirstBinAbove(0.);
  if (h.GetBinContent(binAbove0) < 0.1*h.GetBinContent(binAbove0+1)) binAbove0++;
  return h.GetBinLowEdge(binAbove0);
}

//________________________________________________________________________
Double_t AliAnalysisTaskGenTunerJpsi::GetFitUpEdge(TH1 &h)
{
  /// adjust the upper edge of the fit range according to the content of the histogram
  Int_t binAbove0 = h.FindLastBinAbove(0.);
  if (h.GetBinContent(binAbove0) < 0.1*h.GetBinContent(binAbove0-1)) binAbove0--;
  return h.GetBinLowEdge(binAbove0+1);
}

//________________________________________________________________________
void AliAnalysisTaskGenTunerJpsi::NormFunc(TF1 *f, Double_t min, Double_t max)
{
  /// normalize the function to its integral in the given range
  Double_t integral = f->Integral(min, max);
  if (integral != 0.) f->SetParameter(0, f->GetParameter(0)/integral);
}

