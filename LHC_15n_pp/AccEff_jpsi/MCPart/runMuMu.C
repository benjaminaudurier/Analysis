///
/// Example macro to run the AliAnalysisTaskMuMu task with help of runTaskUtilities.C
///
/// \author Benjamin Audurier
///


//test data : Find;BasePath=/alice/data/2015/LHC15o/000244918/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;
//test MC : Find;FileName=AliAOD.Muons.root;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tuned/CMUL7-B-NOPF-MUFAST/244340/*;

//______________________________________________________________________________
AliAnalysisTask* runMuMu(TString runMode, 
                TString analysisMode,
                TString inputName       = "Find;FileName=AliAOD.Muons.root;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tuned/CMUL7-B-NOPF-MUFAST/244340/*",
                TString inputOptions    = "MC",
                TString analysisOptions = "NOCENTR",
                TString softVersions    = "",
                TString taskOptions     = "" )
{
    // path for macro usefull for saf3
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));
    
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "libPWGPPMUONlite.so","$ALICE_ROOT/include $ALICE_PHYSICS/include");
    
    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);
    
    
    // Fill the trigger list with desired trigger combinations (See on ALICE log book for denomination)
    //==============================================================================
    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC)
    {
        // pA trigger
        // triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));// MB
        // triggers->Add(new TObjString("CINT7-B-NOPF-CENT"));// MB
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));// Dimuon
    }
    // Load centrality task
    //==============================================================================
    gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
    AliMultSelectionTask *mult = AddTaskMultSelection(kFALSE);
    if(analysisMode.Contains("local")) mult->SetAlternateOADBforEstimators("LHC15n"); // if running locally

  
    // Load task
    //==============================================================================
  	TString outputname = AliAnalysisManager::GetAnalysisManager()->GetCommonFileName();
    gROOT->LoadMacro("AddTaskMuMu.C");
    AddTaskMuMu(outputname.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
  
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);     
   
    delete triggers;
}

