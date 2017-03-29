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
  triggers->Add(new TObjString("CMUL7-B-NOPF-MUFAST"));
  // triggers->Add(new TObjString("CINT7-B-NOPF-MUFAST"));


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

  TFile* f =TFile::Open("test_distribution.root");
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

  Double_t etaMin = -1.;
  Double_t etaMax = 1.;

  Double_t zMin = -10.;
  Double_t zMax = 10.;


  AliAnalysisTaskMuMu* task            = new AliAnalysisTaskMuMu; // Call the task
  // AliAnalysisMuMuNch* nch              = new AliAnalysisMuMuNch(0x0,0x0,-1.,etaMin,etaMax,zMin,zMax,kFALSE);//Raw Tracklets Analysis
  AliAnalysisMuMuNch* nch              = new AliAnalysisMuMuNch(0x0,hT,0.,etaMin,etaMax,zMin,zMax,kFALSE);//Raw Tracklets Analysis
  AliAnalysisMuMuMinv* minvAnalysis    = new AliAnalysisMuMuMinv; // Call the task
  AliAnalysisMuMuSingle* singleAnalysis= new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  task->SetBeamYear(beamYear);


  // ========= Configure cut on event =========

  // --- standard Analysis ---

  // Do not change that :)
  AliAnalysisMuMuEventCutter* eventCutter         = new AliAnalysisMuMuEventCutter(triggers,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr                  = task->CutRegistry(); // Set CutRegistry

  AliAnalysisMuMuCutElement* eventTrue           = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&",""); // ALL Events
  AliAnalysisMuMuCutElement* cutSPDZ10           = cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","10");
  AliAnalysisMuMuCutElement* triggerSelection    = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement* cutHasSPD           = cr->AddEventCut(*eventCutter,"HasSPDVertex","const AliVEvent&","");
  AliAnalysisMuMuCutElement* cutZQA              = cr->AddEventCut(*eventCutter,"IsSPDzQA","const AliVEvent&,Double_t,Double_t","0.25,0.5");


  // Here you can change the physic selction as you want, see AliAnalysisMuMuEventCutter.h. It is similar to SetCollisionCandidates()
  AliAnalysisMuMuCutElement * ps1                 = eventTrue;
  AliAnalysisMuMuCutElement * ps2                 = eventTrue;
  if (!simulations)
  {
    ps1 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedMUL","const AliInputEventHandler&","");
    ps2 = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedINT7inMUON","const AliInputEventHandler&","");
  }

  // Here you add theme
  cr->AddCutCombination(ps1,cutHasSPD,cutZQA,cutSPDZ10,triggerSelection);
  // cr->AddCutCombination(ps1,cutHasSPD,cutZQA,triggerSelection);
  // cr->AddCutCombination(cutHasSPD,cutZQA,cutSPDZ10,triggerSelection);
  // cr->AddCutCombination(ps2,cutHasSPD,cutZQA,triggerSelection);

  if ( singleAnalysis ){

    // --- Create combination of cuts to apply ---
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs      = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow  = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta       = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    cr->AddCutCombination(trackTrue,rabs,matchlow,eta);

    // --- Disable some histo. Comment the one you want ---
    // singleAnalysis->DisableHistograms("BCX");
    // singleAnalysis->DisableHistograms("EtaRapidityMu*");
    // singleAnalysis->DisableHistograms("PtEtaMu*");
    // singleAnalysis->DisableHistograms("PtRapidityMu*");
    // singleAnalysis->DisableHistograms("PEtaMu*");
    // singleAnalysis->DisableHistograms("PtPhiMu*");
    // singleAnalysis->DisableHistograms("Chi2Mu*");
    // singleAnalysis->DisableHistograms("dcaP23Mu*");
    // singleAnalysis->DisableHistograms("dcaPwPtCut23Mu*");
    // singleAnalysis->DisableHistograms("dcaP310Mu*");
    // singleAnalysis->DisableHis∏git stattograms("dcaPwPtCut310Mu*");

    // --- separate positive and negative muons ---
    // singleAnalysis->ShouldSeparatePlusAndMinus(kTRUE);

    // Adding the sub analysis
    task->AdoptSubAnalysis(singleAnalysis);

    if ( minvAnalysis ){

      // --- Array of cut elements ---
      TObjArray cutElements;
      TObjArray cutElementsmix;

      // --- Cuts on track level ---
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
      // minvAnalysis->DisableHistograms("Pt");
      // minvAnalysis->DisableHistograms("Y");

      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }


  task->AdoptSubAnalysis(nch);


  // Configure the binning. Here as an example it is versus centrality. If you want something else, see AliAnalysisTaskMuMu::FillCounters()
  // For example you can select ntrcorr,ntr,nch,v0a,v0acorr,v0ccorr,v0mcorr instead of "V0M", but keep "centrality"

  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"


    binning->AddBin("psi","integrated");

              //______ PT Binning (Just to compare with Igor results)

   binning->AddBin("psi","ntrcorr",-1.5,-0.5,""); // 1 - 8 // Only to check that FNorm matches with previous analysis
   binning->AddBin("psi","ntrcorr",-0.5,0.5,""); // 1 - 8
   binning->AddBin("psi","ntrcorr",0.5,8.5,""); // 1 - 8
   binning->AddBin("psi","ntrcorr",8.5,13.5,""); // 9 - 14
   binning->AddBin("psi","ntrcorr",13.5,16.5,""); // 20 - 24
   binning->AddBin("psi","ntrcorr",16.5,20.5,""); // 25 - 33
   binning->AddBin("psi","ntrcorr",20.5,24.5,""); // 34 - 41
   binning->AddBin("psi","ntrcorr",24.5,28.5,""); // 34 - 41
   binning->AddBin("psi","ntrcorr",28.5,32.5,""); // 42 - 50
   binning->AddBin("psi","ntrcorr",32.5,38.5,""); // 51 - 59
   binning->AddBin("psi","ntrcorr",38.5,44.5,""); // 51 - 59
   binning->AddBin("psi","ntrcorr",44.5,54.5,""); // 51 - 59
   binning->AddBin("psi","ntrcorr",54.5,74.5,""); // 100 - 199
   binning->AddBin("psi","ntrcorr",74.5,140.5,""); // 100 - 199

   binning->AddBin("psi","ntrcorr",140.5,200.0,""); // 100 - 199 // Only to check that FNorm matches with previous analysis

    // v0 centrality binning

    binning->AddBin("centrality","v0a");

    // For FNorm
    task->SetCountInBins("psi","ntrcorr","");


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
