#include <Riostream.h>
#include <iostream> 
#include <cmath> 

// ROOT includes
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1.h"
#include "THnSparse.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TAttMarker.h"
#include "TMarker.h"
#include "TLorentzVector.h"
#include "TList.h"
#include "TArray.h"
#include "TLegend.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TMatrix.h"




// STEER includes
#include "AliAODTrack.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliVEvent.h"
#include "AliMuonPairCuts.h" 
#include "AliMuonEventCuts.h" 


// ANALYSIS includes
#include "AliAnalysisTaskMass.h"


// example of an analysis task creating a Invariante Mass spectrum
// Author: Benjamin Audurier




ClassImp(AliAnalysisTaskMass)// for ROOT

//________________________________________________________________________
AliAnalysisTaskMass::AliAnalysisTaskMass(const char *name) 
  : AliAnalysisTaskSE(name), fAOD(0), fOutputList(0), fHist(0)
{
  // Constructor

  // Define input and output slots here
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #0 id reserved by the base class for AOD
  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class());
}


//________________________________________________________________________
AliAnalysisTaskMass::~AliAnalysisTaskMass()
{
    // Destructor
    delete fOutputList;
    delete fHist;
    
}


//________________________________________________________________________
void AliAnalysisTaskMass::UserCreateOutputObjects()
{
  // Create histograms
  // Called once
    
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
    
    // ____ Constants_____

    ndim=3; // number of dimension for fHistMass (3 here for M, pt and y)
    
    ymin=2.5; // minimum rapidity
    ymax=5; // max rapidity
    
    Mmin=2; // minimum mass
    Mmax=5; // max mass
    
    Ptmin=0; // minimum pt
    Ptmax=8; // max pt
    
    ybin=20; // binning for rapidity
    Mbin=60; // binning for rapidity
    Ptbin=160; // binning for rapidity
    
    
    Double_t min[3]={Mmin,ymin,Ptmin};// Get all the minimums
    
    Double_t max[3]={Mmax,ymax,Ptmax};// Get all the max
    
    Int_t bins[3]={Mbin,ybin,Ptbin};// Get all the bins

    
    //...............................

    
    fOutputList = new TList();
    fOutputList->SetOwner(kTRUE);
    fHist = new THnSparseF("fHist", "fHist",ndim,bins,min,max);
    fOutputList->Add(fHist);
    PostData(1, fOutputList);

}


//________________________________________________________________________
void AliAnalysisTaskMass::UserExec(Option_t *) 
{
  ///////////////////////////
  // Main loop
  // Called for each event
  ///////////////////////////
  

  ///////////////////////////
  // cast it to AOD and make sure it is actually AOD  
  ///////////////////////////


    // Casting the cut
    fMuonPairCuts = new AliMuonPairCuts("fMuonPairCuts","fMuonPairCuts");
    fMuonPairCuts->ApplySharpPtCutInMatching(kTRUE);
  


  AliVEvent *evt = InputEvent();
    
  fAOD = dynamic_cast<AliAODEvent*>(evt); // Convert AliVEvent=>AliAODEvent
  
  if (!fAOD) {
  printf("ERROR: fAOD not available\n");
  return;
  }
      
  // printf("There are %d tracks in this event\n", fAOD->GetNumberOfTracks());
  
    // Loop on each tracks
    for (Int_t iTracks = 0; iTracks < fAOD->GetNumberOfTracks(); iTracks++)
        {
        AliAODTrack *particle = static_cast<AliAODTrack *>(fAOD->GetTrack(iTracks));// Get the particle from iTracks

        if (! particle->IsMuonTrack()) continue; // Make sure it is a muon

        TLorentzVector vec(particle->Px(),particle->Py(),particle->Pz(),particle->E());// Create the particle 4-vector

        // Loop on track after the one selected
        for (int i = iTracks+1; i < fAOD->GetNumberOfTracks(); ++i)
            {
            // Get and create the second particle 4-Vector
            AliAODTrack *particle2 = static_cast<AliAODTrack *>(fAOD->GetTrack(i));

            // Make sure it is a muon
            if (!particle2->IsMuonTrack() ) continue;

            TLorentzVector vec2(particle2->Px(),particle2->Py(),particle2->Pz(),particle2->E());

            // Condition to fill the diagram : opposite sign
            if (! fMuonPairCuts->IsSelected(particle,particle2)) continue;
            
            // Add the two 4-vectors. In the 4rth component of vec2 is the Invariant Mass
            vec2+=vec;
            
            // Create a n-dimensional element for THnSparse
            Double_t x[] ={vec2.M(),abs(vec2.Rapidity()),vec2.Pt()};

            // Fill the diagram
            fHist->Fill(x);
        }

    }


  PostData(1, fOutputList);
  delete fMuonPairCuts;

}      

//________________________________________________________________________
void AliAnalysisTaskMass::Terminate(Option_t *) 
{
  // Draw result to the screen
  // Called once at the end of the query

    
  

//------------------------------------
// Create Outputs
//------------------------------------


    
  fOutputList = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutputList) {
    printf("ERROR: Output list not available\n");
    return;
  } // Checkpoint
  
  fHist = dynamic_cast<THnSparseF*> (fOutputList->At(0));
  if (!fHist) {
    printf("ERROR: fHistMass not available\n");
    return;
  } // Checkpoint


//    fMuonPairCuts->Print(); // To see the proint, run in local mode

    
}

///////////////////////////
// Muon cut mask
///////////////////////////

//  *** Muon pair filter mask: ***
//  0xc0000023
//  mu1 && mu2 pass eta cut
//  mu1 && mu2 pass theta_abs cut
//  mu1 && mu2 match Lpt
//  Unlike sign
//  -4 < y_{mumu} < -2.5
//  ********************
//  *** Muon track filter mask: ***
//  0xf
//  -4 < eta < -2.5
//  2 < theta_abs < 10 deg
//  pxDCA cut
//  match Apt && sharp pt from tracker
//  ********************
//  *** Muon track parameter summary: ***
//  Mean vertex DCA: (0, 0, 0)
//  Mean p correction (GeV/c): theta2-3 = 0  theta3-10 = 0
//  Sigma p x DCA (cm x GeV/c): theta2-3 = 0  theta3-10 = 0
//  Cut p x DCA in units of sigma: 0
//  Cut on track chi2/NDF: 0
//  Momentum resolution: 0
//  Slope resolution: 0
//  Sharp pt cut: 0 (Apt)  0 (Lpt)  0 (Hpt)
//  ********************************






//BROUILLONS
//    fHistMass->GetXaxis()->SetTitle("Dimuon invariant mass M_{#mu+,#mu-} (GeV/c)");
//    fHistMass->GetYaxis()->SetTitle("dN/dM");
//    fHistMass->SetMarkerStyle(kDot);


