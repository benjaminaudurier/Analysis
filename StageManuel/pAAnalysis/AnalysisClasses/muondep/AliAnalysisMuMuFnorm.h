#ifndef ALIANALYSISMUMUFNORM_H
#define ALIANALYSISMUMUFNORM_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#ifndef ROOT_TObject
#  include "TObject.h"
#endif

#ifndef ROOT_TString
#  include "TString.h"
#endif

#include <set>
#include <vector>

class TObjArray;
class TGraphErrors;
class TGraph;
class AliAnalysisMuMuResult;
class AliCounterCollection;
class AliMergeableCollection;

class AliAnalysisMuMuFnorm : public TObject
{

public:
  
  enum ETriggerType
  {
    kMB=1,
    kMUL=2,
    kMSL=3,
    kMSH=4
  };
  
  AliAnalysisMuMuFnorm(AliCounterCollection& cc,
                       AliAnalysisMuMuFnorm::ETriggerType triggerType=AliAnalysisMuMuFnorm::kMUL,
                       const char* ocdbpath="raw://",
                       Bool_t compactGraphs=kFALSE);
  
  virtual ~AliAnalysisMuMuFnorm();
  
  void ComputeFnorm();
  
  void ComputeCorrectionFactors(Int_t eventSelectionCorrected);

  void ComputeFnormOffline(Int_t nstep, Bool_t pileUpCorrected, Int_t eventSelectionCorrected);


  void ComputeFnormScalers(Bool_t pileUpCorrected, Int_t eventSelectionCorrected);

  void ComputeGraphRelDif(const char* a, const char* b) const;

  void ComputeNofEvents(ETriggerType triggerType, Bool_t pileUpCorrected, Int_t eventSelectionCorrected);

  void ComputePileUpGraph(ETriggerType tt, Int_t eventSelectionCorrected=0);
  
  void ComputeEventSelectionGraph(ETriggerType tt, Int_t eventSelectionCorrected);

  void ComputeResultsFromGraphs();

  void ComputeTriggerFractions(ETriggerType triggerType, Bool_t physicsSelectionCorrected);
  
  void ComputeTriggerL0B(ETriggerType tt);

  //  void ComputeTSGraph(ETriggerType tt); // Commented out in the implementation file

  AliMergeableCollection* DetachMC();

  void DrawWith2Scales(const char* graphName1, const char* graphName2);
  
  TString GetEventSelectionName(Int_t eventSelectionCorrected) const;

  //  void GetFnorm(Int_t runNumber, const char* eventSelection, Double_t& value, Double_t& error) const; // Not implemented
  
  TGraphErrors* GetGraph(const char* name) const;
  
  void GetPurity(const char* triggerClassName, Int_t runNumber, Double_t& value, Double_t& error, Int_t eventSelectionCorrected) const;

  void GetValueAndErrorFromGraph(TGraphErrors* graph,
                                 Int_t runNumber,
                                 Double_t& value,
                                 Double_t& error) const;
  
  AliAnalysisMuMuResult* GetResult(const char* name) const;

  AliAnalysisMuMuResult* GetRunIntegratedResult(const TGraphErrors& g, const char* basename="Fnorm");

  AliMergeableCollection* MC() const;
  
  void MultiplyGraphs(const char* g1, const char* g2, const char* name="");
  
  TString OCDBPath() const { return fOCDBPath; }
  
  void Print(Option_t* opt="") const;

  void ScalerFnorm(Double_t& value, Double_t& error,
                   Double_t L0bCMUL7, Double_t purityCMUL7, Double_t purityCMUL7error,
                   Double_t L0bCINT7, Double_t purityCINT7, Double_t purityCINT7error,
                   Double_t pileUpFactor, Double_t pileUpFactorError);
  
  void ShowFnorm(const TObjArray& a) const;

  Bool_t TriggerClassnameTest(const char* triggerClassName, Int_t runNumber) const;

  void WeightedMeanGraphs(const char* pattern, const char* name="");
  
private:
  
  AliAnalysisMuMuFnorm(const AliAnalysisMuMuFnorm& rhs); // not implemented on purpose
  AliAnalysisMuMuFnorm& operator=(const AliAnalysisMuMuFnorm& rhs); // not implemented on purpose

  TGraphErrors* CreateAndAddGraph(const TString& name,
                                  const TString& title,
                                  const std::vector<double>& vx,
                                  const std::vector<double>& vxerr,
                                  const std::vector<double>& vy,
                                  const std::vector<double>& vyerr) const;

  Double_t GetSum(const char* triggerClassName, Int_t runNumber, Int_t eventSelectionCorrected) const;

  TString GetTriggerClassName(ETriggerType tt, Int_t runNumber) const;

  TString GetTriggerTypeName(ETriggerType tt) const;

  std::set<int> RunNumbers() const;

  TString MBTriggerClassName(Int_t runNumber) const;
  TString MSLTriggerClassName(Int_t runNumber) const;
  TString MULTriggerClassName(Int_t runNumber) const;
  TString MSHTriggerClassName(Int_t runNumber) const;

private:
  
  /*const*/ AliCounterCollection& fCounterCollection; // collection of trigger counters (not owner)
  mutable AliMergeableCollection* fMergeableCollection; // collection of results, histograms, graphs (ownership is in fIsOwner)
  Bool_t fIsOwner; // whether we are the owner of the mergeable collection
  TString fOCDBPath; // OCDB to be used (raw:// by default)
  mutable AliAnalysisMuMuResult* fResult; // combined result of the various computations
  Bool_t fIsCompactGraphs; // whether the graph produced should be compact
  ETriggerType fReferenceTriggerType; // reference trigger to get the weighting factors
  
  ClassDef(AliAnalysisMuMuFnorm,0) // class to compute MB to MUON trigger normalization factor
};

#endif
