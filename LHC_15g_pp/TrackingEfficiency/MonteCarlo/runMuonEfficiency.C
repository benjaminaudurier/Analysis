/*
 *  runMuonEfficiency.C
 *  aliroot
 *
 *  Created by Philippe Pillot on 16/12/10.
 *  Copyright 2010 SUBATECH. All rights reserved.
 *
 */

TString rootVersion = "v5-34-30-1";
TString alirootVersion = "v5-06-33";
TString aliphysicsVersion = "vAN-20150722";
TString dataDir = "/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/simsinglemuon/pp/new/CMSL7-B-NOPF-MUON";
TString dataPattern = "*AliESDs.root";
TString runFormat = "%06d";
TString outDir = "Analysis/LHC15g/TrackingEfficiency/MonteCarlo/singleMuon/new";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 150;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 4;

// --- prepare environment ---
  TString extraLibs="";
 TString extraIncs="include";
 TString extraTasks="AliAnalysisTaskMuonTrackingEffLocal";

TString alignStorage = "alien://folder=/alice/data/2015/OCDB";
// TString alignStorage = "alien://folder=/alice/simulation/2008/v4-15-Release/Residual";

//______________________________________________________________________________
void runMuonEfficiency(TString smode = "full", TString inputFileName = "runlist_15-07-2015-ESD.txt",
		       Bool_t applyPhysSel = kFALSE, Bool_t mc = kTRUE, Bool_t embedding = kFALSE)
{
  /// Study the MUON performances

  gROOT->LoadMacro("/Users/audurier/Documents/Analysis/Macro_Utile/runTaskFacilities.C");

  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) {
    Error("runMuonEfficiency","Please provide either an ESD root file a collection of ESDs or a dataset.");
    return;
  }

  // --- copy files needed for this analysis ---
  TList pathList; pathList.SetOwner();
  pathList.Add(new TObjString("/Users/audurier/Documents/Analysis/Tasks"));

  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString("AliAnalysisTaskMuonTrackingEffLocal.cxx"));
  fileList.Add(new TObjString("AliAnalysisTaskMuonTrackingEffLocal.h"));

  CopyFileLocally(pathList, fileList);

  LoadAlirootLocally(extraLibs, extraIncs, extraTasks);

  AliAnalysisGrid *alienHandler = 0x0;

  if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, alirootVersion, extraLibs, extraIncs, extraTasks, kTRUE);
  else if (mode == kGrid || mode == kTerminate) {
    TString analysisMacroName = "Eff";
    alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, rootVersion, alirootVersion, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
    if (!alienHandler) return;
  }

  // --- Create the analysis train ---
  CreateAnalysisTrain(applyPhysSel, mc, embedding, alienHandler);

  // --- Create input object ---
  TObject* inputObj = CreateInputObject(mode, inputFileName);

  // --- start analysis ---
  StartAnalysis(mode, inputObj);

}

//______________________________________________________________________________
void CreateAnalysisTrain(Bool_t applyPhysSel, Bool_t mc, Bool_t embedding, TObject* alienHandler)
{
  /// create the analysis train and configure it

  // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("MuonEfficiencyAnalysis");

  // Connect plugin to the analysis manager if any
  if (alienHandler) mgr->SetGridHandler(static_cast<AliAnalysisGrid*>(alienHandler));

  // ESD input
  AliESDInputHandler* esdH = new AliESDInputHandler();
  esdH->SetReadFriends(kFALSE);
  esdH->SetInactiveBranches("*");
  esdH->SetActiveBranches("MuonTracks MuonClusters MuonPads AliESDRun. AliESDHeader. AliMultiplicity. AliESDFMD. AliESDVZERO. SPDVertex. PrimaryVertex. AliESDZDC. AliESDTZERO.");
  mgr->SetInputEventHandler(esdH);

  // event selectionMuon
  UInt_t offlineTriggerMask;
  if (applyPhysSel) {
    gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask* physicsSelection = AddTaskPhysicsSelection(mc && !embedding);
    if(!physicsSelection) {
      Error("CreateAnalysisTrain","AliPhysicsSelectionTask not created!");
      return;
    }
    //offlineTriggerMask = AliVEvent::kAny;
    offlineTriggerMask = AliVEvent::kMUS7;
    // offlineTriggerMask = AliVEvent::kMUU7 | AliVEvent::kMuonUnlikeLowPt8;
  }
  /*
  // centrality selection
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskCentrality.C");
  AliCentralitySelectionTask *taskCentrality = AddTaskCentrality();
  if(!taskCentrality) {
    Error("CreateAnalysisTrain","AliCentralitySelectionTask not created!");
    return;
  }
  if (applyPhysSel) taskCentrality->SelectCollisionCandidates(offlineTriggerMask);
  if (mc && !embedding) taskCentrality->SetMCInput();
  */
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
//  trackCuts.SetFilterMask(0);
//  trackCuts.SetCustomParamFromRun(169099, "pass2_muon");
//  trackCuts.CustomParam()->SetChi2NormCut(3.5);
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  trackCuts.SetIsMC(mc && !embedding);

  // Muon efficiency analysis
  gROOT->LoadMacro("AddTaskMUONTrackingEfficiency.C");
  AliAnalysisTaskMuonTrackingEffLocal* muonEfficiency = AddTaskMUONTrackingEfficiency(trackCuts,"");
  if(!muonEfficiency) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency->SelectCollisionCandidates(offlineTriggerMask);
  if (!alignStorage.IsNull()) muonEfficiency->SetAlignStorage(alignStorage.Data());
  //muonEfficiency->SetRecoParamStorage("alien://folder=/alice/cern.ch/user/p/ppillot/OCDB2012_newReco");
  muonEfficiency->SetMuonPtCut(1.);
  muonEfficiency->UseMCLabel(kFALSE);
  muonEfficiency->EnableDisplay(kTRUE);

  // Muon efficiency analysis (old without cut but with MC label)
  // gROOT->LoadMacro("AddTaskMUONTrackingEfficiency_old.C");
  // AliAnalysisTaskMuonTrackingEff_old* muonEfficiency_old_wocut_wMClabel = AddTaskMUONTrackingEfficiency_old(kFALSE, kFALSE,"old_wocut_wMClabel");
  // if(!muonEfficiency_old_wocut_wMClabel) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wocut_wMClabel->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wocut_wMClabel->UseMCLabel(kTRUE);

  // Muon efficiency analysis (old with cut)
  // AliAnalysisTaskMuonTrackingEff_old* muonEfficiency_old_wcut = AddTaskMUONTrackingEfficiency_old(kTRUE, kTRUE,"old_wcut");
  // if(!muonEfficiency_old_wcut) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wcut->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wcut->PtCut(1.);

  // Muon efficiency analysis (old with cut and MC label)
  // AliAnalysisTaskMuonTrackingEff_old* muonEfficiency_old_wcut_wMClabel = AddTaskMUONTrackingEfficiency_old(kTRUE, kTRUE,"old_wcut_wMClabel");
  // if(!muonEfficiency_old_wcut_wMClabel) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wcut_wMClabel->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wcut_wMClabel->PtCut(1.);
  // muonEfficiency_old_wcut_wMClabel->UseMCLabel(kTRUE);
  /*
  // Muon efficiency analysis -- with pDCA cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  AliAnalysisTaskMuonTrackingEff* muonEfficiency2 = AddTaskMUONTrackingEfficiency(trackCuts,"pDCA");
  if(!muonEfficiency2) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency2->SelectCollisionCandidates(offlineTriggerMask);

  // Muon efficiency analysis -- with chi2 cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuTrackChiSquare);
  AliAnalysisTaskMuonTrackingEff* muonEfficiency3 = AddTaskMUONTrackingEfficiency(trackCuts,"chi2");
  if(!muonEfficiency3) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency3->SelectCollisionCandidates(offlineTriggerMask);

  // Muon efficiency analysis -- with pDCA and chi2 cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca |
			  AliMuonTrackCuts::kMuTrackChiSquare);
  AliAnalysisTaskMuonTrackingEff* muonEfficiency4 = AddTaskMUONTrackingEfficiency(trackCuts,"pDCAchi2");
  if(!muonEfficiency4) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency4->SelectCollisionCandidates(offlineTriggerMask);
  */
  // Physics results
  // gROOT->LoadMacro("AddTaskMuonPhysics.C");
  // AliAnalysisTaskMuonPhysics* physics = AddTaskMuonPhysics();
  // if(!physics) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
  //   return;
  // }
  // if (applyPhysSel) physics->SelectCollisionCandidates(offlineTriggerMask);
  // physics->SetMuonTrackCuts(trackCuts);

}

