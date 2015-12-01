/*
 *  runJPsiAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */

// --- prepare environment ---
TString extraTasks="AliAnalysisTaskGenTunerLocal";
Bool_t applyPhysicsSelection = kFALSE;

//__________generator parameters used in the simulation
// tune1 LHC13d pt param. (see AliAnalysisTaskGenTunerJpsi::Pt)
Double_t oldPtParam[3] = {0.288315, 0.0593389, 3.42939};
Bool_t oldFixPtParam[3] = {kFALSE, kFALSE, kFALSE};
Double_t newPtParam[3] = {0.288315, 0.0593389, 3.42939};
Bool_t newFixPtParam[3] = {kFALSE, kFALSE, kFALSE};

Double_t ptRange[2] = {0.1, 6.5};

// tune1 LHC13d y param. (see AliAnalysisTaskGenTunerJpsi::Y)
Double_t oldYParam[2] = {1.50913, 0.171768};
Bool_t oldFixYParam[2] = {kFALSE, kFALSE};
Double_t newYParam[2] =  {1.50913, 0.171768};
Bool_t newFixYParam[2] = {kFALSE, kFALSE};

Double_t yRange[2] = {-3.8, -2.3};
//__________

//__________Setting for spectra path
TString striggerDimuon  ="CMUL7-B-NOPF-MUON";
TString seventType      ="ALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETA";
TString scentrality     ="V0A";
TString subResultName   = ""/*"YVSPT_BENJ_02.00_04.00_m03.50_m03.00";*/
//__________

// Sim :
// Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root
// 
// Data:
// Find;BasePath=/alice/data/2013/LHC13d/000195760/ESDs/muon_pass2/AOD134;FileName=AliAOD.root

 

//______________________________________________________________________________
void runGenTuner(TString runMode, TString analysisMode,
TString inputName       = "Find;BasePath=/alice/data/2013/LHC13d/000195682/ESDs/muon_pass2/AOD134;FileName=AliAOD.root;Mode=cache",
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
    
  // --- Create the analysis train ---
  AliAnalysisTaskGenTunerJpsi *genTuner = static_cast<AliAnalysisTaskGenTunerJpsi*>(CreateAnalysisTrain(iStep,isMC));
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
}

//______________________________________________________________________________
TObject* CreateAnalysisTrain(Int_t iStep , Bool_t mc )
{
  /// create the analysis train and configure it
  
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager(); 
  
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
			  AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);

  //Flag for MC
  trackCuts.SetIsMC(mc);
  
  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");

  //__________Config. Task
  AliAnalysisTaskGenTunerJpsi* genTuner = AddTaskGenTuner();
  if(!genTuner) 
  {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTunerJpsi not created!");
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

  new TCanvas;
  hpt->Draw("");
  new TCanvas;
  hy->Draw("");
  return;

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

