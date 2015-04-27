#ifndef AliAnalysisTaskMass_cxx
#define AliAnalysisTaskMass_cxx


class THnSparse;
class AliAODEvent;
class AliMuonPairCuts;





#include "AliAnalysisTaskSE.h"


class AliAnalysisTaskMass : public AliAnalysisTaskSE {
 public:
    AliAnalysisTaskMass(const char *name="");
    virtual ~AliAnalysisTaskMass();
  
    virtual void   UserCreateOutputObjects();
    virtual void   UserExec(Option_t *option);
    virtual void   Terminate(Option_t *);
  
 private:
    AliAODEvent *fAOD;    //! AOS object
    TList       *fOutputList; //! Output list
    THnSparseF        *fHist; //! Mass spectrum
    AliMuonPairCuts* fMuonPairCuts; //! Cut for dimuons
    
    Int_t ndim; // number of dimension for fHistMass (3 here for M, pt and y)
    
    double ymin; // minimum rapidity
    double ymax; // max rapidity
    
    double Mmin; // minimum mass
    double Mmax; // max mass
    
    double Ptmin; // minimum pt
    double Ptmax; // max pt
    
    int ybin; // binning for rapidity
    int Mbin; // binning for rapidity
    int Ptbin; // binning for rapidity
    
    


   
  AliAnalysisTaskMass(const AliAnalysisTaskMass&); // not implemented
  AliAnalysisTaskMass& operator=(const AliAnalysisTaskMass&); // not implemented
  
  ClassDef(AliAnalysisTaskMass, 1); // example of analysis
};

#endif