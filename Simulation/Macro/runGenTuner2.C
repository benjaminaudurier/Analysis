///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author L. Aphecetche
///

// dataset on caf : /default/ilakomov/LHC13f_muon_pass2_AliAOD_000196721
// dataset on saf : Find;FileName=AliAOD.Muons.root;BasePath=/alice/data/2011/LHC11h/000170040/ESDs/pass2_muon/AOD119


TString dataDir = "/alice/data/2010/LHC10h";
TString dataPattern = "pass2/*AliESDs.root";
TString runFormat = "%09d";
TString outDir = "Data/LHC10h/pass2/Eff/pDCAChi2";

// Settings for grid
Int_t ttl = 30000;
Int_t maxFilesPerJob = 100;
Int_t maxMergeFiles = 10;
Int_t maxMergeStages = 2;

// generator parameters used in the simulation
/*
// tune0 LHC13de
Double_t oldPtParam[6] = {371.909, 0.84614, 0.560486, 9.34831, 0.000474983, -0.853963};
Bool_t oldFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
Double_t newPtParam[6] = {371.909, 0.84614, 0.560486, 9.34831, 0.000474983, -0.853963};
Bool_t newFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
*//*
// tune1 LHC13de
Double_t oldPtParam[6] = {371.665, 0.845642, 0.56192, 9.34859, 0.000474519, -0.851091};
Bool_t oldFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
Double_t newPtParam[6] = {371.665, 0.845642, 0.56192, 9.34859, 0.000474519, -0.851091};
Bool_t newFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
*//*
// tune0 LHC13f
Double_t oldPtParam[6] = {522.811, 0.997725, 0.705636, 8.52259, 0., -1.};
Bool_t oldFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kTRUE, kTRUE};
Double_t newPtParam[6] = {522.811, 0.997725, 0.705636, 8.52259, 0.0001, -1.};
Bool_t newFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
*/
// tune1 LHC13f
Double_t oldPtParam[6] = {455.614, 0.942071, 0.706755, 8.69856, 0.000168775, -0.925487};
Bool_t oldFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};
Double_t newPtParam[6] = {455.614, 0.942071, 0.706755, 8.69856, 0.000168775, -0.925487};
Bool_t newFixPtParam[6] = {kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE};

Double_t ptRange[2] = {0.8, 999.};

/*
// tune0 LHC13de
Double_t oldYParam[8] = {0.539134, 1, 0, 0.0499378, 0, -0.00450342, 0, 2};
Bool_t oldFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
Double_t newYParam[8] = {0.539134, 1, 0, 0.0499378, 0, -0.00450342, 0, 2};
Bool_t newFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
*//*
// tune1 LHC13de
Double_t oldYParam[8] = {0.777922, 1, 0, -0.0184202, 0, -0.00107081, 0, 2};
Bool_t oldFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
Double_t newYParam[8] = {0.777922, 1, 0, -0.0184202, 0, -0.00107081, 0, 2};
Bool_t newFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
*//*
// tune0 LHC13f
Double_t oldYParam[8] = {1.75646, 1., 8.70262e-05, -0.129939, -0.0190949, 0., 0., 2.};
Bool_t oldFixYParam[8] = {kFALSE, kTRUE, kFALSE, kFALSE, kFALSE, kTRUE, kTRUE, kTRUE};
Double_t newYParam[8] = {1.5712, 1., 0., -0.0893785, 0., 0.00228603, 0., 2.};
Bool_t newFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
//Double_t newYParam[8] = {1.8216, 0., 0., 0., 0., 0., 1., 2.0016};
//Bool_t newFixYParam[8] = {kFALSE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kTRUE, kFALSE};
*/
// tune1 LHC13f
Double_t oldYParam[8] = {1.29511, 1., 0., -0.0767846, 0., 0.00176313, 0., 2.};
Bool_t oldFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};
Double_t newYParam[8] = {1.29511, 1., 0., -0.0767846, 0., 0.00176313, 0., 2.};
Bool_t newFixYParam[8] = {kFALSE, kTRUE, kTRUE, kFALSE, kTRUE, kFALSE, kTRUE, kTRUE};

Double_t yRange[2] = {-4.3, -2.3};


Bool_t isMC = kTRUE;
Bool_t applyPhysicsSelection = kFALSE;


//______________________________________________________________________________
AliAnalysisTask* runGenTuner2(const char* dataset="Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root",
                         const char* where="saf",
                         const char* alirootMode="",
                         Int_t iStep = -1, 
                         char overwrite = '\0')
{

    TString sds(dataset); // To work with dataset
    
    if ( sds.Length()>0 )
    {
        LoadAlirootOnProof(where,"v5-34-08","vAN-20150422","","","AliAnalysisTaskGenTuner",kFALSE,alirootMode); // Macro to connect to proof
    }
    

    LoadLocalLibs(kTRUE); // Macro to load Local Libraries
  
    AliAnalysisManager *mgr = new AliAnalysisManager("MuMu"); // Create the manager
  
    AliInputEventHandler* input(0x0); // Pointer for imputs

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
    

    
 
  
    TString outputname("test.MuMu.AOD.1.root"); // Create output name in case of no dataset selected
  

    
    // Loop to configure the .root output file's name
    //==============================================================================
    if ( sds.Length()>0 )
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

    // generator tuner
    gROOT->LoadMacro("AddTaskGenTuner.C");

    AliAnalysisTaskGenTuner* genTuner=0x0
    AliAnalysisTaskGenTuner* genTuner = AddTaskGenTuner();
    if(!genTuner) 
    {
        Error("CreateAnalysisTrain","AliAnalysisTaskGenTuner not created!");
        return 0x0;
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
      mgr->StartAnalysis("proof",sds.Data());
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

    // --- save fitting functions ---
  TString outFileName = AliAnalysisManager::GetCommonFileName();
  TFile *outFile = (TFile*)gROOT->GetListOfFiles()->FindObject(outFileName.Data());
  if (outFile) outFile->ReOpen("UPDATE");
  else outFile = TFile::Open(outFileName.Data(),"UPDATE");
  if (outFile && outFile->IsOpen()) 
  {
    outFile->Cd(Form("%s:/MUON_GenTuner",outFileName.Data()));
    
    if (genTuner->GetOldPtFunc()) genTuner->GetOldPtFunc()->Write(0x0, TObject    ::kOverwrite);
    if (genTuner->GetOldPtFuncMC()) genTuner->GetOldPtFuncMC()->Write(0x0, TObject::kOverwrite);
    if (genTuner->GetNewPtFunc()) genTuner->GetNewPtFunc()->Write(0x0, TObject    ::kOverwrite);
    if (genTuner->GetOldYFunc()) genTuner->GetOldYFunc()->Write(0x0, TObject      ::kOverwrite);
    if (genTuner->GetOldYFuncMC()) genTuner->GetOldYFuncMC()->Write(0x0, TObject  ::kOverwrite);
    if (genTuner->GetNewYFunc()) genTuner->GetNewYFunc()->Write(0x0, TObject      ::kOverwrite);
    if (genTuner->GetResults()) genTuner->GetResults()->Write(0x0, TObject        ::kOverwrite);
    if (genTuner->GetRatios()) genTuner->GetRatios()->Write(0x0, TObject          ::kOverwrite);
    
    outFile->Close();
  }


  
    AliCodeTimer::Instance()->Print();// Timer
  
    // Congifure in case of PAR file
    //==============================================================================
    if (alirootMode=="PAR")
    {
        TProofLog *pl = TProof::Mgr(where)->GetSessionLogs(); pl->Save("*","aod.log");
    }
  
    delete triggers;
  
    return task;
}



//______________________________________________________________________________
void LoadAlirootOnProof(TString& aaf, TString rootVersion, TString aliphysicsVersion, TString& extraLibs,
                        TString& extraIncs, TString& extraTasks, Bool_t notOnClient = kFALSE, TString& alirootMode )
{
    // Load aliroot packages and set environment on Proof
    
    // set general environment
    gEnv->SetValue("XSec.GSI.DelegProxy","2");
    
    // connect
    // if (aaf == "prooflite") TProof::Open("");
    if (aaf == "prooflite") TProof::Open("workers=2");
    
    else {
        TString location, nWorkers;
        if (aaf == "caf") {
            location = "alice-caf.cern.ch";
            nWorkers = ""; // "workers=40";
        } else if (aaf == "saf") {
            location = "baudurie@nansafmaster2.in2p3.fr"; // "localhost:1093"
            nWorkers = ""; // "workers=8x";
        } else return;
        
        TString user = (gSystem->Getenv("alien_API_USER") == NULL) ? "" : Form("%s@",gSystem->Getenv("alien_API_USER"));
        TProof::Mgr(Form("%s%s",user.Data(), location.Data()))->SetROOTVersion(Form("VO_ALICE@ROOT::%s",rootVersion.Data()));
        TProof::Open(Form("%s%s/?N",user.Data(), location.Data()), nWorkers.Data());
        }
    
    if (!gProof)
        {
        cout << "Cannot connect to Proof : " << where << endl;
        return;
        }
    
    // set environment and load libraries on workers
    TList* list = new TList();
    
    if (!alirootMode.IsNull())// Check if you have datas
        {
            if ( alirootMode == "PAR" )
            {
            cout << "Will work with PAR files" << endl;
            
            std::vector<std::string> pars;
            
            pars.push_back("~/alicesw/aliroot/mumu/build/STEERBase");
            pars.push_back("~/alicesw/aliroot/mumu/build/ESD");
            pars.push_back("~/alicesw/aliroot/mumu/build/AOD");
            pars.push_back("~/alicesw/aliroot/mumu/build/ANALYSIS");
            pars.push_back("~/alicesw/aliroot/mumu/build/OADB");
            pars.push_back("~/alicesw/aliroot/mumu/build/ANALYSISalice");
            pars.push_back("~/alicesw/aliroot/mumu/build/CORRFW");
            pars.push_back("~/alicesw/aliroot/mumu/build/PWGmuon");
            
            Bool_t ok(kTRUE);
            
            for ( std::vector<std::string>::size_type i = 0; i < pars.size(); ++i )
                {
                std::string package = pars[i];
                
                if ( gProof->UploadPackage(package.c_str()) )ok = kFALSE;
                
                if ( gProof->EnablePackage(package.c_str(),"",kTRUE) )ok = kFALSE;
            
                if (!ok)
                    {
                    cout << "Problem with PAR " << package.c_str() << endl;
                    return 0;
                    }
                }
            }
            else { list->Add(new TNamed("ALIROOT_MODE", alirootMode.Data()));}
        }
    else
        {
            list->Add(new TNamed("ALIROOT_MODE",""));
        }
    //list->Add(new TNamed("ALIROOT_MODE", alirootMode.Data()));
    list->Add(new TNamed("ALIROOT_EXTRA_LIBS", extraLibs.Data()));// Load extra Libraries
    list->Add(new TNamed("ALIROOT_EXTRA_INCLUDES", extraIncs.Data()));// Load extra Includes
    if (aaf == "prooflite")
        {
        cout << "Will work in LITE mode" << endl;
        gProof->UploadPackage("$ALICE_ROOT/ANALYSIS/macros/AliRootProofLite.par");
        gProof->EnablePackage("$ALICE_ROOT/ANALYSIS/macros/AliRootProofLite.par", list);
        }
    else gProof->EnablePackage(Form("VO_ALICE@AliPhysics::%s",aliphysicsVersion.Data()), list, notOnClient);
    
    
    // compile additional tasks on workers. To add tasks, use ":" (see in the main loop)
    TObjArray* tasks = extraTasks.Tokenize(":");
    for (Int_t i = 0; i < tasks->GetEntriesFast(); i++)
        gProof->Load(Form("%s.cxx++g",static_cast<TObjString*>(tasks->UncheckedAt(i))->GetName()), notOnClient);
    delete tasks;
}

//______________________________________________________________________________
TObject* CreateAnalysisTrain(TObject* alienHandler, Int_t iStep)
{
  /// create the analysis train and configure it
  
  // analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("GenTunerAnalysis");
  
  // Connect plugin to the analysis manager if any
  if (alienHandler) mgr->SetGridHandler(static_cast<AliAnalysisGrid*>(alienHandler));
  
  // AOD handler
  AliInputEventHandler* aodH = new AliAODInputHandler;
  mgr->SetInputEventHandler(aodH);
  
  // track selection
  AliMuonTrackCuts trackCuts("stdCuts", "stdCuts");
  trackCuts.SetAllowDefaultParams();
  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchLpt | AliMuonTrackCuts::kMuEta |
              AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
//  trackCuts.SetFilterMask(AliMuonTrackCuts::kMuMatchHpt | AliMuonTrackCuts::kMuEta |
//            AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
  if (isMC) trackCuts.SetIsMC(kTRUE);
  
  // generator tuner
  gROOT->LoadMacro("AddTaskGenTuner.C");
  AliAnalysisTaskGenTuner* genTuner = AddTaskGenTuner();
  if(!genTuner) 
  {
    Error("CreateAnalysisTrain","AliAnalysisTaskGenTuner not created!");
    return 0x0;
  }
  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUU7);
//  if (applyPhysicsSelection) genTuner->SelectCollisionCandidates(AliVEvent::kMUSH7);
  //genTuner->SelectCentrality(0., 90.);
  genTuner->SetMuonTrackCuts(trackCuts);
  genTuner->SetMuonPtCut(1.);
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13d/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13d/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13e/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13e/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13de/muon_pass2/AOD/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13de/muon_pass2/AOD/GenTuner/pT2GeV/AnalysisResults.root");
  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT1GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT2GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT4GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT6GeV/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT1GeV_y2.5-3/AnalysisResults.root");
//  if (isMC) genTuner->SetDataFile("/Users/pillot/Work/Alice/Work/Data/2013/LHC13f/muon_calo/AOD127/GenTuner/pT1GeV_y3-4/AnalysisResults.root");
  
  if (iStep == 0) {
    
    // set the generator parameters used in simulation
    genTuner->SetPtParam(oldPtParam, oldFixPtParam, newPtParam, newFixPtParam, ptRange[0], ptRange[1]);
    genTuner->SetYParam(oldYParam, oldFixYParam, newYParam, newFixYParam, yRange[0], yRange[1]);
    
  } else if (iStep > 0) {
    /*
    // get the original generator parameters from first step if any
    TFile *inFile = TFile::Open("Results_step0.root","READ");
    if (inFile && inFile->IsOpen()) {
      TF1 *fOldPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFunc"));
      TF1 *fOldYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFunc"));
      if (fOldPtFunc && fOldYFunc) {
    fOldPtFunc->GetParameters(oldPtParam);
    fOldYFunc->GetParameters(oldYParam);
      }
      inFile->Close();
    }
    */
    // get the new generator parameters from previous step if any and configure the tuner
    TString inFileName = Form("Results_step%d.root",iStep-1);
    inFile = TFile::Open(inFileName.Data(),"READ");
    if (inFile && inFile->IsOpen()) {
      TF1 *fNewPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncNew"));
      TF1 *fNewYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFuncNew"));
      if (fNewPtFunc && fNewYFunc) {
    genTuner->SetPtParam(oldPtParam, newFixPtParam, fNewPtFunc->GetParameters(), newFixPtParam, fNewPtFunc->GetXmin(), fNewPtFunc->GetXmax());
    genTuner->SetYParam(oldYParam, newFixYParam, fNewYFunc->GetParameters(), newFixYParam, fNewYFunc->GetXmin(), fNewYFunc->GetXmax());
      }
      inFile->Close();
    }
    
    // enable the weighing
    genTuner->Weight(kTRUE);
    
  }
  
  return genTuner;
  
}

//______________________________________________________________________________
void LoadLocalLibs(Bool_t localAnalysis=kTRUE)
{
    gSystem->Load("libVMC");
    gSystem->Load("libMinuit");
    gSystem->Load("libTree");
    gSystem->Load("libProofPlayer");
    gSystem->Load("libXMLParser");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libSTEER");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    gSystem->Load("libCORRFW");
    
    if (!localAnalysis)
        {
        //    gSystem->Load("libCORRFW");
        }
    else
        {
        gSystem->Load("libPWGmuon");
        }
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

//______________________________________________________________________________
TString GetInputType(const TString& sds)
{
    if (sds.Length()==0 ) return "AOD";
    
    if (sds.Contains("SIM_JPSI")) return "AOD";
    
    if (sds.Contains("AOD")) return "AOD";
    if (sds.Contains("ESD")) return "ESD";
    
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(sds.Data())) )
        {
        // dataset is not a local file so it must be a dataset name
        if (!gProof) return "NOPROOF";
        
        TFileCollection* fc = gProof->GetDataSet(sds.Data());
        if (!fc) return "NODATASET";
        
        TIter next(fc->GetList());
        TFileInfo* fi;
        while ( ( fi = static_cast<TFileInfo*>(next()) ) )
            {
            TUrl url(*(fi->GetFirstUrl()));
            TString surl(url.GetUrl());
            
            if (surl.Contains("AOD")) return "AOD";
            if (surl.Contains("AliESD")) return "ESD";
            }
        
        }
    else
        {
        std::cout << "Will use datasets from file " << sds.Data() << std::endl;
        
        // dataset is a local text file containing a list of dataset names
        std::ifstream in(sds.Data());
        char line[1014];
        
        while (in.getline(line,1023,'\n'))
            {
            TString sline(line);
            sline.ToUpper();
            if ( sline.Contains("SIM_JPSI") ) return "AOD";
            if ( sline.Contains("AOD") ) return "AOD";
            if ( sline.Contains("ESD") ) return "ESD";
            }
        }
    
    return "BUG";   
}
