#ifndef __CINT__
#include <TString.h>
#include <ANALYSIS/AliAnalysisManager.h>
#include <ANALYSIS/AliAnalysisDataContainer.h>
#include "AliAnalysisTaskEvil.h"
#endif
AliAnalysisTask *AddEventMixingTestTask(TString format = "esd", Bool_t useMC = kFALSE,TString postfix="")
{
  // create manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) mgr = new AliAnalysisManager("MIX");
  
  // Create AliMuonTrackCuts
  AliMuonTrackCuts* trackCuts = new AliMuonTrackCuts("stdMuonCuts","stdMuonCuts");
  trackCuts->SetFilterMask(AliMuonTrackCuts::kMuEta | AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca | AliMuonTrackCuts::kMuMatchLpt);
  
  // create our task
  AliAnalysisTaskEx02 *task = new AliAnalysisTaskEx02("AliAnalysisTaskEx02");
  task->SelectCollisionCandidates(AliVEvent::kMuonUnlikePB|AliVEvent::kMuonLikePB);
  task->SetMuonTrackCuts(trackCuts);
  
  // Set pt and y bins
  const Int_t nPtBins = 19;
  Float_t dPtLowEdge[nPtBins] = {0., 0., 2., 5., 0., 1., 2., 3., 4., 5., 6., 8., 2., 0., 3., 0.3, 0.3, 0.3, 0.3};
  Float_t dPtUpEdge[nPtBins] = {8., 2., 5., 8., 1., 2., 3., 4., 5., 6., 8., 20., 8., 3., 8., 1., 8., 2., 3.};
  task->SetPtLowEdge(nPtBins, dPtLowEdge);
  task->SetPtUpEdge(nPtBins, dPtUpEdge);
  
  const Int_t nYBins = 10;
  Float_t dYLowEdge[nYBins] = { -2.5, -2.5, -3., -3.5, -2.5, -2.75, -3., -3.25, -3.5, -3.75};
  Float_t dYUpEdge[nYBins] = { -4., -3., -3.5, -4., -2.75, -3., -3.25, -3.5, -3.75, -4.};
  task->SetYLowEdge(nYBins, dYLowEdge);
  task->SetYUpEdge(nYBins, dYUpEdge);
  
  // create output container
  AliAnalysisDataContainer *output1 = mgr->CreateContainer("cOut", TList::Class(), AliAnalysisManager::kOutputContainer, "Output.root");
  
  // add our task to the manager
  mgr->AddTask(task);
  
  // finaly connect input and output
  mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
  mgr->ConnectOutput(task, 1, output1);
  
  return task;
}

