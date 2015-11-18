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
                TString inputName       = "Find;BasePath=/alice/data/2015/LHC15g/000228936/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;",
                TString inputOptions    = "",
                TString analysisOptions = "",
                TString softVersions    = "aliphysics=vAN-20151115-1",
                TString taskOptions     = "" )
{
    // path for macro usefull for saf3
    gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C")); 
    
     
    // Macro to connect to proof. First argument useless for saf3
    SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "libPWGPPMUONlite.so","$ALICE_ROOT/include $ALICE_PHYSICS/include");
    cout <<"toto"<< endl;
    
    //Flag for MC
    Bool_t isMC = IsMC(inputOptions);
    
    
    // Fill the trigger list with desired trigger combinations (See on ALICE log book for denomination)
    //==============================================================================
    TList* triggers = new TList; // Create pointer for trigger list
    triggers->SetOwner(kTRUE); // Give rights to trigger liser
    if (!isMC)
    {
        // pA trigger
        triggers->Add(new TObjString("CINT7-B-NOPF-MUON"));// MB
        triggers->Add(new TObjString("CINT7-B-NOPF-MUON|CINT7-B-NOPF-CENTNOTRD"));// MB &MUL
        triggers->Add(new TObjString("CINT7-B-NOPF-MUON&0MUL"));// MB &MUL
        triggers->Add(new TObjString("CMUL7-B-NOPF-MUON"));// Dimuon
    }
  
    // Load task
    //==============================================================================
    TString sds(inputName);
    sds.ReplaceAll("/","-");
    TString outputname = Form("%s.%s.MuMu.root",sds.Data(),analysisMode.Data()); // Create output name in case of no dataset selected
  	// TString outputname = AliAnalysisManager::GetAnalysisManager()->GetCommonFileName();
    gROOT->LoadMacro("AddTaskMuMu.C");
  	cout <<"tata"<< endl;
    AddTaskMuMu(outputname.Data(),triggers,"pp2015",isMC);
    cout <<"add task mumu done"<< endl;

    // Start analysis
    //==============================================================================
  
    StartAnalysis(runMode,analysisMode,inputName,inputOptions);     
   
    delete triggers;
}

