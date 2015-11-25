#ifndef __CINT__
#include <AliVEvent.h>
#endif

///Macro that create and configure an AliMixInputEventHandler (necessary for Mixing event purposes)

//______________________________________________________________________
Bool_t AddAMEventMixingTest(TString analysisMode = "test",TString analysisSource = "proof",TString input="aod",Bool_t inputMC="", TString postfix = "",TString idStr="0")
{
  
   // Int_t usePhysSel              = 0;
   
   //Flag for mc
   Bool_t useMC = inputMC;

   // ALICE stuff
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) { Printf("Error[AddAMEventMixingTest] mgr is null !!!"); return kFALSE; }
   
   
   // AliAnalysisGrid *analysisPlugin = mgr->GetGridHandler();
   // if (!analysisPlugin) { Printf("Error[AddAMEventMixingTest] : analysisPlugin is null !!!"); return kFALSE; }

   // TString myAdditionalLibs;
   // if (useEventMixingPar) { AliAnalysisAlien::SetupPar("EventMixing"); myAdditionalLibs += " EventMixing.par"; }
   // else { gSystem->Load("libEventMixing"); myAdditionalLibs += " libEventMixing.so"; }
   
   // gROOT->LoadMacro("AliAnalysisTaskEx02.cxx++g");
   // analysisPlugin->SetAnalysisSource("AliAnalysisTaskEx02.cxx+");
   // myAdditionalLibs+=" AliAnalysisTaskEx02.h AliAnalysisTaskEx02.cxx";
   // analysisPlugin->SetAdditionalLibs(myAdditionalLibs.Data());
   
   // Pointer to manager handler
   AliMultiInputEventHandler *multiInputHandler = mgr->GetInputEventHandler();

   // if (usePhysSel) {
   //    gROOT->LoadMacro("$ALICE_ROOT/OADB/macros/AddTaskPhysicsSelection.C");
   //    AddTaskPhysicsSelection(useMC);

   //    // maybe we can put it in $ALICE_ROOT/OADB/macros/AddTaskPhysicsSelection.C
   //    AliMultiInputEventHandler *multiIH = dynamic_cast<AliMultiInputEventHandler *>(mgr->GetInputEventHandler());
   //    if (multiIH){
   //       AliESDInputHandler *esdIH = dynamic_cast<AliESDInputHandler *>(multiIH->GetFirstInputEventHandler());
   //       if (esdIH) esdIH->SetEventSelection(multiIH->GetEventSelection());
   //       AliAODInputHandler *aodIH = dynamic_cast<AliAODInputHandler *>(multiIH->GetFirstInputEventHandler());
   //       if (aodIH) aodIH->SetEventSelection(multiIH->GetEventSelection());
   //    }
   // }

  // add mixing handler (uncomment to turn on Mixnig)
  AddMixingHandler(multiInputHandler, input, useMC,postfix);
  
  // load and run AddTask macro
  AddEventMixingTestTask(input, useMC, postfix);

   return kTRUE;
}

//______________________________________________________________________
void AddMixingHandler ( AliMultiInputEventHandler *multiInputHandler,TString format = "esd", Bool_t useMC = kFALSE, TString opts = "" ) 
{
    // Configure AliMultiInputEventHandler
    // 
   const Int_t bufferSize = 1;// Size of pool
   const Int_t mixNum = 5;// ??
   if ( !multiInputHandler ) return;

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   AliMixInputEventHandler *mixHandler = new AliMixInputEventHandler ( bufferSize, mixNum );
   mixHandler->SetInputHandlerForMixing ( dynamic_cast<AliMultiInputEventHandler *> ( mgr->GetInputEventHandler() ) );
   

   // configure pool
   AliMixEventPool *evPool = new AliMixEventPool();



   // AliMixEventCutObj *multi = new AliMixEventCutObj ( AliMixEventCutObj::kMultiplicity, 2, 102, 10 );
   // AliMixEventCutObj *zvertex = new AliMixEventCutObj ( AliMixEventCutObj::kZVertex, -10, 10, 1 );

   AliMixEventCutObj *centrality = new AliMixEventCutObj(AliMixEventCutObj::kCentrality, 0, 20, 10, "V0A");
//    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 102, 10);
//    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -5, 5, 1);
//
//    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 5002, 100);
//    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -10, 10, 1);
//
//     AliMixEventCutObj *centrality = new AliMixEventCutObj ( AliMixEventCutObj::kCentrality, 0, 100, 10, "V0M" );
//     AliMixEventCutObj *zvertex = new AliMixEventCutObj ( AliMixEventCutObj::kZVertex, -10, 10, 1 );

   evPool->AddCut(centrality);
   // evPool->AddCut(multi);
   // evPool->AddCut ( zvertex );

   // adds event pool (comment it and u will have default mixing)
   mixHandler->SetEventPool ( evPool );

   mixHandler->SelectCollisionCandidates(AliVEvent::kAny);
//    mixHandler->SelectCollisionCandidates(AliVEvent::kAnyINT);


//    mixHandler->DoMixIfNotEnoughEvents(kFALSE);

   multiInputHandler->AddInputEventHandler ( mixHandler );

   // adds mixing info task
   AddAnalysisTaskMixInfo (opts);

}

//______________________________________________________________________
AliAnalysisTask *AddEventMixingTestTask(TString format = "esd", Bool_t useMC = kFALSE,TString postfix="")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("MIX test");

   // create our task
   AliAnalysisTaskEx02 *task = new AliAnalysisTaskEx02("AliAnalysisTaskEx02");

   // create output container
   AliAnalysisDataContainer *output1 = mgr->CreateContainer("cEx2", TList::Class(), AliAnalysisManager::kOutputContainer, "MixTestOutput.root");

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, output1);


   return task;
}

//______________________________________________________________________
void AddAnalysisTaskMixInfo(TString opts = "")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) return;

   // create our task
   AliAnalysisTaskMixInfo *task = new AliAnalysisTaskMixInfo("AliAnalysisTaskMixInfo");
   Int_t debugLevel = 1;
   TString myclasses = "";
//    myclasses += ":AliAnalysisTaskMixInfo";
//    myclasses += ":AliAnalysisTaskEx02";

   if (!myclasses.IsNull()) task->SetLogType(AliLog::kDebug + debugLevel, myclasses);

   // create mix output container
   AliAnalysisDataContainer *outputMix = mgr->CreateContainer("cMixInfoList", TList::Class(), AliAnalysisManager::kOutputContainer, Form("MixInfo%s.root", opts.Data()));

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, outputMix);

}

