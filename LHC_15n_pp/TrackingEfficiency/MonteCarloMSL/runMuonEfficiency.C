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
TString aliphysicsVersion = "vAN-20160105-1";
TString dataDir = "/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/singleMuon/tuned/CMSL7-B-NOPF-MUFAST";
TString dataPattern = "*/AliESDs.root";
TString runFormat = "%06d";
TString outDir = "Analysis/LHC15n/TrackingEfficiency/MonteCarlo/singleMuon/tuned/WiththnSparse";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 150;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 4;

 // --- prepare environment ---
  TString extraLibs="";
  TString extraIncs="include";
  TString extraTasks="";
  TString extraPkgs="";

//TString alignStorage = "alien://folder=/alice/simulation/2008/v4-15-Release/Residual";
TString alignStorage = "alien://folder=/alice/data/2015/OCDB";
Bool_t setDefaultTrackCuts =kTRUE;

//______________________________________________________________________________
void runMuonEfficiency(TString smode = "full", TString inputFileName = "runList.txt",
		       Bool_t applyPhysSel = kFALSE, Bool_t mc = kTRUE, Bool_t embedding = kFALSE)
{
  /// Study the MUON performances
  
  gROOT->LoadMacro("$MACRODIR/Philippe/runTaskFacilities.C");

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
    
  }

  LoadAlirootLocally(extraLibs, extraIncs, extraTasks, extraPkgs);
  AliAnalysisGrid *alienHandler = 0x0;
  if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, kTRUE);
  else if (mode == kGrid || mode == kTerminate) {
    TString analysisMacroName = "GenTuner";
    alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, alirootVersion, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, extraPkgs, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
    if (!alienHandler) return;
  }

  // --- Create the analysis train ---
  AliAnalysisTaskMuonTrackingEff *muonEfficiency = static_cast<AliAnalysisTaskMuonTrackingEff*>(CreateAnalysisTrain(applyPhysSel,mc,embedding,alienHandler));
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
    gROOT->LoadMacro("/Users/audurier/alicesw/aliphysics/mumu/src/OADB/macros/AddTaskPhysicsSelection.C");
    AliPhysicsSelectionTask* physicsSelection = AddTaskPhysicsSelection(mc && !embedding);
    if(!physicsSelection) {
      Error("CreateAnalysisTrain","AliPhysicsSelectionTask not created!");
      return;
    }
    //offlineTriggerMask = AliVEvent::kAny;
    offlineTriggerMask = AliVEvent::kMUS7;
    // offlineTriggerMask = AliVEvent::kMUU7 | AliVEvent::kMuonUnlikeLowPt8;
  }
  
  // Muon efficiency analysis
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGPP/MUON/dep/AddTaskMUONTrackingEfficiency.C");
  AliAnalysisTaskMuonTrackingEff* muonEfficiency = AddTaskMUONTrackingEfficiency(setDefaultTrackCuts,mc);
  if(!muonEfficiency) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMuonTrackingEff not created!");
    return;
  }
  if (applyPhysSel) muonEfficiency->SelectCollisionCandidates(offlineTriggerMask);
  if (!alignStorage.IsNull()) muonEfficiency->SetAlignStorage(alignStorage.Data());
  muonEfficiency->SetMuonPtCut(0.);
  muonEfficiency->UseMCLabel(kFALSE);
  muonEfficiency->EnableDisplay(kTRUE);
  
  return muonEfficiency;
} 

