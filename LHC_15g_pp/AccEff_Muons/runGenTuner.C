/*
 *  runAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */

// tune1 LHC15g
Double_t oldPtParam[4]  = {0.966783, 0.653789, 70.1811,  0.059057};
Bool_t oldFixPtParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE};
Double_t newPtParam[4]  = {0.966783, 0.653789, 70.1811,  0.059057};
Bool_t newFixPtParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE};

Double_t ptRange[2] = {0.15, 999.};

// tune1 LHC15g
Double_t oldYParam[4]  = {0.7, 0.5,0.15,0.2};
Bool_t oldFixYParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE};
Double_t newYParam[4]  = {0.7, 0.5,0.15, 0.2};
Bool_t newFixYParam[4] = {kFALSE, kFALSE, kFALSE, kFALSE};

Double_t yRange[2] = {-4.3, -2.3};

Bool_t applyPhysicsSelection = kFALSE;

 // --- prepare environment ---
TString extraTasks="AliAnalysisTaskGenTunerLocal";

//______________________________________________________________________________
void runGenTuner(TString runMode, TString analysisMode,
TString inputName       = "Find;BasePath=/alice/data/2015/LHC15g/000228936/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;",
TString inputOptions    = "",
Int_t iStep             = -1,
TString analysisOptions = "",
TString softVersions    = "aliphysics=vAN-20151115-1",
TString taskOptions     = "")
{
  /// Study the MUON performances
  
  // Load macro used to connect to saf3
  gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

  // Set Mc Flag
  Bool_t isMC = IsMC(inputOptions);

  // Macro to connect to proof. First argument useless for saf3
  SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions,Form("libPWGmuon.so %s.cxx",extraTasks.Data()),". $ALICE_ROOT/include $ALICE_PHYSICS/include");
    
  // --- Create the analysis train with the task ---
  AliAnalysisTaskGenTunerLocal *genTuner = static_cast<AliAnalysisTaskGenTunerLocal*>(CreateAnalysisTrain(iStep,isMC));
  if (!genTuner) return;
  
  // --- start analysis ---
  StartAnalysis(runMode,analysisMode,inputName,inputOptions);     

  // --- save fitting functions ---
  if(!IsPod(analysisMode) || IsPodMachine(analysisMode))
  {
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
    if (iStep > -1) gSystem->Exec(Form("cp -f %s Results_step%d.root", outFileName.Data(),iStep));
    else gSystem->Exec(Form("cp -f %s AnalysisResultsData.root", outFileName.Data()));
  }
}

//______________________________________________________________________________
TObject* CreateAnalysisTrain(Int_t iStep,Bool_t mc)
{
  /// create the analysis train and configure it
  
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
        AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  if (mc) trackCuts.SetIsMC(mc);
  
  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");
  AliAnalysisTaskGenTunerLocal* genTuner = AddTaskGenTuner();
  if(!genTuner) {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTunerLocal not created!");
    return 0x0;
  }
  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
//  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUSH7);
  //genTuner->SelectCentrality(0., 90.);
  genTuner->SetMuonTrackCuts(trackCuts);
  genTuner->SetMuonPtCut(1.);

  if (mc) genTuner->SetDataFile("AnalysisResultsData.root");

  printf("File %s plugged to the analysis \n",genTuner->GetDataFile().Data() );

  if (iStep == 0) {
    // set the generator parameters used in simulation
    genTuner->SetPtParam(oldPtParam, oldFixPtParam, newPtParam, newFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetYParam(oldYParam, oldFixYParam, newYParam, newFixYParam, yRange[0], yRange[1]);
    
  } else if (iStep > 0) {

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

