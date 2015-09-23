/*
 *  runJPsiAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */

// "Find;FileName=AliESDs.root;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/simsinglemuon/pp/CMSL7-B-NOPF-MUON"


TString rootVersion = "v5-34-30";
TString aliphysicsVersion = "vAN-20150902";
TString alirootVersion = "v5-06-33";
TString dataDir = "/alice/data/2015/LHC15g";
TString dataPattern = "*AliAOD.Muons.root";
TString runFormat = "%09d";
TString outDir = "Analysis/LHC15g/AccEff/Muons/Data";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 150;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 4;

// generator parameters used in the simulation

// tune1 LHC13f
Double_t oldPtParam[4] = {0.966783, 0.653789, 70.1811,  0.059057};
Bool_t oldFixPtParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE/*, kFALSE, kFALSE*/};
Double_t newPtParam[4] = {0.966783, 0.653789, 70.1811,  0.059057};
Bool_t newFixPtParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE/*, kFALSE, kFALSE*/};

Double_t ptRange[2] = {0.2, 15.};

// tune1 LHC13f
Double_t oldYParam[4] = {0.000127694, -339.678,200.776, -54.9561};
Bool_t oldFixYParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE/*, kFALSE, kFALSE, kTRUE, kTRUE*/};
Double_t newYParam[4] = {0.000127694, -339.678,200.776, -54.9561};
Bool_t newFixYParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE/*, kFALSE, kFALSE, kTRUE, kTRUE*/};

Double_t yRange[2] = {-3.8, -2.5};


Bool_t isMC = kFALSE;
Bool_t applyPhysicsSelection = kFALSE;

 // --- prepare environment ---
 TString extraLibs="PWGmuon";
TString extraIncs="include";
TString extraTasks="AliAnalysisTaskGenTuner";

//______________________________________________________________________________
void runGenTuner(TString smode = "saf", TString inputFileName = "Find;BasePath=/alice/data/2015/LHC15g/000228979/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root",
		 Int_t iStep = -1, char overwrite = '\0')
{
  /// Study the MUON performances
  
  gROOT->LoadMacro("/Users/audurier/Documents/Analysis/Macro_Utile/runTaskFacilities.C");
  
  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
  }
  
 // --- copy files needed for this analysis ---
  // TList pathList; pathList.SetOwner();
  // pathList.Add(new TObjString("/Users/audurier/Documents/Analysis/Tasks"));

  // TList fileList; fileList.SetOwner();
  // fileList.Add(new TObjString("AliAnalysisTaskGenTuner.cxx"));
  // fileList.Add(new TObjString("AliAnalysisTaskGenTuner.h"));

  // CopyFileLocally(pathList, fileList);
  

  // LoadAlirootLocally(extraLibs, extraIncs, extraTasks);

  gEnv->SetValue("XSec.GSI.DelegProxy","2");
  TProof::Open("baudurie@nansafmaster2.in2p3.fr/?N","");
  TProof::Mgr("baudurie@nansafmaster2.in2p3.fr")->SetROOTVersion("VO_ALICE@ROOT::v5-34-30");
  TList* list = new TList;
  list->Add(new TNamed("ALIROOT_MODE","base"));
  list->Add(new TNamed("ALIROOT_EXTRA_LIBS","PWGmuon"));
  list->Add(new TNamed("ALIROOT_EXTRA_INCLUDES","include"));
  gProof->EnablePackage("VO_ALICE@AliPhysics::vAN-20150902",list,kFALSE);
  gProof->UploadPackage("VO_ALICE@AliPhysics::vAN-20150902");
  gROOT->LoadMacro("AliAnalysisTaskGenTuner.cxx++g");
   gProof->Load("AliAnalysisTaskGenTuner.cxx++g",kFALSE);


  AliAnalysisGrid *alienHandler = 0x0;
  // if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, kTRUE,"");
  // else if (mode == kGrid || mode == kTerminate) {
  //   TString analysisMacroName = "Eff";
  //   alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, rootVersion, alirootVersion, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
  //   if (!alienHandler) return;
  // }
  
  // --- Create the analysis train ---
  AliAnalysisTaskGenTuner *genTuner = static_cast<AliAnalysisTaskGenTuner*>(CreateAnalysisTrain(alienHandler, iStep));
  if (!genTuner) return;
  
  // --- Create input object ---
  TObject* inputObj = CreateInputObject(mode, inputFileName);
  
  // --- start analysis ---
  StartAnalysis(mode, inputObj);
  
  // --- save fitting functions ---
  TString outFileName = AliAnalysisManager::GetCommonFileName();
  TFile *outFile = (TFile*)gROOT->GetListOfFiles()->FindObject(outFileName.Data());
  if (outFile) outFile->ReOpen("UPDATE");
  else outFile = TFile::Open(outFileName.Data(),"UPDATE");
  if (outFile && outFile->IsOpen()) {
    outFile->Cd(Form("%s:/MUON_GenTuner",outFileName.Data()));
    if (genTuner->GetOldPtFunc()) genTuner->GetOldPtFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetOldPtFuncMC()) genTuner->GetOldPtFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewPtFunc()) genTuner->GetNewPtFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetOldYFunc()) genTuner->GetOldYFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetOldYFuncMC()) genTuner->GetOldYFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewYFunc()) genTuner->GetNewYFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetResults()) genTuner->GetResults()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetRatios()) genTuner->GetRatios()->Write(0x0, TObject::kOverwrite);
    outFile->Close();
  }
  
  // save results of current step if running in a loop
  if (iStep > -1) gSystem->Exec(Form("cp -f AnalysisResults.root Results_step%d.root", iStep));
  
}

//______________________________________________________________________________
TObject* CreateAnalysisTrain(TObject* alienHandler, Int_t iStep)
{
  /// create the analysis train and configure it
  
  // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("GenTunerAnalysis");
  
  // Connect plugin to the analysis manager if any
  if (alienHandler) mgr->SetGridHandler(static_cast<AliAnalysisGrid*>(alienHandler));

  
  // AOD handler
  AliInputEventHandler* aodH = new AliAODInputHandler;
  mgr->SetInputEventHandler(aodH);
  

  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
//  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchHpt | AliMuonTrackCuts::kMuEta |
//			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  if (isMC) trackCuts.SetIsMC(kTRUE);
  
  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");
  AliAnalysisTaskGenTuner* genTuner = AddTaskGenTuner();
  if(!genTuner) {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTuner not created!");
    return 0x0;
  }
  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
//  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUSH7);
  //genTuner->SelectCentrality(0., 90.);
  genTuner->SetMuonTrackCuts(trackCuts);
  genTuner->SetMuonPtCut(1.);
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13d/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13d/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13e/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13e/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13de/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13de/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
  if (isMC) genTuner->SetDataFile("AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT4GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT6GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT1GeV_y2.5-3/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT1GeV_y3-4/AnalysisResults.root");
  
  if (iStep == 0) {
    
    // set the generator parameters used in simulation
    genTuner->SetPtParam(oldPtParam, oldFixPtParam, newPtParam, newFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetYParam(oldYParam, oldFixYParam, newYParam, newFixYParam, yRange[0], yRange[1]);
    
  } else if (iStep > 0) {
    /*
    // get the original generator parameters from first step if any
    TFile *inFile = TFile::Open("Results_step0.root","READ");
    if (inFile && inFile->IsOpen()) {
      TF1 *fOldPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFunc"));
      TF1 *fOldYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFunc"));
      if (fOldPtFunc && fOldYFunc) {
	fOldPtFunc->GetParameters(oldPtParam);
	fOldYFunc->GetParameters(oldYParam);
      }
      inFile->Close();
    }
    */
    // get the new generator parameters from previous step if any and configure the tuner
    TString inFileName = Form("Results_step%d.root",iStep-1);
    inFile = TFile::Open(inFileName.Data(),"READ");
    if (inFile && inFile->IsOpen()) {
      TF1 *fNewPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncNew"));
      TF1 *fNewYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFuncNew"));
      if (fNewPtFunc && fNewYFunc) {
	genTuner->SetPtParam(oldPtParam, newFixPtParam, fNewPtFunc->GetParameters(), newFixPtParam, fNewPtFunc->GetXmin(), fNewPtFunc->GetXmax());
	genTuner->SetYParam(oldYParam, newFixYParam, fNewYFunc->GetParameters(), newFixYParam, fNewYFunc->GetXmin(), fNewYFunc->GetXmax());
      }
      inFile->Close();
    }
    
    // enable the weighing
    genTuner->Weight(kTRUE);
    
  }
  
  return genTuner;
  
}

