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
  cout << "inputDataType=" << inputDataType.Data() << endl;
  
    
  // Add new trigger in case of Simulation
  //===========================================================================
  if (simulations && triggerClassesToConsider )
  {
//    triggerClassesToConsider->Add(new TObjString("CMULLO-B-NOPF-MUON"));
//    triggerClassesToConsider->Add(new TObjString("CMSNGL-B-NOPF-MUON"));
//    triggerClassesToConsider->Add(new TObjString("V0L"));
//    triggerClassesToConsider->Add(new TObjString("V0R"));
//
// for dpmjet simulations (at least) we have the following "triggers" :
//    C0T0A,C0T0C,MB1,MBBG1,V0L,V0R,MULow,EMPTY,MBBG3,MULL,MULU,MUHigh
  }
  //===========================================================================
  
  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  //===========================================================================
  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);
  
  // triggerInputsMap->Add(new TObjString("0VBA:1,")); 
 triggerInputsMap->Add(new TObjString("0VBA:1"));
 triggerInputsMap->Add(new TObjString("0VBC:2"));
 triggerInputsMap->Add(new TObjString("0TVX:3"));
triggerInputsMap->Add(new TObjString("0V0M:4"));
triggerInputsMap->Add(new TObjString("0T0A:5"));
triggerInputsMap->Add(new TObjString("0SMB:6"));
triggerInputsMap->Add(new TObjString("0UBA:7"));
triggerInputsMap->Add(new TObjString("0UBC:8"));
triggerInputsMap->Add(new TObjString("0PH0:9"));
triggerInputsMap->Add(new TObjString("0T0C:10"));
triggerInputsMap->Add(new TObjString("0VHM:11"));
triggerInputsMap->Add(new TObjString("0OMU:12"));
triggerInputsMap->Add(new TObjString("0SH1:13"));
triggerInputsMap->Add(new TObjString("0STP:14"));
triggerInputsMap->Add(new TObjString("0BPA:15"));
triggerInputsMap->Add(new TObjString("0BPC:16"));
triggerInputsMap->Add(new TObjString("0EMC:17"));
triggerInputsMap->Add(new TObjString("0MSL:18"));
triggerInputsMap->Add(new TObjString("0MSH:19"));
triggerInputsMap->Add(new TObjString("0MLL:20"));
triggerInputsMap->Add(new TObjString("0MUL:21"));
triggerInputsMap->Add(new TObjString("0OM2:22"));
triggerInputsMap->Add(new TObjString("0DMC:23"));
triggerInputsMap->Add(new TObjString("0LSR:24"));
triggerInputsMap->Add(new TObjString("1EJ1:1"));
triggerInputsMap->Add(new TObjString("1EG1:2"));
triggerInputsMap->Add(new TObjString("1EJ2:3"));
triggerInputsMap->Add(new TObjString("1EG2:4"));
triggerInputsMap->Add(new TObjString("1PHL:5"));
triggerInputsMap->Add(new TObjString("1PHM:6"));
triggerInputsMap->Add(new TObjString("1PHH:7"));
triggerInputsMap->Add(new TObjString("1HCO:9"));
triggerInputsMap->Add(new TObjString("1HJT:10"));
triggerInputsMap->Add(new TObjString("1HSE:11"));
triggerInputsMap->Add(new TObjString("1H12:12"));
triggerInputsMap->Add(new TObjString("1HQU:13"));
triggerInputsMap->Add(new TObjString("1H14:14"));
triggerInputsMap->Add(new TObjString("1ZED:15"));
triggerInputsMap->Add(new TObjString("1ZMS:16"));
triggerInputsMap->Add(new TObjString("1ZMB:17"));
triggerInputsMap->Add(new TObjString("1ZMD:18"));
triggerInputsMap->Add(new TObjString("1ZAC:19"));
triggerInputsMap->Add(new TObjString("1DJ1:20"));
triggerInputsMap->Add(new TObjString("1DG1:21"));
triggerInputsMap->Add(new TObjString("1DJ2:22"));
triggerInputsMap->Add(new TObjString("1DG2:23"));
triggerInputsMap->Add(new TObjString("2DUM:12"));

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
  
  if (!simulations)
  {
    ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliInputEventHandler&","");
  }

  // Apply default cut
  cr->AddCutCombination(eventTrue);
  cr->AddCutCombination(ps);
  cr->AddCutCombination(triggerSelection);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = new AliAnalysisMuMuGlobal; // Basic histograms analysis;
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
    
    cr->AddCutCombination(trackTrue);
    task->AdoptSubAnalysis(singleAnalysis); 

    if ( minvAnalysis )
    {
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,10.");

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

    // Integrated
    binning->AddBin("psi","integrated");

    // pt binning
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
   
  }

  // v0 centrality binning  
  binning->AddBin("centrality","V0M",0.,10.);
  binning->AddBin("centrality","V0M",10.,30.);
  binning->AddBin("centrality","V0M",30.,50.);
  binning->AddBin("centrality","V0M",50.,70.);
  binning->AddBin("centrality","V0M",70.,90.);


  task->Print("-");

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

