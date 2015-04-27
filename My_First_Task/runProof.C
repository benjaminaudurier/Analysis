void runProof()
{

    // --- Load and set Aliroot ---

    LoadAlirootOnProof("saf", "v5-34-08", "vAN-20140923", "CORRFW:PWGmuon","include:PWG/muon","AliAnalysisTaskMass",kFALSE);

    
    // --- Create the analysis train ---
    CreateAnalysisTrain();


    // --- start analysis ---
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr->InitAnalysis()) return;
    mgr->PrintStatus();
    mgr->StartAnalysis("proof","Find;FileName=AliAOD.Muons.root;BasePath=/alice/data/2011/LHC11h/000170040/ESDs/pass2_muon/AOD119");
    
    printf("!!!! ANALYSIS DONE !!!!!\n");

}



//______________________________________________________________________________
//______________________________________________________________________________
// MACRO FOR PROOF


void LoadAlirootOnProof(TString& aaf, TString rootVersion, TString alirootVersion, TString& extraLibs,
                        TString& extraIncs, TString& extraTasks, Bool_t notOnClient = kFALSE, TString alirootMode = "")
{
    // Load aliroot packages and set environment on Proof
    
    
    // set general environment
    gEnv->SetValue("XSec.GSI.DelegProxy","2");
    
    
    // connect
    // if (aaf == "prooflite") TProof::Open("");
    if (aaf == "prooflite") TProof::Open("workers=2");
    else {
        TString location, nWorkers;
        if (aaf == "caf") {
            location = "alice-caf.cern.ch";
            nWorkers = ""; // "workers=40";
        } else if (aaf == "saf") {
            location = "audurier@nansafmaster2.in2p3.fr"; // "localhost:1093"
            nWorkers = ""; // "workers=8x";
        } else return;
        TString user = (gSystem->Getenv("alien_API_USER") == NULL) ? "" : Form("%s@",gSystem->Getenv("alien_API_USER"));
        TProof::Mgr(Form("%s%s",user.Data(), location.Data()))->SetROOTVersion(Form("VO_ALICE@ROOT::%s",rootVersion.Data()));
        TProof::Open(Form("%s%s/?N",user.Data(), location.Data()), nWorkers.Data());
    }
    if (!gProof) return;
    
    
    // set environment and load libraries on workers
    TList* list = new TList();
    list->Add(new TNamed("ALIROOT_MODE", alirootMode.Data()));
    list->Add(new TNamed("ALIROOT_EXTRA_LIBS", extraLibs.Data()));
    list->Add(new TNamed("ALIROOT_EXTRA_INCLUDES", extraIncs.Data()));
    if (aaf == "prooflite") {
        gProof->UploadPackage("$ALICE_ROOT/ANALYSIS/macros/AliRootProofLite.par");
        gProof->EnablePackage("$ALICE_ROOT/ANALYSIS/macros/AliRootProofLite.par", list);
    } else gProof->EnablePackage(Form("VO_ALICE@AliRoot::%s",alirootVersion.Data()), list, notOnClient);
    
    
    // compile additional tasks on workers. To add tasks, use ":" (see in th emain loop)
    TObjArray* tasks = extraTasks.Tokenize(":");
    for (Int_t i = 0; i < tasks->GetEntriesFast(); i++)
        gProof->Load(Form("%s.cxx++g",static_cast<TObjString*>(tasks->UncheckedAt(i))->GetName()), notOnClient);
    delete tasks;
    
    
}


//______________________________________________________________________________
//______________________________________________________________________________
// MACRO FOR ANALYSIS TRAIN


void CreateAnalysisTrain()
{
  /// create the analysis train and configure it
  
 // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");


  // Create input handler
  AliAODInputHandler* aodH = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodH);
  

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
//    Error("CreateAnalysisTrain","AliAnalysisTaskPt not created");
//    return;
//  }
//  

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer(); // For Invariant Mass
//  AliAnalysisDataContainer *cinputpt = mgr->GetCommonInputContainer(); // For Pt
  


  AliAnalysisDataContainer *coutputmass = mgr->CreateContainer("chistmass", TList::Class(),
  AliAnalysisManager::kOutputContainer, "Mass.AOD.2.root"); // For Invariant Mass

//  AliAnalysisDataContainer *coutputpt = mgr->CreateContainer("chistpt", TList::Class(),
//  AliAnalysisManager::kOutputContainer, "pt.AOD.2.root"); // For Pt

  // Connect input/output
  mgr->ConnectInput(taskmass, 0, cinput);
//  mgr->ConnectInput(taskpt, 0, cinputpt);


  // No need to connect to a common AOD output container if the task does not
  // fill AOD info.

  mgr->ConnectOutput(taskmass, 1, coutputmass);
//  mgr->ConnectOutput(taskpt, 1, coutputpt);




  // Enable debug printouts
  mgr->SetDebugLevel(2);
}



