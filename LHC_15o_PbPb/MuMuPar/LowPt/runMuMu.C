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
                        TString inputOptions    = "AOD",
                        TString analysisOptions = "",
                        TString softVersions    = "",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));     
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMuPar/LowPt/AddTaskMuMu.C",". $ALICE_ROOT/include $ALICE_PHYSICS/include");
    
    TString outputdir = "Analysis/LHC15o/MuMuPar/";
    if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdir.Data());

    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);


    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC) {
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));// Dimuon
    }
    
    // Load centrality task
    //==============================================================================
    if(analysisOptions.Contains("")&& analysisMode.Contains("local") ) {
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
        task -> SetAlternateOADBforEstimators ("LHC15o");
    }

    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"PbPb2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);     
   
    // delete triggers;
}

