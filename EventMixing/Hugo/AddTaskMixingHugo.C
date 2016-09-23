///
/// Configuration example of a task to get mixed invariant mass spectrum of dimuons
///
/// \author: B. Audurier (Subatech) (benjamin.audurier - at - subatech.in2p3.fr)
///

AliAODInvMassEvaluationTask* AddTaskMixingHugo(const char* outputname)
{

    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager(); // Get the manager
    if (!mgr) {
        ::Error("AddTaskMixingHugo", "No analysis manager to connect to.");
        return NULL;
    }

    // Check the analysis type using the event handlers connected to the analysis manager.
    if (!mgr->GetInputEventHandler()) {
        ::Error("AddTaskMixingHugo", "This task requires an input event handler");
        return NULL;
    }

    // ROOT->LoadMacro( "AliAODInvMassEvaluationTask.cxx++O" );

    // invariant mass evaluation
    AliAODInvMassEvaluationTask *invMassEvaluation = new AliAODInvMassEvaluationTask( "InvMassEvaluation" );

    // trigger and physics selection for same event pairs
    invMassEvaluation->SetPhysicsSelectionMask( AliVEvent::kMUU7 | AliVEvent::kMUL7 );

    // trigger and physics selection for mixed event pairs
    // invMassEvaluation->SetPhysicsSelectionMaskMixing( AliVEvent::kMUU7 | AliVEvent::kMUL7 );
    invMassEvaluation->SetPhysicsSelectionMaskMixing( AliVEvent::kMUS7 );
    invMassEvaluation->SetMaxPoolSize( 20 );

    // add task to manager
    mgr->AddTask(invMassEvaluation);

    // Create containers for input/output
    AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

    AliAnalysisDataContainer *coutput = mgr->CreateContainer( "cList2", TList::Class(), AliAnalysisManager::kOutputContainer, outputname );
    
    analysisManager->ConnectInput( invMassEvaluation, 0, cinput );
    analysisManager->ConnectOutput( invMassEvaluation, 1, coutput );
}


