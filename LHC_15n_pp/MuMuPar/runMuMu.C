///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///



//______________________________________________________________________________
AliAnalysisTask* runMuMu(TString runMode, 
                TString analysisMode,
                TString inputName       = "Find;BasePath=/alice/data/2015/LHC15n/000244340/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;",
                TString inputOptions    = "",
                TString analysisOptions = "",
                TString softVersions    = "aliphysics=vAN-20151115-1",
                TString taskOptions     = "" )
{
    // path for macro usefull for saf3
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C")); 
    
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "libPWGmuon.so","$ALICE_ROOT/include $ALICE_PHYSICS/include");
    
    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);
    
    
    // Fill the trigger list with desired trigger combinations (See on ALICE log book for denomination)
    //==============================================================================
    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC)
    {
        // pp trigger
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));// Dimuon
    }
  
    // Load task
    //==============================================================================
    TString sds(inputName);
    sds.ReplaceAll("/","-");
    // TString outputname = Form("%s.%s.MuMu.root",sds.Data(),analysisMode.Data()); // Create output name in case of no dataset selected
  	TString outputname = AliAnalysisManager::GetAnalysisManager()->GetCommonFileName();
    gROOT->LoadMacro("AddTaskMuMu.C");
    AddTaskMuMu(outputname.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);

    gSystem->Exec(Form("cp -f %s %s",outputname.Data(),sds.Data()));
     
   
    delete triggers;
}

