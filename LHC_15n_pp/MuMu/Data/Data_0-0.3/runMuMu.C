///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///


// Sim :
// Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root
//
// Data:
// Find;BasePath=/alice/data/2013/LHC13d/000195760/ESDs/muon_pass2/AOD134;FileName=AliAOD.root


//______________________________________________________________________________
AliAnalysisTask* runMuMu(TString runMode,
                        TString analysisMode,
                        TString inputName       = "Find;BasePath=/alice/data/2015/LHC15n/000244340/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;",
                        TString inputOptions    = "AOD,Data",
                        TString analysisOptions = "NOPHYSSEL",
                        TString softVersions    = "",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/Data/Data_0-0.3/AddTaskMuMu.C",". $ALICE_ROOT/include $ALICE_PHYSICS/include");

    TString outputdirGrid = "Analysis/LHC15n/MuMu/T0Custom/";
    if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdirGrid.Data());

    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);

    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    // pp trigger
    if (!isMC) triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));

    // // Load baseline
    // //==============================================================================
    // gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddTaskBaseLine.C");
    // AddTaskBaseLine();

    // Load centrality task
    //==============================================================================
    if(!analysisOptions.Contains("CENT")&& analysisMode.Contains("local") )
    {
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
        task->SetAlternateOADBforEstimators ("LHC15n");
    }

    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);

    delete triggers;
}
