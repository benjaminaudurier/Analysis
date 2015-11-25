#ifndef AliAnalysisTaskMass_cxx
#define AliAnalysisTaskMass_cxx

// Class used
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
    AliAODEvent    *fAOD;    //! AOS object
    TList          *fOutputList; //! Output list
    THnSparseF     *fHist; //! Mass spectrum
    AliMuonPairCuts* fMuonPairCuts; //! Cut for dimuons
    

    
    
   
  AliAnalysisTaskMass(const AliAnalysisTaskMass&); // not implemented
  AliAnalysisTaskMass& operator=(const AliAnalysisTaskMass&); // not implemented
  
  ClassDef(AliAnalysisTaskMass, 1); // example of analysis
};

#endif