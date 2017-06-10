///
/// Configuration example of a task to get invariant mass spectrum of dimuons
///
/// \author: L. Aphecetche (Subatech) (laurent.aphecetche - at - subatech.in2p3.fr)
///

Int_t GetNbins(Double_t xmin, Double_t xmax, Double_t xstep)
{
  if ( TMath::AreEqualRel(xstep,0.0,1E-9) ) return 1;
  
  return TMath::Nint(TMath::Abs((xmax-xmin)/xstep));
}

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

  if (simulations && triggerClassesToConsider )
  {
    triggerClassesToConsider->Add(new TObjString("CMULLO-B-NOPF-MUON"));
//    triggerClassesToConsider->Add(new TObjString("CMSNGL-B-NOPF-MUON"));
    
    triggerClassesToConsider->Add(new TObjString("ANY"));

//    triggerClassesToConsider->Add(new TObjString("MB1"));
//    triggerClassesToConsider->Add(new TObjString("C0T0A"));

    //    triggerClassesToConsider->Add(new TObjString("MULow"));
//    triggerClassesToConsider->Add(new TObjString("V0L"));
//    triggerClassesToConsider->Add(new TObjString("V0R"));
//
// for dpmjet simulations (at least) we have the following "triggers" :
//    C0T0A,C0T0C,MB1,MBBG1,V0L,V0R,MULow,EMPTY,MBBG3,MULL,MULU,MUHigh
  }

  AliAnalysisTaskMuMu* task = new AliAnalysisTaskMuMu; //Base Analysis Task
  
  AliAnalysisMuMuEventCutter* eventCutter = new AliAnalysisMuMuEventCutter(triggerClassesToConsider); // Event Cutter Class 
  
  AliAnalysisMuMuCutRegistry* cr = task->CutRegistry(); // Cut Registry
  

//  if (!simulations)
//  {
//    ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliVEventHandler&","");
//  }
  
//  AliAnalysisMuMuCutElement* cutVDM = cr->AddEventCut(*eventCutter,"IsPhysicsSelectedVDM","const AliVEvent&","");
//
//  AliAnalysisMuMuCutElement* cutTZEROPileUp = cr->AddEventCut(*eventCutter,"IsTZEROPileUp","const AliVEvent&","");
//
//  AliAnalysisMuMuCutElement* notTZEROPileUp = cr->Not(*cutTZEROPileUp);


  task->SetBeamYear(beamYear);

  AliAnalysisMuMuGlobal* globalAnalysis = 0x0;//new AliAnalysisMuMuGlobal;
  
  AliAnalysisMuMuSingle* singleAnalysis = new AliAnalysisMuMuSingle;
  
  TFile fA("~/Analysis/pA/pAanalysis/MCAccef/AccEffHistopA.root");
  TH2* hA = static_cast<TH2*>(fA.Get("COUNTJPSI:1"));
  if (!hA)
  {
    std::cout << "Error reading AccxEff histo" << std::endl;
    return;;
  }
  
  AliAnalysisMuMuMinv* minvAnalysis = new AliAnalysisMuMuMinv(hA);//new AliAnalysisMuMuMinv(hA,kTRUE);

  TFile f("~/Analysis/pA/pAanalysis/DPMJET/ResponseMatrix_QASPDZPSALL_ANY.root");
  TH2* h = static_cast<TH2*>(f.Get("ResponseMatrix"));
  
  if (!h)
  {
    std::cout << "Error reading SPD correction histo" << std::endl;
    return;;
  }
 
  Double_t etaMin = -0.5;
  Double_t etaMax = 0.5;
//  Double_t dEta = 0.1;
//  Int_t nbinsEta = (etaMax - etaMin)/ dEta;
  
  Double_t zMin = -10.;
  Double_t zMax = 10.;
//  Double_t dZ = 0.25;
//  Int_t nbinsZ = (zMax - zMin)/ dZ;
  
//  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch(0x0,etaMin,etaMax,zMin,zMax,kTRUE);
  
//  AliAnalysisMuMuNch* nchAnalysis = new AliAnalysisMuMuNch(0x0,etaMin,etaMax,zMin,zMax);
  
  AliAnalysisMuMuNch* nchAnalysis = 0x0; //new AliAnalysisMuMuNch(h);
  
//  AliAnalysisMuMuNch(TH2* spdCorrection=0x0,
//                     Int_t nbinsEta=10, Double_t etaMin=-0.5, Double_t etaMax=0.5,
//                     Int_t nbinsZ=320, Double_t zmin=-40, Double_t zmax=40);

    
  if ( singleAnalysis && minvAnalysis)
  {
    //_____________________Event cuts
    
    AliAnalysisMuMuCutElement* eventTrue = cr->AddEventCut(*eventCutter,"IsTrue","const AliVEvent&",""); // ALL Events
    
    AliAnalysisMuMuCutElement* ps = cr->AddEventCut(*eventCutter,"IsPhysicsSelected","const AliInputEventHandler&",""); // Physiscs selected Ev.

    AliAnalysisMuMuCutElement* cutSPDZ10 = cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","10"); 
    
    AliAnalysisMuMuCutElement* cutHasSPD = cr->AddEventCut(*eventCutter,"HasSPDVertex","const AliVEvent&","");
    
    AliAnalysisMuMuCutElement* cutZQA = cr->AddEventCut(*eventCutter,"IsSPDzQA","const AliVEvent&,Double_t,Double_t","0.25,0.5");
    
    if ( !simulations ) cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ10); // Event Cut Combination
    
    else cr->AddCutCombination(eventTrue);
    
    //____________________Trigger cuts
    
    AliAnalysisMuMuCutElement* triggerSelection = cr->AddTriggerClassCut(*eventCutter,"SelectTriggerClass","const TString&,TString&,UInt_t,UInt_t,UInt_t","");
    
    cr->AddCutCombination(triggerSelection); // Trigger cut combination (just one at a time)
  
    //___________________Track cuts
    
              //______ Single track cuts
    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&","");
    
    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
    
    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&","");
    
    AliAnalysisMuMuCutElement* pDCA = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
    
    AliAnalysisMuMuCutElement* matchingTrigger = cr->AddTrackCut(*singleAnalysis,"IsMatchingTriggerLowPt","const AliVParticle&","");
    
              //______ Pair track cuts    
    AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&","");
    
    AliAnalysisMuMuCutElement* pairpt = cr->AddTrackPairCut(*minvAnalysis,"IsPtInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","0.0,25.0");
    
    cr->AddCutCombination(rabs,eta,pDCA,matchingTrigger,pairy,pairpt); // Track cut combination
    
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

//    binning->AddBin("psi","dnchdeta",0.0,13.,"JAVI"); // 0 - 12
//    binning->AddBin("psi","dnchdeta",13.0,18.0,"JAVI"); // 12 - 18
//    binning->AddBin("psi","dnchdeta",18.0,24.0,"JAVI"); // 18 - 24
//    binning->AddBin("psi","dnchdeta",24.0,30.0,"JAVI"); // 23 - 30
//    binning->AddBin("psi","dnchdeta",30.0,36.0,"JAVI"); // 30 - 36
//    binning->AddBin("psi","dnchdeta",36.0,45.0,"JAVI"); // 36 - 45
//    binning->AddBin("psi","dnchdeta",45.0,60.0,"JAVI"); // 45 - 60
//    binning->AddBin("psi","dnchdeta",60.0,90.0,"JAVI"); // 60 - 90
//    binning->AddBin("psi","dnchdeta",90.0,130.0,"JAVI"); // 90 - 130
    
              //______ V0A centrality Binning (Not used in the inv mass analysis unless add "v0a" to the CreateBinObjArray in AliAnalysisMuMuMinv.cxx )
    
    binning->AddBin("centrality","v0a"); //This is ANY V0A centrality
    
              //______ y-pt Binning
    if (simulations)
    {
      
      /// y e [-4.,-3.625] ; Dy = 0.375
      Double_t rapidityMin = -4;
      Double_t rapidityMax = -3.625;
      Double_t Dy = 0.375;
      Int_t nbinsRapidity = GetNbins(rapidityMin,rapidityMax,Dy);
      
      Double_t PtMin = 0.;
      Double_t PtMax = 3.75;
      Double_t DPt = 0.25;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 3.75;
      Double_t PtMax = 4.75;
      Double_t DPt = 0.375;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 4.75;
      Double_t PtMax = 5.75;
      Double_t DPt = 0.5;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 5.75;
      Double_t PtMax = 7.;
      Double_t DPt = 0.725;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 7.;
      Double_t PtMax = 9.;
      Double_t DPt = 1.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 9.;
      Double_t PtMax = 13.;
      Double_t DPt = 2.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 13.;
      Double_t PtMax = 17.;
      Double_t DPt = 4.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 17.;
      Double_t PtMax = 25.;
      Double_t DPt = 8.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      
      //----------------
      
      /// y e [-3.625.,-2.75] ; Dy = 0.175
      Double_t rapidityMin = -3.625;
      Double_t rapidityMax = -2.75;
      Double_t Dy = 0.175;
      Int_t nbinsRapidity = GetNbins(rapidityMin,rapidityMax,Dy);
      
      Double_t PtMin = 0.;
      Double_t PtMax = 3.75;
      Double_t DPt = 0.25;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 3.75;
      Double_t PtMax = 4.75;
      Double_t DPt = 0.375;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 4.75;
      Double_t PtMax = 5.75;
      Double_t DPt = 0.5;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 5.75;
      Double_t PtMax = 7.;
      Double_t DPt = 0.725;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 7.;
      Double_t PtMax = 9.;
      Double_t DPt = 1.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 9.;
      Double_t PtMax = 13.;
      Double_t DPt = 2.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 13.;
      Double_t PtMax = 17.;
      Double_t DPt = 4.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 17.;
      Double_t PtMax = 25.;
      Double_t DPt = 8.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      
      //----------------
      
      /// y e [-2.75,-2.5] ; Dy = 0.25
      Double_t rapidityMin = -2.75;
      Double_t rapidityMax = -2.5;
      Double_t Dy = 0.25;
      Int_t nbinsRapidity = GetNbins(rapidityMin,rapidityMax,Dy);
      
      Double_t PtMin = 0.;
      Double_t PtMax = 3.75;
      Double_t DPt = 0.25;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 3.75;
      Double_t PtMax = 4.75;
      Double_t DPt = 0.375;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 4.75;
      Double_t PtMax = 5.75;
      Double_t DPt = 0.5;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 5.75;
      Double_t PtMax = 7.;
      Double_t DPt = 0.725;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 7.;
      Double_t PtMax = 9.;
      Double_t DPt = 1.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 9.;
      Double_t PtMax = 13.;
      Double_t DPt = 2.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 13.;
      Double_t PtMax = 17.;
      Double_t DPt = 4.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      Double_t PtMin = 17.;
      Double_t PtMax = 25.;
      Double_t DPt = 8.;
      Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      
      Double_t x(0),y(0);
      
      for (Int_t i = 0 ; i < nbinsPt ; i++)
      {
        x = PtMin + i*DPt;
        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
        {
          y = rapidityMin + j*Dy;
          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
        }
      }
      
      
      //---------------- Inv Mass Binning
//      binning->AddBin("psi","minv",1.5,3.025,"MINV");
//      binning->AddBin("psi","minv",3.025,3.100,"MINV");
//      binning->AddBin("psi","minv",3.100,3.175,"MINV");
//      binning->AddBin("psi","minv",3.175,3.8,"MINV");
      
      
      
      
      //----------------
      
      //    Double_t rapidityMin = -4;
      //    Double_t rapidityMax = -2.5;
      //    Double_t Dy = 0.125;
      //    Int_t nbinsRapidity = GetNbins(rapidityMin,rapidityMax,Dy);
      //    
      //    Double_t PtMin = 0.25;
      //    Double_t PtMax = 25.;
      //    Double_t DPt = 0.125;
      //    Int_t nbinsPt = GetNbins(PtMin,PtMax,DPt);
      //    
      //    
      //    Double_t x(0),y(0);
      //    
      //    if (simulations)
      //    {
      //      for (Int_t i = 0 ; i < nbinsPt ; i++)
      //      {
      //        x =  i*DPt;
      //        for (Int_t j = 0 ; j < nbinsRapidity ; j++)
      //        {
      //          y = rapidityMin + j*Dy;
      //          binning->AddBin("psi","yvspt",x,x + DPt ,y,y + Dy,"YVSPT");
      //        }
      //      }
      //      
    }

    //================ Add SubAnalysis to the main Analysis=============
    task->AdoptSubAnalysis(singleAnalysis);
    //    task->AdoptSubAnalysis(nchAnalysis);
    task->AdoptSubAnalysis(minvAnalysis);
  }

//  if ( globalAnalysis )
//  {
//    AliAnalysisMuMuCutElement* triggerAll = cr->AddTriggerClassCut(*globalAnalysis,"SelectAnyTriggerClass","const TString&,TString&","");
//    
//    cr->AddCutCombination(triggerAll);
//    
//    task->AdoptSubAnalysis(globalAnalysis);
//  }
  
  
  //____________ Just dNchdEta analysis and/or (depending on the etaTracklets-zVertex cuts) the counters to normalize the JPsi yields vs dNchdEta____________//

//  if ( nchAnalysis && (!globalAnalysis /*&& !minvAnalysis && !singleAnalysis*/) )
//  {
////    AliAnalysisMuMuCutElement* cutZ18= cr->AddEventCut(*eventCutter,"IsAbsZBelowValue","const AliVEvent&,Double_t","18");
////    
////    AliAnalysisMuMuCutElement* cutZ10 = cr->AddEventCut(*eventCutter,"IsAbsZBelowValue","const AliVEvent&,Double_t","10");
//    
//    AliAnalysisMuMuCutElement* cutSPDZ18= cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","18");
//    
//    AliAnalysisMuMuCutElement* cutSPDZ10 = cr->AddEventCut(*eventCutter,"IsAbsZSPDBelowValue","const AliVEvent&,Double_t","10");
//    
//    AliAnalysisMuMuCutElement* cutHasSPD = cr->AddEventCut(*eventCutter,"HasSPDVertex","const AliVEvent&","");
//    
//    AliAnalysisMuMuCutElement* cutZQA = cr->AddEventCut(*eventCutter,"IsSPDzQA","const AliVEvent&,Double_t,Double_t","0.25,0.5");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta0 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","0.0,13.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta1 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","13.0,18.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta2 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","18.0,24.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta3 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","24.0,30.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta4 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","30.0,36.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta5 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","36.0,45.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta6 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","45.0,60.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta7 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","60.0,90.0");
//    
//    AliAnalysisMuMuCutElement* cutdNdEta8 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","90.0,130.0");
//    
//    cr->AddCutCombination(eventTrue);
//    cr->AddCutCombination(ps);
//    cr->AddCutCombination(ps,cutHasSPD);
//    cr->AddCutCombination(ps,cutHasSPD,cutZQA);
//    
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18);
//    
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta0);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta1);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta2);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta3);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta4);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta5);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta6);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta7);
////    cr->AddCutCombination(ps,cutHasSPD,cutZQA,cutSPDZ18,cutdNdEta8);
//    
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA);
////    
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta0);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta1);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta2);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta3);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta4);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta5);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta6);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta7);
////    cr->AddCutCombination(ps,cutHasSPD,cutSPDZ10,cutZQA,cutdNdEta8);
//
//    task->AdoptSubAnalysis(nchAnalysis);
//  }
  
  
  //____________dNchdEta analysis and invariant mass analysis ____________//
//
//  if ( nchAnalysis && minvAnalysis && (!globalAnalysis && !singleAnalysis) )
//  {
//
//  }
//  AliAnalysisMuMuBinning* binning = task->Binning();
//  if ( singleAnalysis )
//  {
//    AliAnalysisMuMuCutElement* trackTrue = cr->AddTrackCut(*cr,"AlwaysTrue","const AliVParticle&","");
//    AliAnalysisMuMuCutElement* rabs = cr->AddTrackCut(*singleAnalysis,"IsRabsOK","const AliVParticle&","");
//    AliAnalysisMuMuCutElement* eta = cr->AddTrackCut(*singleAnalysis,"IsEtaInRange","const AliVParticle&,Double_t&,Double_t&","-4.0,-2.5");
//    
//    AliAnalysisMuMuCutElement* pDCA = cr->AddTrackCut(*singleAnalysis,"IsPDCAOK","const AliVParticle&","");
//    AliAnalysisMuMuCutElement* matchingTrigger = cr->AddTrackCut(*singleAnalysis,"IsMatchingTrigger","const AliVParticle&,Int_t","2");
////    cr->AddCutCombination(trackTrue);
////    cr->AddCutCombination(rabs);
////    cr->AddCutCombination(rabs,eta);
//    
//   // task->AdoptSubAnalysis(singleAnalysis);
//
//    if ( minvAnalysis )
//    {
//      AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","-4,-2.5");
//  
//      cr->AddCutCombination(pDCA,matchingTrigger,rabs,eta,pairy);
//      
//      binning->AddBin("psi","pt");
//      
//      task->AdoptSubAnalysis(minvAnalysis);
//    }
//  }
  
  /// below are the kind of configurations that can be performed :
  /// - adding cuts (at event, track or pair level)
  /// - adding bins (in pt, y, centrality, etc...) for minv (and meanpt)

//  AliAnalysisMuMuBinning* binning = task->Binning();
  
//  if (minvAnalysis && !singleAnalysis)
//  {
//    AliAnalysisMuMuCutElement* pairy = cr->AddTrackPairCut(*minvAnalysis,"IsRapidityInRange","const AliVParticle&,const AliVParticle&,Double_t&,Double_t&","-4,-2.5");
//    cr->AddCutCombination(eventTrue);
//    cr->AddCutCombination(ps);
//    
//    cr->AddCutCombination(pairy);
//
//    binning->AddBin("psi","integrated");
//    
    // pt binning
    
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
    
    // y binning
    
//    binning->AddBin("psi","y",-4,-2.5,"ILAB");
//    
//    for ( Int_t i = 0; i < 6; ++i )
//    {
//      Double_t y = -4+i*0.25;
//      
//      binning->AddBin("psi","y",y,y+0.25,"6PACK");
//    }
    
    // nch binning
//    if (nchAnalysis)
//    {
//      AliAnalysisMuMuCutElement* cutdNdEta0 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","0.0,13.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta1 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","13.0,18.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta2 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","18.0,24.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta3 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","24.0,30.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta4 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","30.0,36.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta5 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","36.0,45.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta6 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","45.0,60.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta7 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","60.0,90.0");
//      
//      AliAnalysisMuMuCutElement* cutdNdEta8 = cr->AddEventCut(*eventCutter,"IsMeandNchdEtaInRange","const AliVEvent&,Double_t,Double_t","90.0,130.0");
      
//      binning->AddBin("psi","dnchdeta",0.0,13.,"JAVI"); // 0 - 12
//      binning->AddBin("psi","dnchdeta",13.0,18.0,"JAVI"); // 12 - 18
//      binning->AddBin("psi","dnchdeta",18.0,24.0,"JAVI"); // 18 - 24
//      binning->AddBin("psi","dnchdeta",24.0,30.0,"JAVI"); // 23 - 30
//      binning->AddBin("psi","dnchdeta",30.0,36.0,"JAVI"); // 30 - 36
//      binning->AddBin("psi","dnchdeta",36.0,45.0,"JAVI"); // 36 - 45
//      binning->AddBin("psi","dnchdeta",45.0,60.0,"JAVI"); // 45 - 60
//      binning->AddBin("psi","dnchdeta",60.0,90.0,"JAVI"); // 60 - 90
//      binning->AddBin("psi","dnchdeta",90.0,130.0,"JAVI"); // 90 - 130
  
//      binning->AddBin("psi","v0a",0.0,22.,"PAPER"); // 0 - 22
//      binning->AddBin("psi","v0a",22.0,52.0,"PAPER"); // 22 - 52
//      binning->AddBin("psi","v0a",52.0,89.0,"PAPER"); // 52 - 89
//      binning->AddBin("psi","v0a",89.0,142.0,"PAPER"); // 89 - 142
//      binning->AddBin("psi","v0a",142.0,187.0,"PAPER"); // 142 - 187
//      binning->AddBin("psi","v0a",187.0,227.0,"PAPER"); // 187 - 227
//      binning->AddBin("psi","v0a",227.0,350.0,"PAPER"); // 227 -
//      
//      binning->AddBin("psi","v0acent",0.0,5.0,"PAPER"); // 0 - 22
//      binning->AddBin("psi","v0acent",5.0,10.0,"PAPER"); // 22 - 52
//      binning->AddBin("psi","v0acent",10.0,20.0,"PAPER"); // 52 - 89
//      binning->AddBin("psi","v0acent",20.0,40.0,"PAPER"); // 89 - 142
//      binning->AddBin("psi","v0acent",40.0,60.0,"PAPER"); // 142 - 187
//      binning->AddBin("psi","v0acent",60.0,80.0,"PAPER"); // 187 - 227
//      binning->AddBin("psi","v0acent",80.0,100.0,"PAPER"); // 227 -

      
//      task->AdoptSubAnalysis(nchAnalysis);
//    }
//    
  
//    task->AdoptSubAnalysis(minvAnalysis);
//  }
  
  // v0 centrality binning
   
//  binning->AddBin("centrality","v0a");
//  binning->AddBin("centrality","v0a",0,5);
//  binning->AddBin("centrality","v0a",5,10);
//  binning->AddBin("centrality","v0a",10,20);
//  binning->AddBin("centrality","v0a",20,40);
//  binning->AddBin("centrality","v0a",40,60);
//  binning->AddBin("centrality","v0a",60,80);
//  binning->AddBin("centrality","v0a",80,100);

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

