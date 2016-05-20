///
/// Configuration example of a task to get invariant mass spectrum of dimuons
///
/// \author: L. Aphecetche (Subatech) (laurent.aphecetche - at - subatech.in2p3.fr)
///

AliAnalysisTask* AddTaskMuMu(const char* outputname,
                             TList* triggerClassesToConsider,
                             const char* beamYear,
                             Bool_t simulations)
{
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager(); // Get the manager
  if (!mgr) {
    ::Error("AddTaskMuMu", "No analysis manager to connect to.");
    return NULL;
  }

  // Check the analysis type using the event handlers connected to the analysis manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskMuMu", "This task requires an input event handler");
    return NULL;
  }

  // Add new trigger in case of Simulation
  //===========================================================================
  if (simulations && triggerClassesToConsider )
  {

      triggerClassesToConsider->Add(new TObjString("ANY"));
      triggerClassesToConsider->Add(new TObjString("CMSNGL"));
      triggerClassesToConsider->Add(new TObjString("MUHigh"));
      triggerClassesToConsider->Add(new TObjString("CMULLO"));
      triggerClassesToConsider->Add(new TObjString("CMULHI"));
      triggerClassesToConsider->Add(new TObjString("CMLKLO"));
      triggerClassesToConsider->Add(new TObjString("CMLKHI"));
      triggerClassesToConsider->Add(new TObjString("MULow"));
      triggerClassesToConsider->Add(new TObjString("MUHigh"));
      triggerClassesToConsider->Add(new TObjString("MULU"));
      triggerClassesToConsider->Add(new TObjString("MULL"));
      triggerClassesToConsider->Add(new TObjString("MUHU"));
      triggerClassesToConsider->Add(new TObjString("MUHL"));
//
// for dpmjet simulations (at least) we have the following "triggers" :
//    C0T0A,C0T0C,MB1,MBBG1,V0L,V0R,MULow,EMPTY,MBBG3,MULL,MULU,MUHigh
  }
  //===========================================================================

  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  //===========================================================================
  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);

  triggerInputsMap->Add(new TObjString("0MSL:17"));
  triggerInputsMap->Add(new TObjString("0MSH:18"));
  triggerInputsMap->Add(new TObjString("0MLL:19"));
  triggerInputsMap->Add(new TObjString("0MUL:20"));
  triggerInputsMap->Add(new TObjString("0V0M:3"));
  triggerInputsMap->Add(new TObjString("0TVX:2"));

  //===========================================================================

  //  Configure task
  //===========================================================================
  AliAnalysisTaskMuMu       * task = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Set CutRegistry

  // Default cuts on trigger and event level
  AliAnalysisMuMuCutElement * eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement* ps = eventTrue;

  // Apply default cut
  cr->AddCutCombination(triggerSelection);
  cr->AddCutCombination(ps);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0 /*new AliAnalysisMuMuGlobal*/; // Basic histograms analysis;
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum
    
  // Configure sub analysis
  if ( globalAnalysis ){
    // Cuts on trigger level
    AliAnalysisMuMuCutElement* triggerAll = cr->AddTriggerClassCut(*globalAnalysis,"SelectAnyTriggerClass","const TString&,TString&","");
    // Adding this cut on trigger level  
    cr->AddCutCombination(triggerAll);
    task->AdoptSubAnalysis(globalAnalysis);
  }
  if ( singleAnalysis ){
    // Cuts on tracks
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    
    // Create combination of cuts to apply
    // cr->AddCutCombination(trackTrue,matchlow,rabs,eta);
    // cr->AddCutCombination(trackTrue,matchlow,rabs,eta,pdca); 
    // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis); 

    if ( minvAnalysis ){
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.0,0.3");

      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      cutElements.Add(pdca);
      // add them
      cr->AddCutCombination(cutElements);    
      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis); 
    }
  }

  /// below are the kind of configurations that can be performed :
  /// - adding cuts (at event, track or pair level)
  /// - adding bins (in pt, y, centrality, etc...) for minv (and meanpt)

  // Configure sub analysis
  //===========================================================================
  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"

  if (minvAnalysis) binning->AddBin("psi","integrated");

  // v0 centrality binning
  binning->AddBin("centrality","pp");

  // add the configured task to the analysis manager
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  AliAnalysisDataContainer *coutputHC =
  mgr->CreateContainer("OC",AliMergeableCollection::Class(),AliAnalysisManager::kOutputContainer,outputname);

  AliAnalysisDataContainer *coutputCC =
  mgr->CreateContainer("CC",AliCounterCollection::Class(),AliAnalysisManager::kOutputContainer,outputname);

  AliAnalysisDataContainer* cparam =
  mgr->CreateContainer("BIN", AliAnalysisMuMuBinning::Class(),AliAnalysisManager::kParamContainer,outputname);

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutputHC);
  mgr->ConnectOutput(task, 2, coutputCC);
  mgr->ConnectOutput(task, 3, cparam);

  return task;
}
