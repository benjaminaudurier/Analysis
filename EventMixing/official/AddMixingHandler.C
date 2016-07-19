#ifndef __CINT__
#include <AliVEvent.h>
#include "AliMixEventCutObj2.h"
#include "AliMixEventPool2.h"
#include "AliMixInputEventHandler2.h"
#endif
void AddMixingHandler ( AliMultiInputEventHandler *multiInputHandler,TString format = "aod", Bool_t useMC = kFALSE, TString opts = "" ) {

   const Int_t bufferSize = 20;
   const Int_t mixNum = 1;
   if ( !multiInputHandler ) return;

   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   AliMixInputEventHandler2 *mixHandler = new AliMixInputEventHandler2 ( bufferSize, mixNum );
   mixHandler->SetInputHandlerForMixing ( dynamic_cast<AliMultiInputEventHandler *> ( mgr->GetInputEventHandler() ) );
   AliMixEventPool2 *evPool = new AliMixEventPool2();


//    AliMixEventCutObj *multi = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 5002, 100);
//    AliMixEventCutObj *zvertex = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -10, 10, 1);
//
      AliMixEventCutObj2 *centrality = new AliMixEventCutObj2 ( AliMixEventCutObj2::kCentrality, 0, 90, 9, "V0M" );


//    evPool->AddCut(centrality);
   //evPool->AddCut(multi);
   evPool->AddCut (centrality);

   // adds event pool (comment it and u will have default mixing)
   mixHandler->SetEventPool ( evPool );

   mixHandler->SelectCollisionCandidates(AliVEvent::kMUS7);
//    mixHandler->SelectCollisionCandidates(AliVEvent::kAnyINT);


   //mixHandler->DoMixIfNotEnoughEvents(kTRUE);

   multiInputHandler->AddInputEventHandler ( mixHandler );

   // adds mixing info task
  //  gROOT->LoadMacro ( "AddAnalysisTaskMixInfo.C" );
  //  AddAnalysisTaskMixInfo (opts);

}
