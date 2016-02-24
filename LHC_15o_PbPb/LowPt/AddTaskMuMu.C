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
  TString inputDataType = mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"
  cout << "inputDataType = " << inputDataType.Data() << endl;
  
    
  // Add new trigger in case of Simulation
  //===========================================================================
  if (simulations && triggerClassesToConsider )
  {
      // possible entries for LHC15n :
      // ANY,
      // CMSNGL:Lpt,
      // MUHigh:Hpt,
      // CMULLO:Lpt2,
      // CMULHI:Hpt2,
      // CMLKLO:Lpt2,
      // CMLKHI:Hpt2,
      // MULow:Lpt,
      // MUHigh:Hpt,
      // MULU:Lpt2,
      // MULL:Lpt2,
      // MUHU:Hpt2,
      // MUHL:Hpt2
      
      // triggerClassesToConsider->Add(new TObjString("ANY"));
      // triggerClassesToConsider->Add(new TObjString("CMSNGL"));
      // triggerClassesToConsider->Add(new TObjString("MUHigh"));
      // triggerClassesToConsider->Add(new TObjString("CMULLO"));
      // triggerClassesToConsider->Add(new TObjString("CMULHI"));
      // triggerClassesToConsider->Add(new TObjString("CMLKLO"));
      // triggerClassesToConsider->Add(new TObjString("CMLKHI"));
      // triggerClassesToConsider->Add(new TObjString("MULow"));
      triggerClassesToConsider->Add(new TObjString("MUHigh"));
      // triggerClassesToConsider->Add(new TObjString("MULU"));
      // triggerClassesToConsider->Add(new TObjString("MULL"));
      // triggerClassesToConsider->Add(new TObjString("MUHU"));
      // triggerClassesToConsider->Add(new TObjString("MUHL"));  
  }
  //===========================================================================

  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  //===========================================================================
  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);
  
  triggerInputsMap->Add(new TObjString("0MSL:5")); 
  triggerInputsMap->Add(new TObjString("0MSH:6")); 
  triggerInputsMap->Add(new TObjString("0MUL:13")); 
  triggerInputsMap->Add(new TObjString("0MUH:14")); 
  triggerInputsMap->Add(new TObjString("0MLL:15")); 
  triggerInputsMap->Add(new TObjString("0MLH:16")); 

  //  Configure task
  //===========================================================================
  AliAnalysisTaskMuMu       * task = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Set CutRegistry

  // Default cuts on trigger and event level
  AliAnalysisMuMuCutElement * eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&",""); 
  AliAnalysisMuMuCutElement * triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");

  AliAnalysisMuMuCutElement* ps = eventTrue;
  
  if (!simulations)
  {
    ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliInputEventHandler&","");
  }  
  // Apply default cut
  cr->AddCutCombination(triggerSelection);
  cr->AddCutCombination(ps,triggerSelection);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = new AliAnalysisMuMuGlobal ;// Basic histograms analysis;
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum

  // Configure sub analysis
  //===========================================================================
  if ( globalAnalysis )
  {
    // Cuts on trigger level
    AliAnalysisMuMuCutElement* triggerAll = cr->AddTriggerClassCut(*globalAnalysis,"SelectAnyTriggerClass","const TString&,TString&","");
    // Adding this cut on trigger level  
    cr->AddCutCombination(triggerAll);
    task->AdoptSubAnalysis(globalAnalysis);
  }
  if ( singleAnalysis )
  {
    // Cuts on tracks
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    // Create combination of cuts to apply
    cr->AddCutCombination(rabs,eta,matchlow,pdca,trackTrue); 
    // // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis); 

    if ( minvAnalysis )
    {
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&, const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,18.");
      
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
  
  if (minvAnalysis)
  {
    minvAnalysis->DefineMinvRange(2,8,0.025);
    // Integrated
    binning->AddBin("psi","integrated");
    // pt binning for low pt exces
    binning->AddBin("psi","pt", 0.0, 0.1,"BENJ");
    binning->AddBin("psi","pt", 0.1, 0.2,"BENJ");
    binning->AddBin("psi","pt", 0.2, 0.3,"BENJ");
    binning->AddBin("psi","pt", 0.3, 0.4,"BENJ");
    binning->AddBin("psi","pt", 0.4, 0.5,"BENJ");
    binning->AddBin("psi","pt", 0.5, 0.6,"BENJ");
    binning->AddBin("psi","pt", 0.6, 0.7,"BENJ");
    binning->AddBin("psi","pt", 0.7, 0.8,"BENJ");
    binning->AddBin("psi","pt", 0.8, 0.9,"BENJ");
    binning->AddBin("psi","pt", 0.9, 1.0,"BENJ");
    binning->AddBin("psi","pt", 1.0, 1.1,"BENJ");
    binning->AddBin("psi","pt", 1.1, 1.2,"BENJ");
    binning->AddBin("psi","pt", 1.2, 1.3,"BENJ");
    binning->AddBin("psi","pt", 1.3, 1.4,"BENJ");
    binning->AddBin("psi","pt", 1.4, 1.5,"BENJ");
    binning->AddBin("psi","pt", 1.5, 1.6,"BENJ");
    binning->AddBin("psi","pt", 1.6, 1.7,"BENJ");
    binning->AddBin("psi","pt", 1.7, 1.8,"BENJ");
    binning->AddBin("psi","pt", 0.0, 0.3,"BENJ");
    binning->AddBin("psi","pt", 0.3, 1.0,"BENJ");
    binning->AddBin("psi","pt", 1.0, 8.0,"BENJ");
    binning->AddBin("psi","pt", 0.3, 8.0,"BENJ");

    // y binning
    binning->AddBin("psi","y",-4,-3.75,"BENJ");
    binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
    binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
    binning->AddBin("psi","y",-3.25,-3,"BENJ");
    binning->AddBin("psi","y",-3,-2.75,"BENJ");
    binning->AddBin("psi","y",-2.75,-2.5,"BENJ");
  }

  // v0 centrality binning for low pt exces
  binning->AddBin("centrality","V0M",0.,90.);
  binning->AddBin("centrality","V0M",0.,10.);
  binning->AddBin("centrality","V0M",10.,30.);
  binning->AddBin("centrality","V0M",30.,50.);
  binning->AddBin("centrality","V0M",50.,70.);
  binning->AddBin("centrality","V0M",70.,90.);
 
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

