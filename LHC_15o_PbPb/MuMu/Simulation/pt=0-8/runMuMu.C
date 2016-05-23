///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///

//______________________________________________________________________________
AliAnalysisTask* runMuMu(TString runMode, 
                        TString analysisMode,
                        TString inputName       = "Find;FileName=AliAOD.Muons.root;BasePath=/alice/sim/2016/LHC16b1/244918/p80/*;",
                        TString inputOptions    = "EMBED,AOD",
                        TString analysisOptions = "",
                        TString softVersions    = "",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));     
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Simulation/pt=0-8/AddTaskMuMu.C",". $ALICE_ROOT/include $ALICE_PHYSICS/include");    
    TString outputdir = "Analysis/LHC15o/MuMuPar/";
    if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdir.Data());

    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);

    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    
    if ( (isMC && inputOptions.Contains("EMBED")) || (!isMC && inputOptions.Contains("")) ) {
        triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));// Dimuon
    }
    
    // Load centrality task
    //==============================================================================
    // if(analysisOptions.Contains("NOPHYSSEL")&& analysisMode.Contains("local") ) {
    //     gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
    //     AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
    //     task -> SetAlternateOADBforEstimators ("LHC15o");
    // }

    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"PbPb2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);     
   
    delete triggers;
}

