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
  if (!mgr->GetInputEventHandler()) {
    ::Error("AddTaskMuMu", "This task requires an input event handler");
    return NULL;
  }

  // Add new trigger in case of Simulation
  if (simulations && triggerClassesToConsider ) {
       triggerClassesToConsider->Add(new TObjString("ANY"));
       triggerClassesToConsider->Add(new TObjString("MB1"));
       triggerClassesToConsider->Add(new TObjString("C0T0A"));
       triggerClassesToConsider->Add(new TObjString("MULow"));
  }

  //  Configure inputmaps (Default on is in AliMuonEventCuts)
  TList* triggerInputsMap = new TList();
  triggerInputsMap->SetOwner(kTRUE);


  //  Configure task
  AliAnalysisTaskMuMu       * task = new AliAnalysisTaskMuMu; // Call the task
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider,triggerInputsMap); // To handle cuts on event
  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Set CutRegistry

  // Default cuts on trigger and event level
  AliAnalysisMuMuCutElement * eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&","");
  AliAnalysisMuMuCutElement * triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
  AliAnalysisMuMuCutElement * ps = eventTrue;

  if (!simulations){
    ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedMUL","const AliInputEventHandler&","");
  }

  // Apply default cut
  cr->AddCutCombination(ps,triggerSelection);

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0/* new AliAnalysisMuMuGlobal*/; // Basic histograms analysis;
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;// Analysis dealing with single muon
  AliAnalysisMuMuMinv  * minvAnalysis   = new AliAnalysisMuMuMinv;// Analysis creating invariant mass spectrum

  // Configure sub analysis
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
    AliAnalysisMuMuCutElement* trackTrue  = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&",""); // Apply "AlwaysTrue" cut on AliVParticle derived from AliAnalysisMuMuSingle
    AliAnalysisMuMuCutElement* rabs       = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    AliAnalysisMuMuCutElement* matchlow   = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    AliAnalysisMuMuCutElement* eta        = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    AliAnalysisMuMuCutElement* pdca       = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");

    // Create combination of cuts to apply
    // cr->AddCutCombination(trackTrue);
    // cr->AddCutCombination(matchlow);
    // cr->AddCutCombination(rabs,eta);
    // cr->AddCutCombination(pdca);
    // Adding the sub analysis
    // task->AdoptSubAnalysis(singleAnalysis);

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
      cutElements.Add(matchlow);
      cutElements.Add(eta);
      // cutElements.Add(pdca);
      // cutElements.Add(ps);
      // add them


      // MC reweighting and cuts
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


      cr->AddCutCombination(cutElements);
      // Adding the sub analysis
      task->AdoptSubAnalysis(minvAnalysis);
    }
  }

  /// below are the kind of configurations that can be performed :
  /// - adding cuts (at event, track or pair level)
  /// - adding bins (in pt, y, centrality, etc...) for minv (and meanpt)

  // Configure sub analysis
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


    // y binning
   binning->AddBin("psi","y",-4,-3.75,"BENJ");
   binning->AddBin("psi","y",-3.75,-3.5,"BENJ");
   binning->AddBin("psi","y",-3.5,-3.25,"BENJ");
   binning->AddBin("psi","y",-3.25,-3,"BENJ");
   binning->AddBin("psi","y",-3,-2.75,"BENJ");
   binning->AddBin("psi","y",-2.75,-2.5,"BENJ");

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

