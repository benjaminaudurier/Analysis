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
       triggerClassesToConsider->Add(new TObjString("ANY"));
       triggerClassesToConsider->Add(new TObjString("MB1"));
       triggerClassesToConsider->Add(new TObjString("C0T0A"));
       
       
       triggerClassesToConsider->Add(new TObjString("MULow"));
//    triggerClassesToConsider->Add(new TObjString("V0L"));
//    triggerClassesToConsider->Add(new TObjString("V0R"));
//
// for dpmjet simulations (at least) we have the following "triggers" :
//    C0T0A,C0T0C,MB1,MBBG1,V0L,V0R,MULow,EMPTY,MBBG3,MULL,MULU,MUHigh
  }
  //===========================================================================
  
  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  //===========================================================================
  triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);

  triggerInputsMap->Add(new TObjString("0VBA:0,"));
  triggerInputsMap->Add(new TObjString("0VBC:1,"));    
  triggerInputsMap->Add(new TObjString("0VLN:2,"));
  triggerInputsMap->Add(new TObjString("0ZAC:3,"));
  triggerInputsMap->Add(new TObjString("0BPC:4,"));
  triggerInputsMap->Add(new TObjString("0MUL:5,"));
  triggerInputsMap->Add(new TObjString("0MSL:6,"));
  triggerInputsMap->Add(new TObjString("0MLL:7,"));
  triggerInputsMap->Add(new TObjString("0MSH:8,"));
  triggerInputsMap->Add(new TObjString("0OMU:9,"));
  triggerInputsMap->Add(new TObjString("0MUH:10,"));
  triggerInputsMap->Add(new TObjString("0SMB:11,"));
  triggerInputsMap->Add(new TObjString("0SM2:12,"));
  triggerInputsMap->Add(new TObjString("0TVX:13,"));
  triggerInputsMap->Add(new TObjString("0LSR:14,"));
  triggerInputsMap->Add(new TObjString("0HWU:15,"));
  triggerInputsMap->Add(new TObjString("0PH0:16,"));
  triggerInputsMap->Add(new TObjString("0VHO:17,"));
  triggerInputsMap->Add(new TObjString("0VHN:18,"));
  triggerInputsMap->Add(new TObjString("0TCE:19,"));
  triggerInputsMap->Add(new TObjString("0TSC:20,"));
  triggerInputsMap->Add(new TObjString("0OM2:21,"));
  triggerInputsMap->Add(new TObjString("0BPA:22,"));
  triggerInputsMap->Add(new TObjString("0EMC:23,"));

  triggerInputsMap->Add(new TObjString("1EJE:0,"));
  triggerInputsMap->Add(new TObjString("1EGA:1,"));    
  triggerInputsMap->Add(new TObjString("1PHL:4,"));
  triggerInputsMap->Add(new TObjString("1PHM:5,"));
  triggerInputsMap->Add(new TObjString("1PHH:6,"));
  triggerInputsMap->Add(new TObjString("1HCO:8,"));
  triggerInputsMap->Add(new TObjString("1HJT:9,"));
  triggerInputsMap->Add(new TObjString("1HSE:10,"));
  triggerInputsMap->Add(new TObjString("1DUM:11,"));
  triggerInputsMap->Add(new TObjString("1ZMD:14,"));
  triggerInputsMap->Add(new TObjString("1ZMB:16,"));
  triggerInputsMap->Add(new TObjString("1ZED:17,"));
  triggerInputsMap->Add(new TObjString("1ZAC:18,"));

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
    ps = cr->AddEventCut(*eventCutter,"IsPhysicsS elected","const AliInputEventHandler&","");
  }

  // Apply default cut
  cr->AddCutCombination(triggerSelection);
  cr->AddCutCombination(eventTrue);
  cr->AddCutCombination(ps,triggerSelection);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0;// Basic histograms analysis;
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum

//  TFile f("$HOME/Downloads/ResponseMatrix_QASPDZPSALL_ANY.root");
//  TH2* h = static_cast<TH2*>(f.Get("ResponseMatrix"));
  
//  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch; // (0x0,-2,2,-40,40);
  
//  AliAnalysisMuMuNch(TH2* spdCorrection=0x0,
//                     Int_t nbinsEta=10, Double_t etaMin=-0.5, Double_t etaMax=0.5,
//                     Int_t nbinsZ=320, Double_t zmin=-40, Double_t zmax=40);
    
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
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&,",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    // Create combination of cuts to apply
    // cr->AddCutCombination(trackTrue);
    // cr->AddCutCombination(matchlow);
    cr->AddCutCombination(rabs,eta,matchlow,pdca,trackTrue,ps); 
    // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis); 

    if ( minvAnalysis )
    {
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&, const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&, const AliVParticle&, Double_t&, Double_t&","0.,18.");
      
      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      cutElements.Add(pdca);
      cutElements.Add(ps);
      cutElements.Add(trackTrue);
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
    binning->AddBin("psi","pt", 6.0, 8.0,"BENJ");
  
    // y binning
   binning->AddBin("psi","y",-4,-3.75,"BENJ");
   binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
   binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
   binning->AddBin("psi","y",-3.25,-3,"BENJ");
   binning->AddBin("psi","y",-3,-2.75,"BENJ");
   binning->AddBin("psi","y",-2.75,-2.5,"BENJ");
  }

  // v0 centrality binning
  binning->AddBin("centrality","v0M",0.,7.5);
  binning->AddBin("centrality","v0M",10.,50.);
  binning->AddBin("centrality","v0M",0.,90.); 
  binning->AddBin("centrality","v0M",0.,10.);
  binning->AddBin("centrality","v0M",10.,20.);
  binning->AddBin("centrality","v0M",20.,30.);
  binning->AddBin("centrality","v0M",30.,40.);
  binning->AddBin("centrality","v0M",40.,50.);
  binning->AddBin("centrality","v0M",50.,60.);
  binning->AddBin("centrality","v0M",60.,70.);
  binning->AddBin("centrality","v0M",70.,80.);
  binning->AddBin("centrality","v0M",80.,90.);
  binning->AddBin("centrality","v0M",70.,90.);
  
  // binning->AddBin("centrality","v0a",90,100);



  // disable some histograms if we don't want them
//  task->DisableHistograms("^V02D");
//  task->DisableHistograms("^dca");
//  task->DisableHistograms("^Chi12");
//  task->DisableHistograms("^Rabs12");

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

