/*
 *  runJPsiAccEffCorr.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 06/03/13.
 *  Copyright 2011 SUBATECH. All rights reserved.
 *
 */
#include <iostream>
#include <vector>

TString rootVersion = "v5-34-30-alice-22";
TString aliphysicsVersion = "vAN-20160414-1";
TString dataDir = "";
TString dataPattern = "";
TString runFormat = "%09d";
TString outDir = "";
Int_t ttl = 30000;
Int_t maxFilesPerJob = 100;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 2;


Bool_t splitDataset =kFALSE;

// tune0 LHC15n jpsi
// TString oldPtFormula =   "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
// Double_t oldPtParam[4] = {0.32231,0.95996,0.0665176,3.70602};
// Bool_t oldFixPtParam[4] = {kFALSE,kFALSE,kFALSE,kFALSE};

// tune1 LHC15n jpsi
// TString oldPtFormula =   "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
// Double_t oldPtParam[4] = {21770.6,15.2993,1.97656,4.01293};
// Bool_t oldFixPtParam[4] = {kFALSE,kFALSE,kFALSE,kFALSE};

// tune2 LHC15n jpsi
TString oldPtFormula =   "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
Double_t oldPtParam[4] =   {4654.3, 12.8133, 1.9647, 3.66641};
Bool_t oldFixPtParam[4] = {kTRUE,kTRUE,kTRUE,kTRUE};


TString newPtFormula = "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
Double_t newPtParam[4] = {4654.3, 12.8133, 1.9647, 3.66641};
Bool_t newFixPtParam[4] = {kTRUE,kTRUE,kTRUE,kTRUE};

Double_t ptRange[2] = {0., 50.};

// tune0 LHC15n jpsi
// TString oldYFormula = " [0] * ( 1 + [1]*x*x ) ";
// Double_t oldYParam[2] = {1.67011e+05, -3.22009e-02};
// Bool_t oldFixYParam[2] = {kFALSE, kFALSE};

// tune1 LHC15n jpsi
// TString oldYFormula = " [0] * ( 1 + [1]*x*x ) ";
// Double_t oldYParam[2] = {1.19752, -0.0412364};
// Bool_t oldFixYParam[2] = {kFALSE, kFALSE};

// tune2 LHC15n jpsi
TString oldYFormula = " [0] * ( 1 + [1]*x*x ) ";
Double_t oldYParam[2] = {1.18296, -0.0405994};
Bool_t oldFixYParam[2] = {kFALSE, kFALSE};

TString newYFormula = "[0] * ( 1 + [1]*x*x)";
Double_t newYParam[2] =  {1.18296, -0.0405994};
Bool_t newFixYParam[2] = {kFALSE, kFALSE};

// TString newYFormula = "[0]*TMath::Exp(-(1./2.)*TMath::Power(((x-[1])/[2]),2))";
// Double_t newYParam[3] =  {1.6,0.,2.};
// Bool_t newFixYParam[3] = {kFALSE, kFALSE,kFALSE};

Double_t yRange[2] = {-4., -2.5};

Bool_t isMC = kTRUE;
Bool_t applyPhysicsSelection = kFALSE;

//For Y projection
Int_t firstybin=lastybin=-1; // Projection bin in rapidity. No projection if =-1
// For pT Projection
Int_t firstxbin=lastxbin=2;


void UpdateParametersAndRanges(Int_t iStep);

//__________Setting for spectra path
TString striggerDimuon  ="CMUL7-B-NOPF-MUFAST";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETAPDCA";
TString scentrality     ="PP";
TString subResultName   = "";/*"YVSPT_BENJ_02.00_04.00_m03.50_m03.00";*/
//__________


//______________________________________________________________________________
void runGenTuner(TString smode = "local", TString inputFileName = "AliAOD.root",
     Int_t iStep = 0, char overwrite = '\0')
{
  /// Tune single muon kinematics distribution

  gROOT->LoadMacro("$HOME/Documents/PhilippeGitHub/Facilities/runTaskFacilities.C");

  // --- Check runing mode ---
  Int_t mode = GetMode(smode, inputFileName);
  if(mode < 0) {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
  }

  // --- copy files needed for this analysis ---
  TList pathList; pathList.SetOwner();
  pathList.Add(new TObjString("$HOME/Documents/Analysis/Tasks"));
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString("AliAnalysisTaskGenTunerJpsi.cxx"));
  fileList.Add(new TObjString("AliAnalysisTaskGenTunerJpsi.h"));
  CopyFileLocally(pathList, fileList, overwrite);
  fileList.Add(new TObjString("runGenTuner.C"));
  fileList.Add(new TObjString("AddTaskGenTuner.C"));
  CopyInputFileLocally("/Users/audurier/Documents/Analysis/LHC_15n_pp/AccEff_jpsi/DataPart/AnalysisResults.root", "AnalysisResultsReference.root", overwrite);
  fileList.Add(new TObjString("AnalysisResultsReference.root"));

  // --- saf3 case ---
  if (mode == kSAF3Connect) {
    // run on SAF3
    if (!RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName, splitDataset)) return;
    // draw the results locally
    outFile = TFile::Open(Form("Results_step%d.root",iStep),"READ");
    if (outFile && outFile->IsOpen()) {
      outFile->FindObjectAny("cRes")->Draw();
      outFile->FindObjectAny("cRat")->Draw();
      outFile->Close();
    }
    // do not try to re-run locally!
    return;
  }

  // --- prepare environment ---
  TString extraLibs="";
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
  if (iStep >= 0) gSystem->Exec(Form("cp -f %s Results_step%d.root", outFileName.Data(),iStep));
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
  // if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
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
  AliAnalysisMuMuSpectra * spectraYVSPT =0x0;
  AliAnalysisMuMuSpectra * spectraPT =0x0;
  AliAnalysisMuMuSpectra * spectraY =0x0;
  Double_t* ptbin;
  Double_t* ybin;
  vector<double> ptBin;
  vector<double> yBin;
  Int_t ptnofbin;
  Int_t ynofbin;

  // // Select spectra according to bin type
  spectraYVSPT = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraYVSPT)cout << Form("Cannot find YVSPT spectra in /%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
  spectraPT = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraPT)cout << Form("Cannot find PT spectra in /%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
  spectraY = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-Y",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
  if(!spectraY)cout << Form("Cannot find Y spectra in /%s/%s/%s/%s/PSI-Y",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;

  // Configure task for 1D bin
  if(spectraPT && spectraY ){
    ptbin    = spectraPT->Binning()->CreateBinArrayX();
    ybin     = spectraY->Binning()->CreateBinArrayX();
    ptnofbin = spectraPT->Binning()->GetNBinsX();
    ynofbin  = spectraY->Binning()->GetNBinsX();

    TH1* hpt= spectraPT->Plot("NofJPsi",subResultName,kTRUE);// new
    TH1* hy= spectraY->Plot("NofJPsi",subResultName,kTRUE);// new
    dataFile->Close();

    new TCanvas;
    hy->DrawCopy();
    hpt->DrawCopy();
    return;

    if(hpt && hy && ptnofbin>0 && ynofbin>0 ){
        genTuner->SetPtRefHisto(hpt);
        genTuner->SetYRefHisto(hy);
        genTuner->SetPtBin(ptnofbin+1,ptbin);
        genTuner->SetYBin(ynofbin+1,ybin);
        printf("here \n");
    } else {
        cout << "Cannot set reference histo !" << endl;
        return;
    }

  // Configure task for 2D bin
  } else if (spectraYVSPT) {
    TH2* hYVSPT = static_cast<TH2*>(spectraYVSPT->Plot("NofJPsi",subResultName,kTRUE));// new Get results as TH2D
    if(!hYVSPT){
      cout << "Cannot get hYVSPT" << endl;
      return;
    }
    dataFile->Close();

    TH1* hpt = (TH1*)hYVSPT->ProjectionX("_px",firstybin,lastybin);// pT results with projection along the y axis
    TH1* hy  = (TH1*)hYVSPT->ProjectionY("_py",firstxbin,lastxbin);// y results with projection along the pT axis

    // new TCanvas;
    // hpt->DrawCopy();
    // new TCanvas;
    // hy->DrawCopy();
    // return;

    // Get bin w/o projection along the x axis
    if(lastxbin<0){ 
      ptbin = spectraYVSPT->Binning()->CreateBinArrayX();
      ptnofbin = spectraYVSPT->Binning()->GetNBinsX();
    // Get bin w/ projection along the x axis
    } else { 
      if(firstxbin < 1 ){printf("firstxbin is null \n"); return;};
      if(firstxbin > hpt->GetXaxis()->GetNbins() ){printf("bin does not exist \n"); return;};

      ptnofbin = lastxbin - firstxbin;
      if( ptnofbin>hpt->GetXaxis()->GetNbins()) ptnofbin = hpt->GetXaxis()->GetNbins() -1;

      ptBin.push_back(hYVSPT->GetXaxis()->GetBinLowEdge(firstxbin));
      ptBin.push_back(hYVSPT->GetXaxis()->GetBinUpEdge(lastxbin));
    }

    // Get bin w/o projection along the y axis
    if(lastybin<0){
      ybin    = spectraYVSPT->Binning()->CreateBinArrayY();
      ynofbin = spectraYVSPT->Binning()->GetNBinsY();
    // Get bin w/ projection along the y axis
    } else { 
      if(firstybin < 1 ){printf("firstybin is null \n"); return;};
      if(firstybin > hy->GetXaxis()->GetNbins()){printf("bin does not exist \n"); return;};

      ynofbin = lastybin-firstybin;
      if( ynofbin>hy->GetXaxis()->GetNbins()) ynofbin = hy->GetXaxis()->GetNbins() -1;

      yBin.push_back(hYVSPT->GetYaxis()->GetBinLowEdge(firstybin));
      yBin.push_back(hYVSPT->GetYaxis()->GetBinUpEdge(firstybin));
    }

    // Set binning
    if (ptbin) genTuner->SetPtBin(ptnofbin+1,ptbin);
    if (ybin) genTuner->SetYBin(ynofbin+1,ybin);
    if (ptBin.size()!=0) genTuner->SetPtBin(ptnofbin+1,ptBin);
    if (yBin.size()!=0) genTuner->SetYBin(ynofbin+1,yBin);

    //Case without projection
    if(hpt && hy && ptBin.size() == 0 && yBin.size() == 0){ 
      genTuner->SetPtRefHisto(hpt);
      genTuner->SetYRefHisto(hy);
    
    // Create and fill new histo proj histo
    } else if ( hy && ptBin.size()>0 ) {
      
      TH1D* hptproj = new TH1D(Form("%s_proj",hpt->GetName()),hpt->GetTitle(),ptnofbin+1,ptBin[0],ptBin[1]);
      for (int i = 0; i < ptnofbin+1; ++i) {
        hptproj->SetBinContent(firstxbin,hpt->GetBinContent(firstxbin+i));
        hptproj->SetBinError(firstxbin,hpt->GetBinError(firstxbin+i));
      }
      // hptproj->DrawClone();
      // return;

      if(hptproj) genTuner->SetPtRefHisto(hptproj);
      genTuner->SetYRefHisto(hy);
      delete hptproj;
    }
    else if ( hpt && yBin.size()>0 ) {
      // Create and fill new histo proj histo
      TH1D* hyproj = new TH1D(Form("%s_proj",hy->GetName()),hy->GetTitle(),ynofbin+1,yBin[0],yBin[1]);
      for (int i = 0; i < ynofbin+1; ++i) {
        hyproj->SetBinContent(1+i,hy->GetBinContent(firstybin+i));
        hyproj->SetBinError(1+i,hy->GetBinError(firstybin+i));
      }
      // hyproj->DrawClone();

      genTuner->SetPtRefHisto(hpt);
      genTuner->SetYRefHisto(hyproj);
      delete hyproj;
    } else {
      cout << "Cannot set reference histo !" << endl;
      return;
    }

  }
  else return;

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

  if (iStep < 1) return;

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

  if ((fNewPtFunc->GetNpar() != (Int_t)(sizeof(newPtParam)/sizeof(Double_t))) || (fNewYFunc->GetNpar() != (Int_t)(sizeof(newYParam)/sizeof(Double_t)))) {
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
