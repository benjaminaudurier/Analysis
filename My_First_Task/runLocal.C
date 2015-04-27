
//______________________________________________________________________________
//MAIN TASK
//______________________________________________________________________________

void runLocal()
{

    
//FOR ROOT

    gSystem->Load("libVMC");
    gSystem->Load("libMinuit");
    gSystem->Load("libTree");
    gSystem->Load("libProofPlayer");
    gSystem->Load("libXMLParser");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libSTEER");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libCORRFW");
    gSystem->Load("libPWGmuon");



//  // load analysis framework
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libCORRFW.so");
    gSystem->Load("libPWGmuon.so");

    gSystem->AddIncludePath("-I$ALICE_ROOT/include");



  // --- Create the analysis train ---
  CreateAnalysisTrain();


  //--- Create input object ---

    TChain* chain = new TChain("aodTree");
    chain->Add("AliAOD.Muons.root");



  // --- start analysis ---
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (mgr->InitAnalysis()) {
    mgr->PrintStatus();
    mgr->StartAnalysis("local", chain);
  }
    printf("!!!! ANALYSIS DONE !!!!!\n");

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
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");


  // Create input handler
  AliAODInputHandler* aodH = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodH);
  
  // Create tasks
  gROOT->LoadMacro("AliAnalysisTaskMass.cxx+g");
//  gROOT->LoadMacro("AliAnalysisTaskPt.cxx+g");


  // Add task(s)

  // -> Invariant Mass Task
  AliAnalysisTaskMass *taskmass = new AliAnalysisTaskMass("TaskMass");
  mgr->AddTask(taskmass);
  if(!taskmass) {
    Error("CreateAnalysisTrain","AliAnalysisTaskMass not created");
    return;
  }

//  // -> Pt Task
//  AliAnalysisTaskPt *taskpt = new AliAnalysisTaskPt("TaskPt");
//  mgr->AddTask(taskpt);
//  if(!taskpt) {
//    Error("CreateAnalysisTrain","AliAnalysisTaskMass not created");
//    return;
//  }
//  

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer(); // For Invariant Mass
//  AliAnalysisDataContainer *cinputpt = mgr->GetCommonInputContainer(); // For Pt
  


  AliAnalysisDataContainer *coutputmass = mgr->CreateContainer("chistmass", TList::Class(),
  AliAnalysisManager::kOutputContainer, "Mass.AOD.1.root"); // For Invariant Mass

//  AliAnalysisDataContainer *coutputpt = mgr->CreateContainer("chistpt", TList::Class(),
//  AliAnalysisManager::kOutputContainer, "pt.AOD.1.root"); // For Pt

  // Connect input/output
  mgr->ConnectInput(taskmass, 0, cinput);
//  mgr->ConnectInput(taskpt, 0, cinputpt);


  // No need to connect to a common AOD output container if the task does not
  // fill AOD info.

  mgr->ConnectOutput(taskmass, 1, coutputmass);
//  mgr->ConnectOutput(taskpt, 1, coutputpt);




  // Enable debug printouts
  mgr->SetDebugLevel(1);
    
    
    
}
//______________________________________________________________________________
//______________________________________________________________________________

