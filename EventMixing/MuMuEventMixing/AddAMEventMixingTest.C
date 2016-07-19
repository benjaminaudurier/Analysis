Bool_t AddAMEventMixingTest(TString input="aod", TString postfix = "")
{

   Bool_t useEventMixingPar      = 0;
   Bool_t useMC = kFALSE;
   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMEventMixingTest] mgr is null !!!"); return kFALSE; }


//   AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
  // if (!analysisPlugin) { Printf("Error[AddAMEventMixingTest] : analysisPlugin is null !!!"); return kFALSE; }
   //
    //TString myAdditionalLibs;
  //  if (useEventMixingPar) { AliAnalysisAlien::SetupPar("EventMixing"); myAdditionalLibs += " EventMixing.par"; }
   gSystem->Load("libEventMixing");
   // myAdditionalLibs += " libEventMixing.so";

   gROOT->LoadMacro("AliMixEventCutObj2.cxx++g");
   gROOT->LoadMacro("AliMixEventPool2.cxx++g");
  gROOT->LoadMacro("AliMixInputEventHandler2.cxx++g");
   gROOT->LoadMacro("TaskJpsiPP.cxx++g");
   //analysisPlugin->SetAnalysisSource("TaskJpsiPP.cxx+");
   //myAdditionalLibs+=" TaskJpsiPP.h TaskJpsiPP.cxx AliMixEventPool2.h AliMixEventPool2.cxx AliMixEventCutObj2.h AliMixEventCutObj2.cxx AliMixInputEventHandler2.h AliMixInputEventHandler2.cxx";
   //analysisPlugin->SetAdditionalLibs(myAdditionalLibs.Data());


  AliMultiInputEventHandler *multiInputHandler = mgr->GetInputEventHandler();
  multiInputHandler->Init("proof");

   // create our task
   gROOT->LoadMacro("$ALICE_PHYSICS/OADB/COMMON/MULTIPLICITY/macros/AddTaskMultSelection.C");
   AliMultSelectionTask * taskCentrality = AddTaskMultSelection(kFALSE); // user mode:

  // add mixing handler (uncomment to turn on Mixnig)
  gROOT->LoadMacro("AddMixingHandler.C");
  AddMixingHandler(multiInputHandler, input, useMC,postfix);



//    // load and run AddTask macro
   gROOT->LoadMacro("AddEventMixingTestTask.C");
   AddEventMixingTestTask(input, useMC, postfix);

   return kTRUE;
}
