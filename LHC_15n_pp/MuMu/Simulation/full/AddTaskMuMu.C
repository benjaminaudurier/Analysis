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
  triggers->Add(new TObjString("ANY"));
  triggers->Add(new TObjString("MB1"));
  triggers->Add(new TObjString("C0T0A"));
  triggers->Add(new TObjString("MULow"));

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
  AliAnalysisMuMuSingle* singleAnalysis   = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis     = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum

  // Get the AccEff Histogram
  TFile* f =TFile::Open("AccEffCorr.root");
  if(!f) {
    printf("Cannot open file \n");
    return;
  }

  TH2* AccEff2D =0x0;
  f->GetObject("ANY-ALL-pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA-PP",AccEff2D);
  if(!AccEff2D){
    printf("Cannot get AccEff Histogram\n");
    return;
  }

  AliAnalysisMuMuMinv  * minvAnalysis     = new AliAnalysisMuMuMinv(AccEff2D);// Analysis creating invariant mass spectrum
  minvAnalysis->FillMeanPtHisto();
  task->SetBeamYear(beamYear);
  f->Close();

  // =========


  // ========= Configure Cut registry =========

  // --- Standard Analysis ---

  AliAnalysisMuMuEventCutter* eventCutter         = new AliAnalysisMuMuEventCutter(triggers,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr                  = task->CutRegistry(); // Set CutRegistry

  AliAnalysisMuMuCutElement * eventTrue           = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection    = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");

  cr->AddCutCombination(eventTrue,triggerSelection);

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
    // singleAnalysis->DisableHistograms("dcaPwPtCut310Mu*");

    // --- separate positive and negative muons ---
    // singleAnalysis->ShouldSeparatePlusAndMinus(kTRUE);

    // Adding the sub analysis
    // task->AdoptSubAnalysis(singleAnalysis);

    if ( minvAnalysis ){

      // --- Array of cut elements ---
      TObjArray cutElements;

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
      // minvAnalysis->DisableHistograms("Y");

      // Adding the sub analysis
      minvAnalysis->SetMCptCut(0.,12.);
       // MC reweighting From Cross-section analysis
      if (simulations){
        // Cut on generated Jpsi
        Double_t mcGenPtCutMin =0.;
        Double_t mcGenPtCutMax =12.;

        // tune3 LHC15n jpsi
        TString oldPtFormula    =   "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
        Double_t oldPtParam[4]  =   {4654.3, 12.8133, 1.9647, 3.66641};

        // Fit from Cross section
        TString newPtFormula    = "[0]*x / TMath::Power([1] + TMath::Power(x,[2]),[3])";
        Double_t newPtParam[4]  = {5.21831e+04, 1.46939e+01,1.93309, 3.93941};

        // tune3 LHC15n jpsi
        TString oldYFormula     = " [0] * ( 1 + [1]*x*x ) ";
        Double_t oldYParam[2]   = {1.18296, -0.0405994};

        TString newYFormula     = "[0] * ( 1 + [1]*x*x)";
        Double_t newYParam[2]   =  {6.36959, -3.99165e-02};

        Double_t ptRangeFunc[2] = {0., 999.};
        Double_t yRangeFunc[2]  = {-4.2, -2.3};

        //Set pt cut on Gen level
        minvAnalysis->SetMCptCut(mcGenPtCutMin,mcGenPtCutMax);

        // set the original function and parameters used in simulation
        minvAnalysis->SetOriginPtFunc(oldPtFormula.Data(), oldPtParam, ptRangeFunc[0], ptRangeFunc[1]);
        minvAnalysis->SetOriginYFunc(oldYFormula.Data(), oldYParam, yRangeFunc[0], yRangeFunc[1]);

        // set the new function and initial parameters
        minvAnalysis->SetNewPtFunc(newPtFormula.Data(), newPtParam, ptRangeFunc[0], ptRangeFunc[1]);
        minvAnalysis->SetNewYFunc(newYFormula.Data(), newYParam, yRangeFunc[0], yRangeFunc[1]);
      }
      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }

  // Configure sub analysis
  AliAnalysisMuMuBinning* binning = task->Binning(); // Create and set the "binning manager"

  if (minvAnalysis){
    // Integrated
    binning->AddBin("psi","integrated");

    // pt binning
    binning->AddBin("psi","pt", 0.0, 1.0,"BENJ");
    binning->AddBin("psi","pt", 0.3, 1.0,"BENJ");
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

    // binning->AddBin("psi","pt", 0.3, 12.0,"INT_PUTCUT");
    binning->AddBin("psi","pt", 0.0, 8.0,"INT");
    binning->AddBin("psi","pt", 0.3, 12.0,"INT_PTCUT");
    binning->AddBin("psi","pt", 0.0, 2.0,"INT_VAR1");
    binning->AddBin("psi","pt", 0.3, 2.0,"INT_VAR2");
    binning->AddBin("psi","pt", 2.0, 5.0,"INT_VAR3");
    binning->AddBin("psi","pt", 5.0, 8.0,"INT_VAR4");
    binning->AddBin("psi","pt", 8.0, 12.0,"INT_VAR5");

     // y binning
     binning->AddBin("psi","y",-4,-3.75,"BENJ");
     binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
     binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
     binning->AddBin("psi","y",-3.25,-3,"BENJ");
     binning->AddBin("psi","y",-3,-2.75,"BENJ");
     binning->AddBin("psi","y",-2.75,-2.5,"BENJ");


     binning->AddBin("psi","yvspt",0,1,-4,-3.25 ,"BENJ");

    binning->AddBin("psi","yvspt", 0.0, 1.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 0.3, 1.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 1.0, 2.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 2.0, 3.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 3.0, 4.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 4.0, 5.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 5.0, 6.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 6.0, 8.0,-4,-3.25,"2DBIN1");
    // binning->AddBin("psi","yvspt", 8.0, 8.0,-4,-3.25,"2DBIN1");
    binning->AddBin("psi","yvspt", 8.0, 12.0,-4,-3.25,"2DBIN1");

    binning->AddBin("psi","yvspt", 0.0, 1.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 0.3, 1.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 1.0, 2.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 2.0, 3.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 3.0, 4.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 4.0, 5.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 5.0, 6.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 6.0, 8.0,-3.25,-2.5,"2DBIN2");
    // binning->AddBin("psi","yvspt", 7.0, 8.0,-3.25,-2.5,"2DBIN2");
    binning->AddBin("psi","yvspt", 8.0, 12.0,-3.25,-2.5,"2DBIN2");




    Double_t dy = 0.25;
    for (int i = 0; i < 6; ++i)
    {
      binning->AddBin("psi","yvspt", 0.0, 1.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 1.0, 2.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 2.0, 3.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 3.0, 4.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 4.0, 5.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 5.0, 6.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 6.0, 7.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 7.0, 8.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
      binning->AddBin("psi","yvspt", 8.0, 12.0,-4+i*dy,-4+dy+i*dy,"2DBINVAR");
    }
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
