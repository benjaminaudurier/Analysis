/
//  FnormMacro.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro with Fnorm related procedure



//_____________________________________________________________________________
void FNormMacro(
const char* filename="../AnalysisResults.root",
const char* associatedSimFileName="",
const char* associatedSimFileName2="",
const char* beamYear="mumu.PbPb2015.config",const int DebugLevel =2)
{


  AliAnalysisMuMu analysis(filename,associatedSimFileName,associatedSimFileName2,beamYear);
  AliLog::SetGlobalDebugLevel(DebugLevel);


  //_____ FNorm
  // analysis.CleanFNorm();
  // analysis.ComputeIntFnormFromCounters(AliAnalysisMuMuFnorm::kMUL,kFALSE); // kTRUE = PileUpCOrrected
  // analysis.ComputeFnormScalers(AliAnalysisMuMuFnorm::kMUL,kTRUE);// kTRUE = PileUpCOrrected

  //_____ Mean
  const char* patternOrList = "Offline";
  const char* graphName= "";

  //   WeightedMeanGraphs("Offline");
//   WeightedMeanGraphs("Scalers");
//   WeightedMeanGraphs("FnormOffline2PUPS,FnormOffline1PUPS","FnormOffline12PUPS");
  
//   WeightedMeanGraphs("FnormOffline2PUPS,FnormScalersPUPS","FnormBest2");
//   
  // analysis.ComputeFnormWeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,patternOrList,graphName);

  // analysis.WeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,"Scalers");
  // analysis.WeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,"FnormOffline2PUPS,FnormOffline1PUPS","FnormOffline12PUPS");
  
  // analysis.WeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,"FnormOffline2PUPS,FnormScalersPUPS","FnormBest2");
  //_____ 
    
  // analysis.ComputeJpsiYield("INTEGRATED");
  

  //_____ Plot all Fnorm
  // TH1* h = analysis.OC()->Histo("/FNORM-CPBI1MUL-B-NOPF-MUON/PSALL/V0M_00.00_10.00/hFNormIntVSrun_CCENT_R2-B-NOPF-ALLNOTRD");
  // if (!h) return;
  // h->Scale()
  // h->Draw("Clone"); 
  
  // PrintCounters(analysis);

  // Compute Mean Fnorm From all Fnorm result   
  // MeanFnorm(analysis);
  
}

//_____________________________________________________________________________
void PrintCounters(AliAnalysisMuMu &ana)
{
    // ana.CC()->Print("trigger/centrality/event","trigger:CVHN-B-NOPF-ALLNOTRD|CVHN_R2-B-NOPF-ALLNOTRD|CCENT-B-NOPF-ALLNOTRD|CCENT_R2-B-NOPF-ALLNOTRD/centrality:V0M_00.00_07.50/event:ALL/run:169557");
    // ana.CC()->Print("trigger/centrality/event","trigger:CVHN-B-NOPF-ALLNOTRD|CVHN_R2-B-NOPF-ALLNOTRD|CCENT-B-NOPF-ALLNOTRD|CCENT_R2-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_00.00_07.50/event:ALL/run:169557");
    // ana.CC()->Print("run/trigger/centrality/event","trigger:CVLN_B2-B-NOPF-ALLNOTRD|CVLN-B-NOPF-ALLNOTRD|CVLN_R1-B-NOPF-ALLNOTRD|CSEMI-B-NOPF-ALLNOTRD|CSEMI_R1-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_10.00_50.00/event:ALL");
    // ana.CC()->Print("run/trigger/centrality/event","trigger:CPBI2_B1-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_00.00_90.00/event:ALL");
    // ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-MUFAST&0MUL/centrality:V0M_00.00_100.00/event:PSALL");
    // ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-MUFAST/centrality:V0M_00.00_100.00/event:PSALL"); 
    ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-CENT&0MUL/centrality:V0M_00.00_100.00/event:PSALL");
    ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-CENT/centrality:V0M_00.00_100.00/event:PSALL");

}



//_____________________________________________________________________________
void MeanFnorm(AliAnalysisMuMu &ana)
{
   if (!ana.OC() || !ana.CC())
 {
   AliError("No mergeable/counter collection. Consider Upgrade()");
   return ;
 }
 else
 {
   cout <<      " ================================================================ " << endl;
   cout <<      "                     ComputeIntFnormFromCounters                 " << endl;
   cout <<      " ================================================================ " << endl;
 }

 //_______ Definitions of objects, pointers ...:
 TString striggerDimuon     = ana.Config()->First(Config()->DimuonTriggerKey(),IsSimulation());
 TString striggerMuon       = ana.Config()->First(Config()->MuonTriggerKey(),IsSimulation());
 TString seventType         = ana.Config()->First(Config()->EventSelectionKey(),IsSimulation());
 TString scentrality        = ana.Config()->First(Config()->CentralitySelectionKey(),IsSimulation());
 TObjArray* triggerMBArray  = ana.Config()->GetListElements(Config()->MinbiasTriggerKey(),IsSimulation());



 AliDebug(1,Form("Muon Trigger      : %s",striggerMuon.Data()));
 AliDebug(1,Form("DiMuon Trigger    : %s",striggerDimuon.Data()));
 AliDebug(1,Form("eventype          : %s",seventType.Data()));
 AliDebug(1,Form("centrality Trigger: %s",scentrality.Data()));

  //________Decoding of the pileup correction file
 Bool_t corrPU(kFALSE);
 TObjArray* pUCorr = new TObjArray();
 if ( strlen(filePileUpCorr) > 0 )
 {
   std::cout << "Extracting Pile-Up correction factors from " << filePileUpCorr << std::endl;
   char line[1024];
   ifstream in(filePileUpCorr);

   while ( in.getline(line,1024,'\n'))
   {
     TString lrun(line);
     TString lvalue(line);

     lrun.Remove(0,4);
     lrun.Remove(6,67);

     lvalue.Remove(0,lvalue.First("=")+1);

     std::cout << "RUN: " << lrun.Data() << " PUFactor = " << lvalue.Data() << std::endl;

     pUCorr->Add(new TParameter<Double_t>(lrun.Data(),lvalue.Atof()));
   }
   corrPU = kTRUE;
 }
 //________


 //________Some needed quantities
 TIter nextTriggerMB(triggerMBArray);
 TObjString *st;

 TH1* h=0x0;
 // TH1* h1=0x0; futur pointer for syst. histo

 const char* year = beamYear;
 const TString syear(year);

 TString sQuantity(quantity);
 //________

 // Run related quantities
 TString sruns = CC()->GetKeyWords("run");
 TObjArray* runs = sruns.Tokenize(",");
 Double_t NofRuns = runs->GetEntries();
 TIter nextRun(runs);
 TObjString* s;
 //________

 //__________ Get binning
 AliAnalysisMuMuBinning* binning = BIN()->Project(what,sQuantity.Data(),flavour);
 if ( !binning )
 {
   AliError(Form("%s-%s-%s binning does not exist",what,sQuantity.Data(),flavour));
   return;
 }
 TObjArray      * bin = binning->CreateBinObjArray(what,sQuantity.Data(),flavour);
 Double_t       * binArray = binning->CreateBinArray();
 Int_t nEntries = bin->GetEntries();
 //___________

 // Histo that will be saved
 TH1* hNofEqMB = new TH1F(Form("hNofEqMBVS%s",sQuantity.Data()),Form("Equivalent MB events per CMUL vs %s",sQuantity.Data()),
                          nEntries,binArray);
 TH1* hFNormTot = new TH1F(Form("hFNormVS%s",sQuantity.Data()),Form("Normalization factor vs %s;%s;FNorm",sQuantity.Data(),
                                                                    sQuantity.Data()),nEntries,binArray);

 //Array to store FNorm
 Double_t* FNormTot = new Double_t[nEntries];
 Double_t* FNormTotError = new Double_t[nEntries];
 Double_t* FNormTotErrorSyst = new Double_t[nEntries];

 TString id(Form("/FNORM-%s/%s/%s/%s",striggerDimuon.Data(),seventType.Data(),scentrality.Data(),syear.Data())); // Path to save the Fnorm and EqNofMB histos in the mergeable collection
 //________

 TList* lRun2Reject = new TList();
 lRun2Reject->SetOwner(kTRUE);

 TIter nextBin(bin);
 AliAnalysisMuMuBinning::Range* r;

}


//_____________________________________________________________________________
  void MeanFnorm(AliAnalysisMuMu &ana)
  {  
  if (!ana.OC() || !ana.CC())
  {
    AliError("No mergeable/counter collection. Consider Upgrade()");
    return ;
  }
  else
  {
    cout <<      " ================================================================ " << endl;
    cout <<      "                  Compute Mean Fnorm From Counters                " << endl;
    cout <<      " ================================================================ " << endl;
  }


  // Get configuration settings
  TObjArray* eventTypeArray   = ana.Config()->GetListElements(AliAnalysisMuMuConfig::kEventSelectionList,IsSimulation());
  TObjArray* triggerMuonArray = ana.Config()->GetListElements(AliAnalysisMuMuConfig::kDimuonTriggerList,IsSimulation());
  TObjArray* triggerMBArray   = ana.Config()->GetListElements(AliAnalysisMuMuConfig::kMinbiasTriggerList,IsSimulation());
  TObjArray* centralityArray  = ana.Config()->GetListElements(AliAnalysisMuMuConfig::kCentralitySelectionList, IsSimulation());

  // Iterator for loops
  TIter nextTriggerMuon(triggerMuonArray);
  TIter nextTriggerMB(triggerMBArray);
  TIter nextEventType(eventTypeArray);
  TIter nextCentrality(centralityArray);

  // Strings
  TObjString* striggerMuon;
  TObjString* striggerMB;
  TObjString* seventType;
  TObjString* scentrality;

  //Pointers on histo
  TH1*h(0x0);
  TH1*h1(0x0);
  TH1*h2(0x0);
  
  Double_t FNormOverStat(0.);
  Double_t FNormTotError(0.);
  Double_t FNormTotErrorInverse(0.);
  Double_t FNormTotErrorSys(0.);
  Double_t Norm(1.);

  Int_t n =0; //counter

  nextEventType.Reset();
  // Loop on each envenType (see MuMuConfig)
  //==============================================================================
  while ( ( seventType = static_cast<TObjString*>(nextEventType())) )
  {
    AliDebug(1,Form("EVENTTYPE %s",seventType->String().Data()));
    nextTriggerMuon.Reset();
    // Loop on each Muon trigger (see MuMuConfig)
    //==============================================================================
    while ( ( striggerMuon = static_cast<TObjString*>(nextTriggerMuon())) )
    {
      AliDebug(1,Form("-MUON TRIGGER %s",striggerMuon->String().Data()));
      nextTriggerMB.Reset();
      // Loop on each MB trigger (not the ones in MuMuConfig but the ones set)
      //==============================================================================
      while ( ( striggerMB = static_cast<TObjString*>(nextTriggerMB())) )
      {
        AliDebug(1,Form("-- MB PAIRCUT %s",striggerMB->String().Data()));
        nextCentrality.Reset();
        // Loop on each centrality
        //==============================================================================
        while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
        {
          TString id(Form("/FNORM-%s/%s/%s/PbPb",striggerMuon->String().Data(),seventType->String().Data(),scentrality->String().Data())); // Path where are saved histos in the mergeable collection

          h = OC()->Histo(id.Data(),Form("hFNormIntVSrun_%s",striggerMB->String().Data()));
          if (!h)
          {
            AliDebug(1,Form("Can't get histo %s/hFNormIntVSrun_%s",id.Data(),striggerMB->String().Data()));
            continue;
          }

          h1 = OC()->Histo(id.Data(),Form("hFNormInt_%s",striggerMB->String().Data()));
          if (!h1)
          {
            AliDebug(1,Form("Can't get histo %s/hFNormInt_%s",id.Data(),striggerMB->String().Data()));
            continue;
          }

          h2 = OC()->Histo(id.Data(),Form("hFNormIntSys_%s",striggerMB->String().Data()));
          if (!h2)
          {
            AliDebug(1,Form("Can't get histo %s/hFNormIntSys_%s",id.Data(),striggerMB->String().Data()));
            continue;
          }

          cout << Form("Fnorm from %s/%s added",id.Data(),h1->GetName()) << endl;
          cout << Form("Fnorm from %s/%s added",id.Data(),h2->GetName()) << endl;

          // Normalise with respect to centrality
          if (scentrality->String().Contains("V0M_00.00_90.00"))
          {
            Norm = 1.;

            FNormOverStat        = FNormOverStat + (Norm*h1->GetBinContent(1)) /(TMath::Power(Norm *h1->GetBinError(1),2.));
            FNormTotError        = FNormTotError + 1./(TMath::Power(Norm*h1->GetBinError(1),2.));
            FNormTotErrorInverse = FNormTotErrorInverse + 1./(TMath::Power(Norm*h1->GetBinError(1),-2.));
            FNormTotErrorSys     = FNormTotErrorSys + 1./(TMath::Power(Norm*h2->GetBinContent(1),2.));

            cout <<"--- Quantities from histogram             :                " << endl;
            cout <<" - Norm                                   = " << Norm << endl;
            cout <<" - FNormHisto                             = " << h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << h2->GetBinContent(1) << endl;
            cout <<"--- Quantities (normalized) from histogram:                " << endl;
            cout <<" - FNormHisto                             = " << Norm*h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << Norm*h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << Norm*h2->GetBinContent(1) << endl;
            cout <<"--- After addition                        :                 " << endl;
            cout <<" - FNormOverStat                          = " << FNormOverStat << endl;
            cout <<" - FNormTotError                          = " << FNormTotError << endl;
            cout <<" - FNormTotErrorInverse                   = " << FNormTotErrorInverse << endl;
            cout <<" - FNormTotErrorSys                       = " << FNormTotErrorSys << endl;
            }
          else if (scentrality->String().Contains("V0M_10.00_50.00"))
          {
            Norm = (1./0.4)*0.445*0.9;

            FNormOverStat        = FNormOverStat + (Norm*h1->GetBinContent(1)) /(TMath::Power(Norm *h1->GetBinError(1),2.));
            FNormTotError        = FNormTotError + 1./(TMath::Power(Norm*h1->GetBinError(1),2.));
            FNormTotErrorInverse = FNormTotErrorInverse + 1./(TMath::Power(Norm*h1->GetBinError(1),-2.));
            FNormTotErrorSys     = FNormTotErrorSys + 1./(TMath::Power(Norm*h2->GetBinContent(1),2.));

            cout <<"--- Quantities from histogram             :                " << endl;
            cout <<" - Norm                                   = " << Norm << endl;
            cout <<" - FNormHisto                             = " << h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << h2->GetBinContent(1) << endl;
            cout <<"--- Quantities (normalized) from histogram:                " << endl;
            cout <<" - FNormHisto                             = " << Norm*h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << Norm*h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << Norm*h2->GetBinContent(1) << endl;
            cout <<"--- After addition                        :                 " << endl;
            cout <<" - FNormOverStat                          = " << FNormOverStat << endl;
            cout <<" - FNormTotError                          = " << FNormTotError << endl;
            cout <<" - FNormTotErrorInverse                   = " << FNormTotErrorInverse << endl;
            cout <<" - FNormTotErrorSys                       = " << FNormTotErrorSys << endl;

          }
          else if (scentrality->String().Contains("V0M_00.00_07.50"))
          {
            Norm = (1./0.075)*0.443*0.9;

            FNormOverStat        = FNormOverStat + (Norm*h1->GetBinContent(1)) /(TMath::Power(Norm *h1->GetBinError(1),2.));
            FNormTotError        = FNormTotError + 1./(TMath::Power(Norm*h1->GetBinError(1),2.));
            FNormTotErrorInverse = FNormTotErrorInverse + 1./(TMath::Power(Norm*h1->GetBinError(1),-2.));
            FNormTotErrorSys     = FNormTotErrorSys + 1./(TMath::Power(Norm*h2->GetBinContent(1),2.));

            cout <<"--- Quantities from histogram             :                " << endl;
            cout <<" - Norm                                   = " << Norm << endl;
            cout <<" - FNormHisto                             = " << h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << h2->GetBinContent(1) << endl;
            cout <<"--- Quantities (normalized) from histogram:                " << endl;
            cout <<" - FNormHisto                             = " << Norm*h1->GetBinContent(1) << endl;
            cout <<" - FNormHistoError                        = " << Norm*h1->GetBinError(1) << endl;
            cout <<" - FNormHistoSys                          = " << Norm*h2->GetBinContent(1) << endl;
            cout <<"--- After addition                        :                 " << endl;
            cout <<" - FNormOverStat                          = " << FNormOverStat << endl;
            cout <<" - FNormTotError                          = " << FNormTotError << endl;
            cout <<" - FNormTotErrorInverse                   = " << FNormTotErrorInverse << endl;
            cout <<" - FNormTotErrorSys                       = " << FNormTotErrorSys << endl;

          }
          else
          {
            AliError("Check this method for centrality selection !");
            return;
          }

          n++;
        }
      }
    }
  }

cout << "Mean FNorm  computed from " << n <<" results = " << FNormOverStat/FNormTotError << " +/- " <<
    TMath::Sqrt(FNormTotErrorInverse) << " (stat) +/-" << TMath::Sqrt(FNormTotErrorSys) << " (sys) " <<endl;

delete triggerMuonArray ;
delete triggerMBArray ;
delete eventTypeArray ;
delete centralityArray ;
}
