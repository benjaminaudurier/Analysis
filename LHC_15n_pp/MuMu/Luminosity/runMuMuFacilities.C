///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///

//______________________________________________________________________________
void runMuMuFacilities(TString smode = "local", TString inputFileName = "AliAOD.root", Bool_t isMC = kFALSE)
{
  /// Fill counters with triggered events
  
  // --- general analysis setup ---
  TString rootVersion = "";
  TString alirootVersion = "";
  TString aliphysicsVersion = "vAN-20160601-1";
  TString extraLibs="";
  TString extraIncs="include";
  TString extraTasks="";
  TString extraPkgs="PWGmuon";
  TList pathList; pathList.SetOwner();
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString("runMuMuFacilities.C"));
  fileList.Add(new TObjString("AddTaskMuMu.C"));
  fileList.Add(new TObjString("PWGmuon.par"));
  
  // --- grid specific setup ---
  TString dataDir = "/alice/cern.ch/user/p/ppillot/Sim/LHC15n/JPsiTune1/VtxShift/results";
  TString dataPattern = "*AliAOD.Muons.root";
  TString runFormat = "%d";
  TString outDir = "Sim/LHC15n/JPsiTune1/VtxShift/AccEff";
  TString analysisMacroName = "TrgCount";
  Int_t ttl = 30000;
  Int_t maxFilesPerJob = 20;
  Int_t maxMergeFiles = 10;
  Int_t maxMergeStages = 2;
  
  // --- saf3 specific setup ---
  Bool_t splitDataset = kFALSE;
  
  gROOT->LoadMacro("$HOME/Documents/PhilippeGitHub/Facilities/runTaskFacilities.C");
  
  // --- prepare the analysis environment ---
  Int_t mode = PrepareAnalysis(smode, inputFileName, extraLibs, extraIncs, extraTasks, extraPkgs, pathList, fileList);
  
  // --- run the analysis (saf3 is a special case as the analysis is launched on the server) ---
  if (mode == kSAF3Connect) {
    
    RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName, splitDataset);
    
  } else {
    
    CreateAnalysisTrain(isMC);
    
    if (smode == "saf3" && splitDataset) AliAnalysisManager::GetAnalysisManager()->SetSkipTerminate(kTRUE);
    
    RunAnalysis(smode, inputFileName, rootVersion, alirootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, dataDir, dataPattern, outDir, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages);
    
  }
  
}

//______________________________________________________________________________
void CreateAnalysisTrain(Bool_t isMC)
{
  /// create the analysis train and configure it
  
  // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("MuMuAnalysis");
  
  // AOD handler
  AliInputEventHandler* aodH = new AliAODInputHandler;
  mgr->SetInputEventHandler(aodH);
  /*
  // Load centrality task
  if(!analysisOptions.Contains("CENT")&& analysisMode.Contains("local") ){
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
    AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
    task->SetAlternateOADBforEstimators("LHC15n");
  }
  */
  TList* triggers = new TList; // Create pointer for trigger list
  triggers->SetOwner(kTRUE); // Give rights to trigger liser
  if (!isMC){  
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));
        // V0 Trigger
        triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));
        triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST&0MUL"));
        triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST&0MSL"));
        // T0 Trigger
        // triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST&0TVX"));
        // triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST&0TVX&0MUL"));
        // triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST&0TVX&0MSL"));
        //For double ratio FNorm
        triggers->Add(new TObjString("CMSL7-B-NOPF-MUFAST&0MUL"));
        triggers->Add(new TObjString("CMSL7-B-NOPF-MUFAST"));

    }
  
  TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
  gROOT->LoadMacro("AddTaskMuMu.C");
  AddTaskMuMu(output.Data(),triggers,"pp2015",isMC);
  
}

