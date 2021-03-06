///
/// Configuration example of a task to get invariant mass spectrum of dimuons
///

AliAnalysisTaskMuMu* AddTaskMuMu(const char* outputname,
                             const char* beamYear = "PP",
                             Bool_t simulations = kFALSE)
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

  // --- Your trigger list, could be what you want ---
  TList* triggers = new TList;
  triggers->SetOwner(kTRUE);
  // triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));
  triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));


  // ========= Configure inputmaps (Default on is in AliMuonEventCuts). Do not touch it for pp@13TeV Analysis ========

  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);
  triggerInputsMap->Add(new TObjString("0MSL:17"));
  triggerInputsMap->Add(new TObjString("0MSH:18"));
  triggerInputsMap->Add(new TObjString("0MLL:19"));
  triggerInputsMap->Add(new TObjString("0MUL:20"));
  triggerInputsMap->Add(new TObjString("0V0M:3"));
  triggerInputsMap->Add(new TObjString("0TVX:2"));

  // =========

  // ========= Create task =========

  // File to plug after the first run of the task.
  // You need to give it the MeanTrackletsVsZVertex TProfile to use the data-driven multiplicity correction method


  TFile* f =TFile::Open("MeanTrkVsVertex.root");
  if(!f) {
    printf("Cannot open file \n");
    return;
  }

  TProfile* hT =0x0;
  f->GetObject("MeanTrackletsVsZVertex",hT);
  if(!hT){
    printf("Cannot get MeanTrackletsVsZVertex Histogram\n");
    return;
  }


  // cut to fill histo from the task
  Double_t etaMin = -1.;
  Double_t etaMax = 1.;

  Double_t zMin = -10.;
  Double_t zMax = 10.;


  //General task that contains all the sub-task analysis
  AliAnalysisTaskMuMu* task            = new AliAnalysisTaskMuMu();

  // Raw Tracklets sub-analysis. The first configuration is meant to produce multiplicity QA plots and stuff (First run)
  // For the second run, you can use the second conf. providing the MeanTrackletsVsZVertex from the first run output file.
  // AliAnalysisMuMuNch* nch              = new AliAnalysisMuMuNch(0x0,0x0,-1.,etaMin,etaMax,zMin,zMax,kFALSE);//
  AliAnalysisMuMuNch* nch              = new AliAnalysisMuMuNch(0x0,hT,8.239,etaMin,etaMax,zMin,zMax,kFALSE);//Raw Tracklets Analysis

  // Inv. Mass sub-analysis. For the first run, you can leave it to NULL, and only plug it for the second run.
  // AliAnalysisMuMuMinv* minvAnalysis    =  0x0; // Call the task
  AliAnalysisMuMuMinv* minvAnalysis    =  new AliAnalysisMuMuMinv(); // Call the task

  // Single muon sub-analysis. Provides the cut methods for single muons, never unplug
  // AliAnalysisMuMuSingle* singleAnalysis= 0x0;
  AliAnalysisMuMuSingle* singleAnalysis= new AliAnalysisMuMuSingle();
  task->SetBeamYear(beamYear);


  // ========= Configure cut on event =========

  // --- standard Analysis ---

  // Do not change that :)
  // Class store all cuts for the task (event, single track, pairs)
  AliAnalysisMuMuCutRegistry* cr                  = task->CutRegistry(); // Set CutRegistry

  // Class where all the event cut methods are stored
  AliAnalysisMuMuEventCutter* eventCutter         = new AliAnalysisMuMuEventCutter(triggers,triggerInputsMap);

  // Call several event cuts
  AliAnalysisMuMuCutElement* eventTrue           = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&",""); // ALL Events
  AliAnalysisMuMuCutElement* cutSPDZ10           = cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","10");
  AliAnalysisMuMuCutElement* triggerSelection    = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement* cutHasSPD           = cr->AddEventCut(*eventCutter,"HasSPDVertex","const AliVEvent&","");
  AliAnalysisMuMuCutElement* cutZQA              = cr->AddEventCut(*eventCutter,"IsSPDzQA","const AliVEvent&,Double_t,Double_t","0.25,0.5");
  AliAnalysisMuMuCutElement* SPDPileUp           = cr->AddEventCut(*eventCutter,"IsSPDPileUp","AliVEvent&","");
  AliAnalysisMuMuCutElement* NoSPDPileUp         = cr->Not(*SPDPileUp);
  AliAnalysisMuMuCutElement* clusterBKG          = cr->AddEventCut(*eventCutter,"IsSPDClusterVsTrackletBackground","const AliVEvent&","");
  AliAnalysisMuMuCutElement* noClusterBKG        = cr->Not(*clusterBKG);


  // Here you can change the physic selction as you want, see AliAnalysisMuMuEventCutter.h. It is similar to SetCollisionCandidates()
  AliAnalysisMuMuCutElement * ps                 = eventTrue;
  if (!simulations) ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedANY","const AliInputEventHandler&","");

  // Add a combination to the cut registry. This is your actual event cuts. You can use several combination at the same time
  TObjArray cutElementsEvent;
  cutElementsEvent.Add(ps);
  cutElementsEvent.Add(cutHasSPD);
  cutElementsEvent.Add(cutZQA);
  cutElementsEvent.Add(cutSPDZ10);
  cutElementsEvent.Add(NoSPDPileUp);
  cutElementsEvent.Add(noClusterBKG);
  cutElementsEvent.Add(triggerSelection);
  cr->AddCutCombination(cutElementsEvent);


  // ========= Configure sub-analysis =========

  if ( singleAnalysis ){

    // --- Create combination of cuts to apply on single muons ---
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs      = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow  = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta       = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca       = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    cr->AddCutCombination(trackTrue,rabs,matchlow,eta,pdca);

    if ( minvAnalysis ){

      // --- Array of cut elements ---
      TObjArray cutElements;

      // --- Cuts on muon pairs level ---
      AliAnalysisMuMuCutElement* pairTrue = cr->AddTrackPairCut(*cr,"AlwaysTrue","const AliVParticle&,const AliVParticle&","");// Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuMinv
      AliAnalysisMuMuCutElement* pairy    = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
      AliAnalysisMuMuCutElement* ptRange  = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.,12.");
      cutElements.Add(pairTrue);
      cutElements.Add(pairy);
      cutElements.Add(ptRange);
      cutElements.Add(rabs);
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      cr->AddCutCombination(cutElements);

      // --- Disable some histo. Comment the one you want ---
      minvAnalysis->DisableHistograms("Eta");
      minvAnalysis->DisableHistograms("Pt");
      minvAnalysis->DisableHistograms("Y");

      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }


  // Addinf raw Tracklet analysis
  task->AdoptSubAnalysis(nch);


  // ========= Configure binning =========

  // Class that hold the binnings.
  AliAnalysisMuMuBinning* binning = task->Binning();

  // --- Histogram binning ---

  // Binning versus "psi" == binning for track Histogram (see AliAnalysisMuMuMinv or AliAnalysisMuMuSingle to see the available binning )
  // binning->AddBin("psi","pt", 0.0, 1.0,"");
  // binning->AddBin("psi","pt", 1.0, 2.0,"");
  // binning->AddBin("psi","pt", 2.0, 3.0,"");
  // binning->AddBin("psi","pt", 3.0, 4.0,"");
  // binning->AddBin("psi","pt", 4.0, 5.0,"");
  // binning->AddBin("psi","pt", 5.0, 6.0,"");
  // binning->AddBin("psi","pt", 6.0, 7.0,"");
  // binning->AddBin("psi","pt", 7.0, 9.0,"");
  // binning->AddBin("psi","pt", 9.0,11.0,"");
  // binning->AddBin("psi","pt",11.0,15.0,"");
  // binning->AddBin("psi","pt",15.0,25.0,"");

  // Last arguments ("DH2" here) is just to add a term to Histo name to help differentiation in case of several binnings.
  // binning->AddBin("psi","ntrcorr",-1.5,-0.5,"BENJ"); // 1 - 8
  binning->AddBin("psi","ntrcorr",-0.5,0.5,"BENJ"); // 1 - 8
  binning->AddBin("psi","ntrcorr",0.5,9.5,"BENJ"); // 1 - 8
  binning->AddBin("psi","ntrcorr",9.5,19.5,"BENJ"); // 9 - 14
  binning->AddBin("psi","ntrcorr",19.5,29.5,"BENJ"); // 20 - 24
  binning->AddBin("psi","ntrcorr",29.5,39.5,"BENJ"); // 25 - 33

  // binning->AddBin("psi","ntrcorr",140.5,200.0,"D2H"); // 100 - 199 // Only to check that FNorm matches with previous analysis

  // --- Centrality binning ---
  // You won't have to modify it i guess
  binning->AddBin("centrality","PP");


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
