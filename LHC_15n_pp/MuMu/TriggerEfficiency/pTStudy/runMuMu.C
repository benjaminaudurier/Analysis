///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///

//______________________________________________________________________________
AliAnalysisTask* runMuMu(TString runMode,
                        TString analysisMode,
                        TString inputName       = "Find;FileName=AliAOD.Muons.root;BasePath=/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tune3/CMUL7-B-NOPF-MUFAST/results/244340/*;Mode=cache;",
                        TString inputOptions    = "AOD,MC",
                        TString analysisOptions = "",
                        TString softVersions    = "aliphysics=vAN-20160530-1",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/TriggerEfficiency/pTStudy/AddTaskMuMu.C",". $ALICE_ROOT/include $ALICE_PHYSICS/include");

    // Grid conf. related
    TString outputdirGrid = "Analysis/LHC15n/MuMu/T0Custom/";
    if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdirGrid.Data());

    // Flag for MC
    Bool_t isMC = IsMC(inputOptions);

    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC) triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));

    // Load centrality task
    if(!analysisOptions.Contains("CENT")&& analysisMode.Contains("local") ){
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
        task->SetAlternateOADBforEstimators ("LHC15n");
    }

    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);

    delete triggers;
}
