/*
 *  runJPsiAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */


TString rootVersion = "v5-34-30";
TString aliphysicsVersion = "vAN-20160116-1";
TString dataDir = "";
TString dataPattern = "";
TString runFormat = "%09d";
TString outDir = "";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 100;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 2;

// tune0 LHC15o jpsi
TString oldPtFormula =   "[0] *x / TMath::Power( 1. + TMath::Power(x/[1],[2]), [3] )";
Double_t oldPtParam[4] = {1.13e9,3.404,3.822,1.962}; 
Bool_t oldFixPtParam[4] = {kFALSE, kFALSE,kFALSE,kFALSE};
TString newPtFormula = "[0] *x / TMath::Power( 1. + TMath::Power(x/[1],[2]), [3] )";
// Double_t newPtParam[4] = {1.13e9,3.404,3.822,1.962};
Double_t newPtParam[4] = {0.603228, 5.57423, 1.5238, 7.38334};
Bool_t newFixPtParam[4] = {kFALSE, kFALSE,kFALSE,kFALSE};

Double_t ptRange[2] = {0., 50.};


// tune0 LHC15o jpsi
TString oldYFormula = " [0] * ( [1] + [2]*x*x ) ";
Double_t oldYParam[3] =  {1.,6.266e5,-3.007e4};
Bool_t oldFixYParam[3] = {kFALSE, kFALSE,kFALSE};
TString newYFormula = "[0] * ( [1] + [2]*x*x  + [3]*x*x*x*x)";
Double_t newYParam[4] =  {1.,580000,-3.007e4,800.};
Bool_t newFixYParam[4] = {kFALSE, kFALSE,kFALSE,kFALSE};

Double_t yRange[2] = {-4.2, -2.3};


Bool_t isMC = kTRUE;
Bool_t applyPhysicsSelection = kFALSE;


void UpdateParametersAndRanges(Int_t iStep);

//__________Setting for spectra path
TString striggerDimuon  ="CMUL7-B-NOPF-MUFAST";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-10.0RABSMATCHLOWETAPDCA";
TString scentrality     ="V0M_00.00_90.00";
TString subResultName   = "";/*"YVSPT_BENJ_02.00_04.00_m03.50_m03.00";*/
//__________


//______________________________________________________________________________
void runGenTuner(TString smode = "local", TString inputFileName = "AliAOD.Muons.root",
     Int_t iStep = 0, char overwrite = '\0')
{
  /// Tune single muon kinematics distribution
  
  gROOT->LoadMacro("$HOME/Documents/Analysis/Macro/Philippe/runTaskFacilities.C");
  
  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
  }
  
  // --- copy files needed for this analysis ---
  TList pathList; pathList.SetOwner();
  // pathList.Add(new TObjString("/Users/audurier/Documents/Analysis/Tasks"));
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString("runGenTuner.C"));
  fileList.Add(new TObjString("AddTaskGenTuner.C"));
  fileList.Add(new TObjString("AliAnalysisTaskGenTunerJpsi.cxx"));
  fileList.Add(new TObjString("AliAnalysisTaskGenTunerJpsi.h"));
  // CopyFileLocally(pathList, fileList, overwrite);
  CopyInputFileLocally("/Users/audurier/Documents/Analysis/LHC_15o_PbPb/AccEff_jpsi/DataPart/AnalysisResults.root", "AnalysisResultsReference.root", overwrite);
  fileList.Add(new TObjString("AnalysisResultsReference.root"));
  
  // --- saf3 case ---
  if (mode == kSAF3Connect) {
    // run on SAF3
    if (!RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName)) return;
    // draw the results locally
    outFile = TFile::Open(Form("Results_step%d.root", iStep),"READ");
    if (outFile && outFile->IsOpen()) {
      outFile->FindObjectAny("cRes")->Draw();
      outFile->FindObjectAny("cRat")->Draw();
      outFile->Close();
    }
    // do not try to re-run locally!
    return;
  }
  
  // --- prepare environment ---
  TString extraLibs="PWGmuon";
  TString extraIncs="include";
  TString extraTasks="AliAnalysisTaskGenTunerJpsi";
  TString extraPkgs="";
  LoadAlirootLocally(extraLibs, extraIncs, extraTasks, extraPkgs);
  AliAnalysisGrid *alienHandler = 0x0;
  if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, kTRUE);
  else if (mode == kGrid || mode == kTerminate) {
    TString analysisMacroName = "GenTuner";
    alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, extraPkgs, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
    if (!alienHandler) return;
  }

  // --- Create the analysis train ---
  AliAnalysisTaskGenTunerJpsi *genTuner = static_cast<AliAnalysisTaskGenTunerJpsi*>(CreateAnalysisTrain(alienHandler, iStep));
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
    if (genTuner->GetCurrentPtFunc()) genTuner->GetCurrentPtFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetCurrentPtFuncMC()) genTuner->GetCurrentPtFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewPtFunc()) genTuner->GetNewPtFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetCurrentYFunc()) genTuner->GetCurrentYFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetCurrentYFuncMC()) genTuner->GetCurrentYFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewYFunc()) genTuner->GetNewYFunc()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetResults()) genTuner->GetResults()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetRatios()) genTuner->GetRatios()->Write(0x0, TObject::kOverwrite);
    outFile->Close();
  }

  // save results of current step if running in a loop
  if (iStep > -1) gSystem->Exec(Form("cp -f AnalysisResults.root Results_step%d.root", iStep));
  // else gSystem->Exec(Form("cp -f AnalysisResults.root ReferenceResults.root"));
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
//        AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  if (isMC) trackCuts.SetIsMC(kTRUE);


  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");

  //Config task
  AliAnalysisTaskGenTunerJpsi* genTuner = AddTaskGenTuner();
  if(!genTuner) 
  {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTunerJpsi not created!");
    return 0x0;
  }
  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
  // genTuner->SelectCentrality(0., 90.);
  genTuner->SetMuonTrackCuts(trackCuts);

  //___________Set Ref. Histo__________
  TFile* dataFile = TFile::Open("AnalysisResultsReference.root","READ");
  if (!dataFile || !dataFile->IsOpen()) return;

  // Get HistoCollection
  AliMergeableCollection * oc = 0x0;
  dataFile->GetObject("OC",oc);
  if (!oc) return;

   // Get spectras
  AliAnalysisMuMuSpectra * spectraPT = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraPT)
  {
      cout << Form("Cannot find PT spectra in /%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
      return;
  }
  AliAnalysisMuMuSpectra * spectraY = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-Y",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraY)
  {
      cout << Form("Cannot find Y spectra in /%s/%s/%s/%s/PSI-Y",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
      return;
  }

  Double_t* ptbin= spectraPT->Binning()->CreateBinArrayX();
  Double_t* ybin= spectraY->Binning()->CreateBinArrayX();

  Int_t ptnofbin= spectraPT->Binning()->GetNBinsX();
  Int_t ynofbin= spectraY->Binning()->GetNBinsX();

  //___________Set ref.Spectra
  TH1* hpt= spectraPT->Plot("NofJPsi",subResultName,kTRUE);// new
  TH1* hy= spectraY->Plot("NofJPsi",subResultName,kTRUE);// new
  dataFile->Close(); 
  
  genTuner->SetPtBin(ptnofbin+1,ptbin);
  genTuner->SetYBin(ynofbin+1,ybin);

  if(hpt && hy){
    genTuner->SetPtRefHisto(hpt);
    genTuner->SetYRefHisto(hy);
  } else {
    cout << "Cannot set reference histo !" << endl;
    return;
  }
  //==================================
  //==================================
    
    // get the new generator parameters from previous step if any and configure the tuner
    if (isMC) {
        
    // update the parameters and the fitting ranges from the previous step if any
    UpdateParametersAndRanges(iStep);
    
    // set the original function and parameters used in simulation
    genTuner->SetOriginPtFunc(oldPtFormula.Data(), oldPtParam, oldFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetOriginYFunc(oldYFormula.Data(), oldYParam, oldFixYParam, yRange[0], yRange[1]);
    
    // set the new function and initial parameters
    genTuner->SetNewPtFunc(newPtFormula.Data(), newPtParam, newFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetNewYFunc(newYFormula.Data(), newYParam, newFixYParam, yRange[0], yRange[1]);
    
    // enable the weighing
    if (iStep > 0) genTuner->Weight(kTRUE);
  }
  
  return genTuner;
}


//______________________________________________________________________________
void UpdateParametersAndRanges(Int_t iStep)
{
  /// update the parameters and the fitting ranges from the previous step
  
  if (iStep <= 0) return;

  TString inFileName = Form("Results_step%d.root",iStep-1);
  inFile = TFile::Open(inFileName.Data(),"READ");
  if (!inFile || !inFile->IsOpen()) {
    printf("cannot open file from previous step\n");
    exit(1);
  }
  
  TF1 *fNewPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncNew"));
  TF1 *fNewYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFuncNew"));
  if (!fNewPtFunc || !fNewYFunc) {
    printf("previous functions not found\n");
    exit(1);
  }
  
  if ((fNewPtFunc->GetNpar() != (Int_t)(sizeof(newPtParam)/sizeof(Double_t))) ||
      (fNewYFunc->GetNpar() != (Int_t)(sizeof(newYParam)/sizeof(Double_t)))) {
    printf("mismatch between the number of parameters in the previous step and in this macro\n");
    exit(1);
  }
  
  for (Int_t i = 0; i < fNewPtFunc->GetNpar(); ++i) newPtParam[i] = fNewPtFunc->GetParameter(i);
  ptRange[0] = fNewPtFunc->GetXmin();
  ptRange[1] = fNewPtFunc->GetXmax();
  
  for (Int_t i = 0; i < fNewYFunc->GetNpar(); ++i) newYParam[i] = fNewYFunc->GetParameter(i);
  yRange[0] = fNewYFunc->GetXmin();
  yRange[1] = fNewYFunc->GetXmax();
  
  inFile->Close();
  
}
