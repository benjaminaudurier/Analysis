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
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
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

  // Configure analysis
  //===========================================================================

  AliAnalysisTaskMuMu* task = new AliAnalysisTaskMuMu; //Base Analysis Task

//  task->SetCountInBins("psi","ntrcorr","D2H");

  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider); // Event Cutter Class

  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Cut Registry

  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0;//new AliAnalysisMuMuGlobal;

  AliAnalysisMuMuSingle* singleAnalysis = 0x0; //new AliAnalysisMuMuSingle;

//
//  TFile fA("~/Analysis/pA/pAanalysis/NewFramework/PureMC/pPb/NewBinning/AccxEffpA.root");
//  TH2* hA = static_cast<TH2*>(fA.Get("PSICOUNT"));
//  if (!hA)
//  {
//    std::cout << "Error reading Jpsi AccxEff histo" << std::endl;
//    return;
//  }
//
  AliAnalysisMuMuMinv* minvAnalysis = 0x0; //new AliAnalysisMuMuMinv(hA,kTRUE);
//
  TFile fT("./MeanTrackletsVsZvertex_CMUL_ALLCUTS.root"); //ALLCUTS
  TProfile* hT = static_cast<TProfile*>(fT.Get("MeanTrackletsVsZVertex"));
  if (!hT)
  {
    std::cout << "Error reading SPD correction histo" << std::endl;
    return;
  }

  Double_t etaMin = -1.;
  Double_t etaMax = 1.;


  Double_t zMin = -10.;
  Double_t zMax = 10.;


//  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch(0x0,0x0,-1.,etaMin,etaMax,zMin,zMax,kFALSE);//Raw Tracklets Analysis

  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch(0x0,hT,29.82,etaMin,etaMax,zMin,zMax,kFALSE);//Mean tracklets correction //29.93

//  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch(0x0,0x0,-1.,etaMin,etaMax,zMin,zMax,kFALSE);



  if ( 1 /*globalAnalysis && nchAnalysis && singleAnalysis && minvAnalysis*/)
  {
    //_____________________Event cuts

    AliAnalysisMuMuCutElement* eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&",""); // ALL Events

    AliAnalysisMuMuCutElement* ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliInputEventHandler&",""); // Physiscs selected Ev.

    AliAnalysisMuMuCutElement* cutSPDZ10 = cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","10");

    AliAnalysisMuMuCutElement* cutHasSPD = cr->AddEventCut(*eventCutter,"HasSPDVertex","const AliVEvent&","");

    AliAnalysisMuMuCutElement* cutZQA = cr->AddEventCut(*eventCutter,"IsSPDzQA","const AliVEvent&,Double_t,Double_t","0.25,0.5");

    AliAnalysisMuMuCutElement* cutZnoQA = cr->Not(*cutZQA);

    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ10); // Event Cut Combination

//    cr->AddCutCombination(ps,cutHasSPD,cutZnoQA,cutSPDZ10);
//
    cr->AddCutCombination(ps,cutHasSPD,cutZQA);
//
    cr->AddCutCombination(ps,cutHasSPD);

//    cr->AddCutCombination(ps,cutHasSPD,cutZQA); //These event Cuts are just for the dNchdEta correction for ev w/o vertex computation
//    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10);
    cr->AddCutCombination(ps);
//    cr->AddCutCombination(eventTrue);

    //____________________Trigger cuts

    AliAnalysisMuMuCutElement* triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");

    cr->AddCutCombination(triggerSelection); // Trigger cut combination (just one at a time)

    //___________________Track cuts

              //______ Single track cuts
//    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&","");
//
//    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
//
//    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
//
//    AliAnalysisMuMuCutElement* pDCA = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
//
//    AliAnalysisMuMuCutElement* matchingTrigger = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
//
//              //______ Pair track cuts
//    AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
//
//    AliAnalysisMuMuCutElement* pairpt = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.0,15.0");
//
//    cr->AddCutCombination(rabs,eta,pDCA,matchingTrigger,pairy,pairpt); // Track cut combination

//    cr->AddCutCombination(rabs,eta,matchingTrigger,pairy,pairpt);
//    cr->AddCutCombination(rabs,eta);
//    cr->AddCutCombination(trackTrue,rabs);

    //___________________Binning definition (For inv mass task)

    AliAnalysisMuMuBinning* binning = task->Binning();

              //______ INTEGRATED Binning

    binning->AddBin("psi","integrated");

              //______ PT Binning (Just to compare with Igor results)

//    binning->AddBin("psi","pt", 0.0, 1.0,"IGOR");
//    binning->AddBin("psi","pt", 1.0, 2.0,"IGOR");
//    binning->AddBin("psi","pt", 2.0, 3.0,"IGOR");
//    binning->AddBin("psi","pt", 3.0, 4.0,"IGOR");
//    binning->AddBin("psi","pt", 4.0, 5.0,"IGOR");
//    binning->AddBin("psi","pt", 5.0, 6.0,"IGOR");
//    binning->AddBin("psi","pt", 6.0, 7.0,"IGOR");
//    binning->AddBin("psi","pt", 7.0, 9.0,"IGOR");
//    binning->AddBin("psi","pt", 9.0,11.0,"IGOR");
//    binning->AddBin("psi","pt",11.0,15.0,"IGOR");
//    binning->AddBin("psi","pt",15.0,25.0,"IGOR");

              //______ Y Binning
//    binning->AddBin("psi","y",-4,-2.5,"ILAB");
//
//    for ( Int_t i = 0; i < 6; ++i )
//    {
//      Double_t y = -4+i*0.25;
//
//      binning->AddBin("psi","y",y,y+0.25,"6PACK");
//    }

              //______ dNchdEta Binning

//    binning->AddBin("psi","ntrcorr",-1.5,-0.5,"D2H"); // 1 - 8 // Only to check that FNorm matches with previous analysis
//
//    binning->AddBin("psi","ntrcorr",-0.5,0.5,"D2H"); // 1 - 8
//    binning->AddBin("psi","ntrcorr",0.5,8.5,"D2H"); // 1 - 8
//    binning->AddBin("psi","ntrcorr",8.5,13.5,"D2H"); // 9 - 14
//    binning->AddBin("psi","ntrcorr",13.5,16.5,"D2H"); // 20 - 24
//    binning->AddBin("psi","ntrcorr",16.5,20.5,"D2H"); // 25 - 33
//    binning->AddBin("psi","ntrcorr",20.5,24.5,"D2H"); // 34 - 41
//    binning->AddBin("psi","ntrcorr",24.5,28.5,"D2H"); // 34 - 41
//    binning->AddBin("psi","ntrcorr",28.5,32.5,"D2H"); // 42 - 50
//    binning->AddBin("psi","ntrcorr",32.5,38.5,"D2H"); // 51 - 59
//    binning->AddBin("psi","ntrcorr",38.5,44.5,"D2H"); // 51 - 59
//    binning->AddBin("psi","ntrcorr",44.5,54.5,"D2H"); // 51 - 59
//    binning->AddBin("psi","ntrcorr",54.5,74.5,"D2H"); // 100 - 199
//    binning->AddBin("psi","ntrcorr",74.5,140.5,"D2H"); // 100 - 199
//
//    binning->AddBin("psi","ntrcorr",140.5,200.0,"D2H"); // 100 - 199 // Only to check that FNorm matches with previous analysis






   //______ Centrality Binning (Not used in the inv mass analysis unless add "v0a" to the CreateBinObjArray in AliAnalysisMuMuMinv.cxx )

    binning->AddBin("centrality","v0a"); //This is ANY V0A centrality
//    binning->AddBin("centrality","zna");

    // v0 centrality binning

    //  binning->AddBin("centrality","v0a");
    //  binning->AddBin("centrality","v0a",0,5);
    //  binning->AddBin("centrality","v0a",5,10);
    //  binning->AddBin("centrality","v0a",10,20);
    //  binning->AddBin("centrality","v0a",20,40);
    //  binning->AddBin("centrality","v0a",40,60);
    //  binning->AddBin("centrality","v0a",60,80);
    //  binning->AddBin("centrality","v0a",80,100);

    //ZN centrality binning

//    binning->AddBin("centrality","zna",0.,5.,"");
//    binning->AddBin("centrality","zna",5.,10.,"");
//    binning->AddBin("centrality","zna",10.,20.,"");
//    binning->AddBin("centrality","zna",20.,40.,"");
//    binning->AddBin("centrality","zna",40.,60.,"");
//    binning->AddBin("centrality","zna",60.,80.,"");
//    binning->AddBin("centrality","zna",80.,102.,"");

//    //================ Add SubAnalysis to the main Analysis=============
//    task->AdoptSubAnalysis(singleAnalysis);
    task->AdoptSubAnalysis(nchAnalysis);
//    task->AdoptSubAnalysis(minvAnalysis);
//    task->AdoptSubAnalysis(globalAnalysis);
  }



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

