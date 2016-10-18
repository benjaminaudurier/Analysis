///
/// Example macro to run the AliAnalysisTaskMuMu task
///
/// \author Benjamin Audurier
///

  // --- general analysis setup ---
  TString rootVersion       = "";
  TString alirootVersion    = "";
  TString aliphysicsVersion = "vAN-20160926-1";
  TString extraLibs         ="";
  TString extraIncs         ="include";
  TString extraTasks        ="";
  TString extraPkgs         ="PWGmuon";
  TString dataType          ="AOD";

  // --- grid specific setup ---
  TString dataDir           = "/alice/cern.ch/user/p/ppillot/Sim/LHC15n/JPsiTune1/VtxShift/results";
  TString dataPattern       = "*AliAOD.Muons.root";
  TString runFormat         = "%d";
  TString outDir            = "Sim/LHC15n/JPsiTune1/VtxShift/AccEff";
  TString analysisMacroName = "TrgCount";
  Int_t ttl                 = 30000;
  Int_t maxFilesPerJob      = 20;
  Int_t maxMergeFiles       = 10;
  Int_t maxMergeStages      = 2;

  // --- saf3 specific setup ---
  Bool_t splitDataset = kFALSE;

//______________________________________________________________________________
void runMuMuFacilities(TString smode = "local", TString inputFileName = "AliAOD.Muons.root", Bool_t isMC = kTRUE)
{
  /// Fill counters with triggered events

  TList pathList; pathList.SetOwner();
  TList fileList; fileList.SetOwner();
  fileList.Add(new TObjString( "runMuMuFacilities.C" ));
  fileList.Add(new TObjString( "AddTaskMuMu.C" ));
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

    CreateAnalysisTrain(isMC);

    if (smode == "saf3" && splitDataset) AliAnalysisManager::GetAnalysisManager()->SetSkipTerminate(kTRUE);

    RunAnalysis(smode, inputFileName, rootVersion, alirootVersion, aliphysicsVersion, extraLibs, extraIncs, extraTasks, extraPkgs, dataDir, dataPattern, outDir, analysisMacroName, runFormat, ttl, maxFilesPerJob, maxMergeFiles, maxMergeStages);
  }
}

//______________________________________________________________________________
void CreateAnalysisTrain(Bool_t isMC)
{
  /// create the analysis train and configure it

   // analysis manager
   AliAnalysisManager        *mgr          = new AliAnalysisManager("MuMuAnalysis");

  if(dataType.Contains("AOD"))      mgr->SetInputEventHandler(new AliAODInputHandler());
  else if(dataType.Contains("ESD")) mgr->SetInputEventHandler(new AliESDInputHandler());
  else {
      printf("Cannot determine data type\n");
      return;
  }

  TString output =  Form("%s",AliAnalysisManager::GetCommonFileName());
  gROOT->LoadMacro("AddTaskMuMu.C");
  AliAnalysisTaskMuMu*         TaskMuMu = AddTaskMuMu(output.Data(),"PbPb2015",isMC);

}

//______________________________________________________________________________
void CreateAndCopyParFile(TString parfile)
{
  ///Create and copy ParFiles locally

  TString aliceBuildDir = gSystem->ExpandPathName("$ALICE_WORK_DIR/BUILD/AliPhysics-latest-ali-master/AliPhysics");
  TString command = "";
  TString workDirFull = gSystem->pwd();

  if(!parfile.Contains(".par"))return;

  command = Form("cd %s; make %s; cd %s; find %s -name %s -exec mv -v {} ./ \\;", aliceBuildDir.Data(), parfile.Data(), workDirFull.Data(), aliceBuildDir.Data(), parfile.Data());

  printf("Executing: %s\n", command.Data());
  gSystem->Exec(command.Data());
}

