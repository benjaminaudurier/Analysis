Bool_t RunALICE(TString runMode, 
                TString analysisMode,
                TString inputName       = "Find;BasePath=/alice/data/2015/LHC15g/000228936/muon_calo_pass1/AOD/*;FileName=AliAOD.Muons.root;Mode=cache;",
                TString inputOptions    = "",
                TString analysisOptions = "MIXED",
                TString softVersions    = "aliphysics=vAN-20151115-1",
                TString taskOptions     = "",
                Long64_t nEvents = 1e10,
                Long64_t nSkip = 0,
                TString dsName="",
                TString alirsnliteManagers ="AddAMEventMixingTest",
                Bool_t useMultiHandler=kTRUE,
                TString alirsnlitesrc ="$ALICE_ROOT",
                TString alirsnlitetasks =""
                ) 
{

   // path for macro usefull for saf3
   gROOT->LoadMacro(gSystem->ExpandPathName("$TASKDIR/runTaskUtilities.C"));

   Bool_t useAODOut = kFALSE;
   // CreateInputHandlers(input,inputMC,useAODOut,useMultiHandler);

   // Macro to connect to proof. First argument useless for saf3
   SetupAnalysis(runMode,analysisMode,inputName,inputOptions,softVersions,analysisOptions, "libPWGmuon.so AliAnalysisTaskEx02.cxx","$ALICE_ROOT/include $ALICE_PHYSICS/include");
   
   //Flag for MC
   Bool_t isMC = IsMC(inputOptions);

   Bool_t isAOD = IsAOD(inputName,inputOptions);
   Bool_t isESD = IsESD(inputName,inputOptions);
   TString dataType;
   if(isAOD)dataType="aod";
   if(isESD)dataType="esd";

   // See inside
   gROOT->LoadMacro("AddAMEventMixingTest.C");
   AddAMEventMixingTest(runMode,analysisMode,dataType,isMC,"","");


   // TList *listManagers = CreateListOfManagersFromDir(alirsnliteManagers,alirsnlitetasks);
   // if (!listManagers) { Printf("Error : CreateListOfManagersFromDir failed !!!"); return kFALSE;}

   // adds all tasks (useless due to Diego's launcher)
   // if (!AddAllManagers(listManagers, anSrc, anMode,input,inputMC)) { Printf("Error : AddAllManagers failed !!!"); return kFALSE;}

   TStopwatch timer;
   timer.Start();

   // runs analysis  (useless due to tiedgo's launcher)
   if (!RunAnalysisManager(runMode, analysisMode,inputName, nEvents, nSkip)) { Printf("Error : RunAnalysisManager failed !!!"); return kFALSE;}
   

   // // Start analysis
   // //==============================================================================
   // StartAnalysis(runMode,analysisMode,inputName,inputOptions);

   timer.Stop();
   timer.Print();
   Printf("Working directory is %s ...", gSystem->WorkingDirectory());
   Printf("Done OK");
   return kTRUE;

}

//______________________________________________________________________
Bool_t RunAnalysisManager( TString rMode, TString aMode, TString input, TString Opt  Long64_t nEvts = 1e10, Long64_t nSkp = 0)
{
  //
  // Run the analysis
  //

  if ( IsPod(aMode) && ! IsPodMachine(aMode) ) {
    GetPodOutput(aMode);
    rMode = "terminate";
  }
  TString sMode = GetMode(rMode,aMode);

  if ( sMode.IsNull() ) return;

  AliAnalysisManager* mgr = AliAnalysisManager::GetAnalysisManager();
  if ( ! mgr->InitAnalysis()) {
    printf("Fatal: Cannot initialize analysis\n");
    return;
  }
  mgr->PrintStatus();

  if ( sMode == "terminateonly" && gSystem->AccessPathName(mgr->GetCommonFileName())) {
    printf("Cannot find %s : noting done\n",mgr->GetCommonFileName());
    return;
  }

  TObject* inputObj = CreateInputObject(rMode,aMode,input,Opt);

  TString mgrMode =( sMode == "terminateonly" ) ? "grid terminate" : sMode.Data();


   if ((!aMode.CompareTo("proof")) || (!aMode.CompareTo("local"))) {
      mgr->StartAnalysis(aMode.Data(), nEvts, nSkp);
   } else {
      mgr->StartAnalysis(aMode.Data());
   }

   return kTRUE;
}

//______________________________________________________________________
Bool_t CreateInputHandlers(TString input,TString inputMC,Bool_t useAODOut=kFALSE,Bool_t useMultiHandler=kTRUE) {

   Bool_t useMC = !inputMC.CompareTo("mc");

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error [CreateInputHandlers] : mgr is null !!!"); return kFALSE; }

   if (useMultiHandler) {
      AliMultiInputEventHandler *inputHandler = new AliMultiInputEventHandler();
      if (!input.CompareTo("esd")) {
         inputHandler->AddInputEventHandler(new AliESDInputHandler());
         if (useMC) inputHandler->AddInputEventHandler(new AliMCEventHandler());
      } else if (!input.CompareTo("aod")) {
         inputHandler->AddInputEventHandler(new AliAODInputHandler());
      }

      mgr->SetInputEventHandler(inputHandler);
   } else {
      if (!input.CompareTo("esd")) {
         mgr->SetInputEventHandler(new AliESDInputHandler());
         if (useMC) mgr->SetMCtruthEventHandler(new AliMCEventHandler());
      } else if (!input.CompareTo("aod")) {
         mgr->SetInputEventHandler(new AliAODInputHandler());
      }
   }

   if (useAODOut) {
      AliAODHandler *aodHandler   = new AliAODHandler();
      aodHandler->SetOutputFileName("AliAOD.root");
      mgr->SetOutputEventHandler(aodHandler);
   }

   return kTRUE;
}

//______________________________________________________________________
TList *CreateListOfManagersFromDir(TString listManagersNames="",TString dir="") {

   TList *listManagers = new TList;
   TString dirsStr;
   TObjArray *dirs=0;

   if (listManagersNames.IsNull()) {
      if (dir.IsNull() || gSystem->AccessPathName(gSystem->ExpandPathName(dir.Data()))) {
         Printf("Error [CreateListOfManagersFromDir] : Dir '%s' doesn't exists !!!",dir.Data());
         return 0;
      }
      dirsStr = gSystem->GetFromPipe(Form("ls %s",dir.Data()));
      dirs = dirsStr.Tokenize("\n");
   } else {
      dirsStr = listManagersNames;
      dirs = dirsStr.Tokenize(" ");
   }

   TIter next(dirs);
   Int_t counter=0;
   TObjString *str,*strtmp;
   TObjArray *mydirstrTok;
   TString mydirstr,main,prefix;
   while ((str = (TObjString *)next.Next())) {
      // TODO add direcotry
      mydirstr = str->GetString();
      if (mydirstr.IsNull()) continue;

      Printf("Adding %s .,,",mydirstr.Data());
      mydirstrTok = mydirstr.Tokenize("_");

      main = ((TObjString *)mydirstrTok->At(0))->GetString();

      strtmp = (TObjString *)mydirstrTok->At(1);
      if (strtmp) prefix = strtmp->GetString(); else prefix="";

      listManagers->Add(new TNamed(main,prefix));
   }

   return listManagers;
}

//______________________________________________________________________
Bool_t AddAllManagers(TList *listManagers,TString anSrc, TString anMode,TString input,TString inputMC) {
   TIter next(listManagers);
   Int_t counter=0;
   TNamed *name;
   while ((name = (TNamed *)next.Next())) {
      if (!AddAnalysisManager(name->GetName(), anSrc, anMode,input,inputMC,name->GetTitle(),Form("%d",counter++))) {
         Printf("Error: Problem adding %s",name->GetName());
         return kFALSE;
      }
   }

   return kTRUE;
}

//______________________________________________________________________
Bool_t AddAnalysisManager(TString managerMacro, TString anSrc, TString anMode,TString input,TString inputMC, TString postfix,TString idStr) {
   gROOT->LoadMacro(Form("%s.C", managerMacro.Data()));
   return gROOT->ProcessLine(Form("%s\(\"%s\",\"%s\",\"%s\"\,\"%s\",\"%s\",\"%s\"\);", managerMacro.Data(), anSrc.Data(), anMode.Data(),input.Data(),inputMC.Data(), postfix.Data(),idStr.Data()));
}

