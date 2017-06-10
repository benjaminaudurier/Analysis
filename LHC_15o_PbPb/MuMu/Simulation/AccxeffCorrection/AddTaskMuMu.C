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

  AliAnalysisTaskMuMu  * task             = new AliAnalysisTaskMuMu(); // Call the task
  AliAnalysisMuMuAccxEff* accxeffAnalysis = new AliAnalysisMuMuAccxEff();// Analysis dealing with single muon
  AliAnalysisMuMuSingle* singleAnalysis   = new AliAnalysisMuMuSingle();// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis     = new AliAnalysisMuMuMinv();// Analysis creating invariant mass spectrum
  task->SetBeamYear(beamYear);

  // =========


  // ========= Configure Cut registry =========

  AliAnalysisMuMuEventCutter* eventCutter         = new AliAnalysisMuMuEventCutter(triggers,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr                  = task->CutRegistry(); // Set CutRegistry

  AliAnalysisMuMuCutElement * eventTrue           = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection    = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement * ps                  = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedINT7inMUON","const AliInputEventHandler&","");
  cr->AddCutCombination(ps,triggerSelection);

  // =========

  // ========= Configure Cut registry =========

  if ( singleAnalysis ){

    // --- Create combination of cuts to apply ---
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs      = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow  = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta       = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca      = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");

    if ( minvAnalysis ){

      // --- Array of cut elements ---
      TObjArray cutElements;
      // TObjArray cutElementspdca;

      // --- Cuts on track level ---
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy    = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");

      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      cr->AddCutCombination(cutElements);

      // cutElementspdca.Add(pairTrue);
      // cutElementspdca.Add(pairy);
      // cutElementspdca.Add(rabs);
      // cutElementspdca.Add(matchlow);
      // cutElementspdca.Add(eta);
      // cutElementspdca.Add(pdca);
      // cr->AddCutCombination(cutElementspdca);

      // --- Disable some histo. Comment the one you want ---
      minvAnalysis->DisableHistograms("*");
      // minvAnalysis->DisableHistograms("Pt");
      // minvAnalysis->DisableHistograms("Y");

      // Adding the sub analysis
      // task->AdoptSubAnalysis(singleAnalysis);
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }

  // Embeding LHC16e function
  TString oldPtFormula = "[0] * x / TMath::Power(1. + TMath::Power(x/[1],[2]), [3])";
  Double_t oldPtParam[4] = {1.00715e6, 3.50274, 1.93403, 3.96363};

  TString oldYFormula = "[0] * TMath::Exp(-(1./2.)*TMath::Power(((x-[1])/[2]),2))";
  Double_t oldYParam[3] = {1.09886e6, 0., 2.12568};

  // Set original generated sim functions
  accxeffAnalysis->SetOriginPtFunc(oldPtFormula,oldPtParam,0.,12.);
  accxeffAnalysis->SetOriginYFunc(oldYFormula,oldYParam,-4,-2.5);

  // Add functions to task
  TObjArray* functionArray = new TObjArray();
  functionArray->Add(new TObjString("V0M_00.00_10.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3622:2.6572:2.6875:2.3173&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.2721"));
  functionArray->Add(new TObjString("V0M_10.00_20.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3421:2.6832:2.6631:2.2200&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.2835"));
  functionArray->Add(new TObjString("V0M_20.00_30.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3152:2.6309:2.8670:1.9650&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.1410"));
  functionArray->Add(new TObjString("V0M_30.00_40.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3115:3.1424:2.3526:2.6296&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.4098"));
  functionArray->Add(new TObjString("V0M_40.00_50.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.2833:2.6862:2.9693:1.8001&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.2950"));
  functionArray->Add(new TObjString("V0M_50.00_60.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3169:3.0985:2.2779:2.6041&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.2457"));
  functionArray->Add(new TObjString("V0M_60.00_90.00&[0]*x/TMath::Power(1.+TMath::Power(x/[1],[2]),[3])&0.3045:3.1222:2.3487:2.5652&[0]*[0]*TMath::Exp(-0.5*((x-[1])/[2])**2)&1.0000:0.0000:2.6233"));

  accxeffAnalysis->SetFunctionList(functionArray);

  task->AdoptSubAnalysis(accxeffAnalysis);

  // Configure sub analysis
  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"

  // v0 centrality binning
  binning->AddBin("centrality","V0M",0.,10.);
  binning->AddBin("centrality","V0M",10.,20.);
  binning->AddBin("centrality","V0M",20.,30.);
  binning->AddBin("centrality","V0M",30.,40.);
  binning->AddBin("centrality","V0M",40.,50.);
  binning->AddBin("centrality","V0M",50.,60.);
  binning->AddBin("centrality","V0M",60.,90.);

  // add the configured task to the analysis manager
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  AliAnalysisDataContainer *coutputHC =
  mgr->CreateContainer("OC",AliMergeableCollection::Class(),AliAnalysisManager::kOutputContainer,outputname);

  AliAnalysisDataContainer *coutputCC =
  mgr->CreateContainer("CC",AliCounterCollection::Class(),AliAnalysisManager::kOutputContainer,outputname);

  AliAnalysisDataContainer* cparam =
  mgr->CreateContainer("BIN", AliAnalysisMuMuBinning::Class(),AliAnalysisManager::kOutputContainer,outputname);

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutputHC);
  mgr->ConnectOutput(task, 2, coutputCC);
  mgr->ConnectOutput(task, 3, cparam);

  return task;
}
