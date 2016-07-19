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

//include root libraries
#include <iostream>
#include <TCanvas.h>
#include <TList.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TNtuple.h>
#include <TChain.h>
#include "TMath.h"
#include <TObjArray.h>
//include aliroot libraries
#include "AliLog.h"
#include "AliAnalysisManager.h"
#include "AliAODv0.h"
#include "AliVEvent.h"
#include "AliESDEvent.h"
#include "AliAODEvent.h"
#include "AliVVZERO.h"
#include "AliESDVZERO.h"
#include "AliAODVZERO.h"
#include "AliVVertex.h"
#include "AliESDVertex.h"
#include "AliAODVertex.h"
#include "AliAnalysisMuonUtility.h"
#include "AliAnalysisUtils.h"
#include "AliMuonTrackCuts.h"
#include "AliOADBMuonTrackCutsParam.h"
#include "AliCentrality.h"
#include "AliAODDimuon.h"
#include "AliVTrack.h"
#include "AliAODTrack.h"
#include "AliAODTracklets.h"
#include "AliAODHeader.h"
#include "AliESDMuonTrack.h"
#include "AliInputEventHandler.h"
#include "AliMultSelection.h"
#include "AliAODDimuon.h"
//include the class header file
#include "TaskJpsiPP.h"
#include <AliMultiInputEventHandler.h>
#include <AliMixInputEventHandler2.h>
ClassImp(TaskJpsiPP)

//________________________________________________________________________
TaskJpsiPP::TaskJpsiPP()
: AliAnalysisTaskSE(),
fArrayHistosUnlikeInvMass(0),
fArrayHistosUnlikeRapidity(0),
fArrayHistosUnlikePt(0),
fArrayHistosPPLikeInvMass(0),
fArrayHistosPPLikeRapidity(0),
fArrayHistosPPLikePt(0),
fArrayHistosMMLikeInvMass(0),
fArrayHistosMMLikeRapidity(0),
fArrayHistosMMLikePt(0),
fArrayHistosMixUnlikeInvMass(0),
fArrayHistosMixUnlikeRapidity(0),
fArrayHistosMixUnlikePt(0),
fArrayHistosMixPPLikeInvMass(0),
fArrayHistosMixPPLikeRapidity(0),
fArrayHistosMixPPLikePt(0),
fArrayHistosMixMMLikeInvMass(0),
fArrayHistosMixMMLikeRapidity(0),
fArrayHistosMixMMLikePt(0),
fHistoTotalEvents(0),
fHistoCMUL7Events(0),
fHistoCINT7Events(0),
fHistoCMSL7Events(0),
fHistoEventCentrality(0),
fHistoMixingInfo(0),
fHistoMuonPt(0),
fHistoMuonEta(0),
fHistoMuonPhi(0),
fListMixUnlikeDimuon(0x0),
fListUnlikeDimuon(0x0),
fListPPLikeDimuon(0x0),
fListMixPPLikeDimuon(0x0),
fListMMLikeDimuon(0x0),
fListMixMMLikeDimuon(0x0),
fListEvent(0x0),
fListMuon(0x0),
fMuonTrackCuts(0),
fFirstRun(0),
fLastRun(1),
fArrayCentralityBins(0),
fArrayPtBins(0),
fArrayRapidityBins(0),
fNumberOfCentralityBins(1),
fNumberOfPtBins(1),
fNumberOfRapidityBins(1),
fMainInputHandler(0),
fMixingInputHandler(0),
fTriggerofEvent(0)
{
  // Dummy constructor ALWAYS needed for I/O.
}

//________________________________________________________________________
TaskJpsiPP::TaskJpsiPP(const char *name, const int triggerofevent, int firstRun, int lastRun,std::vector<Double_t> arrayCentralityBins, std::vector<Double_t> arrayRapidityBins, std::vector<Double_t> arrayPtBins)
: AliAnalysisTaskSE(name),
fArrayHistosUnlikeInvMass(0),
fArrayHistosUnlikeRapidity(0),
fArrayHistosUnlikePt(0),
fArrayHistosPPLikeInvMass(0),
fArrayHistosPPLikeRapidity(0),
fArrayHistosPPLikePt(0),
fArrayHistosMMLikeInvMass(0),
fArrayHistosMMLikeRapidity(0),
fArrayHistosMMLikePt(0),
fArrayHistosMixUnlikeInvMass(0),
fArrayHistosMixUnlikeRapidity(0),
fArrayHistosMixUnlikePt(0),
fArrayHistosMixPPLikeInvMass(0),
fArrayHistosMixPPLikeRapidity(0),
fArrayHistosMixPPLikePt(0),
fArrayHistosMixMMLikeInvMass(0),
fArrayHistosMixMMLikeRapidity(0),
fArrayHistosMixMMLikePt(0),
fHistoTotalEvents(0),
fHistoCMUL7Events(0),
fHistoCINT7Events(0),
fHistoCMSL7Events(0),
fHistoEventCentrality(0),
fHistoMixingInfo(0),
fHistoMuonPt(0),
fHistoMuonEta(0),
fHistoMuonPhi(0),
fListMixUnlikeDimuon(0x0),
fListUnlikeDimuon(0x0),
fListPPLikeDimuon(0x0),
fListMixPPLikeDimuon(0x0),
fListMMLikeDimuon(0x0),
fListMixMMLikeDimuon(0x0),
fListEvent(0x0),
fListMuon(0x0),
fMuonTrackCuts(0),
fFirstRun(firstRun),
fLastRun(lastRun),
fArrayCentralityBins(arrayCentralityBins),
fArrayPtBins(arrayPtBins),
fArrayRapidityBins(arrayRapidityBins),
fNumberOfCentralityBins(1),
fNumberOfPtBins(1),
fNumberOfRapidityBins(1),
fMainInputHandler(0),
fMixingInputHandler(0),
fTriggerofEvent(triggerofevent)
{

  fNumberOfCentralityBins =0;
  while (fArrayCentralityBins[fNumberOfCentralityBins+1] != -1) {
    fNumberOfCentralityBins++;
  }
  fNumberOfRapidityBins =0;
  while (fArrayRapidityBins[fNumberOfRapidityBins+1] != -1) {
    fNumberOfRapidityBins++;
  }
  fNumberOfPtBins =0;
  while (fArrayPtBins[fNumberOfPtBins+1] != -1) {
    fNumberOfPtBins++;
  }

  // Input slot #0 works with a TChain - it is connected to the default input container
  DefineInput(0, TChain::Class());
  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class());                                            // for output objarray
  DefineOutput(2, TList::Class());                                            // for output objarray
  DefineOutput(3, TList::Class());                                            // for output objarray
  DefineOutput(4, TList::Class());                                            // for output objarray
  DefineOutput(5, TList::Class());                                            // for output objarray
  DefineOutput(6, TList::Class());                                            // for output objarray
  DefineOutput(7, TList::Class());                                            // for output objarray
  DefineOutput(8, TList::Class());                                            // for output objarray

}

//________________________________________________________________________
TaskJpsiPP::~TaskJpsiPP()
{
  // Destructor. Clean-up the output list, but not the histograms that are put inside
  // (the list is owner and will clean-up these histograms). Protect in PROOF case.
  if (fListEvent && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListEvent;
  }
  if (fListMuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListMuon;
  }
  if (fListUnlikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListUnlikeDimuon;
  }
  if (fListPPLikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListPPLikeDimuon;
  }
  if (fListMMLikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListMMLikeDimuon;
  }
  if (fListMixUnlikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListMixUnlikeDimuon;
  }
  if (fListMixPPLikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListMixPPLikeDimuon;
  }
  if (fListMixMMLikeDimuon && !AliAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fListMixMMLikeDimuon;
  }

}

//________________________________________________________________________
void TaskJpsiPP::NotifyRun()
{
  /// Set run number for cuts
  if ( fMuonTrackCuts ) fMuonTrackCuts->SetRun(fInputHandler);
}

//________________________________________________________________________
void TaskJpsiPP::UserCreateOutputObjects()
{
  //Create the event, single muon, dimuon histgrams, to do that, it is only needed to add the name of the histogram to the corresponding list, it will be created and the bins will be set according to the name


  fListEvent = new TList();
  fListEvent->SetName("fListEvent");
  fListEvent->SetOwner(kTRUE);

  fListMuon = new TList();
  fListMuon->SetName("fListMixUnlikeDimuon");
  fListMuon->SetOwner(kTRUE);

  fListUnlikeDimuon = new TList();
  fListUnlikeDimuon->SetName("fListUnlikeDimuon");
  fListUnlikeDimuon->SetOwner(kTRUE);

  fListMixUnlikeDimuon = new TList();
  fListMixUnlikeDimuon->SetName("fListMixUnlikeDimuon");
  fListMixUnlikeDimuon->SetOwner(kTRUE);

  fListPPLikeDimuon = new TList();
  fListPPLikeDimuon->SetName("fListPPLikeDimuon");
  fListPPLikeDimuon->SetOwner(kTRUE);

  fListMixPPLikeDimuon = new TList();
  fListMixPPLikeDimuon->SetName("fListMixPPLikeDimuon");
  fListMixPPLikeDimuon->SetOwner(kTRUE);

  fListMMLikeDimuon = new TList();
  fListMMLikeDimuon->SetName("fListMMLikeDimuon");
  fListMMLikeDimuon->SetOwner(kTRUE);

  fListMixMMLikeDimuon = new TList();
  fListMixMMLikeDimuon->SetName("fListMixMMLikeDimuon");
  fListMixMMLikeDimuon->SetOwner(kTRUE);

  fHistoTotalEvents = new TH1I("fHistoTotalEvents","",(fLastRun - fFirstRun), fFirstRun, fLastRun );
  fHistoTotalEvents->Sumw2();
  fListEvent->Add(fHistoTotalEvents);

  fHistoCMUL7Events = new TH1I("fHistoCMUL7Events","",(fLastRun - fFirstRun), fFirstRun, fLastRun );
  fHistoCMUL7Events->Sumw2();
  fListEvent->Add(fHistoCMUL7Events);

  fHistoCINT7Events = new TH1I("fHistoCINT7Events","",(fLastRun - fFirstRun), fFirstRun, fLastRun );
  fHistoCINT7Events->Sumw2();
  fListEvent->Add(fHistoCINT7Events);

  fHistoCMSL7Events = new TH1I("fHistoCMSL7Events","",(fLastRun - fFirstRun), fFirstRun, fLastRun );
  fHistoCMSL7Events->Sumw2();
  fListEvent->Add(fHistoCMSL7Events);

  fHistoEventCentrality = new TH1F("fHistoEventCentrality","",100,0,100);
  fHistoEventCentrality->Sumw2();
  fListEvent->Add(fHistoEventCentrality);

  fHistoMixingInfo = new TH1I("fHistoMixingInfo","",9,0,9);
  fListEvent->Add(fHistoMixingInfo);

  //Muon histograms

  fHistoMuonPt = new TH1F("fHistoMuonPt","",200, 0, 20 );
  fHistoMuonPt->Sumw2();
  fListMuon->Add(fHistoMuonPt);

  fHistoMuonEta = new TH1F("fHistoMuonEta","",60, -2, -5 );
  fHistoMuonEta->Sumw2();
  fListMuon->Add(fHistoMuonEta);

  fHistoMuonPhi = new TH1F("fHistoMuonPhi","",360, 0, 360 );
  fHistoMuonPhi->Sumw2();
  fListMuon->Add(fHistoMuonPhi);

  //Dimuon Histograms
  const int numberOfCentralityBins = fNumberOfCentralityBins+1;
  const int numberOfRapidityBins = fNumberOfRapidityBins+1;
  const int numberOfPtBins = fNumberOfPtBins+1;

  fArrayHistosUnlikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosUnlikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosUnlikePt = new TH1F***[numberOfCentralityBins];

  fArrayHistosPPLikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosPPLikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosPPLikePt = new TH1F***[numberOfCentralityBins];

  fArrayHistosMMLikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosMMLikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosMMLikePt = new TH1F***[numberOfCentralityBins];

  fArrayHistosMixUnlikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixUnlikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixUnlikePt = new TH1F***[numberOfCentralityBins];

  fArrayHistosMixPPLikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixPPLikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixPPLikePt = new TH1F***[numberOfCentralityBins];

  fArrayHistosMixMMLikeInvMass = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixMMLikeRapidity = new TH1F***[numberOfCentralityBins];
  fArrayHistosMixMMLikePt = new TH1F***[numberOfCentralityBins];

  for(int iCentrality =0; iCentrality <= fNumberOfCentralityBins; iCentrality++){
    fArrayHistosUnlikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosUnlikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosUnlikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    fArrayHistosPPLikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosPPLikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosPPLikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    fArrayHistosMMLikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMMLikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMMLikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    fArrayHistosMixUnlikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixUnlikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixUnlikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    fArrayHistosMixPPLikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixPPLikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixPPLikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    fArrayHistosMixMMLikeInvMass[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixMMLikeRapidity[iCentrality] = new TH1F**[numberOfRapidityBins];
    fArrayHistosMixMMLikePt[iCentrality] = new TH1F**[numberOfRapidityBins];

    for(int iRapidity =0; iRapidity <= fNumberOfRapidityBins; iRapidity++){
      fArrayHistosUnlikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosUnlikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosUnlikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];

      fArrayHistosPPLikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosPPLikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosPPLikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];

      fArrayHistosMMLikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMMLikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMMLikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];

      fArrayHistosMixUnlikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixUnlikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixUnlikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];

      fArrayHistosMixPPLikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixPPLikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixPPLikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];

      fArrayHistosMixMMLikeInvMass[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixMMLikeRapidity[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      fArrayHistosMixMMLikePt[iCentrality][iRapidity] = new TH1F*[numberOfPtBins];
      for(int iPt =0; iPt <= fNumberOfPtBins; iPt++){
        TString histoNameSuffix = "";

        if(iCentrality == (fNumberOfCentralityBins) ){
          histoNameSuffix += "AllCent";
        }
        else{
          histoNameSuffix += "CentBin";
          histoNameSuffix += fArrayCentralityBins[iCentrality];
          histoNameSuffix += "to";
          histoNameSuffix += fArrayCentralityBins[iCentrality+1];
        }
        if(iRapidity == (fNumberOfRapidityBins)){
          histoNameSuffix += "AllRap";
        }
        else{
          histoNameSuffix += "RapBin";
          histoNameSuffix += fArrayRapidityBins[iRapidity];
          histoNameSuffix += "to";
          histoNameSuffix += fArrayRapidityBins[iRapidity+1];
        }
        if(iPt == (fNumberOfPtBins)){
          histoNameSuffix += "Allpt";
        }
        else{
          histoNameSuffix += "ptBin";
          histoNameSuffix += fArrayPtBins[iPt];
          histoNameSuffix += "to";
          histoNameSuffix += fArrayPtBins[iPt+1];
        }
        fArrayHistosUnlikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoUnlikeInvMass"+histoNameSuffix,"",1000,0,10);
        fArrayHistosUnlikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListUnlikeDimuon->Add(fArrayHistosUnlikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosUnlikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoUnlikeRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosUnlikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListUnlikeDimuon->Add(fArrayHistosUnlikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosUnlikePt[iCentrality][iRapidity][iPt] =new TH1F("histoUnlikePt"+histoNameSuffix,"",200,0,20);
        fArrayHistosUnlikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListUnlikeDimuon->Add(fArrayHistosUnlikePt[iCentrality][iRapidity][iPt]);

        fArrayHistosPPLikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoLikePPInvMass"+histoNameSuffix,"hh",1000,0,10);
        fArrayHistosPPLikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListPPLikeDimuon->Add(fArrayHistosPPLikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosPPLikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoLikePPRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosPPLikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListPPLikeDimuon->Add(fArrayHistosPPLikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosPPLikePt[iCentrality][iRapidity][iPt] =new TH1F("histoLikePPPt"+histoNameSuffix,"",200,0,20);
        fArrayHistosPPLikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListPPLikeDimuon->Add(fArrayHistosPPLikePt[iCentrality][iRapidity][iPt]);

        fArrayHistosMMLikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoLikeMMInvMass"+histoNameSuffix,"",1000,0,10);
        fArrayHistosMMLikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListMMLikeDimuon->Add(fArrayHistosMMLikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosMMLikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoLikeMMRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosMMLikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListMMLikeDimuon->Add(fArrayHistosMMLikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosMMLikePt[iCentrality][iRapidity][iPt] =new TH1F("histoLikeMMPt"+histoNameSuffix,"",200,0,20);
        fArrayHistosMMLikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListMMLikeDimuon->Add(fArrayHistosMMLikePt[iCentrality][iRapidity][iPt]);

        fArrayHistosMixUnlikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoMixUnlikeInvMass"+histoNameSuffix,"",1000,0,10);
        fArrayHistosMixUnlikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixUnlikeDimuon->Add(fArrayHistosMixUnlikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosMixUnlikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoMixUnlikeRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosMixUnlikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixUnlikeDimuon->Add(fArrayHistosMixUnlikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosMixUnlikePt[iCentrality][iRapidity][iPt] =new TH1F("histoMixUnlikePt"+histoNameSuffix,"",200,0,20);
        fArrayHistosMixUnlikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixUnlikeDimuon->Add(fArrayHistosMixUnlikePt[iCentrality][iRapidity][iPt]);

        fArrayHistosMixPPLikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikePPInvMass"+histoNameSuffix,"",1000,0,10);
        fArrayHistosMixPPLikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixPPLikeDimuon->Add(fArrayHistosMixPPLikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosMixPPLikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikePPRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosMixPPLikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixPPLikeDimuon->Add(fArrayHistosMixPPLikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosMixPPLikePt[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikePPPt"+histoNameSuffix,"",200,0,20);
        fArrayHistosMixPPLikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixPPLikeDimuon->Add(fArrayHistosMixPPLikePt[iCentrality][iRapidity][iPt]);

        fArrayHistosMixMMLikeInvMass[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikeMMInvMass"+histoNameSuffix,"",1000,0,10);
        fArrayHistosMixMMLikeInvMass[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixMMLikeDimuon->Add(fArrayHistosMixMMLikeInvMass[iCentrality][iRapidity][iPt]);

        fArrayHistosMixMMLikeRapidity[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikeMMRapidity"+histoNameSuffix,"",60,-5,-2);
        fArrayHistosMixMMLikeRapidity[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixMMLikeDimuon->Add(fArrayHistosMixMMLikeRapidity[iCentrality][iRapidity][iPt]);

        fArrayHistosMixMMLikePt[iCentrality][iRapidity][iPt] =new TH1F("histoMixLikeMMPt"+histoNameSuffix,"",200,0,20);
        fArrayHistosMixMMLikePt[iCentrality][iRapidity][iPt]->Sumw2();
        fListMixMMLikeDimuon->Add(fArrayHistosMixMMLikePt[iCentrality][iRapidity][iPt]);

      }
    }
  }



  fMuonTrackCuts = new AliMuonTrackCuts("StandardMuonTrackCuts","StandardMuonTrackCuts");
  fMuonTrackCuts->SetAllowDefaultParams(kTRUE);
  fMuonTrackCuts->SetFilterMask (AliMuonTrackCuts::kMuEta | AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca | AliMuonTrackCuts::kMuMatchLpt);

  // sets helper pointers for Mixing
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  SetMainInputHandler(mgr);
  if (fMainInputHandler) SetMixingInputHandler(fMainInputHandler);

  PostData(1, fListEvent);
  PostData(2, fListMuon);
  PostData(3, fListUnlikeDimuon);
  PostData(4, fListPPLikeDimuon);
  PostData(5, fListMMLikeDimuon);
  PostData(6, fListMixUnlikeDimuon);
  PostData(7, fListMixPPLikeDimuon);
  PostData(8, fListMixMMLikeDimuon);

}

//________________________________________________________________________
void TaskJpsiPP::UserExec(Option_t *)
{

  AliAODEvent *aod = 0;
  AliAODHeader *aodHeader = NULL;
  Bool_t isGoodEvent = kTRUE;
  TString strFiredTriggers;
  TString strRequiredTriggerClass;

  switch (fTriggerofEvent) {
    case AliVEvent::kMuonUnlikePB:
    strRequiredTriggerClass = "CMUL7-B-NOPF-MUFAST";
    break;

    case AliVEvent::kMUS7:
    strRequiredTriggerClass = "CMSL7-B-NOPF-MUFAST";
    break;

    case AliVEvent::kMuonLikePB:
    strRequiredTriggerClass = "CMLL7-B-NOPF-MUFAST";
    break;

    case AliVEvent::kINT7:
    strRequiredTriggerClass = "CINT7-B-NOPF-MUFAST";
    break;
  }

  //Check if the event is AOD or ESD
  if (AliAnalysisMuonUtility::IsAODEvent(InputEvent())){
    aod = static_cast<AliAODEvent *>(InputEvent());
    if(aod){
      strFiredTriggers = aod->GetFiredTriggerClasses();
      aodHeader = (AliAODHeader*)aod->GetHeader();
    }
  }
  if (!aod){
    AliError("ERROR: Could not retrieve AOD event !!");
    return;
  }


  int runNumber = aod->GetRunNumber();
  fHistoTotalEvents->Fill(runNumber);
  fHistoMixingInfo->Fill(1);

  UInt_t IsSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected());
  if (!(IsSelected & fTriggerofEvent)) {
    isGoodEvent = kFALSE;
  }

  if(!strFiredTriggers.Contains(strRequiredTriggerClass)){
    isGoodEvent = kFALSE;
  }
  else{
    //fill the histograms for the calculation of Fnorm

    UInt_t L0TriggerInputs = aodHeader->GetL0TriggerInputs();
    UInt_t L0InputMUU = TMath::Power(2,20);
    UInt_t L0InputV0A = TMath::Power(2,0);
    UInt_t L0InputV0C = TMath::Power(2,1);
    UInt_t L0InputMSL = TMath::Power(2,17);

    if( (L0TriggerInputs & L0InputV0A) && (L0TriggerInputs & L0InputV0A) ){//MB condition
      fHistoCINT7Events->Fill(runNumber);
    }

    if(L0TriggerInputs & L0InputMSL){
      fHistoCMSL7Events->Fill(runNumber);
    }

    if(L0TriggerInputs & L0InputMUU){
      fHistoCMUL7Events->Fill(runNumber);
    }

  }

  if ( isGoodEvent ) {
    fHistoMixingInfo->Fill(2);
    AliMultSelection *multSelection = (AliMultSelection * ) aod->FindListObject("MultSelection");
    Float_t lPercentile = multSelection->GetMultiplicityPercentile("V0M");
    Double_t centralityFromV0 = multSelection->GetMultiplicityPercentile("V0M", false);
    fHistoEventCentrality->Fill(centralityFromV0);

    //loop over dimuons

    for(Int_t i=0; i<aod->GetNumberOfDimuons(); i++) {
      AliAODDimuon *dimu = dynamic_cast<AliAODDimuon*>( aod->GetDimuon(i) );
      if ( !dimu ) {
        AliError(Form("ERROR: Could not retrieve AOD or ESD dimuon %d", i));
        continue;
      }

      if ( ! fMuonTrackCuts->IsSelected(dimu->GetMu(0)) ) continue;//include cuts on pDCA, Eta, Rabs.

      if ( ! fMuonTrackCuts->IsSelected(dimu->GetMu(1)) ) continue;//include cuts on pDCA, Eta, Rabs.

      if ( (dimu->Y() < -4) || (dimu->Y() > -2.5)) continue;

      if(centralityFromV0 >= 90) continue;

      //fill single muon histograms
      if((AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0)))){
        fHistoMuonEta->Fill((dimu->GetMu(0))->Eta());
        fHistoMuonPt->Fill((dimu->GetMu(0))->Pt());
        fHistoMuonPhi->Fill((dimu->GetMu(0))->Phi());
      }
      if((AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1)))){
        fHistoMuonEta->Fill((dimu->GetMu(1))->Eta());
        fHistoMuonPt->Fill((dimu->GetMu(1))->Pt());
        fHistoMuonPhi->Fill((dimu->GetMu(1))->Phi());
      }

      if( dimu->Charge() == 0 )
      {
        if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
        FillHisto(fArrayHistosUnlikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
        FillHisto(fArrayHistosUnlikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
        FillHisto(fArrayHistosUnlikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
        }
      }

      if( dimu->Charge() >0 )
      {
        if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
          FillHisto(fArrayHistosPPLikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
          FillHisto(fArrayHistosPPLikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
          FillHisto(fArrayHistosPPLikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
        }
      }

      if( dimu->Charge() <0 )
      {
        if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
          if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
            FillHisto(fArrayHistosMMLikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
            FillHisto(fArrayHistosMMLikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
            FillHisto(fArrayHistosMMLikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
          }
        }
      }



    }//end of the loop over dimuons

    // end condition on event
  }

  PostData(1, fListEvent);
  PostData(2, fListMuon);
  PostData(3, fListUnlikeDimuon);
  PostData(4, fListPPLikeDimuon);
  PostData(5, fListMMLikeDimuon);
  PostData(6, fListMixUnlikeDimuon);
  PostData(7, fListMixPPLikeDimuon);
  PostData(8, fListMixMMLikeDimuon);

}

//________________________________________________________________________
void TaskJpsiPP::UserExecMix(Option_t *)
{

  TString strFiredTriggers;
  TString strRequiredTriggerClass;
  Bool_t isGoodMainEvent = kTRUE;

  if (!fMixingInputHandler){
    return;
  }
  Int_t bufferSize = fMixingInputHandler->BufferSize();
  Int_t numberMixed = fMixingInputHandler->NumberMixed();
  AliAODEvent *aodMainEvent = dynamic_cast<AliAODEvent *>(GetMainEvent());

  if (aodMainEvent) {
    UInt_t IsSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected());
    if (!(IsSelected & AliVEvent::kMUS7)) {
      isGoodMainEvent = kFALSE;
    }

    strFiredTriggers = aodMainEvent->GetFiredTriggerClasses();
    if(!strFiredTriggers.Contains("CMSL7-B-NOPF-MUFAST")){
      isGoodMainEvent = kFALSE;
    }
    fHistoMixingInfo->Fill(3);
    if ( isGoodMainEvent ) {
      
      fHistoMixingInfo->Fill(4);
      AliMultSelection *multSelection = (AliMultSelection * ) aodMainEvent->FindListObject("MultSelection");
      Double_t centralityFromV0 = multSelection->GetMultiplicityPercentile("V0M", false);
      if(centralityFromV0 >= 90) return;
      
      for(Int_t iBuffer=0; iBuffer<bufferSize; iBuffer++) {

        AliAODEvent *aodMixEvent = dynamic_cast<AliAODEvent *>(GetMixedEvent(iBuffer));
        if (!aodMixEvent ){
          AliError("ERROR: Could not retrieve AOD event !!");
          return;
        }

        //loop over dimuons
        fHistoMixingInfo->Fill(5);
        for(Int_t i=0; i<aodMixEvent->GetNumberOfDimuons(); i++) {
          
          AliAODDimuon *dimu = dynamic_cast<AliAODDimuon*>( aodMixEvent->GetDimuon(i) );
          if ( !dimu ) {
            AliError(Form("ERROR: Could not retrieve AOD  dimuon %d", i));
            continue;
          }

          if ( ! fMuonTrackCuts->IsSelected(dimu->GetMu(0)) ) continue;//include cuts on pDCA, Eta, Rabs.

          if ( ! fMuonTrackCuts->IsSelected(dimu->GetMu(1)) ) continue;//include cuts on pDCA, Eta, Rabs.

          if ( (dimu->Y() < -4) || (dimu->Y() > -2.5)) continue;


          if( dimu->Charge() ==0 )
          {
            if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
              FillHisto(fArrayHistosMixUnlikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixUnlikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixUnlikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
            }
          }
          if( dimu->Charge() >0 )
          {
            if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
              FillHisto(fArrayHistosMixPPLikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixPPLikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixPPLikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
            }
          }

          if( dimu->Charge() <0 )
          {
            if( (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(0))) && (AliAnalysisMuonUtility::MatchLpt(dimu->GetMu(1))) ){
              FillHisto(fArrayHistosMixMMLikeInvMass,dimu->M(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixMMLikePt,dimu->Pt(),dimu->Pt(),dimu->Y(),centralityFromV0);
              FillHisto(fArrayHistosMixMMLikeRapidity,dimu->Y(),dimu->Pt(),dimu->Y(),centralityFromV0);
            }
          }


        }//end of the loop over dimuons

        // end condition on event
        //
      }

    }
  }
  PostData(1, fListEvent);
  PostData(2, fListMuon);
  PostData(3, fListUnlikeDimuon);
  PostData(4, fListPPLikeDimuon);
  PostData(5, fListMMLikeDimuon);
  PostData(6, fListMixUnlikeDimuon);
  PostData(7, fListMixPPLikeDimuon);
  PostData(8, fListMixMMLikeDimuon);
}
//________________________________________________________________________

void TaskJpsiPP::Terminate(Option_t *)
{
  fListEvent = dynamic_cast<TList *>(GetOutputData(1));
  if (!fListEvent) { AliError("Could not retrieve TList* fListEvent"); return; }

  fListMuon = dynamic_cast<TList *>(GetOutputData(2));
  if (!fListMuon) { AliError("Could not retrieve TList* fListMuon"); return; }

  fListUnlikeDimuon = dynamic_cast<TList *>(GetOutputData(3));
  if (!fListUnlikeDimuon) { AliError("Could not retrieve TList* fListUnlikeDimuon"); return; }

  fListPPLikeDimuon = dynamic_cast<TList *>(GetOutputData(4));
  if (!fListPPLikeDimuon) { AliError("Could not retrieve TList* fListPPLikeDimuon"); return; }

  fListMMLikeDimuon = dynamic_cast<TList *>(GetOutputData(5));
  if (!fListMMLikeDimuon) { AliError("Could not retrieve TList* fListMMLikeDimuon"); return; }

  fListMixUnlikeDimuon = dynamic_cast<TList *>(GetOutputData(6));
  if (!fListMixUnlikeDimuon) { AliError("Could not retrieve TList* fListMixUnlikeDimuon"); return; }

  fListMixPPLikeDimuon = dynamic_cast<TList *>(GetOutputData(7));
  if (!fListMixPPLikeDimuon) { AliError("Could not retrieve TList* fListMixPPLikeDimuon"); return; }

  fListMixMMLikeDimuon = dynamic_cast<TList *>(GetOutputData(8));
  if (!fListMixMMLikeDimuon) { AliError("Could not retrieve TList* fListMixMMLikeDimuon"); return; }


}

Int_t TaskJpsiPP::CentralityToBin(Double_t centrality){
  for(int iCentrality =0; iCentrality < fNumberOfCentralityBins; iCentrality++){
    if( (centrality < fArrayCentralityBins[iCentrality+1]) && (centrality >= fArrayCentralityBins[iCentrality]) )
    return iCentrality;
  }
  return -1;
}

Int_t TaskJpsiPP::RapidityToBin(Double_t rapidity){
  for(int iRapidity =0; iRapidity < fNumberOfRapidityBins; iRapidity++){
    if( (rapidity < fArrayRapidityBins[iRapidity+1]) && (rapidity >= fArrayRapidityBins[iRapidity]) )
    return iRapidity;
  }
  return -1;
}

Int_t TaskJpsiPP::PtToBin(Double_t pT){
  for(int iPt =0; iPt < fNumberOfPtBins; iPt++){
    if( (pT < fArrayPtBins[iPt+1]) && (pT >= fArrayPtBins[iPt]) )
    return iPt;
  }
  return -1;
}

AliVEvent *TaskJpsiPP::GetMainEvent()
{
   //
   // Access to MainEvent
   //

   AliMultiInputEventHandler *inEvHMainMulti = fMainInputHandler;
   if (inEvHMainMulti) {
      AliInputEventHandler *inEvMain = dynamic_cast<AliInputEventHandler *>(inEvHMainMulti->GetFirstInputEventHandler());
      if (inEvMain) {
        return inEvMain->GetEvent();
      }
   }
   return 0;
}

//________________________________________________________________________
AliVEvent *TaskJpsiPP::GetMixedEvent(Int_t buffId)
{
   //
   // Access to Mixed event with buffer id
   //

   AliMultiInputEventHandler *inEvHMain = fMainInputHandler;
   if (inEvHMain) {

      AliMixInputEventHandler2 *mixIH = fMixingInputHandler;
      if (!mixIH) return 0;
      if (mixIH->CurrentBinIndex() < 0) {
         AliDebug(AliLog::kDebug + 1, "Current event mixEH->CurrentEntry() == -1");
         return 0;
      }
//       AliMultiInputEventHandler *inEvHMixedCurrent = mixEH->GetFirstMultiInputHandler();
      AliMultiInputEventHandler *inEvHMixedCurrent = dynamic_cast<AliMultiInputEventHandler *>(mixIH->InputEventHandler(buffId));
      if (!inEvHMixedCurrent) return 0;
      AliInputEventHandler *ihMixedCurrent = inEvHMixedCurrent->GetFirstInputEventHandler();
      if (ihMixedCurrent) return ihMixedCurrent->GetEvent();
   }

   return 0;
}

//________________________________________________________________________
AliMultiInputEventHandler *TaskJpsiPP::SetMainInputHandler(AliAnalysisManager *mgr)
{
   //
   // Sets main input handler
   //

   if (!fMainInputHandler) fMainInputHandler = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());

   return fMainInputHandler;
}

//________________________________________________________________________
AliMixInputEventHandler2 *TaskJpsiPP::SetMixingInputHandler(AliMultiInputEventHandler *mainIH)
{
   //
   // Sets mixing input handler
   //

   if (!fMixingInputHandler) fMixingInputHandler = dynamic_cast<AliMixInputEventHandler2 *>(mainIH->GetFirstMultiInputHandler());

   return fMixingInputHandler;
}





void TaskJpsiPP::FillHisto(TH1F ****arrayOfHistos, Double_t valueToFill, Double_t pT, Double_t rapidity, Double_t centrality){

    int pTBin = PtToBin(pT);
    int centralityBin = CentralityToBin(centrality);
    int rapidityBin = RapidityToBin(rapidity);

    if( (centralityBin != -1) && (rapidityBin != -1) && (pTBin != -1)){
      arrayOfHistos[centralityBin][rapidityBin][pTBin]->Fill(valueToFill);
    }

    if( (centralityBin != -1) && (rapidityBin != -1) ){
      arrayOfHistos[centralityBin][rapidityBin][fNumberOfPtBins]->Fill(valueToFill);
    }

    if( (centralityBin != -1) && (pTBin != -1)){
      arrayOfHistos[centralityBin][fNumberOfRapidityBins][pTBin]->Fill(valueToFill);
    }

    if( (rapidityBin != -1) && (pTBin != -1)){
      arrayOfHistos[fNumberOfCentralityBins][rapidityBin][pTBin]->Fill(valueToFill);
    }

    if( (centralityBin != -1) ){
      arrayOfHistos[centralityBin][fNumberOfRapidityBins][fNumberOfPtBins]->Fill(valueToFill);
    }

    if( (rapidityBin != -1) ){
      arrayOfHistos[fNumberOfCentralityBins][rapidityBin][fNumberOfPtBins]->Fill(valueToFill);
    }

    if( (pTBin != -1) ){
      arrayOfHistos[fNumberOfCentralityBins][fNumberOfRapidityBins][pTBin]->Fill(valueToFill);
    }


      arrayOfHistos[fNumberOfCentralityBins][fNumberOfRapidityBins][fNumberOfPtBins]->Fill(valueToFill);
}
