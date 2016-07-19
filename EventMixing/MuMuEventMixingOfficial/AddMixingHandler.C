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
   
   AliAnalysisManager          *mgr        = AliAnalysisManager::GetAnalysisManager();
   AliMixInputEventHandler     *mixHandler = new AliMixInputEventHandler ( bufferSize, mixNum );
   mixHandler->SetInputHandlerForMixing ( dynamic_cast<AliMultiInputEventHandler*> ( mgr->GetInputEventHandler() ) );
   AliMixEventPool             *evPool     = new AliMixEventPool();

  AliMixEventCutObj *centrality = new AliMixEventCutObj ( AliMixEventCutObj::kCentrality, 0, 90, 10, "V0M" );
  // AliMixEventCutObj     *multi      = new AliMixEventCutObj(AliMixEventCutObj::kMultiplicity, 2, 5002, 100);
  // AliMixEventCutObj     *zvertex    = new AliMixEventCutObj(AliMixEventCutObj::kZVertex, -10, 10, 1);
 
  evPool->AddCut(centrality);
  //evPool->AddCut(multi);
  
  // adds event pool (comment it and u will have default mixing)
  mixHandler->SetEventPool ( evPool );

  mixHandler->SelectCollisionCandidates(AliVEvent::kMUS7);

  // mixHandler->DoMixIfNotEnoughEvents(kTRUE);
  multiInputHandler->AddInputEventHandler ( mixHandler );
}
