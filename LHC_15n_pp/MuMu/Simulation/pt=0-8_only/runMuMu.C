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
                        TString inputName       = "Find;BasePath=/alice/data/2015/LHC15o/000244918/muon_calo_pass1/AOD/;FileName=AliAOD.Muons.root;",
                        TString inputOptions    = "MC,AOD",
                        TString analysisOptions = "",
                        TString softVersions    = "",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions,"PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/Simulation/pt=0-8_only/AddTaskMuMu.C", ". $ALICE_ROOT/include $ALICE_PHYSICS/include");

    // Grid config.
    TString outputdir = "Analysis/LHC15n/MuMu/";
    if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdir.Data());

    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);

    // pp trigger
    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC) triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));

    // // Load baseline
    // gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddTaskBaseLine.C");
    // AddTaskBaseLine();

    // Load centrality task
    //==============================================================================
    if(analysisOptions.Contains("NOCENTR")&& analysisMode.Contains("local") ) 
    {
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
        task -> SetAlternateOADBforEstimators ("LHC15n");
    }

    // gROOT->LoadMacro("AddTaskMuMu.C");
    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);

    // delete triggers;
}
