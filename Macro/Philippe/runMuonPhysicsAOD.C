/*
 *  runMuonPhysics.C
 *  aliroot
 *
 *  Created by Philippe Pillot on 11/12/12.
 *  Copyright 2010 SUBATECH. All rights reserved.
 *
 */

//______________________________________________________________________________
void runMuonPhysicsAOD(TString smode = "local", TString inputFileName = "AliAOD.Muons.root")
{
  /// Extract some physical quantities
  
  // --- analysis setup ---
  TString rootVersion = "";
  TString alirootVersion = "";
  TString aliphysicsVersion = "vAN-20151210-1";
  TString dataDir = "/alice/cern.ch/user/p/ppillot/Data/LHC15n/reco/PbPbParam_AlignV6";
  TString dataPattern = "*AliESDs.root";
  TString runFormat = "%09d";
  TString outDir = "Data/LHC15n/reco/PbPbParam_AlignV6/Phys/all";
  TString analysisMacroName = "Physics";
  Int_t ttl = 30000;
  Int_t maxFilesPerJob = 100;
  Int_t maxMergeFiles = 10;
  Int_t maxMergeStages = 2;
  TString extraLibs="";
  TString extraIncs="include";
  TString extraTasks="AliAnalysisTaskMuonPhysics";
  TString extraPkgs="";
  TList pathList; pathList.SetOwner();
  pathList.Add(new TObjString("$WORK/Macros/MuonPhysics"));
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString("runMuonPhysicsAOD.C"));
  fileList.Add(new TObjString("AddTaskMuonPhysics.C"));
  fileList.Add(new TObjString("AliAnalysisTaskMuonPhysics.cxx"));
  fileList.Add(new TObjString("AliAnalysisTaskMuonPhysics.h"));
  
  gROOT->LoadMacro("$HOME/Work/Alice/Macros/Facilities/runTaskFacilities.C");
  
  // --- prepare the analysis environment ---
  Int_t mode = PrepareAnalysis(smode, inputFileName, extraLibs, extraIncs, extraTasks, extraPkgs, pathList, fileList);
  
  // --- run the analysis (saf3 is a special case as the analysis is launched on the server) ---
  if (mode == kSAF3Connect) {
    
    RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName);
    
  } else {
    
    CreateAnalysisTrain();
    
    RunAnalysis(smode, inputFileName, rootVersion, alirootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, dataDir, dataPattern, outDir, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages);
    
  }
  
}

//______________________________________________________________________________
void CreateAnalysisTrain()
{
  /// create the analysis train and configure it
  
  // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("MuonPhysicsAnalysis");
  
  // Debug mode
  //mgr->SetDebugLevel(3);
  
  // AOD handler
  AliInputEventHandler* aodH = new AliAODInputHandler;
  mgr->SetInputEventHandler(aodH);
  /*
  // multiplicity/centrality selection
  gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
  AliMultSelectionTask *mult = AddTaskMultSelection(kFALSE);
  mult->SelectCollisionCandidates(AliVEvent::kMUU7);
  */
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
//  trackCuts.SetFilterMask(0);
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
//  trackCuts.SetIsMC(kTRUE);
//  trackCuts.SetCustomParamFromRun(169859, "pass2_muon");
//  trackCuts.CustomParam()->SetMeanDCA(0., 0., 0.);
//  trackCuts.CustomParam()->SetSigmaPdca(80., 54.);
//  trackCuts.CustomParam()->SetRelPResolution(0.0004);
//  trackCuts.CustomParam()->SetSlopeResolution(0.0005);
//  trackCuts.CustomParam()->SetNSigmaPdca(5.);
//  trackCuts.Print();
/*  
  // track rejection
  AliMuonTrackCuts trackCuts2("stdCuts2", "stdCuts2");
  trackCuts2.SetFilterMask(AliMuonTrackCuts::kMuPdca);
  trackCuts2.SetCustomParamFromRun(169859, "pass2_muon");
//  trackCuts2.CustomParam()->SetMeanDCA(0., 0., 0.);
  trackCuts2.CustomParam()->SetSigmaPdca(80, 54.);
//  trackCuts2.CustomParam()->SetNSigmaPdca(5.);
//  trackCuts2.CustomParam()->SetRelPResolution(0.0005);
//  trackCuts2.CustomParam()->SetSlopeResolution(0.0004);
//  trackCuts2.CustomParam()->SetChi2NormCut(3.5);
*/  
  // physics task
  gROOT->LoadMacro("AddTaskMuonPhysics.C");
  AliAnalysisTaskMuonPhysics* physics = AddTaskMuonPhysics("");
  if(!physics) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
//  physics->SelectCollisionCandidates(AliVEvent::kAnyINT);
//  physics->SelectCollisionCandidates(AliVEvent::kMuonUnlikePB);
//  physics->SelectCollisionCandidates(AliVEvent::kMuonLikePB | AliVEvent::kMuonUnlikePB);
//  physics->SelectCollisionCandidates(AliVEvent::kMUU7 | AliVEvent::kMuonUnlikeLowPt8);
//  physics->SelectCollisionCandidates(AliVEvent::kMUSH7 | AliVEvent::kMuonSingleHighPt8);
  physics->SelectCollisionCandidates(AliVEvent::kMUU7);
//  physics->SelectCollisionCandidates(AliVEvent::kMUL7 | AliVEvent::kMUU7 |
//				     AliVEvent::kMuonLikeLowPt8 | AliVEvent::kMuonUnlikeLowPt8);
//  physics->SelectCentrality(0., 90.);
  physics->SetMuonTrackCuts(trackCuts);
//  physics->SetMuonTrackCuts2(trackCuts2);
  physics->VersusRun(kTRUE);
  /*
  AliAnalysisTaskMuonPhysics* physics2 = AddTaskMuonPhysics("MClabel");
  if(!physics2) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
  physics2->SetMuonTrackCuts(trackCuts);
  physics2->UseMCLabel();
   */
  /*
  // track selection 2
  AliMuonTrackCuts trackCuts2("stdCuts2", "stdCuts2");
  trackCuts2.SetAllowDefaultParams();
  trackCuts2.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
  			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  
  // physics task 2
  AliAnalysisTaskMuonPhysics* physics2 = AddTaskMuonPhysics("wpDCA");
  if(!physics2) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
  //physics2->SelectCollisionCandidates(AliVEvent::kAnyINT);
  physics2->SetMuonTrackCuts(trackCuts2);
  
  // track selection 3
  AliMuonTrackCuts trackCuts3("stdCuts3", "stdCuts3");
  trackCuts3.SetCustomParamFromRun(191485, "muon_calo_pass2");
  trackCuts3.CustomParam()->SetNSigmaPdca(10.);
  trackCuts3.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
  			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  
  // physics task 3
  AliAnalysisTaskMuonPhysics* physics3 = AddTaskMuonPhysics("wpDCA_10sigma");
  if(!physics3) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
  //physics3->SelectCollisionCandidates(AliVEvent::kAnyINT);
  physics3->SetMuonTrackCuts(trackCuts3);
  */
  /*
  // track selection 4
  AliMuonTrackCuts trackCuts4("stdCuts4", "stdCuts4");
  trackCuts4.SetAllowDefaultParams();
  trackCuts4.SetFilterMask(AliMuonTrackCuts::kMuEta | AliMuonTrackCuts::kMuThetaAbs);
  
  // physics task 4
  AliAnalysisTaskMuonPhysics* physics4 = AddTaskMuonPhysics("accCuts");
  if(!physics4) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
  physics4->SelectCollisionCandidates(AliVEvent::kAnyINT);
  physics4->SetMuonTrackCuts(trackCuts4);
  
  // track selection 5
  AliMuonTrackCuts trackCuts5("stdCuts5", "stdCuts5");
  trackCuts5.SetAllowDefaultParams();
  trackCuts5.SetFilterMask(AliMuonTrackCuts::kMuEta | AliMuonTrackCuts::kMuThetaAbs |
                           AliMuonTrackCuts::kMuPdca);
  
  // physics task 5
  AliAnalysisTaskMuonPhysics* physics5 = AddTaskMuonPhysics("accCuts_pDCA");
  if(!physics5) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonPhysics not created!");
    return;
  }
  physics5->SelectCollisionCandidates(AliVEvent::kAnyINT);
  physics5->SetMuonTrackCuts(trackCuts5);
  */
}

