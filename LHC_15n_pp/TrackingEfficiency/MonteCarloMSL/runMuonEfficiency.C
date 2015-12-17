/*
 *  runMuonEfficiency.C
 *  aliroot
 *
 *  Created by Philippe Pillot on 16/12/10.
 *  Copyright 2010 SUBATECH. All rights reserved.
 *
 */

TString rootVersion = "v5-34-30";
TString alirootVersion = "v5-07-01-3";
TString aliphysicsVersion = "vAN-20151015-1";
TString dataDir = "/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/singleMuon/tuned/CMSL7-B-NOPF-MUFAST";
TString dataPattern = "*/AliESDs.root";
TString runFormat = "%06d";
TString outDir = "Analysis/LHC15n/TrackingEfficiency/MonteCarlo/singleMuon/tuned";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 150;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 4;

// --- prepare environment ---
 TString extraLibs="";
 TString extraIncs="include";
 TString extraTasks="AliAnalysisTaskMuonTrackingEffLocal";

//TString alignStorage = "alien://folder=/alice/simulation/2008/v4-15-Release/Residual";
TString alignStorage = "alien://folder=/alice/data/2015/OCDB";

//______________________________________________________________________________
void runMuonEfficiency(TString smode = "full", TString inputFileName = "runList.txt",
		       Bool_t applyPhysSel = kFALSE, Bool_t mc = kTRUE, Bool_t embedding = kFALSE)
{
  /// Study the MUON performances
  
  gROOT->LoadMacro("/Users/audurier/Documents/Analysis/Macro_Utile/runTaskFacilities.C");

  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
  }

   // --- saf3 case ---
  if (mode == kSAF3Connect) {
    
    // run on SAF3
    if (!RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName)) return;
    
    // // draw the results locally
    // outFile = TFile::Open(Form("Results_step%d.root", iStep),"READ");
    // if (outFile && outFile->IsOpen()) {
    //   outFile->FindObjectAny("cRes")->Draw();
    //   outFile->FindObjectAny("cRat")->Draw();
    //   outFile->Close();
    // }
    
    // // do not try to re-run locally!
    // return; 
  }
  
 // --- prepare environment ---
  TString extraLibs="PWGmuon";
  TString extraIncs="include";
  TString extraTasks="AliAnalysisTaskMuonTrackingEffLocal";
  TString extraPkgs="";
  LoadAlirootLocally(extraLibs, extraIncs, extraTasks, extraPkgs);
  AliAnalysisGrid *alienHandler = 0x0;
  if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, kTRUE);
  else if (mode == kGrid || mode == kTerminate) {
    TString analysisMacroName = "GenTuner";
    alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, alirootVersion, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, extraPkgs, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
    if (!alienHandler) return;
  }

  // --- Create the analysis train ---
  AliAnalysisTaskMuonTrackingEffLocal *muonEfficiency = static_cast<AliAnalysisTaskMuonTrackingEffLocal*>(CreateAnalysisTrain(applyPhysSel,mc,embedding,alienHandler));
  if (!muonEfficiency) return;
    
  // --- Create input object ---
  TObject* inputObj = CreateInputObject(mode, inputFileName);
  
  // --- start analysis ---
  StartAnalysis(mode, inputObj);  
  
}

//______________________________________________________________________________
TObject* CreateAnalysisTrain(Bool_t applyPhysSel, Bool_t mc, Bool_t embedding, TObject* alienHandler)
{
  /// create the analysis train and configure it
  
  // LoadLocalLibs(kTRUE);

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
  
  // event selection in case of physicselection
  UInt_t offlineTriggerMask;
  if (applyPhysSel) {
    gROOT->LoadMacro("/Users/audurier/alicesw/aliphysics/mumu/inst/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask* physicsSelection = AddTaskPhysicsSelection(mc && !embedding);
    if(!physicsSelection) {
      Error("CreateAnalysisTrain","AliPhysicsSelectionTask not created!");
      return;
    }
    //offlineTriggerMask = AliVEvent::kAny;
    offlineTriggerMask = AliVEvent::kMUU7;
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
  // AliAnalysisTaskMuonTrackingEffLocal_old* muonEfficiency_old_wocut_wMClabel = AddTaskMUONTrackingEfficiency_old(kFALSE, kFALSE,"old_wocut_wMClabel");
  // if(!muonEfficiency_old_wocut_wMClabel) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wocut_wMClabel->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wocut_wMClabel->UseMCLabel(kTRUE);
  
  // Muon efficiency analysis (old with cut)
  // AliAnalysisTaskMuonTrackingEffLocal_old* muonEfficiency_old_wcut = AddTaskMUONTrackingEfficiency_old(kTRUE, kTRUE,"old_wcut");
  // if(!muonEfficiency_old_wcut) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wcut->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wcut->PtCut(1.);
  
  // Muon efficiency analysis (old with cut and MC label)
  // AliAnalysisTaskMuonTrackingEffLocal_old* muonEfficiency_old_wcut_wMClabel = AddTaskMUONTrackingEfficiency_old(kTRUE, kTRUE,"old_wcut_wMClabel");
  // if(!muonEfficiency_old_wcut_wMClabel) {
  //   Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal_old not created!");
  //   return;
  // }
  // if (applyPhysSel) muonEfficiency_old_wcut_wMClabel->SelectCollisionCandidates(offlineTriggerMask);
  // muonEfficiency_old_wcut_wMClabel->PtCut(1.);
  // muonEfficiency_old_wcut_wMClabel->UseMCLabel(kTRUE);
  /*
  // Muon efficiency analysis -- with pDCA cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  AliAnalysisTaskMuonTrackingEffLocal* muonEfficiency2 = AddTaskMUONTrackingEfficiency(trackCuts,"pDCA");
  if(!muonEfficiency2) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency2->SelectCollisionCandidates(offlineTriggerMask);
  
  // Muon efficiency analysis -- with chi2 cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuTrackChiSquare);
  AliAnalysisTaskMuonTrackingEffLocal* muonEfficiency3 = AddTaskMUONTrackingEfficiency(trackCuts,"chi2");
  if(!muonEfficiency3) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency3->SelectCollisionCandidates(offlineTriggerMask);
  
  // Muon efficiency analysis -- with pDCA and chi2 cut
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca |
			  AliMuonTrackCuts::kMuTrackChiSquare);
  AliAnalysisTaskMuonTrackingEffLocal* muonEfficiency4 = AddTaskMUONTrackingEfficiency(trackCuts,"pDCAchi2");
  if(!muonEfficiency4) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEffLocal not created!");
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
  return muonEfficiency;
} 

