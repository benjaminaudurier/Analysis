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
const char* filename="../AnalysisResults_saf_0-12_CINT7MUFAST.root",
const char* associatedSimFileName="",
const char* associatedSimFileName2="",
const char* beamYear="mumu.pp2015.config",const int DebugLevel =0)
{


  AliAnalysisMuMu analysis(filename,associatedSimFileName,associatedSimFileName2,beamYear);
  AliLog::SetGlobalDebugLevel(DebugLevel);
  // analysis.CleanFNorm();

  //_____ EquivalentNUmber
  // analysis.ComputeNumberOfEvent();

  //_____ FNorm

  // analysis.ComputeNumberOfEvent(); // kTRUE = PileUpCOrrected
  analysis.ComputeIntFnormFromCounters(AliAnalysisMuMuFnorm::kMUL,kTRUE); // kTRUE = PileUpCOrrected
  analysis.ComputeFnormScalers(AliAnalysisMuMuFnorm::kMUL,kTRUE);// kTRUE = PileUpCOrrected

  //_____ Mean
  // const char* patternOrList= "FnormOffline2PUPS,FnormOffline1PUPS,FnormScalers1PUPS";
  // analysis.ComputeFnormWeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,patternOrList);
  //_____

  // analysis.ComputeJpsiYield("INTEGRATED");


  //_____ Plot all Fnorm
  // TH1* h = analysis.OC()->Histo("/FNORM-CPBI1MUL-B-NOPF-MUON/PSALL/V0M_00.00_10.00/hFNormIntVSrun_CCENT_R2-B-NOPF-ALLNOTRD");
  // if (!h) return;
  // h->Scale()
  // h->Draw("Clone");

  PrintCounters(analysis);

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
    // ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-MUFAST&0MUL/centrality:V0M_00.00_90.00/event:ALL");
    // ana.CC()->Print("run/event","trigger:CINT7-B-NOPF-MUFAST/centrality:V0M_00.00_90.00");
    // ana.CC()->Print("run/event","trigger:CINT7-B-NOPF-MUFAST&0MUL/centrality:V0M_00.00_90.00");
    // ana.CC()->Print("run/event","trigger:CMUL7-B-NOPF-MUFAST/centrality:V0M_00.00_90.00");
    // ana.CC()->Print("centrality/event/trigger");
    ana.CC()->Print("run/trigger","trigger:C0TVX-B-NOPF-CENTNOTRD");
    ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-CENTNOTRD");
    ana.CC()->Print("run/trigger/centrality","trigger:CINT7-B-NOPF-MUFAST");
    // ana.CC()->Print("centrality/event","trigger:C0V0M-B-NOPF-CENTNOTRD&0MUL");
    // Int_t NofMUL = TMath::Nint(ana.CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/event:PSALL/centrality:V0M_00.00_90.00")));
    // cout <<"Nofmul = " << NofMUL << endl;
    // ana.CC()->Print("");
    // ana.CC()->PrintKeyWords();
}
