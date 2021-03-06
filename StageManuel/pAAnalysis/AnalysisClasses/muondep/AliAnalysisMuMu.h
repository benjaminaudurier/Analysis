#ifndef ALIANALYSISMUMU_H
#define ALIANALYSISMUMU_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// $Id$

///
/// AliAnalysisMuMu : helper class to digest/plot/massage results from
/// AliAnalysisTaskMuMu
///
/// author : Laurent Aphecetche (Subatech) and Javier Martin Blanco

#include "AliAnalysisMuMuBinning.h"
#include "TNamed.h"
#include <map>
#include <set>
#include <string>
#include <TString.h>
#include <vector>
#include "RQ_OBJECT.h"
#include "TH1.h"
#include "TH2.h"

class AliAnalysisMuMuConfig;
class AliAnalysisMuMuResult;
class AliAnalysisMuMuJpsiResult;
class AliAnalysisMuMuSpectra;
class AliCounterCollection;
class AliMergeableCollection;
class TF1;
class TFile;
class TGraph;
class TGraphErrors;
class TH1;
class TMap;

class AliAnalysisMuMu : public TObject, public TQObject
{

public:
  
  AliAnalysisMuMu(const char* filename="LHC12c_muons_AOD000_000179687.saf.root",
                  const char* associatedSimFileName="",
                  const char* associatedSimFileName2="",
                  const char* beamYear="pPb2013");
  
  virtual ~AliAnalysisMuMu();
  
  /* Basic checks */
  void BasicCounts(Bool_t detailTrigger=kFALSE,
                   ULong64_t* totalNmb=0x0,
                   ULong64_t* totalNmsl=0x0,
                   ULong64_t* totalNmul=0x0);
  
  void TriggerCountCoverage(const char* triggerList, Bool_t compact=kTRUE,
                            Bool_t orderByTriggerCount=kTRUE);
  
  void SelectRunByTrigger(const char* triggerList);
  
  AliAnalysisMuMuSpectra* FitParticle(const char* particle,
                                      const char* trigger,
                                      const char* eventType,
                                      const char* pairCut,
                                      const char* centrality,
                                      const AliAnalysisMuMuBinning& binning,
                                      const char* spectraType="minv",
                                      Bool_t corrected=kFALSE);

  AliAnalysisMuMuSpectra* CorrectSpectra(const char* type, const char* flavour="");
  
  TH2* ComputeSPDCorrection(const char* type="oneOverAccEff", const char* eventSel="PSALL", const char* triggerSel="ANY", Bool_t bkgReject=kTRUE);
  
  void ComputeFnorm();
  
  TH1* ComputeDiffFnormFromHistos(const char* what="psi",const char* quantity="ntrcorr",const char* flavour="JAVI",Bool_t printout=kFALSE);
  
  void ComputeDiffFnormFromInt(const char* triggerCluster="MUON", const char* eventSelection="PSALL", AliMergeableCollection* mc=0x0, const char* what="psi",const char* quantity="ntrcorr",const char* flavour="JAVI",Bool_t printout=kTRUE);
  
  void ComputeDiffFnormFromCounters(const char* filePileUpCorr="", const char* what="psi", const char* quantity="ntrcorr",
                                    const char* flavour="D2H",const char* triggerCluster="MUON",
                                    const char* eventSelectionFNorm="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00",
                                    const char* eventSelectionYield="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00", Bool_t printout=kTRUE);

  void ComputeDiffFnormFromGlobal(const char* what="psi",const char* quantity="ntrcorr",const char* flavour="D2H",const char* triggerCluster="MUON",
                                  const char* eventSelectionFnorm="PSALL",
                                  const char* eventSelectionYield="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00", Bool_t printout=kTRUE);
  
  void ComputeMeanFnorm(Bool_t weight=kTRUE, const char* triggerCluster="MUON", const char* eventSelectionFnorm="PSALL",
                        const char* eventSelectionYield="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00", const char* what="psi",const char* quantity="ntrcorr",
                        const char* flavour="D2H");

  void ComputeIntFnormFromCounters(const char* filePileUpCorr="", const char* triggerCluster="MUON", const char* eventSelectionFNorm="PSALL",
                                   const char* eventSelectionYield="PSALL", Bool_t printout=kTRUE);

  void PlotYiedWSyst(const char* triggerCluster="MUON");

  void ComputeRelativeValueAndSESystematics(const char* quantity,const char* flavour,const char* value2Test, const char* binListToExclude,
                                            const char* fNormType="mean", const char* evSelInt="PSALL",
                                            const char* evSelDiff="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00",
                                            const char* triggerCluster="MUON");

  void ComputeJpsiYield( Bool_t relative=kTRUE, const char* fNormType="mean", const char* evSelInt="PSALL",
                        const char* evSelDiff="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00",
                        const char* triggerCluster="MUON", const char* whatever="PSI-NTRCORR-AccEffCorr", const char* sResName="");

  void ComputeJpsiMPt(Bool_t relative=kTRUE, const char* evSelInt="PSALL", const char* evSelDiff="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00"
                      ,const char* spectra="PSI-NTRCORR-AccEffCorr-MeanPtVsMinvUS",const char* sResName="");

  void ComputeMBXSectionFractionInBins(const char* filePileUpCorr="", const char* eventSelection="PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00",
                                       const char* what="psi",const char* quantity="ntrcorr", const char* flavour="D2H");

  TH1* ComputeEquNofMB(const char* what="psi",const char* quantity="dnchdeta",const char* flavour="JAVI",Bool_t printout=kFALSE);

  Double_t ErrorPropagationAxBoverCxD(Double_t a,Double_t b,Double_t c,Double_t d);

  void TwikiOutputFnorm(const char* series="FnormOffline2PUPS,FnormScalersPUPS,FnormBest2,RelDifFnormScalersPUPSvsFnormOffline2PUPS,FnormScalersPUVDM,RelDifFnormScalersPUPSvsFnormScalersPUVDM") const;

  AliAnalysisMuMuSpectra* ComputeYield(const char* type, const char* flavour="");

  void CleanAllSpectra();
  
  ///------
  
//  static AliAnalysisMuMuSpectra* ComputeYield(const char* realFile="ds.list.saf.root",
//                                              const char* simFile="ds.sim.list.saf.root",
//                                              const  char* type="PSI-Y VS PT");

   AliAnalysisMuMuSpectra* RABy(const char* type="", const char* direction="pPb");

  ///-------
  
  TGraph* PlotEventSelectionEvolution(const char* trigger1="CINT7-B-NOPF-MUON", const char* event1="PSALL",
                                   const char* trigger2="CINT7-B-NOPF-MUON", const char* event2="PSALL",
                                      Bool_t drawFills=kFALSE,
                                      Bool_t asRejection=kTRUE) const;

  Bool_t Upgrade();
  
   Bool_t Upgrade(const char* filename);
  
   TObjArray* CompareJpsiPerCMUUWithBackground(const char* jpsiresults="results.root",
                                                     const char* backgroundresults="background.lhc11d.root");
  
   TGraph* CompareJpsiPerCMUUWithSimu(const char* realjpsiresults="results.root",
                                      const char* simjpsiresults="results.sim.root");
  
  
  static TFile* FileOpen(const char* file);
  
  static TString ExpandPathName(const char* file);
  
  
  Bool_t GetCollections(const char* rootfile,
                        AliMergeableCollection*& oc,
                        AliCounterCollection*& cc,
                        AliAnalysisMuMuBinning*& bin,
                        std::set<int>& runnumbers);
  
  AliAnalysisMuMuSpectra* GetSpectra(const char* what, const char* flavour="") const;
  
  TH1* PlotAccEfficiency(const char* whatever="PSI-INTEGRATED");
  
  TH1* PlotJpsiYield(const char* whatever="PSI-DNCHDETA-AccEffCorr");
  
  UInt_t GetSum(AliCounterCollection& cc, const char* triggerList, const char* eventSelection, Int_t runNumber=-1);
  
  ULong64_t GetTriggerScalerCount(const char* triggerList, Int_t runNumber);
  
  Int_t Jpsi(const char* what="integrated", const char* binningFlavour="", Bool_t fitMinv=kTRUE, Bool_t fitmPt=kTRUE, Bool_t fitmPt2=kTRUE, Bool_t onlyCorrected=kTRUE);
  
  Bool_t IsSimulation() const;
  
  AliMergeableCollection* OC() const { return fMergeableCollection; }
  AliCounterCollection* CC() const { return fCounterCollection; }
  AliAnalysisMuMuBinning* BIN() const { return fBinning; }

  void Print(Option_t* opt="") const;
  
  const std::set<int>& RunNumbers() const { return fRunNumbers; }
  
  void DrawMinv(const char* type,
                const char* particle,
                const char* trigger,
                const char* eventType,
                const char* pairCut,
                const char* centrality,
                const char* subresultname="",
                const char* flavour="") const;

  void DrawMinv(const char* type="PT", const char* particle="PSI", const char* flavour="", const char* subresultname="") const;
  
  Bool_t SetCorrectionPerRun(const TGraph& corr, const char* formula="");
  
  void UnsetCorrectionPerRun();
  
  void ExecuteCanvasEvent(Int_t event, Int_t px, Int_t py, TObject *sel);

  std::vector<Double_t> GetMCCB2Tails(const AliAnalysisMuMuBinning::Range& bin) const;
  
  AliAnalysisMuMu* SIM() const { return fAssociatedSimulation; }
  
  AliAnalysisMuMu* SIM2() const { return fAssociatedSimulation2; }
  
  AliAnalysisMuMuSpectra* SPECTRA(const char* fullpath) const;
  
  void SetParticleName(const char* particleName) { fParticleName = particleName; }
  
  const char* GetParticleName() { return fParticleName; }
  
  void Update();

  AliAnalysisMuMuConfig* Config();

  AliAnalysisMuMuConfig* Config() const { return fConfig; }
  
  void SetConfig(const AliAnalysisMuMuConfig& config);

  void SetCentralitySelectionList(const char* centralitySelectionList);
  
private:
  AliAnalysisMuMu(const AliAnalysisMuMu& rhs); // not implemented on purpose
  AliAnalysisMuMu& operator=(const AliAnalysisMuMu& rhs); // not implemented on purpose
  
  void ShowList(const char* title, const TString& list, const char separator=',') const;

  TFile* ReOpen(const char* filename, const char* mode) const;

  TString First(const TString& list) const;
  
  void GetParametersFromMC(TString& fitType, const char* pathCentrPairCut, const char* spectraName, AliAnalysisMuMuBinning::Range* bin) const;
  void GetParametersFromResult(TString& fitType, AliAnalysisMuMuJpsiResult* minvResult) const;

private:

  void SetNofInputParticles(AliAnalysisMuMuJpsiResult& r);

  
  TString fFilename; // file containing the result collections (of objects and counters) from AliAnalysisTaskMuMu
  AliCounterCollection* fCounterCollection; // collection of counters in file

  AliAnalysisMuMuBinning* fBinning; // binning
  
  AliMergeableCollection* fMergeableCollection; // collection of objects in file

  std::set<int> fRunNumbers; // run numbers
  
  TGraph* fCorrectionPerRun; // correction factor per run
  
  AliAnalysisMuMu* fAssociatedSimulation; // associated simulations (if any)
  AliAnalysisMuMu* fAssociatedSimulation2; // second associated simulations (if any)
  
  TString fParticleName; // Name of the simulated particle in the associated simulations

  AliAnalysisMuMuConfig* fConfig; // configuration
  
  ClassDef(AliAnalysisMuMu,12) // class to analysis results from AliAnalysisTaskMuMuXXX tasks
};

#endif
