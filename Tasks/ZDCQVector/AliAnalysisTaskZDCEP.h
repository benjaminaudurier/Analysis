#ifndef  AliAnalysisTaskZDCEP_H
#define  AliAnalysisTaskZDCEP_H

#include "AliAnalysisTaskSE.h"
#include "AliAnalysisUtils.h"
#include "AliMultSelection.h"

class AliAnalysisUtils;
class AliAnalysisTaskSE;
class AliMultSelection;
class TProfile;
class TProfile2D;
class TProfile3D;
class TH3D;
class AliFlowEvent;

class  AliAnalysisTaskZDCEP : public  AliAnalysisTaskSE
{
public:
  //  two  class  constructors
  AliAnalysisTaskZDCEP();
  AliAnalysisTaskZDCEP(const  char *name);
  //  class  destructor
  virtual ~AliAnalysisTaskZDCEP();
  //  called  once  at  beginning  or  runtime
  virtual void UserCreateOutputObjects();
  //  called  for  each  event
  virtual void UserExec(Option_t* option);
  // get centrality bin
  virtual Int_t GetCenBin(Double_t Centrality);
  //  called  at  end  of  analysis
  virtual void Terminate(Option_t* option);

  void SetZDCCalibList(TList* const wlist) {this->fZDCCalibList = wlist;}
  TList* GetZDCCalibList() const {return this->fZDCCalibList;}
  void SetTowerEqList(TList* const wlist) {this->fTowerEqList = wlist;}
  TList* GetTowerEqList() const {return this->fTowerEqList;}
  void GetZDCQVectors(Double_t QAX, Double_t QAY, Double_t QCX, Double_t QCY);
  Double_t GetEventPlaneV1(Double_t QX, Double_t QY);
  

private:

  TList* fOutputList;             // output list containing ZDC q-vectors
  TList* fOutputHistoList;             // output list containing ZDC q-vectors
  TList* fHistList;               // output list containing QA histograms
  Double_t fZDCGainAlpha;         //
  TList *fZDCCalibList;           // list for ZDC Q-vector re-centering
  TList *fTowerEqList;            // list for ZDC gain equalization
  TProfile* fZDCQHist[4];         //!
  TProfile3D* fZDCVtxHist[4];     //!
  TProfile2D* fZDCEcomTotHist[4]; //!
  TProfile3D* fZDCVtxCenHist[10][4]; //!
  TH1D *fTowerGainEq[2][5];       //!

  AliFlowVector* fZDCFlowVect[2]; //! ZDC q-vectors
  Int_t fCachedRunNum;            //
  const static Int_t fnRun = 90;  //
  Int_t fRunList[fnRun];          // run list
  TList *fQVecListRun[fnRun];     //! run-by-run list
  TArrayD fAvVtxPosX;             // average vx position vs run number
  TArrayD fAvVtxPosY;             // average vy position vs run number
  TArrayD fAvVtxPosZ;             // average vz position vs run number
  AliFlowEvent* fFlowEvent;       // flowevent

  AliAnalysisUtils* fAnalysisUtils; //!
  AliMultSelection* fMultSelection; //!

  ClassDef(AliAnalysisTaskZDCEP,1);
};

#endif