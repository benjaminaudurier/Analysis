///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///


  // --- general analysis setup ---
  TString rootVersion       = "";
  TString alirootVersion    = "";
  TString aliphysicsVersion = "vAN-20160822-1";
  TString extraLibs         ="";
  TString extraIncs         ="include";
  TString extraTasks        ="";
  TString extraPkgs         ="EventMixing:PWGmuon";
  TString dataType          ="AOD";

  // --- grid specific setup ---
  TString dataDir           = "/alice/data/2015/LHC15o";
  TString dataPattern       = "*/muon_calo_pass1/AOD175/*/AliAOD.Muons.root";
  TString runFormat         = "000%d";
  TString outDir            = "Analysis/LHC15o/MuMuMixEvent/pt=0-12/";
  TString analysisMacroName = "MuMuMix";
  Int_t ttl                 = 60000;
  Int_t maxFilesPerJob      = 1000;
  Int_t maxMergeFiles       = 100;
  Int_t maxMergeStages      = 8;

  // --- saf3 specific setup ---
  Bool_t splitDataset = kTRUE;

  // ---     Pool setup      ---
  Bool_t Mix = kTRUE;
  const Int_t bufferSize = 20; // pool size
  const Int_t mixNum = 1; // ??

  Bool_t isMC = kFALSE;

//______________________________________________________________________________
void runMuMuFacilities(TString smode = "local", TString inputFileName = "AliAOD.Muons.root")
{
  /// Fill counters with triggered events

  TList pathList; pathList.SetOwner();
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString( "runMuMuFacilities.C" ));
  fileList.Add(new TObjString( "AddTaskMuMu.C" ));
  fileList.Add(new TObjString( "AddMixingHandler.C" ));
  fileList.Add(new TObjString( "AddAnalysisTaskMixInfo.C" ));
  fileList.Add(new TObjString( "EventMixing.par" ));
  fileList.Add(new TObjString( "PWGmuon.par" ));

  // Automatically generate parfile
  TObjString* Obj=0x0;
  for (int i = 0; i < fileList.GetEntries(); ++i){
    Obj= static_cast<TObjString*>(fileList.At(i));
    if(Obj->String().Contains(".par")) CreateAndCopyParFile(Obj->String().Data());
  }

  gROOT->LoadMacro("$HOME/Documents/PhilippeGitHub/Facilities/runTaskFacilities.C");

  // --- prepare the analysis environment ---
  Int_t mode = PrepareAnalysis(smode, inputFileName, extraLibs, extraIncs, extraTasks, extraPkgs, pathList, fileList);


  // --- run the analysis (saf3 is a special case as the analysis is launched on the server) ---
  if (mode == kSAF3Connect) {

    RunAnalysisOnSAF3(fileList, aliphysicsVersion, inputFileName, splitDataset);

  } else {

    CreateAnalysisTrain(isMC,Mix);

    if (smode == "saf3" && splitDataset) AliAnalysisManager::GetAnalysisManager()->SetSkipTerminate(kTRUE);

    RunAnalysis(smode, inputFileName, rootVersion, alirootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, dataDir, dataPattern, outDir, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages);
  }
}

//______________________________________________________________________________
void CreateAnalysisTrain(Bool_t isMC,Bool_t Mix)
{
  /// create the analysis train and configure it

   // analysis manager
   AliAnalysisManager        *mgr          = new AliAnalysisManager("MuMuAnalysis");
   AliMultiInputEventHandler *inputHanlder = 0x0;

  if (Mix) {
    // Create MultiInputEventHandler
    inputHandler = new AliMultiInputEventHandler();

    // Select correct handler and create a first input handler
    if ( dataType.Contains("AOD") )      inputHandler->AddInputEventHandler(new AliAODInputHandler());
    else if ( dataType.Contains("ESD") ) inputHandler->AddInputEventHandler(new AliESDInputHandler());
    else if ( isMC )                     inputHandler->AddInputEventHandler(new AliMCEventHandler());
    else {
      printf("Cannot determine data type\n");
      return;
    }
  }
  // Conncet handler to manager
  if ( inputHandler )                  mgr->SetInputEventHandler(inputHandler);
  else if ( isMC )                     mgr->SetMCtruthEventHandler(new AliMCEventHandler());
  else if ( dataType.Contains("AOD") ) mgr->SetInputEventHandler(new AliAODInputHandler());
  else if ( dataType.Contains("ESD") ) mgr->SetInputEventHandler(new AliESDInputHandler());
  else {
      printf("Cannot determine data type\n");
      return;
  }

  // add mixing handler
  if(Mix) {
    gROOT->LoadMacro("AddMixingHandler.C");
    AddMixingHandler(inputHandler,isMC,bufferSize,mixNum,"");
  }

  // Add Mix Info
  if(Mix) {
    gROOT->LoadMacro ("AddAnalysisTaskMixInfo.C");
    AddAnalysisTaskMixInfo("");
  }

  // Create trigger list
  TList* triggers = new TList;
  triggers->SetOwner(kTRUE);
  if ( !isMC ) {
    triggers->Add(new TObjString("CMSL7-B-NOPF-MUFAST"));
    triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));
    triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST||CMLL7-B-NOPF-MUFAST"));
  }

  TString output =  Form("%s",AliAnalysisManager::GetCommonFileName());
  gROOT->LoadMacro("AddTaskMuMu.C");
  AliAnalysisTaskMuMu* TaskMuMu = AddTaskMuMu(output.Data(),triggers,"PbPb2015",isMC);
}

//______________________________________________________________________________
void CreateAndCopyParFile(TString parfile)
{
  ///Create and copy ParFiles locally

  TString aliceBuildDir = gSystem->ExpandPathName("$ALICE_WORK_DIR/BUILD/AliPhysics-latest-ali-master/AliPhysics");
  TString command       = "";
  TString workDirFull   = gSystem->pwd();

  if(!parfile.Contains(".par"))return;

  command = Form("cd %s; make %s; cd %s; find %s -name %s -exec mv -v {} ./ \\;", aliceBuildDir.Data(), parfile.Data(), workDirFull.Data(), aliceBuildDir.Data(), parfile.Data());

  printf("Executing: %s\n", command.Data());
  gSystem->Exec(command.Data());
}

