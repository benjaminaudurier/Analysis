void RunPoD(
  Int_t runNumber = 245700,
  TString dataset = "Find;"
                    "BasePath=/alice/data/2015/LHC15o/%09d/muon_calo_pass1/AOD/*/;"
                    "FileName=AliAOD.Muons.root;"
                    "Tree=/aodTree;"
                    "Mode=remote;",  // <-- much faster dataset creation
  Bool_t usePhysicsSelection = kTRUE,
  Int_t numEvents = 99999999,
  Int_t firstEvent = 0
) {

  // Not needed on the VAF
  //gEnv->SetValue("XSec.GSI.DelegProxy","2");

  TList *list = new TList();
  //list->Add(new TNamed("ALIROOT_EXTRA_LIBS", "ANALYSIS:ANALYSISalice"));  // normally not needed, only used in special cases
  list->Add(new TNamed("ALIROOT_ENABLE_ALIEN", "1"));  // important: creates token on every PROOF worker

  // Not needed on the VAF
  //TProof::Mgr("alice-caf.cern.ch")->SetROOTVersion("VO_ALICE@ROOT::v5-34-08");

  // Note the difference between CAF and VAF
  //TProof::Open("alice-caf.cern.ch");
  TProof::Open("pod://");

  // Assemble dataset from format
  TString datasetWithRun;
  datasetWithRun.Form(dataset.Data(), runNumber);

  // Check the dataset before running the analysis!
  gProof->ShowDataSet(datasetWithRun.Data());
  //return;  // <-- uncomment this to test search before running the analysis!

  // Not needed on the VAF
  //gProof->EnablePackage("VO_ALICE@AliRoot::v5-04-81-AN", list);

  // A single AliRoot package for *all* AliRoot versions: new on VAF
  TFile::Cp("http://alibrary.web.cern.ch/alibrary/vaf/AliceVaf.par", "AliceVaf.par");
  gProof->UploadPackage("AliceVaf.par");
  gProof->EnablePackage("AliceVaf.par", list);  // this "list" is the same as always
//  gProof->SetParallel(2);

  AliAnalysisManager *mgr  = new AliAnalysisManager("Analysis Train");
  AliMultiInputEventHandler *inputHandler = new AliMultiInputEventHandler();
  inputHandler->AddInputEventHandler(new AliAODInputHandler());
  mgr->SetInputEventHandler(inputHandler);

  // gProof->Load("TaskJpsiPP.cxx+");  // DON'T use double '+' when running multiple times: it uselessly recompiles everything!
  // gROOT->LoadMacro("addTaskJpsiPP.C");
  gSystem->Load("libEventMixing");
  gProof->Load("AliMixEventCutObj2.cxx+");
  gProof->Load("AliMixEventPool2.cxx+");
  gProof->Load("AliMixInputEventHandler2.cxx+");
  gProof->Load("TaskJpsiPP.cxx+");

  gROOT->LoadMacro("AddAMEventMixingTest.C");
  AddAMEventMixingTest();


  TString outputFileName;
  outputFileName.Form("AnalysisResults_run%09d.root", runNumber);
  //TaskJpsiPP *task = addTaskJpsiPP(kFALSE,outputFileName);

//TString str = gSystem->GetFromPipe("cat dataset.txt");

  if (!mgr->InitAnalysis()) return;
  mgr->StartAnalysis("proof",datasetWithRun, numEvents, firstEvent);

}
