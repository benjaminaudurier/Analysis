//
//  PileUpNtrCorrDistribution.h
//  
//
//  Created by Javier Martin Blanco on 29/06/15.
//
//

#ifndef ____PileUpNtrCorrDistribution__
#define ____PileUpNtrCorrDistribution__

#include <stdio.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TGraphErrors.h>


void PileUpNtrDistribution(Float_t nBC=3000000000, const char* muList="0.0385",Double_t yieldEnhancement=100.,
                           Bool_t contProb = kFALSE, Bool_t puEvtON = kTRUE, Bool_t puJpsiON = kTRUE, Bool_t mainMaxMult = kTRUE,
                           const char* fileY="YieldVsNtrCorr.root",const char* fileNtr="NtrCorrDistr_LHC13c_CINT7.root",
                           const char* fileZv="SPDZvertexDistr_LHC13de_CMUL7.root");

void SetPUConfig(Bool_t contProb, Bool_t puEvtON, Bool_t puJpsiON, Bool_t mainMaxMult);

Bool_t LoadDistrHistos(const char* fileY, const char* fileNtr, const char* fileZv);

void CreateStoringObjects();

void DrawHistos();

void SetEvent(TArrayD& zVtxsEventArray,TArrayI& nTrsEventArray,TArrayI& nJpsiEventArray);

void PileUpMerging(UInt_t& puMask, UInt_t& puMaskR, TArrayD& zVtxsEventArray,
                   TArrayI& nTrsEventArray, TArrayI& nJpsiEventArray);

void SetMainCol(UInt_t& mainColMask, UInt_t puMaskR, Int_t& posMax, Double_t& spdZvMain,
                Int_t& nTrEvent, TArrayD* zVtxsEventArray,TArrayI* nTrsEventArray);

void SetNofJpsi(Int_t& nJpsiEvent, Int_t posMax, TArrayI* nJpsiEventArray);

void GetPUFraction(Bool_t colPU, Int_t nTrEvent, Double_t nJpsiEvent);

void PrintValues();


//____________________________________
Double_t fYieldEnhancement(1.);

Bool_t fcontProb(kFALSE);
Bool_t fPUEvON(kFALSE);
Bool_t fpuJpsiON(kFALSE);
Bool_t fmainMaxMult(kFALSE);

TH1* fNofJpsi(0x0);
TH1* fYieldVsNtrCorr(0x0);
TH1* fNtrCorr(0x0);
TH1* fZVtx(0x0);

TGraphErrors* fgYield(0x0);

TF1* ffYield(0x0);

Double_t* faxis(0x0);
Int_t fsize(0);

TArrayI* fnEventsBin(0x0);
TArrayI* fnEventsBinPU(0x0);
TArrayI* fnEventsBinwJpsi(0x0);
TArrayI* fnEventsBinPUwJpsi(0x0);

TRandom3* frand(0x0);

TObjArray* fhNArray(0x0);
TObjArray* fhNBinsArray(0x0);
TObjArray* fhMeanXArray(0x0);
TObjArray* fhNBinsRatioArray(0x0);
TObjArray* fhMeanXRatioArray(0x0);
TObjArray* fhYArray =(0x0);
TObjArray* fhYieldArray(0x0);

TLegend *flegendNtr(0x0);
TLegend *flegendNofJpsi(0x0);
TLegend *flegendYield(0x0);

Int_t fnCollisions(0);

Double_t fnEvJpsiPileUp(0.);
Double_t fnMBWitSecondaryJpsi(0.);
Double_t fnMBWithJpsiFromSecColl(0.);
Double_t fnMBEventsWithJpsi(0.);
Double_t fnMBEventsWithMore1Jpsi(0.);
Double_t fnofTotalJpsi(0.);

Int_t fnMBEvents(0);
Int_t fnMBEventsMore1Col(0);
Int_t fnEventsPileUp(0);


#endif /* defined(____PileUpNtrCorrDistribution__) */
