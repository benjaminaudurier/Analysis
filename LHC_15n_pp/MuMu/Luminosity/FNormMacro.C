//
//  FnormMacro.c
//
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro with Fnorm related procedure



//_____________________________________________________________________________
void FNormMacro(
const char* filename   ="AnalysisResults_CINT7-B-NOPF-MUFAST_safnewMUSPB.root",
const char* associatedSimFileName="",
const char* associatedSimFileName2="",
const char* beamYear="mumu.pp2015.fnorm.config",const int DebugLevel = 0)
{
  AliAnalysisMuMu analysis(filename,associatedSimFileName,associatedSimFileName2,beamYear);
  AliLog::SetGlobalDebugLevel(DebugLevel);
  // analysis.CleanFNorm();

  //_____ FNorm
  // analysis.ComputeNumberOfEvent(); // kTRUE = PileUpCOrrected
  // analysis.ComputeIntFnormFromCounters(AliAnalysisMuMuFnorm::kMUL,kTRUE); // kTRUE = PileUpCOrrected
  analysis.ComputeFnormScalers(AliAnalysisMuMuFnorm::kMUL,kTRUE);// kTRUE = PileUpCOrrected

  //_____ Mean
  // const char* patternOrList= "FnormOffline2PUPS,FnormOffline1PUPS,FnormScalers1PUPS";
  // analysis.ComputeFnormWeightedMeanGraphs(AliAnalysisMuMuFnorm::kMUL,patternOrList);

  PrintCounters(analysis);
  // Compute Mean Fnorm From all Fnorm result
  // MeanFnorm(analysis);
}

//_____________________________________________________________________________
void PrintCounters(AliAnalysisMuMu &ana)
{
    // ana.CC()->Print("centrality/event/trigger");
    // ana.CC()->Print("run/event/trigger");
    ana.CC()->Print("run/event/trigger","trigger:CINT7-B-NOPF-MUFAST&0TVX&0MUL");
    ana.CC()->Print("run/event/trigger","trigger:CINT7-B-NOPF-MUFAST&0TVX");
    ana.CC()->Print("run/event/trigger","trigger:CMUL7-B-NOPF-MUFAST");
    // ana.CC()->Print("run/trigger","trigger:CINT7-B-NOPF-CENTNOTRD");
    // Int_t NofMUL = TMath::Nint(ana.CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/event:PSALL/centrality:V0M_00.00_90.00")));
    // cout <<"Nofmul = " << NofMUL << endl;
    // ana.CC()->Print("");
    // ana.CC()->PrintKeyWords();
}
