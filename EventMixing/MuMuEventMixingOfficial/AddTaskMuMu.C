///
/// Configuration example of a task to get invariant mass spectrum of dimuons
///
/// \author: L. Aphecetche (Subatech) (laurent.aphecetche - at - subatech.in2p3.fr)
///

AliAnalysisTaskMuMu* AddTaskMuMu(const char* outputname,
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
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskMuMu", "This task requires an input event handler");
    return NULL;
  }

  // Add new trigger in case of Simulation
  if (simulations && triggerClassesToConsider ) triggerClassesToConsider->Add(new TObjString("ANY"));



  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);
  triggerInputsMap->Add(new TObjString("0MSL:17"));
  triggerInputsMap->Add(new TObjString("0MSH:18"));
  triggerInputsMap->Add(new TObjString("0MLL:19"));
  triggerInputsMap->Add(new TObjString("0MUL:20"));
  triggerInputsMap->Add(new TObjString("0V0M:3"));
  triggerInputsMap->Add(new TObjString("0TVX:2"));

  //  Configure task
  AliAnalysisTaskMuMu       * task        = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr          = task->CutRegistry(); // Set CutRegistry

  // Default cuts on trigger and event level
  AliAnalysisMuMuCutElement * eventTrue        = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement * ps               = eventTrue;
  AliAnalysisMuMuCutElement * ps1              = eventTrue;
  AliAnalysisMuMuCutElement * ps2              = eventTrue;

  if (!simulations){
    ps  = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedMSL","const AliInputEventHandler&","");
    ps1 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedMUL","const AliInputEventHandler&","");
    ps2 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedMULORMLL","const AliInputEventHandler&","");
  }

  // Apply default cut
  // cr->AddCutCombination(triggerSelection);
  // cr->AddCutCombination(eventTrue);
  cr->AddCutCombination(ps,triggerSelection);
  cr->AddCutCombination(ps1,triggerSelection);

  task->SetBeamYear(beamYear);

  // AliAnalysisMuMuGlobal* globalAnalysis = new AliAnalysisMuMuGlobal; // Basic histograms analysis;
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis   = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum

  // Configure sub analysis
  // if ( globalAnalysis ) {

  //   AliAnalysisMuMuCutElement* triggerAll = cr->AddTriggerClassCut(*globalAnalysis,"SelectAnyTriggerClass","const TString&,TString&","");
  //   cr->AddCutCombination(triggerAll);
  //   task->AdoptSubAnalysis(globalAnalysis);
  // }

  if ( singleAnalysis ){

    // Cuts on tracks
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs      = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow  = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta       = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");

    // Create combination of cuts to apply
    cr->AddCutCombination(trackTrue,rabs,matchlow,eta);
    // cr->AddCutCombination(trackTrue,rabs,matchlow,eta,pdca);
    
    // Disable some histo. Comment the one you want
    // singleAnalysis->DisableHistograms("BCX");
    singleAnalysis->DisableHistograms("EtaRapidityMu*");
    singleAnalysis->DisableHistograms("PtEtaMu*");
    // singleAnalysis->DisableHistograms("PtRapidityMu*");
    singleAnalysis->DisableHistograms("PEtaMu*");
    singleAnalysis->DisableHistograms("PtPhiMu*");
    // singleAnalysis->DisableHistograms("Chi2Mu*");
    singleAnalysis->DisableHistograms("dcaP23Mu*");
    singleAnalysis->DisableHistograms("dcaPwPtCut23Mu*");
    singleAnalysis->DisableHistograms("dcaP310Mu*");
    singleAnalysis->DisableHistograms("dcaPwPtCut310Mu*");

    // separate positive and negative muons
    singleAnalysis->ShouldSeparatePlusAndMinus(kTRUE);

    // Adding the sub analysis
    // task->AdoptSubAnalysis(singleAnalysis);

    if ( minvAnalysis ){

      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy    = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange  = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,8.");
      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      
      // add them
      cr->AddCutCombination(cutElements);

      // Disable some histo. Comment the one you want
      minvAnalysis->DisableHistograms("Eta");
      // minvAnalysis->DisableHistograms("Pt");
      // minvAnalysis->DisableHistograms("Y");

      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }

  // Configure sub analysis
  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"

  if (minvAnalysis){
    // Integrated
    binning->AddBin("psi","integrated");

   //   // pt binning
    binning->AddBin("psi","pt", 0.0, 1.0,"BENJ");
   //  binning->AddBin("psi","pt", 1.0, 2.0,"BENJ");
   //  binning->AddBin("psi","pt", 2.0, 3.0,"BENJ");
   //  binning->AddBin("psi","pt", 3.0, 4.0,"BENJ");
   //  binning->AddBin("psi","pt", 4.0, 5.0,"BENJ");
   //  binning->AddBin("psi","pt", 5.0, 6.0,"BENJ");
   //  binning->AddBin("psi","pt", 6.0, 8.0,"BENJ");

   //  // y binning
   // binning->AddBin("psi","y",-4,-3.75,"BENJ");
   // binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
   // binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
   // binning->AddBin("psi","y",-3.25,-3,"BENJ");
   // binning->AddBin("psi","y",-3,-2.75,"BENJ");
   // binning->AddBin("psi","y",-2.75,-2.5,"BENJ");
  }


  // v0 centrality binning
  // binning->AddBin("centrality","V0M",0.,90.);
  binning->AddBin("centrality","V0M",0.,10.);
  binning->AddBin("centrality","V0M",10.,20.);
  binning->AddBin("centrality","V0M",20.,30.);
  binning->AddBin("centrality","V0M",30.,40.);
  binning->AddBin("centrality","V0M",40.,50.);
  binning->AddBin("centrality","V0M",50.,60.);
  binning->AddBin("centrality","V0M",60.,70.);
  binning->AddBin("centrality","V0M",70.,80.);
  binning->AddBin("centrality","V0M",80.,90.);

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
