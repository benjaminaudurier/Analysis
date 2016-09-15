#ifndef __CINT__
#include <AliVEvent.h>
#include "AliMixEventCutObj.h"
#include "AliMixEventPool.h"
#include "AliMixInputEventHandler.h"
#endif

void AddMixingHandler ( AliMultiInputEventHandler *multiInputHandler, Bool_t useMC = kFALSE, Int_t bufferSize = 20, Int_t mixNum = 1, TString opts = "" ) 
{

   if ( !multiInputHandler )  {
    ::Error("AddTaskMuMu", "No analysis manager to connect to.");
    return NULL;
  }
   
   AliAnalysisManager* mgr = AliAnalysisManager::GetAnalysisManager();

   // ---- create and configure handler for mixing ----
   
  AliMixInputEventHandler* mixHandler = new AliMixInputEventHandler(bufferSize,mixNum);
  mixHandler->SetInputHandlerForMixing(dynamic_cast<AliMultiInputEventHandler*>(mgr->GetInputEventHandler())); // make <buffer> copy of the main input handler (ALiAOD, AliESDs...) and store them in a data member
  mixHandler->SelectCollisionCandidates(AliVEvent::kMuonSingleLowPt7);

  // ---- create, configure and add the event pool  ----
  
  AliMixEventPool  *evPool     = new AliMixEventPool();
  AliMixEventCutObj*centrality = new AliMixEventCutObj( AliMixEventCutObj::kCentrality, 0, 90, 10, "V0M" );
  // AliMixEventCutObj     *multi      = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 5002, 100);
  // AliMixEventCutObj     *zvertex    = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -10, 10, 1);
 
  evPool->AddCut(centrality);
  // adds event pool (comment it and u will have default mixing)
  mixHandler->SetEventPool(evPool);
  // mixHandler->DoMixIfNotEnoughEvents(kTRUE);
  
  // Finaly add mixHandler to our AliMultiInputEventHandler
  multiInputHandler->AddInputEventHandler(mixHandler);
}
