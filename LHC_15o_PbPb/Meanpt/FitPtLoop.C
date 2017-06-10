#include "Riostream.h"
#include "TH1.h"
#include <vector>
#include "TMath.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include <TROOT.h>
#include <TSystem.h>
#include "TGraph.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TMatrixD.h"

//
// Macro to get the <pt> from a pt distribution
//

//Input file should have the format :
//
//0- 1     117.697  2.759   9.044
//1- 2     242.073  3.866  18.778
//2- 3     207.527  3.313  15.971
//3- 4     138.534  2.675  10.836
//4- 5      81.585  1.919   6.353
//5- 6      44.827  1.130   3.484
//6- 8      17.842  0.442   1.453
//8-15       2.419  0.071   0.187



/*
TH1* FitPt(const char* input="dsigmadydpt_PbPb020.txt", Bool_t norm = kFALSE, Int_t errorType = 2, Double_t delta = 0.1)
{
  // norm = kTRUE if yields have been divided by pT
  // errorType: stat = 0, sys = 1, all = other int
  // delta: range for <pT> scan to <pT> ± delta*<pT>

.x FitPt.C("dsigmadydpt_AuAu200_1.2y2.2_0cent20.txt", kTRUE, 2 ,0.1)


*/



// range to integrate the function
Double_t xmin          = 0.;
Double_t xmax          = 8.;
Double_t cutefficiency = 0.8;


void FitPt(const char* input="dsigmadydpt_pp.txt" , const char* inputbis ="NJpsi_05to1.txt", Bool_t norm = kFALSE, Int_t errorType = 0, Double_t delta = 0.1);
Double_t MeanPtVsp0(Double_t *x, Double_t *p);
Double_t MeanPtVsn(Double_t *x, Double_t *p);
Double_t MeanPt2Vsp0(Double_t *x, Double_t *p);
Double_t MeanPt2Vsn(Double_t *x, Double_t *p);



//------------------------------------------------------------------
void FitPtLoop(const char* directory="YieldPbPb_2015" , const char* inputbis = "NJpsi_05to1.txt", Bool_t norm = kFALSE, Int_t errorType = 0, Double_t delta = 0.1)
{
  // norm = kTRUE if yields have been divided by pT
  // errorType: stat = 0, sys = 1, all = other int
  // delta    : range for <pT> scan to <pT> ± delta*<pT>


    //Check directories
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(directory))) {
        printf("Error : Dir '%s' doesn't exists !!!",directory);
        return;
    }

    // Get what's inside
    TString fileStr  = gSystem->GetFromPipe(Form("ls %s",directory));
    TObjArray* file  = fileStr.Tokenize("\n");

    // Double where results are stored

    TIter nextfile(file);
    TObjString* sfile;
    nextfile.Reset();
    while ( ( sfile = static_cast<TObjString*>(nextfile()) ) )
    {
      printf("\n --- file : %s --- \n",sfile->String().Data() );
      FitPt(Form("%s/%s",directory,sfile->String().Data()),inputbis,norm,errorType,delta);
    }


  return;
}

//------------------------------------------------------------------
void FitPt(const char* input ,const char* inputbis , Bool_t norm , Int_t errorType , Double_t delta )
{
  // norm = kTRUE if yields have been divided by pT
  // errorType: stat = 0, sys = 1, all = other int
  // delta: range for <pT> scan to <pT> ± delta*<pT>


  // Get the yield form first file
  ifstream in(input);
  char line[1024];

  float a,b;
  float value,stat,sys;
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> yerr;

  while (in.getline(line,1023,'\n'))
  {

    if (strstr(line, "//")) continue;

    sscanf(line,"%e-%e %e %e %e",&a,&b,&value,&stat,&sys);

    if (norm)
    {
      Double_t s = 0.5*(a+b);
      value      *= s;
      stat       *= s;
      sys        *= s;
    }

    if (errorType == 1) stat=0;
    else if (errorType == 0) sys=0;

    // printf("%e-%e %e %e %e\n",a,b,value,stat,sys);

    x.push_back(a);
    y.push_back(value);

    Double_t error = TMath::Sqrt(stat*stat/value/value+sys*sys/value/value)*value;
    yerr.push_back(error);

    // printf("%e-%e %g +- %g\n",a,b,value,error);

  }

  gStyle->SetOptFit(1);
  x.push_back(b);


   TH1* h = new TH1F("pt","pt",x.size()-1,&x[0]);

  for ( Int_t i = 1; i <= h->GetXaxis()->GetNbins(); ++i )
  {
    h->SetBinContent(i,y[i-1]);
    h->SetBinError(i,yerr[i-1]);
  }

  TF1* f = new TF1("f","[0]*x/((1+(x/[1])**2)**[2])",0,12);

  f->SetParNames("C","p0","n");

  f->SetParameter(0,1);
  //  f->FixParameter(0,2.49424e+02);
  f->SetParameter(1,10);
  //  f->FixParameter(1,4.32592e+00);
  f->SetParameter(2,10);

  // TFitResultPtr r = h->Fit(f,"0QIRS","",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  TFitResultPtr r = h->Fit(f,"0QISE","",0.5,12);


  float centlow,centhigh;
  float value_05_1,value_0_1;
  std::vector<double> centlowvector;
  std::vector<double> centhighvector;
  std::vector<double> corrfactor;

  ifstream inbis(inputbis);
  char linebis[1024];

  while (inbis.getline(linebis,1023,'\n'))
  {

    if (strstr(linebis, "//")) continue;

    sscanf(linebis,"%e-%e %e %e",&centlow,&centhigh,&value_05_1,&value_0_1);
    // printf("%e-%e %e %e\n",centlow,centhigh,value_05_1,value_0_1);

    // double NJpsi_h =  value_05_1 - ((1-cutefficiency)/cutefficiency)*(value_0_1-value_05_1)*(1-f->Integral(0,0.5)/h->GetBinContent(1)/0.5);
    double NJpsi_h =  value_05_1 - ((1-cutefficiency)/cutefficiency)*(value_0_1-value_05_1);
    printf("NJpsi_h = %f\n",NJpsi_h );
    printf("f->Integral(0,0.5)/0.5 = %f\n",f->Integral(0,0.5)/0.5 );
    printf("h->GetBinContent(1) = %f\n",h->GetBinContent(1) );
    double factor  = NJpsi_h/value_05_1;
    printf("factor = %f\n",factor );

    centlowvector.push_back(centlow);
    centhighvector.push_back(centhigh);
    corrfactor.push_back(factor);

  }


  TH1* h_effcut = new TH1F("pt_effcut","pt_effcut",x.size()-1,&x[0]);

  for ( Int_t i = 1; i <= h_effcut->GetXaxis()->GetNbins(); ++i )
  {
    if(x[i-1] == 0.5 && x[i] == 1 )
    {
      // name of the file should contain and match the cent intervalle
      TString sinput(input);
      // printf("%s\n",sinput.Data() );
      for (int j = 0; j < static_cast<int>(corrfactor.size()); ++j)
      {
        // printf("%.0f-%.0f\n",centlowvector[j],centhighvector[j]);
        if( sinput.Contains(Form("%.0fto%.0f",centlowvector[j],centhighvector[j])) )
        {
          // printf("corrfactor = %f\n",corrfactor[j]);
          h_effcut->SetBinContent(i,corrfactor[j]*y[i-1]);
          h_effcut->SetBinError(i,corrfactor[j]*yerr[i-1]);
        }
      }
    } else {
      h_effcut->SetBinContent(i,y[i-1]);
      h_effcut->SetBinError(i,yerr[i-1]);
    }
  }


  TCanvas* c1 = new TCanvas(input,input);
  c1->Divide(1,2);
  c1->cd(1);

  gPad->SetLogy();

  h->Draw("histe");


  //  TVirtualFitter::SetDefaultFitter("Minuit");

  //	C*pT/(1+(pT/p0)^2)n
  //   TF1* f = new TF1("f","[0]*x/((1+(x/[1])**2)**[2])",h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
  //   f->SetParNames("C","p0","n");


  f->Draw("same");

  c1->cd(2);

  gPad->SetLogy();

  h_effcut->Draw("histe");

  TF1* f_effcut = new TF1("f_effcut","[0]*x/((1+(x/[1])**2)**[2])",0,12);

  f_effcut->SetParNames("C","p0","n");

  f_effcut->SetParameter(0,1);
  //  f_effcut->FixParameter(0,2.49424e+02);
  f_effcut->SetParameter(1,10);
  //  f_effcut->FixParameter(1,4.32592e+00);
  f_effcut->SetParameter(2,10);




  TFitResultPtr r_effcut = h_effcut->Fit(f_effcut,"0QISE","",0.5,12);
  f_effcut->Draw("same");

  // r->Print("V");

  // std::cout << std::endl;
  // for ( Int_t i = 1; i <= h->GetXaxis()->GetNbins(); ++i )
  // {
  //   Double_t x1 = h->GetBinLowEdge(i);
  //   Double_t x2 = x1+h->GetBinWidth(i);

    // std::cout << Form("Pt %3.1f - %3.1f Mean %4.2f",x1,x2,f->Mean(x1,x2)) << std::endl;
  // }

  std::cout << std::endl;
  Double_t meanptval = f->Mean(xmin,xmax);
  std::cout << Form("<pT>                               = %e (%f,%f)",meanptval,xmin,xmax) << std::endl;
  Double_t meanpt2val = f->Moment(2,xmin,xmax);
  std::cout << Form("<pT2>                              = %e (%f,%f)",meanpt2val,xmin,xmax) << std::endl;
  std::cout << std::endl;
  Double_t meanptval_effcut = f_effcut->Mean(xmin,xmax);
  std::cout << Form("<pT> with efficiency correction    = %e (%f,%f)",meanptval_effcut,xmin,xmax) << std::endl;
  Double_t meanpt2val_effcut = f_effcut->Moment(2,xmin,xmax);
  std::cout << Form("<pT2> with efficiency correction   = %e (%f,%f)",meanpt2val_effcut,xmin,xmax) << std::endl;
  std::cout << std::endl;
  std::cout << Form("<pT> variation                     = %f",(meanptval-meanptval_effcut)/meanptval*100) << std::endl;
  std::cout << Form("<pT2> variation                    = %f",(meanpt2val-meanpt2val_effcut)/meanpt2val*100) << std::endl;
  std::cout << std::endl;



  return ;

  //  std::cout << Form("Mean Pt %3.2f (0-8)",f->Mean(0,8)) << std::endl;
  //  std::cout << Form("Mean Pt %3.2f (0-20)",f->Mean(0,20)) << std::endl;

  // // --- mean pT from fit function and mean pT error from error propagation ---
  // TMatrixD cov(r->GetCovarianceMatrix());

  // TMatrixD jac(1,3);
  // jac(0,0) = 0.;
  // TF1* fMeanpTVspO = new TF1("fMeanpTVspO",MeanPtVsp0,
		// 	     f->GetParameter(1)-f->GetParError(1),f->GetParameter(1)+f->GetParError(1),1);
  // fMeanpTVspO->SetParameter(0,f->GetParameter(2));
  // jac(0,1) = fMeanpTVspO->Derivative(f->GetParameter(1),0x0,0.005);
  // std::cout << Form("d<pT>/dp0 = %e ± %e", jac(0,1), fMeanpTVspO->DerivativeError()) << std::endl;
  // TF1* fMeanpTVsn = new TF1("fMeanpTVsn",MeanPtVsn,
		// 	    f->GetParameter(2)-f->GetParError(2),f->GetParameter(2)+f->GetParError(2),1);
  // fMeanpTVsn->SetParameter(0,f->GetParameter(1));
  // jac(0,2) = fMeanpTVsn->Derivative(f->GetParameter(2),0x0,0.005);
  // std::cout << Form("d<pT>/dn = %e ± %e", jac(0,2), fMeanpTVsn->DerivativeError()) << std::endl;

  // TMatrixD tmp(jac,TMatrixD::kMult,cov);
  // TMatrixD meanpTErr2(tmp,TMatrixD::kMultTranspose,jac);

  // // --- mean pT**2 from fit function and mean pT**2 error from error propagation ---
  // TMatrixD jac2(1,3);
  // jac2(0,0) = 0.;
  // TF1* fMeanpT2VspO = new TF1("fMeanpT2VspO",MeanPt2Vsp0,
		// 	      f->GetParameter(1)-f->GetParError(1),f->GetParameter(1)+f->GetParError(1),1);
  // fMeanpT2VspO->SetParameter(0,f->GetParameter(2));
  // jac2(0,1) = fMeanpT2VspO->Derivative(f->GetParameter(1),0x0,0.005);
  // std::cout << Form("d<pT2>/dp0 = %e ± %e", jac2(0,1), fMeanpT2VspO->DerivativeError()) << std::endl;
  // TF1* fMeanpT2Vsn = new TF1("fMeanpT2Vsn",MeanPt2Vsn,
		// 	     f->GetParameter(2)-f->GetParError(2),f->GetParameter(2)+f->GetParError(2),1);
  // fMeanpT2Vsn->SetParameter(0,f->GetParameter(1));
  // jac2(0,2) = fMeanpT2Vsn->Derivative(f->GetParameter(2),0x0,0.005);
  // std::cout << Form("d<pT2>/dn = %e ± %e", jac2(0,2), fMeanpT2Vsn->DerivativeError()) << std::endl;

  // TMatrixD tmp2(jac2,TMatrixD::kMult,cov);
  // TMatrixD meanpT2Err2(tmp2,TMatrixD::kMultTranspose,jac2);

  // // --- mean pT, mean pT**2 and related errors from contour plot ---

  // // cout << "done " << endl; return h;
  // c1->cd(2);

  // //   gMinuit->SetErrorDef(4); // 2 sigmas (2^2)
  // //   TGraph* g12at2sigmas = static_cast<TGraph*>(gMinuit->Contour(100,1,2));
  // //   g12at2sigmas->SetFillColor(4);
  // //   g12at2sigmas->Draw("alf");

  // gMinuit->SetErrorDef(1); // 1 sigmas (1^2)
  // TGraph* g12 = static_cast<TGraph*>(gMinuit->Contour(1000,1,2));
  // Double_t meanPtmin2 = -1.e9;
  // Double_t meanPtmax2 = 1.e9;
  // Double_t meanPt2min2 = -1.e9;
  // Double_t meanPt2max2 = 1.e9;
  // if (g12) {

  //   g12->SetLineColor(2);
  //   g12->Draw("al");
  //   Int_t nPoints = g12->GetN();

  //   TH1* hmeanpterror = new TH1F("hmeanpterror","h mean pt error",nPoints/5,meanptval*(1.-delta),meanptval*(1.+delta));
  //   hmeanpterror->SetLineColor(1);
  //   TH1* hmeanpt2error = new TH1F("hmeanpt2error","h mean pt**2 error",nPoints/5,meanpt2val*(1.-2.*delta),meanpt2val*(1.+2.*delta));
  //   hmeanpt2error->SetLineColor(1);

  //   Double_t meanPtmin = 1.e9;
  //   Double_t meanPtmax = -1.e9;
  //   Double_t meanPt2min = 1.e9;
  //   Double_t meanPt2max = -1.e9;
  //   for ( Int_t i = 1; i <= nPoints; ++i )
  //   {
  //     Double_t p0,n;

  //     g12->GetPoint(i,p0,n);
  //     f->FixParameter(1,p0);
  //     f->FixParameter(2,n);
  //     //h->Fit(f);

  //     Double_t meanpt = f->Mean(xmin,xmax);
  //     if (meanpt < meanPtmin) meanPtmin = meanpt;
  //     if (meanpt > meanPtmax) meanPtmax = meanpt;
  //     hmeanpterror->Fill(meanpt);

  //     Double_t meanpt2 = f->Moment(2,xmin,xmax);
  //     if (meanpt2 < meanPt2min) meanPt2min = meanpt2;
  //     if (meanpt2 > meanPt2max) meanPt2max = meanpt2;
  //     hmeanpt2error->Fill(meanpt2);
  //   }
  //   // cout << "done " << endl; return h;

  //   c1->cd(3);
  //   hmeanpterror->Draw("histe");
  //   c1->cd(4);
  //   hmeanpt2error->Draw("histe");

  //   /*
  //    std::cout << Form("Mean Pt %5.6f +- %5.6f   (with contour (RMS))",
  //    meanptval,hmeanpterror->GetRMS()) << std::endl;

  //    std::cout << Form("Mean Pt %5.6f +- %5.6f   (with contour (extreme values))",
  //    meanptval,0.5*(meanPtmax-meanPtmin)) << std::endl;
  //    */

  //   for ( Int_t i = 1; i <= hmeanpterror->GetNbinsX(); ++i ) {
  //     if (hmeanpterror->GetBinContent(i) > 20.) {
  //       meanPtmin2 = hmeanpterror->GetBinCenter(i);
  //       break;
  //     }
  //   }

  //   for ( Int_t i = hmeanpterror->GetNbinsX(); i >= 1; --i ) {
  //     if (hmeanpterror->GetBinContent(i) > 20.) {
  //       meanPtmax2 = hmeanpterror->GetBinCenter(i);
  //       break;
  //     }
  //   }

  //   for ( Int_t i = 1; i <= hmeanpt2error->GetNbinsX(); ++i ) {
  //     if (hmeanpt2error->GetBinContent(i) > 20.) {
  //       meanPt2min2 = hmeanpt2error->GetBinCenter(i);
  //       break;
  //     }
  //   }

  //   for ( Int_t i = hmeanpt2error->GetNbinsX(); i >= 1; --i ) {
  //     if (hmeanpt2error->GetBinContent(i) > 20.) {
  //       meanPt2max2 = hmeanpt2error->GetBinCenter(i);
  //       break;
  //     }
  //   }

  // }

  // std::cout << std::endl;
  // std::cout << Form("<pT> = %5.6f +- %5.6f   (with error propagation)",
		//     meanptval,TMath::Sqrt(meanpTErr2(0,0))) << std::endl;

  // if (g12) std::cout << Form("<pT> = %5.6f +- %5.6f   (with contour (extreme peaks))",
		//     meanptval,0.5*(meanPtmax2-meanPtmin2)) << std::endl;

  // std::cout << Form("<pT2> = %5.6f +- %5.6f   (with error propagation)",
		//     meanpt2val,TMath::Sqrt(meanpT2Err2(0,0))) << std::endl;

  // if (g12) std::cout << Form("<pT2> = %5.6f +- %5.6f   (with contour (extreme peaks))",
		//     meanpt2val,0.5*(meanPt2max2-meanPt2min2)) << std::endl;
  // std::cout << std::endl;

  // return;
}


//------------------------------------------------------------------
Double_t MeanPtVsp0(Double_t *x, Double_t *p)
{
  /// <pT> with x[0] = p0 and p[0] = n

  static TF1 *fp0 = new TF1("fp0","x/((1+(x/[0])**2)**[1])",xmin,xmax);

  fp0->SetParameters(x[0], p[0]);

  return fp0->Mean(xmin,xmax);
}

//------------------------------------------------------------------
Double_t MeanPtVsn(Double_t *x, Double_t *p)
{
  /// <pT> with x[0] = n and p[0] = p0

  static TF1 *fn = new TF1("fn","x/((1+(x/[0])**2)**[1])",xmin,xmax);

  fn->SetParameters(p[0], x[0]);

  return fn->Mean(xmin,xmax);
}

//------------------------------------------------------------------
Double_t MeanPt2Vsp0(Double_t *x, Double_t *p)
{
  /// <pT2> with x[0] = p0 and p[0] = n

  static TF1 *fp02 = new TF1("fp02","x/((1+(x/[0])**2)**[1])",xmin,xmax);

  fp02->SetParameters(x[0], p[0]);

  return fp02->Moment(2,xmin,xmax);
}

//------------------------------------------------------------------
Double_t MeanPt2Vsn(Double_t *x, Double_t *p)
{
  /// <pT2> with x[0] = n and p[0] = p0

  static TF1 *fn2 = new TF1("fn2","x/((1+(x/[0])**2)**[1])",xmin,xmax);

  fn2->SetParameters(p[0], x[0]);

  return fn2->Moment(2,xmin,xmax);
}