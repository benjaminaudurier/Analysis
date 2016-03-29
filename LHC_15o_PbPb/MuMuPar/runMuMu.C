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
                        TString inputOptions    = "",
                        TString analysisOptions = "",
                        TString softVersions    = "",
                        TString taskOptions     = "" )
{
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));     
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "PWGmuon.par /Users/audurier/Documents/Analysis/LHC_15n_pp/MuMuPar/AddTaskMuMu.C",". $ALICE_ROOT/include $ALICE_PHYSICS/include");
    
    // TString outputdir = "Analysis/LHC15o/MuMuPar/";

    // if(analysisMode.Contains("grid")) AliAnalysisManager::GetAnalysisManager()->GetGridHandler()->SetGridWorkingDir(outputdir.Data());

    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);

    // TString Triggers = "CMUL7-B-NOPF-MUFAST";
    // TString Triggers = "CINT7-B-NOPF-MUFAST,CINT7-B-NOPF-MUFAST&0MSL,CINT7-B-NOPF-MUFAST&0MUL,CMUL7-B-NOPF-MUFAST,CMSL7-B-NOPF-MUFAST,CMSL7-B-NOPF-MUFAST&0MUL";
    // TString inputs = "0MSL:17,0MSH:18,0MLL:19,0MUL:20,0V0M:3";

    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC)
    {
        // PbPb trigger
        // triggers->Add(new TObjString("CINT7-B-NOPF-ALLNOTRD"));//MB
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));// Dimuon
    }
    
    // Load centrality task
    //==============================================================================
    if(analysisOptions.Contains("")&& runMode.Contains("local") )
    {
        gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
        AliMultSelectionTask * task = AddTaskMultSelection(kFALSE); // user
        task -> SetAlternateOADBforEstimators ("LHC15o");
    }
    
    // gROOT->LoadMacro("$ALICE_PHYSICS/PWG/muon/AddTaskMuMuMinvBA.C");
    // AddTaskMuMuMinvBA("MuMuBA",Triggers.Data(),inputs.Data(),"PbPb2015",isMC);

    TString output = Form("%s",AliAnalysisManager::GetCommonFileName());
    AddTaskMuMu(output.Data(),triggers,"PbPb2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);     
   
    // delete triggers;
}

