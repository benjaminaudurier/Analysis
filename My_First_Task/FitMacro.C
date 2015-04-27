//
//  InvariantMassFitMacro.cpp
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//


#include "Riostream.h"
#include "TFile.h"
#include "TPad.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TH2F.h"
#include "TF2.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TMatrixDSym.h"
#include "TMatrixD.h"
#include "TMatrix.h"
#include "THnSparse.h"
#include "TCut.h"



// Call functions
extern Double_t CrystalBall(Double_t *, Double_t *);
extern Double_t Background(Double_t *, Double_t *);
extern Double_t gaus(Double_t *, Double_t *);
extern Double_t Sum1(Double_t *, Double_t *);
extern Double_t Sum2(Double_t *, Double_t *);


void FitMacro(void)
{
    
//------------------------------------
// Fit functions
//------------------------------------
    
    
    
    // ____ CrystalBall_____
    TF1 *cb=(TF1 *)gROOT->FindObject("CrystalBall");
    if(!cb)
        {
        cb = new TF1("CrystalBall",CrystalBall,1.5,5,7);
        cb->SetParameters(4.20313e-01, -7.70542e+05, 4, 4,3.07831,6.07556e-02,4.06774e+02 );
        cb->SetParNames("#alpha_{cb}","#alpha'_{cb}","n","n'","#mu_{cb}","#sigma_{cb}","N_{cb}");
        cb->SetLineColor(46);
        cb->SetLineWidth(1);
        }
    else{
        printf("ERROR: Cannot Load Crystall Ball\n");
        return;
        }
    //...............................

    
    // ____ background_____
    TF1 *bkgrd=(TF1 *)gROOT->FindObject("Background");
    if(!bkgrd)
        {
        bkgrd = new TF1("Background",Background,0,10,4);
        bkgrd->SetParameters(2.45209e+06, 6.25996e+01, -8.08483e+01, -1.00861e+02 );
        bkgrd->SetParNames("N_{background}","alpha","#beta","#gamma");
        bkgrd->SetLineColor(36);
        bkgrd->SetLineStyle(5);
        }
    else{
        printf("ERROR: Cannot Load Background\n");
        return;
        }
    //...............................
    

    
    
    // ____ sum1_____
    TF1 *sm1=(TF1 *)gROOT->FindObject("Sum1");
    if(!sm1)
        {
        sm1 = new TF1("Sum1",Sum1,1.5,5,11);
        sm1->SetParameters(3000,1.7,0.6,0.3,4.203e-01, -7.706e+05, 4.3, 4.3,3.079,6.076e-02,4.068e+02 );
        sm1->SetParNames("N_{background}","#alpha_{background}","#beta_{background}","#gamma_{background}","#alpha_{cb}","#alpha'_{cb}","n","n'","#mu_{cb}","#sigma_{cb}","N_{cb}");
        sm1->FixParameter(4,0.96);
        sm1->FixParameter(5,2.26);
        sm1->FixParameter(6,6.40);
        sm1->FixParameter(7,2.60);
        sm1->SetLineStyle(2);
        sm1->SetLineColor(kRed);
        }
    else{
        printf("ERROR: Cannot Load Sum1\n");
        return;
        }
    //...............................
    

    
    //------------------------------------
    // Get Histogram
    //------------------------------------
    
    
    // Ouverture du fichier "FitMaxwell.root"
    TFile *f=(TFile *)gROOT->FindObject("Mass.AOD.2.root");
    if(!f){f=new TFile("Mass.AOD.2.root");}
    else{
        printf("ERROR: Cannot Load Tree\n");
        return;
        }
    
    // On recupere le pointeur sur la liste
    TList *l1=(TList *)f->Get("chistmass");
    if(!l1)
        {
        printf("ERROR: Cannot Load List\n");
        return;
        }
    
    // On recupere le pointeur sur l'histogramme
    THnSparse *fHist=(THnSparse *)l1->FindObject("fHist");
    if(!fHist)
        {
        printf("ERROR: Cannot Load Histogram\n");
        return;
        }
    
    
    
    
    //------------------------------------
    //  Draw Canvas
    //------------------------------------
    
    
    
    //    ____ Methods description _____
    
    //    we draw final results. After fitting datas with a sum (background + signal) function, we extract the fitting parameters and set them to the corresponding functions. We stock also the fitting parameters errors corresponding to the signal part of the sum function. With thoses errors and parameters, we integrate and calculate the number of J/Psi plus the relative errors.
    TCanvas *c = new TCanvas("AliAnalysisTaskMass","Invariant Mass for Several Rapidity",900,500); // Create Canvas
    gStyle->SetOptFit(kTRUE);
    gStyle->SetOptStat(kTRUE);
    
    c->Divide(2,1); // Divide canvas
    //...............................
    
    
    
    
    //____ The first subcanvas  _____
    c->cd(1); // Windows 1
    
    TH1D *fHistInvariantMass =  fHist->Projection(0,"E");// Projection to get the invariant mass
    fHistInvariantMass->SetTitle("Invariant Mass spectrum");
    fHistInvariantMass->GetXaxis()->SetTitle("M_{#mu+#mu-}");
    fHistInvariantMass->GetYaxis()->SetTitle("dN/dM");

    TFitResultPtr fitResult = fHistInvariantMass->Fit(sm1,"SIREM");// Fit and create TObject to get Par. Error
    TMatrixDSym cov1 = fitResult->GetCovarianceMatrix();// get the covariante matrice
    TMatrixDSym SubCov1 = cov1.GetSub(4,10,4,10);// Get a submatrix with the parameters of interest

    Double_t param1[sm1->GetNpar()];   // Array for the 11 sum function parameters
    sm1->GetParameters(param1);    // Fill array with parameters
    bkgrd->SetParameters(param1[0],param1[1],param1[2],param1[3]); // Set parameters for background function
    cb->SetParameters(&param1[4]); // Set parameters for gaussian function
    
    fHistInvariantMass->Draw("E0");// Draw with error bars
    cb->DrawCopy("same"); // Draw gaus
    //...............................
    
    
    
    //____ The second subcanvas  _____
    c->cd(2); // Windows 2
    
    fHist->GetAxis(2)->SetRange(1,4);// Define Pt Range (problem here)
    
    TH1D *fHistInvariantMass2 =  fHist->Projection(0,"E");// Projection to get the invariant mass
    
    fHistInvariantMass2->SetTitle("Invariant Mass spectrum for 3<Pt<4");
    fHistInvariantMass2->GetXaxis()->SetTitle("M_{#mu+#mu-}");
    fHistInvariantMass2->GetYaxis()->SetTitle("dN/dM");
    
    TFitResultPtr fitResult2 = fHistInvariantMass2->Fit(sm1,"SIREM");// Fit and create TObject to get Par. Error
    TMatrixDSym cov2 = fitResult2->GetCovarianceMatrix();// get the covariante matrice
    TMatrixDSym SubCov2 = cov2.GetSub(4,10,4,10);// Get a submatrix with the parameters of interest
    
    Double_t param2[sm1->GetNpar()];   // Array for the 11 sum function parameters
    sm1->GetParameters(param2);    // Fill array with parameters
    bkgrd->SetParameters(param2[0],param2[1],param2[2],param2[3]); // Set parameters for background function
    cb->SetParameters(&param2[4]); // Set parameters for gaussian function
    
    fHistInvariantMass2->Draw("E0");// Draw with error bars
    cb->DrawCopy("same"); // Draw gaus
    //...............................
    
    
    
    //____ JPsi calculation and Error Estimation _____
    Double_t JpsiNumber = cb->Integral(2.7,3.4)/fHistInvariantMass->GetBinWidth(1) ;
    Double_t JpsiError = cb->IntegralError(2.7,3.4,&param1[4],SubCov1.GetMatrixArray())/fHistInvariantMass->GetBinWidth(1);
    //...............................

     c->Print("Invariant_Mass_Spectrum.eps"); // Save canvas
    
    
    
    
    
    
    
    
    
    //____ Delete pointer _____
    delete cb;
    delete bkgrd;
    delete sm1;
    delete c;
    //...............................
  
    
    
    
    return;
    
    
    
    
    
    
    
    
    
    
    
    
    
//    Brouillon
//    
//    gPad->SetPad(0.5,0,1,0.7);
//    
//    TH1D *fHistInvariantMass2 =  fHistMass->Projection(0,"E");// Projection to get the invariant mass
//    
//    TF1 *bkgrd2 =  (TF1*)bkgrd->Clone("BackGround2");// Copy the background function
//    
//    TFitResultPtr fitResult2 = fHistInvariantMass2->Fit(sm2,"SIREM");// Fit and create TObject to get Par. Error
//    
//    TMatrixDSym cov2= fitResult2->GetCovarianceMatrix();// get the covariante matrice
//    
//    //    cov2.Print();// checkpoint ok
//    //
//    //    fitResult2->Print("V");// checkpoint
//    
//    Double_t param2[sm2->GetNpar()];   // Array for the 7 sum function parameters
//    
//    sm2->GetParameters(param2);    // Fill array with parameters
//    
//    bkgrd2->SetParameters(param2[0],param2[1],param2[2],param2[3]); // Set parameters for background function
//    
//    gs->SetParameters(&param2[4]); // Set parameters for gaussian function
//    
//    fHistInvariantMass2->Draw("E0");// draw with error bars
//    
//    gs->Draw("same"); // Draw gaus
    
    
//    // ____ sum2_____
//    
//    TF1 *sm2=(TF1 *)gROOT->FindObject("Sum2");
//    if(!sm2)
//        {
//        sm2 = new TF1("Sum2",Sum2,1.5,5,7);
//        sm2->SetParameters(3000,1.7,0.6,0.3, 4.07e+02,3.08, 6.08e-02);
//        sm2->SetParNames("N_{background}","#alpha_{background}","#beta_{background}","#gamma_{background}","N_{gaus}","#mu_{gaus}","#sigma_{gaus}");
//        sm2->SetLineStyle(2);
//        sm2->SetLineColor(kRed);
//        }
//    else{
//        printf("ERROR: Cannot Load Sum2\n");
//        return;
//    }
//    
//    //...............................
//    
//    Double_t JpsiNumber2 = gs->Integral(2.7,3.4)/fHistInvariantMass2->GetBinWidth(1) ;
//    Double_t JpsiError2 = gs->IntegralError(2.7,3.4,&param2[4],&cov2(4,4))/fHistInvariantMass2->GetBinWidth(1);
    
    
//    //____ Set Legend _____
//    
//    TLegend *legend=new TLegend(0.5,0.75,1,0.9,"Legend : ");
//    legend->AddEntry(sm1,"Signal");
//    legend->AddEntry(bkgrd,"Background signal");
//    
//    legend->AddEntry(cb,Form("J/#psi signal with CB : %.0f +/- %.0f J/#psi",JpsiNumber,JpsiError));
//    
//    legend->DrawClone("same");
//    
//    //...............................
    
}


