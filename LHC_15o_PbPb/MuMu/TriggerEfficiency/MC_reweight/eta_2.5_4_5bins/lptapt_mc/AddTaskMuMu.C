///
/// Configuration example of a task to get invariant mass spectrum of dimuons
///
/// \author: L. Aphecetche (Subatech) (laurent.aphecetche - at - subatech.in2p3.fr)
///

AliAnalysisTaskMuMu* AddTaskMuMu(const char* outputname,
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

  // =========== list of trigger =========

  // --- Standard trigger ---

  TList* triggers = new TList;
  triggers->SetOwner(kTRUE);
  triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));

  // ========= Configure inputmaps (Default on is in AliMuonEventCuts) ========

  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);
  triggerInputsMap->Add(new TObjString("0MSL:17"));
  triggerInputsMap->Add(new TObjString("0MSH:18"));
  triggerInputsMap->Add(new TObjString("0MLL:19"));
  triggerInputsMap->Add(new TObjString("0MUL:20"));
  triggerInputsMap->Add(new TObjString("0V0M:3"));
  triggerInputsMap->Add(new TObjString("0TVX:2"));

  // =========


  // ========= Create task and subtask =========
  AliAnalysisTaskMuMu  * task             = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuSingle* singleAnalysis   = new AliAnalysisMuMuSingle;// Analysis dealing with single muon (for single muon cuts)
  AliAnalysisMuMuMinv  * minvAnalysis     = new AliAnalysisMuMuMinv;// For pair cut



  AliAnalysisMuMuTriggerResponse* triggResponse    =  0x0;// Analysis dealing with single muon
  AliMergeableCollection* coll = 0x0;
  TFile *f = TFile::Open("ReferenceResults_MC.root");
  f->GetObject("OC",coll);
  if(!coll)
  {
    printf("cannot Get mergeable Collection \n");
    return;
  } else triggResponse = new AliAnalysisMuMuTriggerResponse(coll);
  f->Close();


  task->SetBeamYear(beamYear);
  // =========


  // ========= Configure Cut registry =========

  // --- standard Analysis ---

  AliAnalysisMuMuEventCutter* eventCutter         = new AliAnalysisMuMuEventCutter(triggers,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr                  = task->CutRegistry(); // Set CutRegistry

  AliAnalysisMuMuCutElement * eventTrue           = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection    = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement * ps                  = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedINT7inMUON","const AliInputEventHandler&","");
  cr->AddCutCombination(ps,triggerSelection);


  if ( singleAnalysis ){

    // --- Create combination of cuts to apply ---
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs      = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchany  = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerAnyPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta       = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca      = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    cr->AddCutCombination(trackTrue,rabs,eta,matchany,pdca);

    // disable
    singleAnalysis->DisableHistograms("BCX");
    singleAnalysis->DisableHistograms("Chi2MatchTrigger");
    singleAnalysis->DisableHistograms("EtaRapidityMu*");
    singleAnalysis->DisableHistograms("PtEtaMu*");
    singleAnalysis->DisableHistograms("BCX");
    singleAnalysis->DisableHistograms("PtRapidityMu*");
    singleAnalysis->DisableHistograms("PEtaMu*");
    singleAnalysis->DisableHistograms("PtPhiMu*");
    singleAnalysis->DisableHistograms("Chi2Mu*");

    // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis);


    if ( minvAnalysis )
    {
      // Array of cut elements
      TObjArray cutElements;

      // Cuts on track level
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy    = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange  = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,12.");

      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchany);
      cutElements.Add(eta);
      cutElements.Add(pdca);
      // add them
      cr->AddCutCombination(cutElements);
      minvAnalysis->DisableHistograms("Pt");
      minvAnalysis->DisableHistograms("Y");
      minvAnalysis->DisableHistograms("Eta");
      minvAnalysis->DisableHistograms("PtPaireVsPtTrack");
      minvAnalysis->DisableHistograms("MinvUS*");
      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }



    if ( triggResponse )
    {
      Double_t etarange[6] = {-4.,-3.7,-3.4,-3.1,-2.8,-2.5};;
      triggResponse->SetLptAptEtaRange(etarange,6);
      task->AdoptSubAnalysis(triggResponse);
    }

  }

  // Configure sub analysis
  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"

  // v0 centrality binning
  binning->AddBin("centrality","V0M",0.,10.);
  binning->AddBin("centrality","V0M",0.,90.);
  binning->AddBin("centrality","V0M",10.,20.);
  binning->AddBin("centrality","V0M",20.,30.);
  binning->AddBin("centrality","V0M",30.,40.);
  binning->AddBin("centrality","V0M",40.,90.);

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
