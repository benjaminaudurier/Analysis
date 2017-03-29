

#include <TROOT.h>
#include <TH2.h>
#include <TGraphErrors.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TMath.h>
#include <TLegendEntry.h>
#include <TStyle.h>
#include <TF1.h>
#include <TBox.h>


void LoadStyle();
Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey);


//______________________________________________
void ComparisonPbPbYield2DBIN(){

  LoadStyle();

  //------------------------------------------------------
  // PbPb values @ 5TeV
  //------------------------------------------------------

  const int npt=8;

  Double_t pt[npt+1]           ={0.,1.,2.,3.,4.,5.,6.,8.0,12.0};
  Double_t epsilonpt[npt]     ={0.5+0.2,1.5+0.2,2.5+0.2,3.5+0.2,4.5+0.2,5.5+0.2,7.0+0.2,10.0+0.2};
  Double_t ept[npt]           ={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  Double_t ept_syst[npt]      ={0.3,0.3,0.3,0.3,0.3,0.3,0.6,0.9};

  Double_t esyst_Trigger2DBIN1[npt] ={5.2,4.1,2.8,1.8,1.2,0.8,0.5,0.3};
  Double_t esyst_Trigger2DBIN2[npt] ={4.5,3.7,2.7,1.7,1.1,0.7,0.5,0.3};
  const Double_t Fnorm        = 11.84;
  const Double_t BR           = 5.96/100;
  const Double_t Mul2015      = 126778700;


  // ====================
  // -- 2.5 < y < 3.25 --
  // ====================

  Double_t NofJpsi2DBIN1PbPb[npt]       ={ 19303,31133,21798,12895,6773,3755,2756,1336};
  Double_t estat_NofJpsi2DBIN1PbPb[npt] ={ 991,1030,679,417,297,181,129,76};
  Double_t esyst_NofJpsi2DBIN1PbPb[npt] ={486,674,525,397,180,70,47,11};

  Double_t Accxeff2DBIN1PbPb[npt]       ={0.1213,0.1059,0.1108,0.1364,0.1750,0.2253,0.2833,0.3605};
  Double_t estat_Accxeff2DBIN1PbPb[npt] ={0.0007,0.0005,0.0006,0.0009,0.0016,0.0026,0.0036,0.0069};
  Double_t esyst_Accxeff2DBIN1PbPb[npt] ={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};

  for (int i = 0; i < npt; ++i){
  esyst_Accxeff2DBIN1PbPb[i] = TMath::Sqrt( 3*3     +     3*3 + 1      + 1.5*1.5 +  1        +   esyst_Trigger2DBIN1[i]*esyst_Trigger2DBIN1[i] ); //%
  //                                        MCinput       tracking       ntrinsic trigg          trigger response
  }

  Double_t Yiedl2DBIN1PbPb[npt];
  Double_t estat_Yiedl2DBIN1PbPb[npt];
  Double_t essyt_Yiedl2DBIN1PbPb[npt];
  Double_t esglob_Yiedl2DBIN1PbPb[npt];

  for (int i = 0; i < npt; ++i)
  {
    Yiedl2DBIN1PbPb[i] = NofJpsi2DBIN1PbPb[i]/(Accxeff2DBIN1PbPb[i]);

    estat_Yiedl2DBIN1PbPb[i] = (estat_NofJpsi2DBIN1PbPb[i]/NofJpsi2DBIN1PbPb[i])*Yiedl2DBIN1PbPb[i];

    essyt_Yiedl2DBIN1PbPb[i] = TMath::Sqrt( esyst_NofJpsi2DBIN1PbPb[i]/NofJpsi2DBIN1PbPb[i]*esyst_NofJpsi2DBIN1PbPb[i]/NofJpsi2DBIN1PbPb[i] +esyst_Accxeff2DBIN1PbPb[i]*esyst_Accxeff2DBIN1PbPb[i]/100./100. )*Yiedl2DBIN1PbPb[i];

    esglob_Yiedl2DBIN1PbPb[i] = TMath::Sqrt(estat_Yiedl2DBIN1PbPb[i]*estat_Yiedl2DBIN1PbPb[i] + essyt_Yiedl2DBIN1PbPb[i]*essyt_Yiedl2DBIN1PbPb[i]);

  }


  // ====================
  // -- 3.25 < y < 4 --
  // ====================

  Double_t NofJpsi2DBIN2PbPb[npt]       ={17050,24586,17116,8699,4590,2483,1944,727};
  Double_t estat_NofJpsi2DBIN2PbPb[npt] ={722,798,507,368,234,136,89,63};
  Double_t esyst_NofJpsi2DBIN2PbPb[npt] ={606,573,324,185,113,51,42,14};

  Double_t Accxeff2DBIN2PbPb[npt]       ={0.1546,0.1329,0.1323,0.1572,0.2010,0.2486,0.3073,0.3795};
  Double_t estat_Accxeff2DBIN2PbPb[npt] ={ 0.0005,0.0003,0.0004,0.0007,0.0011,0.0019,0.0026,0.0051};
  Double_t esyst_Accxeff2DBIN2PbPb[npt] ={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00};

  for (int i = 0; i < npt; ++i){
    esyst_Accxeff2DBIN2PbPb[i] = TMath::Sqrt( 3*3     +     3*3 + 1      + 1.5*1.5+1        +   esyst_Trigger2DBIN2[i]*esyst_Trigger2DBIN2[i] );
    //                                    MCinput       tracking      intrinsic trigg         trigger response
  }

  Double_t Yiedl2DBIN2PbPb[npt];
  Double_t estat_Yiedl2DBIN2PbPb[npt];
  Double_t essyt_Yiedl2DBIN2PbPb[npt];
  Double_t esglob_Yiedl2DBIN2PbPb[npt];

  for (int i = 0; i < npt; ++i)
  {
    Yiedl2DBIN2PbPb[i] = NofJpsi2DBIN2PbPb[i]/(Accxeff2DBIN2PbPb[i]);

    estat_Yiedl2DBIN2PbPb[i] = (estat_NofJpsi2DBIN2PbPb[i]/NofJpsi2DBIN2PbPb[i])*Yiedl2DBIN2PbPb[i];

    essyt_Yiedl2DBIN2PbPb[i] =
    TMath::Sqrt( esyst_NofJpsi2DBIN2PbPb[i]/NofJpsi2DBIN2PbPb[i]*esyst_NofJpsi2DBIN2PbPb[i]/NofJpsi2DBIN2PbPb[i] + esyst_Accxeff2DBIN2PbPb[i]*esyst_Accxeff2DBIN2PbPb[i]/100./100. )*Yiedl2DBIN2PbPb[i];

    esglob_Yiedl2DBIN2PbPb[i] = TMath::Sqrt(estat_Yiedl2DBIN2PbPb[i]*estat_Yiedl2DBIN2PbPb[i] + essyt_Yiedl2DBIN2PbPb[i]*essyt_Yiedl2DBIN2PbPb[i]);

  }


  //------------------------------------------------------
  // pp values @ 5TeV
  //------------------------------------------------------


  //  --- 2.5 < y < 3.25 ---

  Double_t cs_2DBIN1[npt]       ={ 0.7506,1.2930,1.0717,0.6259,0.2874,0.1472,0.0634,0.0089};
  Double_t estat_cs_2DBIN1[npt] ={ 0.0447,0.0494,0.0445,0.0301,0.0196,0.0117,0.0052,0.0013};
  Double_t esyst_cs_2DBIN1[npt] ={ 0.0461,0.0674,0.0522,0.0353,0.0154,0.0071,0.0029,0.0004};
  Double_t etotglobal_cs_2DBIN1[npt];
  Double_t etotfull_cs_2DBIN1[npt];
  for(int i=0;i<npt;i++){
    // global error
    etotglobal_cs_2DBIN1[i]=(2.16/100.)*cs_2DBIN1[i];
    // global2 + stat2 + syst2
    etotfull_cs_2DBIN1[i]=TMath::Sqrt( esyst_cs_2DBIN1[i]*esyst_cs_2DBIN1[i]+estat_cs_2DBIN1[i]*estat_cs_2DBIN1[i]);
  }


  //  --- 3.25 < y < 4 ---

  Double_t cs_2DBIN2[npt]       ={ 0.5561,1.0520,0.7466,0.4223,0.1746,0.1082,0.0280,0.0067};
  Double_t estat_cs_2DBIN2[npt] ={ 0.0292,0.0396,0.0354,0.0252,0.0144,0.0095,0.0038,0.0010};
  Double_t esyst_cs_2DBIN2[npt] ={ 0.0297,0.0521,0.0364,0.0217,0.0101,0.0061,0.0017,0.0004};
  Double_t etotglobal_cs_2DBIN2[npt];
  Double_t etotfull_cs_2DBIN2[npt];
  for(int i=0;i<npt;i++){
    // global error
    etotglobal_cs_2DBIN2[i]=(2.16/100.)*cs_2DBIN2[i];
    // global2 + stat2 + syst2
    etotfull_cs_2DBIN2[i]=TMath::Sqrt( esyst_cs_2DBIN2[i]*esyst_cs_2DBIN2[i]+estat_cs_2DBIN2[i]*estat_cs_2DBIN2[i]);
  }


  // --- Create Histograms ---

  TH1F* h_2DBIN1PbPb = new TH1F("2DBIN1PbPb","2DBIN1PbPb",npt,pt);
  h_2DBIN1PbPb->Sumw2();
  TH1F* h_2DBIN2PbPb = new TH1F("2DBIN2PbPb","2DBIN2PbPb",npt,pt);
  h_2DBIN2PbPb->Sumw2();
  for (int i = 0; i < npt; ++i)
  {

    h_2DBIN1PbPb->SetBinContent(i+1,Yiedl2DBIN1PbPb[i]);
    h_2DBIN1PbPb->SetBinError(i+1,esglob_Yiedl2DBIN1PbPb[i] );

    h_2DBIN2PbPb->SetBinContent(i+1,Yiedl2DBIN2PbPb[i]);
    h_2DBIN2PbPb->SetBinError(i+1,esglob_Yiedl2DBIN2PbPb[i]);


  }
  h_2DBIN1PbPb->SetMarkerStyle(20);
  h_2DBIN1PbPb->SetMarkerColor(4);
  h_2DBIN1PbPb->SetMarkerSize(0.5);
  h_2DBIN1PbPb->SetMarkerSize(0.5);
  h_2DBIN1PbPb->SetFillStyle(0);
  h_2DBIN1PbPb->SetLineColor(4);

  h_2DBIN2PbPb->SetMarkerStyle(20);
  h_2DBIN2PbPb->SetMarkerColor(3);
  h_2DBIN2PbPb->SetMarkerSize(0.5);
  h_2DBIN2PbPb->SetMarkerSize(0.5);
  h_2DBIN2PbPb->SetFillStyle(0);
  h_2DBIN2PbPb->SetLineColor(3);

  // h_2DBIN1PbPb->Print("Range");
  // h_2DBIN2PbPb->Print("Range");


  TH1F* h_2DBIN1PP = new TH1F("2DBIN1PP","2DBIN1PP",npt,pt);
  h_2DBIN1PP->Sumw2();
  TH1F* h_2DBIN2PP = new TH1F("2DBIN2PP","2DBIN2PP",npt,pt);
  h_2DBIN2PP->Sumw2();
  for (int i = 0; i < npt; ++i)
  {

    h_2DBIN1PP->SetBinContent(i+1,cs_2DBIN1[i]);
    h_2DBIN1PP->SetBinError(i+1,etotfull_cs_2DBIN1[i] );

    h_2DBIN2PP->SetBinContent(i+1,cs_2DBIN2[i]);
    h_2DBIN2PP->SetBinError(i+1,etotfull_cs_2DBIN2[i]);


  }
  h_2DBIN1PP->SetMarkerStyle(20);
  h_2DBIN1PP->SetMarkerColor(4);
  h_2DBIN1PP->SetMarkerSize(0.5);
  h_2DBIN1PP->SetMarkerSize(0.5);
  h_2DBIN1PP->SetFillStyle(0);
  h_2DBIN1PP->SetLineColor(4);

  h_2DBIN2PP->SetMarkerStyle(20);
  h_2DBIN2PP->SetMarkerColor(3);
  h_2DBIN2PP->SetMarkerSize(0.5);
  h_2DBIN2PP->SetMarkerSize(0.5);
  h_2DBIN2PP->SetFillStyle(0);
  h_2DBIN2PP->SetLineColor(3);

  h_2DBIN1PP->Print("Range");
  h_2DBIN2PP->Print("Range");


  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  // --- Create PbPb canvas --

  TCanvas *c1 = new TCanvas("c1_nofjpsi","c1_nofjpsi",20,20,800,800);
  c1->Divide(1,2);

  c1->cd(1);
  gPad->SetLogy(1);

  h_2DBIN1PbPb->GetYaxis()->SetTitle("NofJPsi/Accxeff");
  TLegend*leg1 = new TLegend(0.2772713,0.6872277,0.5569771,0.8642429,NULL,"brNDC");
  leg1->AddEntry(h_2DBIN1PbPb," 2.5 < y < 3.25 (stat. + uncorr)","PE");
  leg1->AddEntry(h_2DBIN2PbPb," 3.25 < y < 4 (stat. + uncorr)","PE");
  leg1->SetBorderSize(0);
  leg1->SetTextSize(0.06);
  leg1->SetLineColor(0);
  leg1->SetLineStyle(0);
  leg1->SetLineWidth(2);
  leg1->SetFillColor(10);
  leg1->SetFillStyle(1);

  TF1 *f2DBIN1PbPb = new TF1("f2DBIN1PbPb","[0]*x/((1+(x/[1])**2)**[2])",0,12);
  f2DBIN1PbPb->SetParameters(100000,1,1);
  f2DBIN1PbPb->SetLineColor(4);
  f2DBIN1PbPb->SetLineWidth(0.5);
  TF1 *f2DBIN2PbPb = new TF1("f2DBIN2PbPb","[0]*x/((1+(x/[1])**2)**[2])",0,12);
  f2DBIN2PbPb->SetParameters(100000,1,1);
  f2DBIN2PbPb->SetLineColor(3);
  f2DBIN2PbPb->SetLineWidth(0.5);

  h_2DBIN1PbPb->Fit(f2DBIN1PbPb,"SERI");
  h_2DBIN2PbPb->Fit(f2DBIN2PbPb,"SERI");

  h_2DBIN1PbPb->DrawCopy();
  h_2DBIN2PbPb->DrawCopy("same");
  f2DBIN1PbPb->DrawCopy("same");
  f2DBIN2PbPb->DrawCopy("same");
  leg1->Draw("same");

  c1->cd(2);

  h_2DBIN2PbPb->Divide(h_2DBIN1PbPb);
  h_2DBIN2PbPb->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
  h_2DBIN2PbPb->GetYaxis()->SetTitle("#frac{NofJPsi/Accxeff(3.25 < y < 4)}{NofJPsi/Accxeff(2.5 < y < 3.25)}");
  h_2DBIN2PbPb->GetYaxis()->SetRangeUser(0,1.2);
  h_2DBIN2PbPb->GetYaxis()->SetTitleOffset(0.7);
  h_2DBIN2PbPb->DrawCopy();

  TF1 *fratioPbPb = new TF1("fratioPbPb","([0]*x/((1+(x/[1])**2)**[2]))/([3]*x/((1+(x/[4])**2)**[5]))",0,12);
  fratioPbPb->SetParameters(f2DBIN2PbPb->GetParameter(0),f2DBIN2PbPb->GetParameter(1),f2DBIN2PbPb->GetParameter(2),f2DBIN1PbPb->GetParameter(0),f2DBIN1PbPb->GetParameter(1),f2DBIN1PbPb->GetParameter(2));
  fratioPbPb->SetLineColor(2);

  fratioPbPb->DrawCopy("same");

  // --- Create pp canvas --

  TCanvas *c2 = new TCanvas("c2","c2",20,20,800,800);
  c2->Divide(1,2);

  c2->cd(1);
  gPad->SetLogy(1);

  h_2DBIN1PP->GetYaxis()->SetTitle("d#sigma/dy/d#it{p}_{T} (#mub/GeV/c)");
  TLegend*leg2 = new TLegend(0.2772713,0.6872277,0.5569771,0.8642429,NULL,"brNDC");
  leg2->AddEntry(h_2DBIN1PP," 2.5 < y < 3.25 (stat. + uncorr)","PE");
  leg2->AddEntry(h_2DBIN2PP," 3.25 < y < 4 (stat. + uncorr)","PE");
  leg2->SetBorderSize(0);
  leg2->SetTextSize(0.06);
  leg2->SetLineColor(0);
  leg2->SetLineStyle(0);
  leg2->SetLineWidth(2);
  leg2->SetFillColor(10);
  leg2->SetFillStyle(1);

  TF1 *f2DBIN1PP = new TF1("f2DBIN1PP","[0]*x/((1+(x/[1])**2)**[2])",0,12);
  f2DBIN1PP->SetParameters(1,1,2);
  f2DBIN1PP->SetLineColor(4);
  f2DBIN1PP->SetLineWidth(0.5);
  TF1 *f2DBIN2PP = new TF1("f2DBIN2PP","[0]*x/((1+(x/[1])**2)**[2])",0,12);
  f2DBIN2PP->SetParameters(1,1,2);
  f2DBIN2PP->SetLineColor(3);
  f2DBIN2PP->SetLineWidth(0.5);


  h_2DBIN1PP->Fit(f2DBIN1PP,"SERI");
  h_2DBIN2PP->Fit(f2DBIN2PP,"SERI");

  h_2DBIN1PP->DrawCopy();
  h_2DBIN2PP->DrawCopy("same");
  f2DBIN1PP->DrawCopy("same");
  f2DBIN2PP->DrawCopy("same");
  leg2->Draw("same");

  c2->cd(2);

  h_2DBIN2PP->Divide(h_2DBIN1PP);
  h_2DBIN2PP->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
  h_2DBIN2PP->GetYaxis()->SetTitle("#frac{d#sigma/d#it{p}_{T}(3.25 < y < 4)}{d#sigma/d#it{p}_{T}(2.5 < y < 3.25)}");
  h_2DBIN2PP->GetYaxis()->SetRangeUser(0,1.2);
  h_2DBIN2PP->GetYaxis()->SetTitleOffset(0.7);
  h_2DBIN2PP->DrawCopy();
  h_2DBIN2PP->Print("Range");
  // return;

  TF1 *fratioPP = new TF1("fratioPP","([0]*x/((1+(x/[1])**2)**[2]))/([3]*x/((1+(x/[4])**2)**[5]))",0,12);
  fratioPP->SetParameters(f2DBIN2PP->GetParameter(0),f2DBIN2PP->GetParameter(1),f2DBIN2PP->GetParameter(2),f2DBIN1PP->GetParameter(0),f2DBIN1PP->GetParameter(1),f2DBIN1PP->GetParameter(2));
  fratioPP->SetLineColor(2);

  fratioPP->DrawCopy("same");

  // --- Create ratio canvas --

  TH1F* h_PBPBPPRatio = new TH1F("PBPBPPRatio","PBPBPPRatio",npt,pt);
  h_PBPBPPRatio->Sumw2();

  for (int i = 0; i < npt; ++i)
  {

    double PbPbPoint     = h_2DBIN2PbPb->GetBinContent(i+1);
    double err_PbPbPoint = h_2DBIN2PbPb->GetBinError(i+1 );

    double PPPoint       = h_2DBIN2PP->GetBinContent(i+1);
    double err_PPPoint   = h_2DBIN2PP->GetBinError(i+1);

    double ratio =  PbPbPoint/PPPoint;
    double err_ratio =  TMath::Sqrt( err_PbPbPoint/PbPbPoint*err_PbPbPoint/PbPbPoint + err_PPPoint/PPPoint*err_PPPoint/PPPoint  )*ratio;

    h_PBPBPPRatio->SetBinContent(i+1,ratio);
    h_PBPBPPRatio->SetBinError(i+1,err_ratio);

  }


  TCanvas *c3 = new TCanvas("c3","c3");

  h_PBPBPPRatio->GetYaxis()->SetTitle("R_{AA}(3.25 < y < 4)/R_{AA}(2.5 < y < 3.25)");

  h_PBPBPPRatio->GetYaxis()->SetRangeUser(0,2.5);
  h_PBPBPPRatio->GetYaxis()->SetTitleOffset(0.7);
  h_PBPBPPRatio->DrawCopy();

  TLine* l = new TLine(0,1,12,1);
  l->Draw("same");

  TF1 *fratioRAA = new TF1("fratioRAA","(([0]*x/((1+(x/[1])**2)**[2]))/([3]*x/((1+(x/[4])**2)**[5])))/(([6]*x/((1+(x/[7])**2)**[8]))/([9]*x/((1+(x/[10])**2)**[11])))",0,12);
  fratioRAA->SetParameter(0,f2DBIN2PbPb->GetParameter(0));
  fratioRAA->SetParameter(1,f2DBIN2PbPb->GetParameter(1));
  fratioRAA->SetParameter(2,f2DBIN2PbPb->GetParameter(2));
  fratioRAA->SetParameter(3,f2DBIN1PbPb->GetParameter(0));
  fratioRAA->SetParameter(4,f2DBIN1PbPb->GetParameter(1));
  fratioRAA->SetParameter(5,f2DBIN1PbPb->GetParameter(2));
  fratioRAA->SetParameter(6,f2DBIN2PP->GetParameter(0));
  fratioRAA->SetParameter(7,f2DBIN2PP->GetParameter(1));
  fratioRAA->SetParameter(8,f2DBIN2PP->GetParameter(2));
  fratioRAA->SetParameter(9,f2DBIN1PP->GetParameter(0));
  fratioRAA->SetParameter(10,f2DBIN1PP->GetParameter(1));
  fratioRAA->SetParameter(11,f2DBIN1PP->GetParameter(2));
  fratioRAA->SetLineColor(2);

  fratioRAA->DrawCopy("same");


}

//______________________________________________
Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey){
  Double_t error = TMath::Power(ex/x,2)+TMath::Power(ey/y,2);
  return TMath::Sqrt(error);
}

//______________________________________________
void LoadStyle(){
    int font = 42;
  gROOT->SetStyle("Plain");
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(10);
  gStyle->SetCanvasColor(10);
  gStyle->SetTitleFillColor(10);
  gStyle->SetTitleBorderSize(1);
  gStyle->SetStatColor(10);
  gStyle->SetStatBorderSize(1);
  gStyle->SetLegendBorderSize(1);
  gStyle->SetDrawBorder(0);
  gStyle->SetTextFont(font);
  gStyle->SetStatFont(font);
  gStyle->SetStatFontSize(0.05);
  gStyle->SetStatX(0.97);
  gStyle->SetStatY(0.98);
  gStyle->SetStatH(0.03);
  gStyle->SetStatW(0.3);
  gStyle->SetTickLength(0.02,"y");
  gStyle->SetEndErrorSize(3);
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelFont(font,"xyz");
  gStyle->SetLabelOffset(0.01,"xyz");
  gStyle->SetTitleFont(font,"xyz");
  gStyle->SetTitleOffset(1.,"x");
  gStyle->SetTitleOffset(0.5,"y");
  gStyle->SetTitleSize(0.05,"x");
  gStyle->SetTitleSize(0.06,"yz");
  gStyle->SetMarkerSize(1.3);
  gStyle->SetPalette(1,0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetLineWidth(2);
  gStyle->SetLegendFont(42);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendFillColor(10);
  gStyle->SetPadTickY(1);
  gStyle->SetPadTickX(1);
}