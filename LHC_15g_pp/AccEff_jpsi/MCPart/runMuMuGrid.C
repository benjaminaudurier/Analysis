///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author L. Aphecetche
///

//__________Global Settings
TString rootVersion = "v5-34-30-1";
TString alirootVersion = "v5-06-33";
TString aliphysicsVersion = "vAN-20150722";
TString dataDir = "/alice/data/2015/LHC15g";
TString dataPattern = "muon_calo_pass1/*AliAOD.Muons.root";
TString runFormat = "%09d";
TString outDir = "Analysis/LHC15g/AccEff/Data";

// extra to be load on CF
TString extraLibs="";
TString extraIncs="";
TString extraTasks="";

Bool_t applyPhysicsSelection = kFALSE;

//__________Config. Param. for grid macro.
Int_t ttl = 30000;
Int_t maxFilesPerJob = 100;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 2;
//__________

//__________

//______________________________________________________________________________
AliAnalysisTask* runMuMuGrid(const char* dataset="",
                         Bool_t simulations=kTRUE,
                         Bool_t baseline=kFALSE,
                         TString where="full",const char* alirootMode="")
{
  gROOT->LoadMacro("/Users/audurier/Documents/Analysis/Macro_Utile/runTaskFacilities.C");// where are macros to load on grid, caf etc.

  TString sds(dataset); // To work with dataset

  // --- Check runing mode ---
  
  if(sds.Length()>0) 
  {
    Int_t mode = GetMode(where, sds);
  
    if(mode < 0) 
    {
    Error("runGenTuner","Please provide either an AOD root file a collection of AODs or a dataset.");
    return;
    }
  }
  
  // --- prepare environment ---
  LoadAlirootLocally(extraLibs, extraIncs, extraTasks);
  
  // Choose the CF
  AliAnalysisGrid *alienHandler = 0x0;
  if(sds.Length()>0 )
  {
    
    if (mode == kProof || mode == kProofLite) LoadAlirootOnProof(where,rootVersion,aliphysicsVersion,extraLibs, extraIncs, extraTasks, kFALSE, alirootMode); // Macro to connect to proof
    else if (mode == kGrid || mode == kTerminate) 
    {
        TString analysisMacroName = "MuMu";
        alienHandler = static_cast<AliAnalysisGrid*>(CreateAlienHandler(where, rootVersion, alirootVersion, aliphysicsVersion, dataset, dataDir, dataPattern, outDir, extraLibs, extraIncs, extraTasks, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages));
        if (!alienHandler) return;
    }
  }
  
  LoadLocalLibs(kFALSE); // Macro to load Local Libraries

    // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("MuMu");
  
  // Connect plugin to the analysis manager if any
  if (alienHandler) mgr->SetGridHandler(static_cast<AliAnalysisGrid*>(alienHandler));

  TString inputType = GetInputType(sds); // Get input type via macro GetInputType
    
  // Checkout input and select the correct input handler
  //==============================================================================
  if ( inputType == "AOD" )
  {
    input = new AliAODInputHandler;
  }
  else if ( inputType == "ESD" )
  {
    input = new AliESDInputHandler;
  }
  else
  {
    std::cout << "Cannot get input type !" << std::endl;
    return 0;
  }

  mgr->SetInputEventHandler(input); // Set the input handler

  TList* triggers = new TList; // Create pointer for trigger list
  triggers->SetOwner(kTRUE); // Give rights to trigger liser
    

    
  // Fill the trigger list with desired trigger combinations (See on ALICE log book for denomination)
  //==============================================================================
  if (!simulations)
  {

    triggers->Add(new TObjString("CINT7-B-NOPF-MUON|CINT7-B-NOPF-CENTNOTRD"));// MB
    triggers->Add(new TObjString("CMUL7-B-NOPF-MUON"));// Dimuon
  }

  TString outputname("AnalysisResultsReference.root"); // Create output name in case of no dataset selected 

  // Loop to configure the .root output file's name
  //==============================================================================
  if ( sds.Length()>0  && mode != kGrid && mode != kTerminate)
  {
    TString af("local");
  
    if ( gProof )
    {
      af="unknown";
      TString master(gProof->GetSessionTag());
      if (master.Contains("lx")) af = "caf";
      if (master.Contains("nansaf")) af = "saf";
      if (master.Contains("skaf")) af = "skaf";
      if (master.Contains("localhost:2093")) af="laf";
    }
    outputname = Form("%s.%s.root",gSystem->BaseName(sds.Data()),af.Data());
    outputname.ReplaceAll("|","-");
    cout << outputname << endl;
  } 
  
  AliAnalysisTask* task(0x0); // Create pointer for task

  // Load the right task depending on 'baseline'
  //==============================================================================
  if (!baseline)
  {
    gROOT->LoadMacro("AddTaskMuMu.C");
    task = AddTaskMuMu(outputname.Data(),triggers,"PbPb2011",simulations);
  }
  else
  {
	  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddTaskBaseLine.C");
	  task = AddTaskBaseLine();
  }

  // Checkpoint
  //==============================================================================
  if (!mgr->InitAnalysis())
  {
	  cout << "Could not InitAnalysis" << endl;
    return 0;
  }

  // Start analysis
  //==============================================================================
  if ( sds.Length()>0 )
  {
    TStopwatch timer;

    // --- Create input object ---
    TObject* inputObj = CreateInputObject(mode, sds);
    
    // --- start analysis ---
    StartAnalysis(mode,inputObj);
    timer.Print();
  }
  else
  {
    mgr->PrintStatus();
 	  task->Print();
//	return task;
    TChain* c = CreateLocalChain("list.aod.txt");
//   	mgr->SetNSysInfo(10);
    TStopwatch timer;
//    mgr->SetDebugLevel(10);
    mgr->StartAnalysis("local",c);
    timer.Print();
//    mgr->ProfileTask("AliAnalysisTaskMuMu");
//    if (baseline) mgr->ProfileTask("baseline");
  }
  
  AliCodeTimer::Instance()->Print();// Timer

  // Congifure in case of PAR file
  //==============================================================================
  if (alirootMode=="PAR")
  {
      TProofLog *pl = TProof::Mgr(where)->GetSessionLogs(); pl->Save("*","aod.log");
  }

  delete triggers;
}

//______________________________________________________________________________
TChain* CreateLocalChain(const char* filelist)
{
    TChain* c = new TChain("aodTree");
    char line[1024];
    
    ifstream in(filelist);
    while ( in.getline(line,1024,'\n') )
        {
        c->Add(line);
        }
    return c;
}




