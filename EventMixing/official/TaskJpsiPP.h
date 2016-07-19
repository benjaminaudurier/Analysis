/*
 *simple DIMuon Analysis task
 *
 */
#ifndef ALIANALYSISTASKJPSIPP_H
#define ALIANALYSISTASKJPSIPP_H

class TH1F;
class TH2F;
class TH3F;
class TNtuple;
class TList;
class TObjArray;
class TArrayF;

class AliAnalysisManager;
class AliVEvent;
class AliMuonTrackCuts;
class AliAnalysisUtils;
class AliMultiInputEventHandler;
class AliMixInputEventHandler2;

#ifndef ALIANALYSISTASKSE_H
#include "AliAnalysisTaskSE.h"
#endif

class TaskJpsiPP : public AliAnalysisTaskSE {
public:
  enum treeIndex {kUnlike = 0,kMixUnlike = 1,kPP=2,kMixPP=3,kMM=4,kMixMM=5 };
    TaskJpsiPP();
    TaskJpsiPP(const char *name, const int triggerofevent, int firstRun, int lastRun, std::vector<Double_t> arrayCentralityBins,std::vector<Double_t> arrayRapidityBins,std::vector<Double_t> arrayPtBins);
    virtual ~TaskJpsiPP();
    virtual void     NotifyRun();
    virtual void     UserCreateOutputObjects();
    virtual void     UserExec(Option_t *option);
    virtual void     UserExecMix(Option_t *);
    virtual void     Terminate(Option_t *);
    void FillHisto(TH1F ****arrayOfHistos, Double_t valueToFill, Double_t pT, Double_t rapidity, Double_t centrality);
    Int_t CentralityToBin(Double_t x);
    Int_t RapidityToBin(Double_t x);
    Int_t PtToBin(Double_t x);
    AliVEvent* GetMainEvent();
    AliVEvent* GetMixedEvent(Int_t buffId=0);

    AliMultiInputEventHandler *SetMainInputHandler(AliAnalysisManager *mgr);
    AliMixInputEventHandler2 *SetMixingInputHandler(AliMultiInputEventHandler *mainIH);
    private:



    AliMuonTrackCuts *fMuonTrackCuts;//!
    UInt_t fTriggerofEvent;
    Int_t fFirstRun;
    Int_t fLastRun;
    std::vector<Double_t> fArrayCentralityBins;
    Int_t fNumberOfCentralityBins;
    std::vector<Double_t> fArrayRapidityBins;
    Int_t fNumberOfRapidityBins;
    std::vector<Double_t> fArrayPtBins;
    Int_t fNumberOfPtBins;
    AliMultiInputEventHandler *fMainInputHandler;    //! tmp pointer to mixing input handler
    AliMixInputEventHandler2   *fMixingInputHandler;  //! tmp pointer to mixing input handler

    TH1I *fHistoTotalEvents;  //!
    TH1I *fHistoCMUL7Events;  //!
    TH1I *fHistoCINT7Events;  //!
    TH1I *fHistoCMSL7Events;  //!
    TH1I *fHistoMixingInfo; //!
    TH1F *fHistoEventCentrality;//!

    TH1F *fHistoMuonPt; //!
    TH1F *fHistoMuonEta;//!
    TH1F *fHistoMuonPhi;//!

    TH1F ****fArrayHistosUnlikeInvMass;//!
    TH1F ****fArrayHistosUnlikeRapidity;//!
    TH1F ****fArrayHistosUnlikePt;//!

    TH1F ****fArrayHistosPPLikeInvMass;//!
    TH1F ****fArrayHistosPPLikeRapidity;//!
    TH1F ****fArrayHistosPPLikePt;//!

    TH1F ****fArrayHistosMMLikeInvMass;//!
    TH1F ****fArrayHistosMMLikeRapidity;//!
    TH1F ****fArrayHistosMMLikePt;//!

    TH1F ****fArrayHistosMixUnlikeInvMass;//!
    TH1F ****fArrayHistosMixUnlikeRapidity;//!
    TH1F ****fArrayHistosMixUnlikePt;//!

    TH1F ****fArrayHistosMixPPLikeInvMass;//!
    TH1F ****fArrayHistosMixPPLikeRapidity;//!
    TH1F ****fArrayHistosMixPPLikePt;//!

    TH1F ****fArrayHistosMixMMLikeInvMass;//!
    TH1F ****fArrayHistosMixMMLikeRapidity;//!
    TH1F ****fArrayHistosMixMMLikePt;//!

    TList *fListEvent; //<
    TList *fListMuon; //<
    TList *fListMixUnlikeDimuon;//<
    TList *fListUnlikeDimuon;//<
    TList *fListPPLikeDimuon;//<
    TList *fListMixPPLikeDimuon;//<
    TList *fListMMLikeDimuon;//<
    TList *fListMixMMLikeDimuon;//<

    ClassDef(TaskJpsiPP, 2);
};

#endif
