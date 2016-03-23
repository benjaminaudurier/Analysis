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

  //===========================================================================

  //  Configure task
  //===========================================================================
  AliAnalysisTaskMuMu       * task = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Set CutRegistry

  // task->SelectCollisionCandidates(AliVEvent::kINT7);

  // Default cuts on trigger and event level
  AliAnalysisMuMuCutElement * eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement* ps = eventTrue;
  AliAnalysisMuMuCutElement* ps1 = eventTrue;

  if (!simulations)
  {
    ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliInputEventHandler&","");
    ps1 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedNew","const AliInputEventHandler&","");
    ps2 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedNewAnyINT","const AliInputEventHandler&","");
  }

  // Apply default cut
  cr->AddCutCombination(triggerSelection);
  cr->AddCutCombination(eventTrue);
  cr->AddCutCombination(ps,triggerSelection);
  cr->AddCutCombination(ps1,triggerSelection);
  cr->AddCutCombination(ps2,triggerSelection);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0 /*new AliAnalysisMuMuGlobal*/; // Basic histograms analysis;
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
    cr->AddCutCombination(trackTrue);
    cr->AddCutCombination(matchlow);
    cr->AddCutCombination(rabs,eta);
    cr->AddCutCombination(pdca);
    // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis);

    if ( minvAnalysis )
    {
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,12.");

      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      cutElements.Add(pdca);
      // cutElements.Add(ps);
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
    binning->AddBin("psi","pt", 0.0, 1.0,"BENJ");
    binning->AddBin("psi","pt", 1.0, 2.0,"BENJ");
    binning->AddBin("psi","pt", 2.0, 3.0,"BENJ");
    binning->AddBin("psi","pt", 3.0, 4.0,"BENJ");
    binning->AddBin("psi","pt", 4.0, 5.0,"BENJ");
    binning->AddBin("psi","pt", 5.0, 6.0,"BENJ");
    binning->AddBin("psi","pt", 6.0, 7.0,"BENJ");
    binning->AddBin("psi","pt", 7.0, 8.0,"BENJ");
    binning->AddBin("psi","pt", 8.0, 9.0,"BENJ");
    binning->AddBin("psi","pt", 9.0, 10.0,"BENJ");
    binning->AddBin("psi","pt", 10.0, 12.0,"BENJ");
    // binning->AddBin("psi","pt", 11.0, 12.0,"BENJ");
    // binning->AddBin("psi","pt", 12.0, 13.0,"BENJ");
    // binning->AddBin("psi","pt", 13.0, 14.0,"BENJ");
    // binning->AddBin("psi","pt", 14.0, 15.0,"BENJ");
    // binning->AddBin("psi","pt", 15.0, 16.0,"BENJ");
    // binning->AddBin("psi","pt", 16.0, 17.0,"BENJ");
    // binning->AddBin("psi","pt", 17.0, 18.0,"BENJ");
    // binning->AddBin("psi","pt", 18.0, 19.0,"BENJ");
    // binning->AddBin("psi","pt", 19.0, 20.0,"BENJ");

    // y binning
   binning->AddBin("psi","y",-4,-3.75,"BENJ");
   binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
   binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
   binning->AddBin("psi","y",-3.25,-3,"BENJ");
   binning->AddBin("psi","y",-3,-2.75,"BENJ");
   binning->AddBin("psi","y",-2.75,-2.5,"BENJ");

   // binning->AddBin("psi","y",-4,-3.5,"BENJ");
   // binning->AddBin("psi","y",-3.5,-3.,"BENJ");
   // binning->AddBin("psi","y",-3,-2.5,"BENJ");

    // phi binning
   // binning->AddBin("psi","phi",0,0.4,"BENJ");
   // binning->AddBin("psi","phi",0.4,0.8,"BENJ");
   // binning->AddBin("psi","phi",0.8,1.2,"BENJ");
   // binning->AddBin("psi","phi",1.2,1.6,"BENJ");
   // binning->AddBin("psi","phi",1.6,2,"BENJ");
   // binning->AddBin("psi","phi",2,2.4,"BENJ");
   // binning->AddBin("psi","phi",2.4,2.8,"BENJ");
   // binning->AddBin("psi","phi",2.4,2.8,"BENJ");
   // binning->AddBin("psi","phi",2.8,3.14,"BENJ");


   // yvspt bin
  // for ( Int_t i = 0; i < 3; ++i ) {
  //   Double_t dy= 0.5;
  //   Double_t y = -4+i*dy;

  //   for (int j = 0; j < 8; ++j) {
  //     Double_t dpt =1.;
  //     Double_t pt = 0.+j*dpt;
  //     binning->AddBin("psi","yvspt",pt, pt+dpt,y,y+dy,"BENJ");
  //     // printf("Pt interval  =[%f,%f] y interval  =[%f,%f]\n",pt,pt+dpt,y,y+dy);
  //   }
  // }

  }


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
