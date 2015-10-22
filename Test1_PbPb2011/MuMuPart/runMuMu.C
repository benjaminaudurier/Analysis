///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author L. Aphecetche
///

// dataset on caf : /default/ilakomov/LHC13f_muon_pass2_AliAOD_000196721
// dataset on saf : Find;FileName=AliAOD.Muons.root;BasePath=/alice/data/2011/LHC11h/000170040/ESDs/pass2_muon/AOD119
//______________________________________________________________________________
AliAnalysisTask* runMuMu(const char* dataset="datasetfull.txt",
                         Bool_t simulations=kFALSE,
                         Bool_t baseline=kFALSE,
                         const char* where="saf",const char* alirootMode="")
{

    TString sds(dataset); // To work with dataset
    
    if ( sds.Length()>0 )
    {
        LoadAlirootOnProof(where,"v5-34-08","vAN-20150422","","","",kFALSE,alirootMode); // Macro to connect to proof
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
    

    
    // Fill the trigger list with desired trigger combinations (See on ALICE log book for denomination)
    //==============================================================================
    if (!simulations)
    {

       triggers->Add(new TObjString("0VBA & 0VBC &0MUL"));// MB 
       triggers->Add(new TObjString("CPBI2_B1-B-NOPF-ALLNOTRD"));// MB 
       triggers->Add(new TObjString("CPBI2_B1-B-NOPF-ALLNOTRD&0MUL"));// MB & 0MUL  


       triggers->Add(new TObjString("CPBI1MSL-B-NOPF-MUON")); // single muon low pt
       triggers->Add(new TObjString("CPBI1MUL-B-NOPF-MUON")); // dimuon low pt

       // triggers->Add(new TObjString("CVHN-B-NOPF-ALLNOTRD|CVHN_R2-B-NOPF-ALLNOTRD|CCENT-B-NOPF-ALLNOTRD|CCENT_R2-B-NOPF-ALLNOTRD ")); // central high treshold (+ TO vertex)
       // triggers->Add(new TObjString("CVHN-B-NOPF-ALLNOTRD|CVHN_R2-B-NOPF-ALLNOTRD|CCENT-B-NOPF-ALLNOTRD|CCENT_R2-B-NOPF-ALLNOTRD")); // central high treshold +TO vertex
       // triggers->Add(new TObjString("(CVHN-B-NOPF-ALLNOTRD&0MUL)|(CVHN_R2-B-NOPF-ALLNOTRD&0MUL)|(CCENT-B-NOPF-ALLNOTRD&0MUL)|(CCENT_R2-B-NOPF-ALLNOTRD&0MUL)")); // central high treshold +TO vertex

       // triggers->Add(new TObjString("CVLN_B2-B-NOPF-ALLNOTRD|CVLN-B-NOPF-ALLNOTRD|CVLN_R1-B-NOPF-ALLNOTRD|CSEMI-B-NOPF-ALLNOTRD|CSEMI_R1-B-NOPF-ALLNOTRD")); // semicentral low treshold +TO vertex
       // triggers->Add(new TObjString("(CVLN_B2-B-NOPF-ALLNOTRD&0MUL)|(CVLN-B-NOPF-ALLNOTRD&0MUL)|(CVLN_R1-B-NOPF-ALLNOTRD&0MUL)|(CSEMI-B-NOPF-ALLNOTRD&0MUL)|(CSEMI_R1-B-NOPF-ALLNOTRD&0MUL)")); // semicentral low treshold +TO vertex
    
        // pA trigger
        // triggers.Add(new TObjString("CINT7-B-NOPF-ALLNOTRD"));//MB
        // triggers.Add(new TObjString("CMUL7-B-NOPF-MUON"));// Dimuon
        // triggers.Add(new TObjString("CINT7-B-NOPF-ALLNOTRD & 0MSL"));
        // triggers.Add(new TObjString("CMSL7-B-NOPF-MUON & 0MUL"));
    }
  
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

    AliAnalysisTask* task(0x0); // Create pointer for task
  
    // Load the right task depending on 'baseline'
    //==============================================================================
    if (!baseline)
    {
  	  gROOT->LoadMacro("AddTaskMuMu.C");
  	  task = AddTaskMuMu(outputname.Data(),triggers,"PbPb2011",simulations);
    }
    else
    {
  	  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/train/AddTaskBaseLine.C");
  	  task = AddTaskBaseLine();
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
            list->Add(new TNamed("ALIROOT_MODE","base"));
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
