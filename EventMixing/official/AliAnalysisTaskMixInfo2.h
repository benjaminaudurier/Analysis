#ifndef ALIANALYSISTASKMIXINFO_H
#define ALIANALYSISTASKMIXINFO_H

//
// Class AliAnalysisTaskMixInfo
//
// AliAnalysisTaskMixInfo is task
// for mixing info
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#include "AliLog.h"
#include "AliMixInputEventHandler2.h"
#include "AliAnalysisTaskSE.h"

class AliMixInputEventHandler2;
class TList;
class AliMixInfo2;
class AliAnalysisTaskMixInfo2 : public AliAnalysisTaskSE {
public:
   AliAnalysisTaskMixInfo2(const char *name = "<default name>");
   virtual ~AliAnalysisTaskMixInfo2();

   virtual void    UserCreateOutputObjects();
   virtual void    UserExec(Option_t *option);
   virtual void    Terminate(Option_t *);
   virtual void    UserExecMix(Option_t *option = "");
   virtual void    FinishTaskOutput();

   void            InitInputHandlers();
   void            InitMixInfo();
   // sets log type to list of classes
   void            SetLogType(AliLog::EType_t type, TString allClasses = "");
   // sets correctly debug level to AliLog for all classes listed in fLogClassesString
   void            SetDebugForAllClasses();

   void            PrintEventInfo();

private:

   AliMultiInputEventHandler  *fInputEHMain;       //! input multi handler
   AliMixInputEventHandler2    *fInputEHMix;        //! mix input handler

   TList                      *fOutputList;        //! output list
   AliMixInfo2                 *fMixInfo;           //! mix info

   Long64_t                    fCurrentEntryTmp;   //! temporary current entry number

   AliLog::EType_t             fLogType;           // log type
   TString                     fLogClassesString;  // all classes string divided with ":"

   AliAnalysisTaskMixInfo2(const AliAnalysisTaskMixInfo2 &); // not implemented
   AliAnalysisTaskMixInfo2 &operator=(const AliAnalysisTaskMixInfo2 &); // not implemented

   ClassDef(AliAnalysisTaskMixInfo2, 1); // example of analysis
};

#endif
