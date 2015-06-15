#ifndef ALIANALYSISTASKGENTUNERJPSI_H
#define ALIANALYSISTASKGENTUNERJPSI_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/// \ingroup muondep
/// \class AliAnalysisTaskGenTunerJpsi
/// \brief task to tune the muon or JPsi pt/y generated distributions
//Author: Philippe Pillot - SUBATECH Nantes

#include <TObject.h>
#include "AliAnalysisTaskSE.h"
#include "AliMuonTrackCuts.h"

class TH1;
class TF1;
class TCanvas;

//________________________________________________________________________
class AliAnalysisTaskGenTunerJpsi : public AliAnalysisTaskSE {
public:
  
  AliAnalysisTaskGenTunerJpsi();
  AliAnalysisTaskGenTunerJpsi(const char *name);
  virtual ~AliAnalysisTaskGenTunerJpsi();
  
  virtual void   UserCreateOutputObjects();
  virtual void   NotifyRun();
  virtual void   UserExec(Option_t *);
  virtual void   Terminate(Option_t *);
  
  /// Select tracks in the given centrality range
  void SelectCentrality(Double_t min, Double_t max) {fCentMin = min; fCentMax = max;}
  
  // set standard cuts to select tracks to be considered
  void SetMuonTrackCuts(AliMuonTrackCuts &trackCuts);
  
  /// set the muon low pT cut
  void SetMuonPtCut(Double_t cut) {fPtCut = cut;}
  
  /// weight simulated/reconstructed particles using using given functions
  void Weight(Bool_t flag) {fWeight = flag;}
  
  // // set the name of the data file used in terminate to tune the generated distributions
  // void SetDataFile(const Char_t* name) {fDataFile = name;}
  
  // set the parameters of the current (and new) pT generated distribution
  void SetPtParam(const Double_t *pOld, const Bool_t *fixOld, const Double_t *pNew,
		  const Bool_t *fixNew, Double_t min, Double_t max);
  
  // set the pt ref. histo to reweight MC
  void SetPtRefHisto(TH1* hptRef) {if(hptRef) fHptRef = static_cast<TH1*>(hptRef->Clone());}

  // set the Y ref. histo to reweight MC
  void SetYRefHisto(TH1* hyRef) {if(hyRef) fHyRef = static_cast<TH1*>(hyRef->Clone());}

  // set the name of the spectra used in terminate to tune the generated distributions
  void SetPtBin(Double_t *bin,Int_t nofbin ) {fPtBin = bin; fPtNofBin = nofbin;}

  // set the name of the spectra used in terminate to tune the generated distributions
  void SetYBin(Double_t *bin,Int_t nofbin ) {fYBin = bin; fYNofBin = nofbin;}
  
  /// get the current parameters of the pT generated distribution
  Double_t* GetOldPtParam() {return fPtFunc ? fPtFunc->GetParameters() : 0x0;}
  /// get the current parameters of the pT generated distribution fitted in the MC range
  Double_t* GetOldPtParamMC() {return fPtFuncMC ? fPtFuncMC->GetParameters() : 0x0;}
  /// get the new parameters of the pT generated distribution
  Double_t* GetNewPtParam() {return fPtFuncNew ? fPtFuncNew->GetParameters() : 0x0;}
  /// get the flags to fix current parameters of the pT generated distribution
  Bool_t* GetOldFixPtParam() {return fPtFix;}
  /// get the flags to fix new parameters of the pT generated distribution
  Bool_t* GetNewFixPtParam() {return fPtFixNew;}
  /// get the generated pT fit function with current parameters
  TF1* GetOldPtFunc() {return fPtFunc;}
  /// get the generated pT fit function with current parameters fitted in the MC range
  TF1* GetOldPtFuncMC() {return fPtFuncMC;}
  /// get the generated pT fit function with new parameters
  TF1* GetNewPtFunc() {return fPtFuncNew;}
  
  // set the parameters of the current (and new) y generated distribution
  void SetYParam(const Double_t *pOld, const Bool_t *fixOld, const Double_t *pNew,
		 const Bool_t *fixNew, Double_t min, Double_t max);
  /// get the current parameters of the y generated distribution
  Double_t* GetOldYParam() {return fYFunc ? fYFunc->GetParameters() : 0x0;}
  /// get the current parameters of the y generated distribution fitted in the MC range
  Double_t* GetOldYParamMC() {return fYFuncMC ? fYFuncMC->GetParameters() : 0x0;}
  /// get the new parameters of the y generated distribution
  Double_t* GetNewYParam() {return fYFuncNew ? fYFuncNew->GetParameters() : 0x0;}
  /// get the flags to fix current parameters of the y generated distribution
  Bool_t* GetOldFixYParam() {return fYFix;}
  /// get the flags to fix new parameters of the y generated distribution
  Bool_t* GetNewFixYParam() {return fYFixNew;}
  /// get the generated y fit function with current parameters
  TF1* GetOldYFunc() {return fYFunc;}
  /// get the generated y fit function with current parameters fitted in the MC range
  TF1* GetOldYFuncMC() {return fYFuncMC;}
  /// get the generated y fit function with new parameters
  TF1* GetNewYFunc() {return fYFuncNew;}
  
  // get canvas containing generated and reconstructed distributions
  TCanvas* GetResults() {return fcRes;}
  // get canvas containing data/MC ratios
  TCanvas* GetRatios() {return fcRat;}
  
private:
  
  /// Not implemented
  AliAnalysisTaskGenTunerJpsi(const AliAnalysisTaskGenTunerJpsi& rhs);
  /// Not implemented
  AliAnalysisTaskGenTunerJpsi& operator = (const AliAnalysisTaskGenTunerJpsi& rhs);
  
  // Compute acc*eff and binomial errors by hand, i.e. not using TGraphAsymmErrors
  TH1* ComputeAccEff(TH1 &hGen, TH1 &hRec, const Char_t *name, const Char_t *title);
  
  // generated pT fit function
  static Double_t Pt(const Double_t *x, const Double_t *p);
  // generated y fit function
  static Double_t Y(const Double_t *x, const Double_t *p);
  // generated pT fit function ratio
  static Double_t PtRat(const Double_t *x, const Double_t *p);
  // generated y fit function ratio
  static Double_t YRat(const Double_t *x, const Double_t *p);
  
  // adjust the lower edge of the fit range according to the content of the histogram
  Double_t GetFitLowEdge(TH1 &h);
  // adjust the upper edge of the fit range according to the content of the histogram
  Double_t GetFitUpEdge(TH1 &h);
  
  // normalize the function to its integral in the given range
  void NormFunc(TF1 *f, Double_t min, Double_t max);
  
private:
  
  static const Int_t fgkNPtParam = 3;
  static const Int_t fgkNYParam  = 2;
  
  enum eList {
    kPtGen   = 0, ///< pT distribution of generated particle
    kPtRec   = 1, ///< pT distribution of reconstructed particle
    kYGen    = 2, ///< y distribution of generated particle
    kYRec    = 3,  ///< y distribution of reconstructed particle
    // kPhiGen  = 4, ///< phi distribution of generated particle
    // kPhiRec  = 5  ///< phi distribution of reconstructed particle
  };
  
  TObjArray*  fList; //!< List of output object
  
  Double_t  fCentMin;               ///< select centrality > fCentMin
  Double_t  fCentMax;               ///< select centrality <= fCentMax
  Double_t*  fPtBin;                 //< select Pt bin
  Double_t*  fYBin;                  ///< select Y bin
  Int_t     fPtNofBin;              ///< Number of Pt bin
  Int_t     fYNofBin;               ///< Number of Y bin
  TH1*       fHptRef;                //!< pt ref. data histo        
  TH1*       fHyRef;                 //!< y ref. data histo 
  AliMuonTrackCuts* fMuonTrackCuts; ///< cuts to select tracks to be considered
  Double_t fPtCut;                  ///< muon low pT cut
  Bool_t   fWeight;                 ///< weight simulated/reconstructed particles using using given functions
  // TString  fDataFile;               ///< data file used in terminate to tune the generated distributions
  TF1     *fPtFunc;                 ///< generated pT fit function with current parameters
  TF1     *fPtFuncMC;               //!< generated pT fit function with current parameters in the MC range
  Bool_t  *fPtFix;                  //!< flags to fix or not the current parameters
  TF1     *fPtFuncNew;              ///< generated pT fit function with new parameters
  Bool_t  *fPtFixNew;               //!< flags to fix or not the new parameters
  TF1     *fYFunc;                  ///< generated y fit function with current parameters
  TF1     *fYFuncMC;                //!< generated y fit function with current parameters in the MC range
  Bool_t  *fYFix;                   //!< flags to fix or not the current parameters
  TF1     *fYFuncNew;               ///< generated y fit function with new parameters
  Bool_t  *fYFixNew;                //!< flags to fix or not the new parameters
  TCanvas *fcRes;                   //!< generated and reconstructed distributions
  TCanvas *fcRat;                   //!< data/MC ratios
  
  // copy functions for weighting particles because they are lost when streamed to a file (proof and grid mode)
  TF1 *fPtCopyFunc;    //!< generated pT fit function with current parameters
  TF1 *fPtCopyFuncNew; //!< generated pT fit function with new parameters
  TF1 *fYCopyFunc;     //!< generated y fit function with current parameters
  TF1 *fYCopyFuncNew;  //!< generated y fit function with new parameters
  
  ClassDef(AliAnalysisTaskGenTunerJpsi, 1);
};

//________________________________________________________________________
inline void AliAnalysisTaskGenTunerJpsi::SetMuonTrackCuts(AliMuonTrackCuts &trackCuts)
{
  /// set standard cuts to select tracks to be considered
  delete fMuonTrackCuts;
  fMuonTrackCuts = new AliMuonTrackCuts(trackCuts);
}

#endif

