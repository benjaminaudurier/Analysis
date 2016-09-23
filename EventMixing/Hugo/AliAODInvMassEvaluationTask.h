#ifndef AliAODInvMassEvaluationTask_h
#define AliAODInvMassEvaluationTask_h

#include <AliAnalysisTaskSE.h>
#include <AliAODDimuon.h>
#include <AliAODTrack.h>
#include <AliMuonTrackCuts.h>

#include <TList.h>
#include <TH2.h>
#include <TObjArray.h>

class AliAODInvMassEvaluationTask : public AliAnalysisTaskSE
{
  public:

  /// constructor
  AliAODInvMassEvaluationTask(const char *name = "AliAODInvMassEvaluationTask");

  /// destructor
  virtual ~AliAODInvMassEvaluationTask();

  virtual void LocalInit();
  virtual void NotifyRun();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(const Option_t*);

  virtual void SetTriggerSelection( const TString& selection )
  { fTriggerSelection = selection; }

  virtual void SetPhysicsSelectionMask( UInt_t mask )
  { fPhysicsSelectionMask = mask; }

  virtual void SetTriggerSelectionMixing( const TString& selection )
  { fTriggerSelectionMixing = selection; }

  virtual void SetPhysicsSelectionMaskMixing( UInt_t mask )
  { fPhysicsSelectionMaskMixing = mask; }

  virtual void SetMaxPoolSize( Int_t value )
  { fMaxPoolSize = value; }

  protected:

  /// event is accepted
  virtual Bool_t IsEventAccepted( TString ) const;
  virtual Bool_t IsEventAcceptedMixing( TString ) const;

  /// true if physics selected
  virtual Bool_t IsPhysicsSelected( UInt_t ) const;
  virtual Bool_t IsPhysicsSelectedMixing( UInt_t ) const;

  /// muon is accepted
  virtual Bool_t IsTrackAccepted( const AliAODTrack* );

  /// pair is accepted
  virtual Bool_t IsPairAccepted( const AliAODDimuon* );

  /// pair is accepted
  virtual Bool_t IsPairAccepted( const TLorentzVector& ) const;

  /// find pool matching given centrality
  TList* FindPool( Double_t ) const;

  private:

  /// copy constructor (not implemented)
  AliAODInvMassEvaluationTask( const AliAODInvMassEvaluationTask& );

  /// assignment operator (not implemented)
  AliAODInvMassEvaluationTask& operator=( const AliAODInvMassEvaluationTask& );

  /// event
  int fEvent;

  /// track cuts
  AliMuonTrackCuts fCuts;

  // max pool size
  Int_t fMaxPoolSize;

  /// trigger selection
  TString fTriggerSelection;
  TString fTriggerSelectionMixing;

  /// physics selection mask
  UInt_t fPhysicsSelectionMask;
  UInt_t fPhysicsSelectionMaskMixing;

  /// track selection mask
  UInt_t fTrackSelectionMask;

  /// output list
  TList* fListOutput;

  /// pools
  TList* fPools;

  ClassDef(AliAODInvMassEvaluationTask,1)

};

#endif
