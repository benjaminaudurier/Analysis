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
* provided"as is" without express or implied warranty.                  *
**************************************************************************/

// $Id$
#include "AliAODInvMassEvaluationTask.h"

#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODHandler.h"
#include "AliAODInputHandler.h"
#include "AliInputEventHandler.h"

#include "AliMultSelection.h"

#include <TChain.h>
#include <TPRegexp.h>

#include <fstream>
#include <iostream>
#include <cassert>

///\cond CLASSIMP
ClassImp(AliAODInvMassEvaluationTask)
///\endcond

// centrality definition
static const Int_t nCentralityBins = 10;
static const Double_t centralityMin[] = { 0, 10, 20, 30, 40, 50, 60, 70, 80, 0 };
static const Double_t centralityMax[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 90 };
static const TString centralityBinNames[] = { "010", "1020", "2030", "3040", "4050", "5060", "6070", "7080", "8090", "090" };

namespace Util
{
  //________________________________________________________________________
  template <typename T> T Square( T x ) { return x*x; }
}

//________________________________________________________________________
AliAODInvMassEvaluationTask::AliAODInvMassEvaluationTask(const char *name) :
  AliAnalysisTaskSE(name),
  fEvent(0),
  fCuts( "MuonTrackSelectionCuts", "MuonTrackSelectionCuts" ),
  fMaxPoolSize( 10 ),
  fPhysicsSelectionMask(0),
  fPhysicsSelectionMaskMixing(0),
  fTrackSelectionMask(
    AliMuonTrackCuts::kMuEta|
    AliMuonTrackCuts::kMuThetaAbs|
    AliMuonTrackCuts::kMuPdca|
    AliMuonTrackCuts::kMuMatchLpt|
    AliMuonTrackCuts::kMuTrackChiSquare ),
  fListOutput(0x0),
  fPools(0x0)
{

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());

}

//________________________________________________________________________
AliAODInvMassEvaluationTask::~AliAODInvMassEvaluationTask()
{

  // delete pools
  delete fPools;

  // only delete output in non proof mode
  if( !AliAnalysisManager::GetAnalysisManager()->IsProofMode() )
  { delete fListOutput; }

}

//________________________________________________________________________
void AliAODInvMassEvaluationTask::LocalInit()
{}

//________________________________________________________________________
void AliAODInvMassEvaluationTask::NotifyRun()
{
  std::cout << "______________________________________________________________________________" << std::endl;

  AliInfo( Form( "run number: %i", fCurrentRunNumber ) );
  AliAODInputHandler* inputHandler = dynamic_cast<AliAODInputHandler*> (AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  fCuts.SetAllowDefaultParams( kTRUE );
  fCuts.SetRun( inputHandler );
  fCuts.Print();

  AliInfo( Form( "fTriggerSelection: %s", fTriggerSelection.Data() ) );
  AliInfo( Form( "fPhysicsSelectionMask: %i", fPhysicsSelectionMask ) );
  AliInfo( "" );
  AliInfo( Form( "fTriggerSelectionMixing: %s", fTriggerSelectionMixing.Data() ) );
  AliInfo( Form( "fPhysicsSelectionMaskMixing: %i", fPhysicsSelectionMaskMixing ) );
  AliInfo( Form( "fMaxPoolSize: %i", fMaxPoolSize ) );
  AliInfo( "" );
  AliInfo( Form( "fTrackSelectionMask: %i", fTrackSelectionMask ) );

  std::cout << "______________________________________________________________________________" << std::endl;
}

//________________________________________________________________________
void AliAODInvMassEvaluationTask::UserCreateOutputObjects()
{

  // pools
  fPools = new TList();
  fPools->SetOwner();

  // output list
  fListOutput = new TList();
  fListOutput->SetOwner( kTRUE );

  // centrality
  fListOutput->Add( new TH1F( "Centrality", "Centrality", 100, 0, 100 ) );

  // mass binning
  const Double_t massMin = 0;
  const Double_t massMax = 15;
  const Int_t nBinsGeV = 50; // number of bins per GeV
  const Int_t nMassBins = nBinsGeV*(massMax-massMin);

  // pt binning
  const Double_t ptMin = 0;
  const Double_t ptMax = 30;
  const Int_t nPtBins = 10*(ptMax-ptMin);

  // y binning
  const Double_t yMin = 2.5;
  const Double_t yMax = 4.0;
  const Int_t nYBins = 120;

  // pair sign definition
  const Int_t nSign = 3;
  TString signName[] = { "PM", "PP", "MM" };

  TString hName;
  for( Int_t iCentrality =0; iCentrality < nCentralityBins; iCentrality++ )
  for( Int_t iSign =0; iSign < nSign; iSign++ )
  {

    // integrated invariant mass
    hName = Form( "InvMass_%s_%s", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH1F( hName, hName, nMassBins, massMin, massMax ) );

    // invariant mass vs pt
    hName = Form( "InvMassPt_%s_%s", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH2F( hName, hName, nMassBins, massMin, massMax, nPtBins, ptMin, ptMax ) );

    // invariant mass vs y
    hName = Form( "InvMassY_%s_%s", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH2F( hName, hName, nMassBins, massMin, massMax, nYBins, yMin, yMax ) );

    // integrated invariant mass
    hName = Form( "InvMass_%s_%s_Mix", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH1F( hName, hName, nMassBins, massMin, massMax ) );

    // invariant mass vs pt
    hName = Form( "InvMassPt_%s_%s_Mix", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH2F( hName, hName, nMassBins, massMin, massMax, nPtBins, ptMin, ptMax ) );

    // invariant mass vs y
    hName = Form( "InvMassY_%s_%s_Mix", signName[iSign].Data(), centralityBinNames[iCentrality].Data() );
    fListOutput->Add( new TH2F( hName, hName, nMassBins, massMin, massMax, nYBins, yMin, yMax ) );

  }

  PostData(1, fListOutput);

}

//________________________________________________________________________
void AliAODInvMassEvaluationTask::UserExec(Option_t *)
{

  // increment event
  if( !(fEvent%1000 ) ) AliInfo( Form( "event: %i", fEvent ) );
  ++fEvent;

  // get the aod event
  AliAODEvent* event = dynamic_cast<AliAODEvent*>(InputEvent());
  if (!event)
  {
    AliInfo("ERROR: event not available");
    return;
  }

  // check if the event match trigger selection
  const Bool_t isEventAccepted( IsEventAccepted( event->GetFiredTriggerClasses() ) );
  const Bool_t isEventAcceptedMixing( IsEventAcceptedMixing( event->GetFiredTriggerClasses() ) );
  if( !(isEventAccepted || isEventAcceptedMixing ) ) return;

  // check if event passes physics selection
  const UInt_t physicsSelectionMask( static_cast<AliInputEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler())->IsEventSelected() );
  const Bool_t isPhysicsSelected( IsPhysicsSelected( physicsSelectionMask ) );
  const Bool_t isPhysicsSelectedMixing( IsPhysicsSelectedMixing( physicsSelectionMask ) );
  if( !(isPhysicsSelected || isPhysicsSelectedMixing ) ) return;

  // read centrality
  AliMultSelection* multSelection = dynamic_cast<AliMultSelection*>( event->FindListObject("MultSelection") );
  if( !multSelection )
  {
    AliInfo( "Invalid centrality" );
    return;
  }

  const Double_t centrality = multSelection->GetMultiplicityPercentile("V0M");
  ( (TH1*) fListOutput->FindObject( "Centrality" ) )->Fill(centrality);

  // std::cout << "centrality: " << centrality << endl;

  // find matching pool
  TList* currentPool = FindPool( centrality );

  // loop over single muons
  const Int_t nTracks = event->GetNumberOfTracks();
  for( Int_t iTrack1 = 0; iTrack1 < nTracks; ++iTrack1 )
  {

    // get first track
    AliAODTrack* muonTrack0 =  dynamic_cast<AliAODTrack*>(event->GetTrack(iTrack1));
    if( !IsTrackAccepted( muonTrack0 ) ) continue;

    // store momentum
    const TLorentzVector trackMomentum0(
      muonTrack0->Px(),
      muonTrack0->Py(),
      muonTrack0->Pz(),
      muonTrack0->E());

    // make same event pairs, if event passes 'same event' selection
    if( isEventAccepted && isPhysicsSelected )
    {

      // loop over second track (same event)
      for( Int_t iTrack2 = iTrack1+1; iTrack2 < nTracks; ++iTrack2 )
      {

        AliAODTrack* muonTrack1 =  dynamic_cast<AliAODTrack*>(event->GetTrack(iTrack2));
        if( !IsTrackAccepted( muonTrack1 ) ) continue;

        // store momentum
        const TLorentzVector trackMomentum1(
          muonTrack1->Px(),
          muonTrack1->Py(),
          muonTrack1->Pz(),
          muonTrack1->E());

        // calculate pair momentum
        const TLorentzVector pairMomentum( trackMomentum0 + trackMomentum1 );
        if( !IsPairAccepted( pairMomentum ) ) continue;

        // pair sign
        TString signName;
        if( muonTrack0->Charge() > 0 && muonTrack1->Charge() > 0 ) signName = "PP";
        else if( muonTrack0->Charge() < 0 && muonTrack1->Charge() < 0 ) signName = "MM";
        else signName = "PM";

        const Double_t mass = pairMomentum.M();
        const Double_t pt = pairMomentum.Pt();
        const Double_t y = pairMomentum.Rapidity();

        // loop over centrality bins
        for( Int_t iCentrality = 0; iCentrality < nCentralityBins; ++iCentrality )
        {

          // check
          if(!( centrality >= centralityMin[iCentrality] && centrality < centralityMax[iCentrality] ) ) continue;

          // fill
          ( (TH1*) fListOutput->FindObject( Form( "InvMass_%s_%s", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass);
          ( (TH2*) fListOutput->FindObject( Form( "InvMassPt_%s_%s", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass, pt);
          ( (TH2*) fListOutput->FindObject( Form( "InvMassY_%s_%s", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass, -y);

        }

      }

    }

    // make same event pairs, if event passes 'same event' selection
    if( currentPool && isEventAcceptedMixing && isPhysicsSelectedMixing )
    {

      for( Int_t iTrack2 = 0; iTrack2 < currentPool->GetSize(); ++iTrack2 )
      {

        // no need to check against cuts, this was already done while filling pools
        AliAODTrack* muonTrack1 =  dynamic_cast<AliAODTrack*>(currentPool->At(iTrack2));

        // store momentum
        const TLorentzVector trackMomentum1(
          muonTrack1->Px(),
          muonTrack1->Py(),
          muonTrack1->Pz(),
          muonTrack1->E() );

        // calculate pair momentum
        const TLorentzVector pairMomentum( trackMomentum0 + trackMomentum1 );
        if( !IsPairAccepted( pairMomentum ) ) continue;

        // pair sign
        TString signName;
        if( muonTrack0->Charge() > 0 && muonTrack1->Charge() > 0 ) signName = "PP";
        else if( muonTrack0->Charge() < 0 && muonTrack1->Charge() < 0 ) signName = "MM";
        else signName = "PM";

        const Double_t mass = pairMomentum.M();
        const Double_t pt = pairMomentum.Pt();
        const Double_t y = pairMomentum.Rapidity();

        // loop over centrality bins
        for( Int_t iCentrality = 0; iCentrality < nCentralityBins; ++iCentrality )
        {

          // check
          if(!( centrality >= centralityMin[iCentrality] && centrality < centralityMax[iCentrality] ) ) continue;

          // fill
          ( (TH1*) fListOutput->FindObject( Form( "InvMass_%s_%s_Mix", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass);
          ( (TH2*) fListOutput->FindObject( Form( "InvMassPt_%s_%s_Mix", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass, pt);
          ( (TH2*) fListOutput->FindObject( Form( "InvMassY_%s_%s_Mix", signName.Data(), centralityBinNames[iCentrality].Data() ) ) )->Fill(mass, -y);

        }

      }

    }

  }

  if( currentPool && isEventAcceptedMixing && isPhysicsSelectedMixing )
  {

    // loop (again) over muons to add to current pool
    for( Int_t i = 0; i < nTracks; ++i )
    {
      AliAODTrack* track =  dynamic_cast<AliAODTrack*>(event->GetTrack(i));
      if( IsTrackAccepted( track ) ) currentPool->AddFirst( track->Clone() );
    }

    // truncate pool if necessary
    while( currentPool->GetSize() > fMaxPoolSize )
    {
      delete currentPool->Last();
      currentPool->RemoveLast();
    }

  }

  PostData(1, fListOutput);

}

//________________________________________________________________________
void AliAODInvMassEvaluationTask::Terminate(const Option_t*)
{}

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsEventAccepted( TString triggerClasses ) const
{ return fTriggerSelection.IsNull() || TPRegexp( fTriggerSelection ).MatchB( triggerClasses ); }

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsEventAcceptedMixing( TString triggerClasses ) const
{ return fTriggerSelectionMixing.IsNull() || TPRegexp( fTriggerSelectionMixing ).MatchB( triggerClasses ); }

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsPhysicsSelected( UInt_t physicsSelectionMask ) const
{ return (fPhysicsSelectionMask == 0 ) || (physicsSelectionMask & fPhysicsSelectionMask); }

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsPhysicsSelectedMixing( UInt_t physicsSelectionMask ) const
{ return (fPhysicsSelectionMaskMixing == 0 ) || (physicsSelectionMask & fPhysicsSelectionMaskMixing); }

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsTrackAccepted( const AliAODTrack* track )
{

  // check track
  if( !( track && track->IsMuonTrack() ) )
  { return kFALSE; }

  const UInt_t selectionMask = fCuts.GetSelectionMask( track );
  return (selectionMask&fTrackSelectionMask) == fTrackSelectionMask;

}

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsPairAccepted( const AliAODDimuon* dimuon )
{

  for( Int_t i=0; i<2; ++i )
  { if( !IsTrackAccepted( dimuon->GetMu(i) ) ) return kFALSE; }

  // select proper rapidity range
  const Double_t y = -dimuon->Y();
  const Double_t yMin = 2.5;
  const Double_t yMax = 4.0;
  if( y < yMin || y > yMax ) return kFALSE;

  // all accepted
  return kTRUE;

}

//________________________________________________________________________
Bool_t AliAODInvMassEvaluationTask::IsPairAccepted( const TLorentzVector& pairMomentum ) const
{
  const Double_t y = -pairMomentum.Rapidity();
  const Double_t yMin = 2.5;
  const Double_t yMax = 4.0;
  return y >= yMin && y <= yMax;
}

//________________________________________________________________________
TList*  AliAODInvMassEvaluationTask::FindPool( Double_t centrality ) const
{

//   const Int_t nCentralityPools = 1;
//   const Double_t poolCentrality[] = {0, 90 };

  // define number of pools and boundary
  // in principle one could also use vertex range
  const Int_t nCentralityPools = 9;
  const Double_t poolCentrality[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90 };

  // create pools if not already done
  if( fPools->IsEmpty() )
  {

    AliInfo( "Creating pools" );
    for( Int_t iPool = 0; iPool < nCentralityPools; ++iPool )
    {
      TList* list = new TList();
      list->SetOwner();
      fPools->Add( list );
    }

  }

  // loop over pools and find matching
  for( Int_t iPool = 0; iPool < nCentralityPools; ++iPool )
  {
    if( centrality >= poolCentrality[iPool] && centrality < poolCentrality[iPool+1] )
    { return dynamic_cast<TList*>(fPools->At( iPool )); }
  }

  return 0x0;

}
