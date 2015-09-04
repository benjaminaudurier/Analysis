/*
 *  runJPsiAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */

//__________Global Settings
TString rootVersion = "v5-34-08";
TString aliphysicsVersion = "vAN-20150422";
TString dataDir = "/alice/data/2010/LHC10h";
TString dataPattern = "pass2/*AliESDs.root";
TString runFormat = "%09d";
TString outDir = "Data/LHC10h/pass2/Eff/pDCAChi2";

// extra to be load on CF
TString extraLibs="CORRFW:PWGmuon";
TString extraIncs="include";
TString extraTasks="";

Bool_t applyPhysicsSelection = kFALSE;
//__________

//__________Config. Param. for grid macro.
Int_t ttl = 30000;
Int_t maxFilesPerJob = 100;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 2;
//__________

//__________generator parameters used in the simulation
// tune1 LHC13d pt param. (see AliAnalysisTaskGenTuner::Pt)
Double_t oldPtParam[3] = {0.288315, 0.0593389, 3.42939};
Bool_t oldFixPtParam[3] = {kFALSE, kFALSE, kFALSE};
Double_t newPtParam[3] = {0.288315, 0.0593389, 3.42939};
Bool_t newFixPtParam[3] = {kFALSE, kFALSE, kFALSE};

Double_t ptRange[2] = {0.1, 6.5};

// tune1 LHC13d y param. (see AliAnalysisTaskGenTuner::Y)
Double_t oldYParam[2] = {1.50913, 0.171768};
Bool_t oldFixYParam[2] = {kFALSE, kFALSE};
Double_t newYParam[2] =  {1.50913, 0.171768};
Bool_t newFixYParam[2] = {kFALSE, kFALSE};

Double_t yRange[2] = {-3.8, -2.3};
//__________

//__________Setting for spectra path
TString striggerDimuon  ="CMUL7-B-NOPF-MUON";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-18.0RABSMATCHLOWETAPDCA";
TString scentrality     ="V0A";
TString subResultName   ="YVSPT_BENJ_02.00_04.00_m03.50_m03.00";
//__________

// Sim :
// Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root
// 
// Data:
// Find;BasePath=/alice/data/2013/LHC13d/000195760/ESDs/muon_pass2/AOD134;FileName=AliAOD.root

//______________________________________________________________________________
void runGenTuner(TString smode = "saf", TString inputFileName = "Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root",
		 Int_t iStep = -1, char overwrite = '\0')
{
  /// Study the MUON performances
  
  gROOT->LoadMacro("/Users/audurier/Documents/Analysis/runTaskFacilities.C");
  
  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) 
  {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
  }
  
  // --- copy files needed for this analysis ---
  // TList pathList; pathList.SetOwner();
  // pathList.Add(new TObjString("/Users/audurier/Documents/Analysis/Tasks"));

  // TList fileList; fileList.SetOwner();
  // fileList.Add(new TObjString("AliAnalysisTaskGenTuner.cxx"));
  // fileList.Add(new TObjString("AliAnalysisTaskGenTuner.h"));
  
  // --- prepare environment ---
  LoadAlirootLocally(extraLibs, extraIncs, extraTasks);
  
  // Choose the CF
  AliAnalysisGrid *alienHandler = 0x0;
  if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(smode, rootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, kTRUE);
  else if (mode == kGrid || mode == kTerminate) 
  {
    TString analysisMacroName = "GenTuner";
    alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(smode, rootVersion, aliphysicsVersion, inputFileName, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
    if (!alienHandler) return;
  }
  
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
  if (outFile && outFile->IsOpen()) 
  {
    outFile->Cd(Form("%s:/MUON_GenTuner",outFileName.Data()));
    if (genTuner->GetOldPtFunc())   genTuner->GetOldPtFunc()->Write(0x0, TObject  ::kOverwrite);
    if (genTuner->GetOldPtFuncMC()) genTuner->GetOldPtFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewPtFunc())   genTuner->GetNewPtFunc()->Write(0x0, TObject  ::kOverwrite);
    if (genTuner->GetOldYFunc())    genTuner->GetOldYFunc()->Write(0x0, TObject   ::kOverwrite);
    if (genTuner->GetOldYFuncMC())  genTuner->GetOldYFuncMC()->Write(0x0, TObject ::kOverwrite);
    if (genTuner->GetNewYFunc())    genTuner->GetNewYFunc()->Write(0x0, TObject   ::kOverwrite);
    if (genTuner->GetResults())     genTuner->GetResults()->Write(0x0, TObject    ::kOverwrite);
    if (genTuner->GetRatios())      genTuner->GetRatios()->Write(0x0, TObject     ::kOverwrite);
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
  AliInputEventHandler* esdH = new AliESDInputHandler;
  mgr->SetInputEventHandler(esdH);
  
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);

  trackCuts.SetIsMC(kTRUE);
  
  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");

  //__________Config. Task
  AliAnalysisTaskGenTuner* genTuner = AddTaskGenTuner();
  if(!genTuner) 
  {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTuner not created!");
    return 0x0;
  }
  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
  // genTuner->SelectCentrality(0., 90.);
  genTuner->SetMuonTrackCuts(trackCuts);

  //___________Set Bin
  // Open file
  TFile* dataFile = TFile::Open("../DataPart/AnalysisResultsReference.root","READ");
  if (!dataFile || !dataFile->IsOpen()) return;

  // Get HistoCollection
  AliMergeableCollection * oc = 0x0;
  dataFile->GetObject("OC",oc);
  if (!oc) return;

  // Get spectras
  AliAnalysisMuMuSpectra * spectraPT = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraPT)
  {
      cout << Form("Cannot find PT spectra in /%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
      return;
  }
  AliAnalysisMuMuSpectra * spectraY = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraY)
  {
      cout << Form("Cannot find Y spectra in /%s/%s/%s/%s/PSI-Y",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) <<endl;
      return;
  }

  Double_t* ptbin= spectraPT->Binning()->CreateBinArray();
  Double_t* ybin= spectraY->Binning()->CreateBinArray();

  Int_t ptnofbin= spectraPT->Binning()->GetNBinsX();
  Int_t ynofbin= spectraY->Binning()->GetNBinsX();

  //___________Set ref.Spectra
  // Pt spectra
  TH1* hpt= spectraPT->Plot("NofJPsi",subResultName,kTRUE);// new
  //Y spectra
  TH1* hy= spectraY->Plot("NofJPsi",subResultName,kTRUE);// new

  dataFile->Close();

  // cout << "ptnofbin =" << ptnofbin << endl;

  // for (int i = 0; i < ptnofbin+1; ++i)
  // {
  //   cout << "ptbin " << i << "=" << ptbin[i] << endl;
  // }

  // for (int i = 0; i < ynofbin+1; ++i)
  // {
  //   cout << "ybin " << i << "=" << ybin[i] << endl;
  // }

  genTuner->SetPtBin(ptnofbin+1,ptbin);
  genTuner->SetYBin(ynofbin+1,ybin);
  //___________

  if(hpt && hy)
  {
    genTuner->SetPtRefHisto(hpt);
    genTuner->SetYRefHisto(hy);
  }
  else 
  {
    cout << "Cannot set reference histo !" << endl;
    return;
  }
  //___________

  if (iStep == 0) // Set param. for fitting functions from this file
  {
    // set the generator parameters used in simulation
    genTuner->SetPtParam(oldPtParam, oldFixPtParam, newPtParam, newFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetYParam(oldYParam, oldFixYParam, newYParam, newFixYParam, yRange[0], yRange[1]); 
  } 
  else if (iStep > 0) // Set param. from precedent fitting results
  {
    // get the original generator parameters from first step if any
    //    TFile *inFile = TFile::Open("Results_step0.root","READ");
    //    if (inFile && inFile->IsOpen()) {
    //      TF1 *fOldPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFunc"));
    //      TF1 *fOldYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFunc"));
    //      if (fOldPtFunc && fOldYFunc) {
    // fOldPtFunc->GetParameters(oldPtParam);
    // fOldYFunc->GetParameters(oldYParam);
    //      }
    //      inFile->Close();
    //    }
    
    // get the new generator parameters from previous step if any and configure the tuner
    TString inFileName = Form("Results_step%d.root",iStep-1);
    inFile = TFile::Open(inFileName.Data(),"READ");
    if (inFile && inFile->IsOpen()) 
    { 
      cout << "I'm in the file !" << endl;
      TF1 *fNewPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncNew"));
      TF1 *fNewYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFuncNew"));
      TF1 *fPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFunc"));
      TF1 *fYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFunc"));

      if (fNewPtFunc && fNewYFunc && fPtFunc && fYFunc ) 
      {
      	genTuner->SetPtParam(oldPtParam, oldFixPtParam, fNewPtFunc->GetParameters(), newFixPtParam, fNewPtFunc->GetXmin(), fNewPtFunc->GetXmax());
      	genTuner->SetYParam(oldYParam, oldFixYParam, fNewYFunc->GetParameters(), newFixYParam, fNewYFunc->GetXmin(), fNewYFunc->GetXmax());
      }
      else
      {
        cout << "Cannot acces to fit functions " << endl;
      }
      inFile->Close();
    }
    // enable the weighing
    genTuner->Weight(kTRUE);
    //__________
    
  }
  
  return genTuner;
  
}

