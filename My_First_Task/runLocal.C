
//______________________________________________________________________________
//MAIN TASK
//______________________________________________________________________________

void runLocal(TString runMode, 
              TString analysisMode,
              TString inputName       = "Find;BasePath=/alice/data/2013/LHC13d/000195682/ESDs/muon_pass2/AOD134;FileName=AliAOD.root;Mode=cache;",
              TString inputOptions    = "",
              TString analysisOptions = "",
              TString softVersions    = "aliphysics=vAN-20151115-1",
              TString taskOptions     = "")
{
  // --- Create the analysis train ---
  gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

  //--- Create input object ---
  SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "AliAnalysisTaskMass.cxx",". $ALICE_ROOT/include $ALICE_PHYSICS/include");

  //--- Set analysis ---
  CreateAnalysisTrain();

  // --- start analysis ---
  StartAnalysis(runMode,analysisMode,inputName,inputOptions);

}
//______________________________________________________________________________
//______________________________________________________________________________



//______________________________________________________________________________
// ANALYSIS TRAIN
//______________________________________________________________________________

void CreateAnalysisTrain()
{
  /// create the analysis train and configure it
  
 // Create the analysis manager
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  
  // Create tasks
  gROOT->LoadMacro("AliAnalysisTaskMass.cxx+g");

  // Add task(s)
  AliAnalysisTaskMass *taskmass = new AliAnalysisTaskMass("TaskMass");
  mgr->AddTask(taskmass);
  if(!taskmass) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMass not created");
    return;
  }
  TString outputname = AliAnalysisManager::GetAnalysisManager()->GetCommonFileName();
  
  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer(); // For Invariant Mass
  AliAnalysisDataContainer *coutputmass = mgr->CreateContainer("histmass", TList::Class(),
  AliAnalysisManager::kOutputContainer, outputname); // For Invariant Mass

  // Connect input/output
  mgr->ConnectInput(taskmass, 0, cinput);
  mgr->ConnectOutput(taskmass, 1, coutputmass);

    
}
//______________________________________________________________________________
//______________________________________________________________________________

