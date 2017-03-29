
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

//______________________________________________
void LoadStyle();

  LoadStyle();


void PlotRaaMultiDiff(){

// Npart
const int n=7;
Double_t NPart_data[n]={356.0,260.1,185.8,128.5,94.7,52.4,29.77};
Double_t ErrNPart_data[n]={8.,8.,8.,8.,8.,8.,8.};

//Pt
const int npt=11;
Double_t Pt_data[11]={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11};
Double_t ErrPt_data[11]={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,1.};

Double_t Pt_data_ratio[7]={0.5,1.5,2.5,3.5,4.5,5.5,7.};
Double_t ErrPt_data_ratio[7]={0.5,0.5,0.5,0.5,0.5,0.5,1.};

Double_t Pt_data_2D[8]={0.5,1.5,2.5,3.5,4.5,5.5,7.,10.};
Double_t ErrPt_data_2D[8]={0.5,0.5,0.5,0.5,0.5,0.5,1.,2.};

//Pt
const int nY=7;
Double_t Y_data[7]={2.5,2.75,3.0,3.25,3.5,3.75,4};
Double_t ErrY_data[7]={0.1,0.1,0.1,0.1,0.1,0.1,0.1};


// --- RAA vs NPART values ---


// 0 < pt < 2
Double_t RAA5_data_pt02[n]={0.739,0.723,0.737,0.625,0.727,0.788,1.097};
Double_t ErrStat_RAA5_data_pt02[n]={0.020,0.024,0.025,0.026,0.031,0.034,0.033};
Double_t ErrSyst_RAA5_data_pt02[n]={0.029,0.027,0.026,0.024,0.033,0.044,0.069};

TGraphErrors *gRAA5_data_pt02 = new
TGraphErrors(n,NPart_data,RAA5_data_pt02,0,ErrStat_RAA5_data_pt02);
gRAA5_data_pt02->SetMarkerColor(kBlue+1);
gRAA5_data_pt02->SetMarkerStyle(20);
gRAA5_data_pt02->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt02 = new
TGraphErrors(n,NPart_data,RAA5_data_pt02,ErrNPart_data,ErrSyst_RAA5_data_pt02);
gRAA5_datas_pt02->SetMarkerColor(kBlue+1);
gRAA5_datas_pt02->SetMarkerStyle(20);
gRAA5_datas_pt02->SetMarkerSize(0.85);
gRAA5_datas_pt02->SetFillStyle(0);
gRAA5_datas_pt02->SetLineColor(kBlue+1);

Double_t globRAA_data_pt02 = 0.079;
TBox *box_globRAA_data_pt02 = new TBox(435.,1.-globRAA_data_pt02,450,1+globRAA_data_pt02);
box_globRAA_data_pt02->SetFillColor(kBlue+1);

Double_t RAA5_data_pt02_ratio[n]={1.010,0.987,1.031,0.922,1.135,0.939,0.993};
Double_t ErrStat_RAA5_data_pt02_ratio[n]={0.054,0.057,0.065,0.066,0.091,0.067,0.065};
Double_t ErrSyst_RAA5_data_pt02_ratio[n]={0.0600.0430.0390.0480.0620.0680.072};

TGraphErrors *gRAA5_data_pt02_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt02_ratio,0,ErrStat_RAA5_data_pt02_ratio);
gRAA5_data_pt02_ratio->SetMarkerColor(kBlue+1);
gRAA5_data_pt02_ratio->SetMarkerStyle(20);
gRAA5_data_pt02_ratio->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt02_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt02_ratio,ErrNPart_data,ErrSyst_RAA5_data_pt02_ratio);
gRAA5_datas_pt02_ratio->SetMarkerColor(kBlue+1);
gRAA5_datas_pt02_ratio->SetMarkerStyle(20);
gRAA5_datas_pt02_ratio->SetMarkerSize(0.85);
gRAA5_datas_pt02_ratio->SetFillStyle(0);
gRAA5_datas_pt02_ratio->SetLineColor(kBlue+1);

Double_t globRAA_data_pt02ratio = 0.169;
TBox *box_globRAA_data_pt02ratio = new TBox(435.,1.-globRAA_data_pt02ratio,450,1+globRAA_data_pt02ratio);
box_globRAA_data_pt02ratio->SetFillColor(kBlue+1);


// 2 < pt < 5
Double_t RAA5_data_pt25[n]={0.570,0.592,0.638,0.615,0.685,0.742,0.860};
Double_t ErrStat_RAA5_data_pt25[n]={0.011,0.013,0.014,0.016,0.019,0.022,0.024};
Double_t ErrSyst_RAA5_data_pt25[n]={0.023,0.024,0.025,0.024,0.031,0.039,0.054};

TGraphErrors *gRAA5_data_pt25 = new
TGraphErrors(n,NPart_data,RAA5_data_pt25,0,ErrStat_RAA5_data_pt25);
gRAA5_data_pt25->SetMarkerColor(kRed+1);
gRAA5_data_pt25->SetMarkerStyle(20);
gRAA5_data_pt25->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt25 = new
TGraphErrors(n,NPart_data,RAA5_data_pt25,ErrNPart_data,ErrSyst_RAA5_data_pt25);
gRAA5_datas_pt25->SetMarkerColor(kRed+1);
gRAA5_datas_pt25->SetMarkerStyle(20);
gRAA5_datas_pt25->SetMarkerSize(0.85);
gRAA5_datas_pt25->SetFillStyle(0);
gRAA5_datas_pt25->SetLineColor(kRed+1);

Double_t globRAA_data_pt25 = 0.071;
TBox *box_globRAA_data_pt25 = new TBox(435.-15,1.-globRAA_data_pt25,450-15,1+globRAA_data_pt25);
box_globRAA_data_pt25->SetFillColor(kRed+1);

Double_t RAA5_data_pt25_ratio[n]={1.341,1.285,1.206,1.235,1.151,1.099,1.191};
Double_t ErrStat_RAA5_data_pt25_ratio[n]={0.066,0.060,0.057,0.070,0.066,0.076,0.080};
Double_t ErrSyst_RAA5_data_pt25_ratio[n]={0.062,0.054,0.049,0.072,0.063,0.070,0.085};

TGraphErrors *gRAA5_data_pt25_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt25_ratio,0,ErrStat_RAA5_data_pt25_ratio);
gRAA5_data_pt25_ratio->SetMarkerColor(kRed+1);
gRAA5_data_pt25_ratio->SetMarkerStyle(20);
gRAA5_data_pt25_ratio->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt25_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt25_ratio,ErrNPart_data,ErrSyst_RAA5_data_pt25_ratio);
gRAA5_datas_pt25_ratio->SetMarkerColor(kRed+1);
gRAA5_datas_pt25_ratio->SetMarkerStyle(20);
gRAA5_datas_pt25_ratio->SetMarkerSize(0.85);
gRAA5_datas_pt25_ratio->SetFillStyle(0);
gRAA5_datas_pt25_ratio->SetLineColor(kRed+1);

Double_t globRAA_data_pt25ratio = 0.157;
TBox *box_globRAA_data_pt25ratio = new TBox(435.-15,1.-globRAA_data_pt25ratio,450-15,1+globRAA_data_pt25ratio);
box_globRAA_data_pt25ratio->SetFillColor(kRed+1);

// 5 < pt < 8
Double_t RAA5_data_pt58[n]={0.311,0.395,0.443,0.544,0.606,0.691,0.775};
Double_t ErrStat_RAA5_data_pt58[n]={0.012,0.015,0.017,0.022,0.024,0.040,0.039};
Double_t ErrSyst_RAA5_data_pt58[n]={0.011,0.014,0.015,0.019,0.024,0.037,0.048};

TGraphErrors *gRAA5_data_pt58 = new
TGraphErrors(n,NPart_data,RAA5_data_pt58,0,ErrStat_RAA5_data_pt58);
gRAA5_data_pt58->SetMarkerColor(kGreen+1);
gRAA5_data_pt58->SetMarkerStyle(20);
gRAA5_data_pt58->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt58 = new
TGraphErrors(n,NPart_data,RAA5_data_pt58,ErrNPart_data,ErrSyst_RAA5_data_pt58);
gRAA5_datas_pt58->SetMarkerColor(kGreen+1);
gRAA5_datas_pt58->SetMarkerStyle(20);
gRAA5_datas_pt58->SetMarkerSize(0.85);
gRAA5_datas_pt58->SetFillStyle(0);
gRAA5_datas_pt58->SetLineColor(kGreen+1);

Double_t globRAA_data_pt58 = 0.075;
TBox *box_globRAA_data_pt58 = new TBox(435.-30,1.-globRAA_data_pt58,450-30,1+globRAA_data_pt58);
box_globRAA_data_pt58->SetFillColor(kGreen+1);

Double_t RAA5_data_pt58_ratio[n]={1.110,1.400,1.080,1.007,1.145,1.177,1.204};
Double_t ErrStat_RAA5_data_pt58_ratio[n]={0.094,0.144,0.087,0.084,0.124,0.161,0.167};
Double_t ErrSyst_RAA5_data_pt58_ratio[n]={0.047,0.058,0.038,0.047,0.068,0.077,0.090};

TGraphErrors *gRAA5_data_pt58_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt58_ratio,0,ErrStat_RAA5_data_pt58_ratio);
gRAA5_data_pt58_ratio->SetMarkerColor(kGreen+1);
gRAA5_data_pt58_ratio->SetMarkerStyle(20);
gRAA5_data_pt58_ratio->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt58_ratio = new
TGraphErrors(n,NPart_data,RAA5_data_pt58_ratio,ErrNPart_data,ErrSyst_RAA5_data_pt58_ratio);
gRAA5_datas_pt58_ratio->SetMarkerColor(kGreen+1);
gRAA5_datas_pt58_ratio->SetMarkerStyle(20);
gRAA5_datas_pt58_ratio->SetMarkerSize(0.85);
gRAA5_datas_pt58_ratio->SetFillStyle(0);
gRAA5_datas_pt58_ratio->SetLineColor(kGreen+1);

Double_t globRAA_data_pt58ratio = 0.195;
TBox *box_globRAA_data_pt58ratio = new TBox(435.-30,1.-globRAA_data_pt58ratio,450-30,1+globRAA_data_pt58ratio);
box_globRAA_data_pt58ratio->SetFillColor(kGreen+1);


// 8 < pt < 12
Double_t RAA5_data_pt812[n]={0.292,0.359,0.435,0.469,0.637,0.711,0.777};
Double_t ErrStat_RAA5_data_pt812[n]={0.021,0.025,0.029,0.039,0.049,0.080,0.090};
Double_t ErrSyst_RAA5_data_pt812[n]={0.011,0.017,0.016,0.019,0.030,0.037,0.052};

TGraphErrors *gRAA5_data_pt812 = new
TGraphErrors(n,NPart_data,RAA5_data_pt812,0,ErrStat_RAA5_data_pt812);
gRAA5_data_pt812->SetMarkerColor(kGreen+4);
gRAA5_data_pt812->SetMarkerStyle(20);
gRAA5_data_pt812->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_pt812 = new
TGraphErrors(n,NPart_data,RAA5_data_pt812,ErrNPart_data,ErrSyst_RAA5_data_pt812);
gRAA5_datas_pt812->SetMarkerColor(kGreen+4);
gRAA5_datas_pt812->SetMarkerStyle(20);
gRAA5_datas_pt812->SetMarkerSize(0.85);
gRAA5_datas_pt812->SetFillStyle(0);
gRAA5_datas_pt812->SetLineColor(kGreen+4);

Double_t globRAA_data_pt812 = 0.079;
TBox *box_globRAA_data_pt812 = new TBox(435.-45,1.-globRAA_data_pt812,450-45,1+globRAA_data_pt812);
box_globRAA_data_pt812->SetFillColor(kGreen+4);


// --- RAA vs Pt ---

// 0-20%
// ! without pt = 0.3 cut
Double_t RAA5_data_cent0020[npt]={0.785,0.748,0.704,0.577,0.578,0.454,0.428,0.397,0.445,0.329,0.431};
Double_t ErrStat_RAA5_data_cent0020[npt]={0.063,0.043,0.045,0.035,0.052,0.037,0.045,0.061,0.084,0.104,0.211};
Double_t ErrSyst_RAA5_data_cent0020[npt]={0.066,0.062,0.056,0.044,0.051,0.034,0.034,0.031,0.036,0.046,0.071};

TGraphErrors *gRAA5_data_cent0020 = new
TGraphErrors(npt,Pt_data,RAA5_data_cent0020,0,ErrStat_RAA5_data_cent0020);
gRAA5_data_cent0020->SetMarkerColor(kBlue+1);
gRAA5_data_cent0020->SetMarkerStyle(20);
gRAA5_data_cent0020->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_cent0020 = new
TGraphErrors(npt,Pt_data,RAA5_data_cent0020,ErrPt_data,ErrSyst_RAA5_data_cent0020);
gRAA5_datas_cent0020->SetMarkerColor(kBlue+1);
gRAA5_datas_cent0020->SetMarkerStyle(20);
gRAA5_datas_cent0020->SetMarkerSize(0.85);
gRAA5_datas_cent0020->SetFillStyle(0);
gRAA5_datas_cent0020->SetLineColor(kBlue+1);

Double_t globRAA_data_cent0020 = 0.081;
TBox *box_globRAA_data_cent0020 = new TBox(11,1.-globRAA_data_cent0020,12,1+globRAA_data_cent0020);
box_globRAA_data_cent0020->SetFillColor(kBlue+1);

// !! to update !!

// Double_t RAA5_data_cent0020_ratio[7]={0.994,1.033,1.221,1.193,1.349,1.180,0.970};
// Double_t ErrStat_RAA5_data_cent0020_ratio[7]={0.117,0.087,0.104,0.121,0.196,0.204,0.219};
// Double_t ErrSyst_RAA5_data_cent0020_ratio[7]={0.154,0.143,0.143,0.137,0.140,0.127,0.125};

// TGraphErrors *gRAA5_data_cent0020_ratio = new
// TGraphErrors(7,NPart_data,RAA5_data_cent0020_ratio,0,ErrStat_RAA5_data_cent0020_ratio);
// gRAA5_data_cent0020_ratio->SetMarkerColor(kBlue+1);
// gRAA5_data_cent0020_ratio->SetMarkerStyle(20);
// gRAA5_data_cent0020_ratio->SetMarkerSize(0.85);

// TGraphErrors *gRAA5_data_cent0020_ratio = new
// TGraphErrors(7,NPart_data,RAA5_data_cent0020_ratio,ErrPt_data_ratio,ErrSyst_RAA5_data_cent0020_ratio);
// gRAA5_data_cent0020_ratio->SetMarkerColor(kBlue+1);
// gRAA5_data_cent0020_ratio->SetMarkerStyle(20);
// gRAA5_data_cent0020_ratio->SetMarkerSize(0.85);

// Double_t globRAA_data_cent0020ratio = 0.083;
// TBox *box_globRAA_data_cent0020ratio = new TBox(435.,1.-globRAA_data_cent0020ratio,450,1+globRAA_data_cent0020ratio);
// box_globRAA_data_cent0020ratio->SetFillColor(kBlue+1);

// 20-40%
Double_t RAA5_data_cent2040[npt]={0.728,0.682,0.663,0.588,0.599,0.471,0.515,0.491,0.445};
Double_t ErrStat_RAA5_data_cent2040[npt]={0.033,0.022,0.018,0.016,0.019,0.020,0.026,0.031,0.023};
Double_t ErrSyst_RAA5_data_cent2040[npt]={0.060,0.052,0.048,0.037,0.047,0.027,0.031,0.028,0.025};

TGraphErrors *gRAA5_data_cent2040 = new
TGraphErrors(npt,Pt_data,RAA5_data_cent2040,0,ErrStat_RAA5_data_cent2040);
gRAA5_data_cent2040->SetMarkerColor(kBlue+1);
gRAA5_data_cent2040->SetMarkerStyle(20);
gRAA5_data_cent2040->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_cent2040 = new
TGraphErrors(npt,Pt_data,RAA5_data_cent2040,ErrPt_data,ErrSyst_RAA5_data_cent2040);
gRAA5_datas_cent2040->SetMarkerColor(kBlue+1);
gRAA5_datas_cent2040->SetMarkerStyle(20);
gRAA5_datas_cent2040->SetMarkerSize(0.85);
gRAA5_datas_cent2040->SetFillStyle(0);
gRAA5_datas_cent2040->SetLineColor(kBlue+1);

Double_t globRAA_data_cent2040 = 0.039;
TBox *box_globRAA_data_cent2040 = new TBox(435.,1.-globRAA_data_cent2040,450,1+globRAA_data_cent2040);
box_globRAA_data_cent2040->SetFillColor(kBlue+1);

Double_t RAA5_data_cent2040_ratio[7]={0.994,1.033,1.221,1.193,1.349,1.180,0.970};
Double_t ErrStat_RAA5_data_cent2040_ratio[7]={0.117,0.087,0.104,0.121,0.196,0.204,0.219};
Double_t ErrSyst_RAA5_data_cent2040_ratio[7]={0.154,0.143,0.143,0.137,0.140,0.127,0.125};

TGraphErrors *gRAA5_data_cent2040_ratio = new
TGraphErrors(7,NPart_data,RAA5_data_cent2040_ratio,0,ErrStat_RAA5_data_cent2040_ratio);
gRAA5_data_cent2040_ratio->SetMarkerColor(kBlue+1);
gRAA5_data_cent2040_ratio->SetMarkerStyle(20);
gRAA5_data_cent2040_ratio->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_cent2040_ratio = new
TGraphErrors(7,NPart_data,RAA5_data_cent2040_ratio,ErrPt_data_ratio,ErrSyst_RAA5_data_cent2040_ratio);
gRAA5_datas_cent2040_ratio->SetMarkerColor(kBlue+1);
gRAA5_datas_cent2040_ratio->SetMarkerStyle(20);
gRAA5_datas_cent2040_ratio->SetMarkerSize(0.85);
gRAA5_datas_cent2040_ratio->SetFillStyle(0);
gRAA5_datas_cent2040_ratio->SetLineColor(kBlue+1);

Double_t globRAA_data_cent2040ratio = 0.083;
TBox *box_globRAA_data_cent2040ratio = new TBox(435.,1.-globRAA_data_cent2040ratio,450,1+globRAA_data_cent2040ratio);
box_globRAA_data_cent2040ratio->SetFillColor(kBlue+1);


// 40-90%
Double_t RAA5_data_cent4090[npt]={0.924,0.736,0.732,0.705,0.769,0.649,0.660,0.749,0.677};
Double_t ErrStat_RAA5_data_cent4090[npt]={0.034,0.023,0.021,0.019,0.025,0.026,0.033,0.042,0.038};
Double_t ErrSyst_RAA5_data_cent4090[npt]={0.076,0.057,0.050,0.047,0.057,0.038,0.037,0.039,0.038};

TGraphErrors *gRAA5_data_cent4090 = new
TGraphErrors(npt,NPart_data,RAA5_data_cent4090,0,ErrStat_RAA5_data_cent4090);
gRAA5_data_cent4090->SetMarkerColor(kRed+1);
gRAA5_data_cent4090->SetMarkerStyle(20);
gRAA5_data_cent4090->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_cent4090 = new
TGraphErrors(npt,NPart_data,RAA5_data_cent4090,ErrPt_data,ErrSyst_RAA5_data_cent4090);
gRAA5_datas_cent4090->SetMarkerColor(kRed+1);
gRAA5_datas_cent4090->SetMarkerStyle(20);
gRAA5_datas_cent4090->SetMarkerSize(0.85);
gRAA5_datas_cent4090->SetFillStyle(0);
gRAA5_datas_cent4090->SetLineColor(kRed+1);

Double_t globRAA_data_cent4090 = 0.052;
TBox *box_globRAA_data_cent4090 = new TBox(435.,1.-globRAA_data_cent4090,450,1+globRAA_data_cent4090);
box_globRAA_data_cent4090->SetFillColor(kRed+1);

Double_t RAA5_data_cent4090_ratio[7]={1.134,1.006,1.186,1.124,1.109,1.327,1.073};
Double_t ErrStat_RAA5_data_cent4090_ratio[7]={0.117,0.087,0.107,0.115,0.159,0.242,0.252};
Double_t ErrSyst_RAA5_data_cent4090_ratio[7]={0.145,0.146,0.141,0.136,0.136,0.126,0.123};

TGraphErrors *gRAA5_data_cent4090_ratio = new
TGraphErrors(7,NPart_data,RAA5_data_cent4090_ratio,0,ErrStat_RAA5_data_cent4090_ratio);
gRAA5_data_cent4090_ratio->SetMarkerColor(kRed+1);
gRAA5_data_cent4090_ratio->SetMarkerStyle(20);
gRAA5_data_cent4090_ratio->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_cent4090_ratio = new
TGraphErrors(7,NPart_data,RAA5_data_cent4090_ratio,ErrPt_data_ratio,ErrSyst_RAA5_data_cent4090_ratio);
gRAA5_datas_cent4090_ratio->SetMarkerColor(kRed+1);
gRAA5_datas_cent4090_ratio->SetMarkerStyle(20);
gRAA5_datas_cent4090_ratio->SetMarkerSize(0.85);
gRAA5_datas_cent4090_ratio->SetFillStyle(0);
gRAA5_datas_cent4090_ratio->SetLineColor(kRed+1);

Double_t globRAA_data_cent4090ratio = 0.093;
TBox *box_globRAA_data_cent4090ratio = new TBox(435.,1.-globRAA_data_cent4090ratio,450,1+globRAA_data_cent4090ratio);
box_globRAA_data_cent4090ratio->SetFillColor(kRed+1);

// 2.5 < y < 3.25 and 3.25 < y < 4

Double_t RAA5_data_25_325[8]={0.760,0.815,0.658,0.542,0.484,0.406,0.275,0.373};
Double_t ErrStat_RAA5_data_25_325[8]={0.039,0.027,0.021,0.018,0.021,0.020,0.013,0.021};
Double_t ErrSyst_RAA5_data_25_325[8]={0.061,0.059,0.047,0.043,0.036,0.028,0.018,0.024};

TGraphErrors *gRAA5_data_25_325 = new
TGraphErrors(8,Pt_data_2D,RAA5_data_25_325,0,ErrStat_RAA5_data_25_325);
gRAA5_data_25_325->SetMarkerColor(kRed+1);
gRAA5_data_25_325->SetMarkerStyle(20);
gRAA5_data_25_325->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_25_325 = new
TGraphErrors(8,Pt_data_2D,RAA5_data_25_325,Pt_data_2D,ErrSyst_RAA5_data_25_325);
gRAA5_datas_25_325->SetMarkerColor(kRed+1);
gRAA5_datas_25_325->SetMarkerStyle(20);
gRAA5_datas_25_325->SetMarkerSize(0.85);
gRAA5_datas_25_325->SetFillStyle(0);
gRAA5_datas_25_325->SetLineColor(kRed+1);

Double_t globRAA_data_25_325 = 0.039;
TBox *box_globRAA_data_25_325 = new TBox(435.,1.-globRAA_data_25_325,450,1+globRAA_data_25_325);
box_globRAA_data_25_325->SetFillColor(kRed+1);


Double_t RAA5_data_325_4[8]={0.711,0.631,0.621,0.470,0.469,0.331,0.405,0.256};
Double_t ErrStat_RAA5_data_325_4[8]={0.030,0.020,0.018,0.020,0.024,0.018,0.019,0.022};
Double_t ErrSyst_RAA5_data_325_4[8]={0.056,0.045,0.043,0.034,0.037,0.025,0.032,0.020};

TGraphErrors *gRAA5_data_325_4 = new
TGraphErrors(8,Pt_data_2D,RAA5_data_325_4,0,ErrStat_RAA5_data_325_4);
gRAA5_data_325_4->SetMarkerColor(kRed+1);
gRAA5_data_325_4->SetMarkerStyle(20);
gRAA5_data_325_4->SetMarkerSize(0.85);

TGraphErrors *gRAA5_datas_325_4 = new
TGraphErrors(8,Pt_data_2D,RAA5_data_325_4,Pt_data_2D,ErrSyst_RAA5_data_325_4);
gRAA5_datas_325_4->SetMarkerColor(kRed+1);
gRAA5_datas_325_4->SetMarkerStyle(20);
gRAA5_datas_325_4->SetMarkerSize(0.85);
gRAA5_datas_325_4->SetFillStyle(0);
gRAA5_datas_325_4->SetLineColor(kRed+1);

Double_t globRAA_data_325_4 = 0.039;
TBox *box_globRAA_data_325_4 = new TBox(435.,1.-globRAA_data_325_4,450,1+globRAA_data_325_4);
box_globRAA_data_325_4->SetFillColor(kRed+1);




// ========= CANVAS =========


TH2D *hRAA = new TH2D("hRAA","",100,0.,450.,100,0.,1.4);
hRAA->GetXaxis()->SetTitle("N_{part}");
hRAA->GetYaxis()->SetTitle("R_{AA}");

TH2D *hpt = new TH2D("h","",100,0.,12.,100,0.,1.4);
hpt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
hpt->GetYaxis()->SetTitle("R_{AA}(5.02 TeV)");


//First Canvas
TCanvas* c1 = new TCanvas();
c1->Divide(1,2);
c1->cd(1);
hRAA->Draw();
gRAA5_data_pt02->Draw("PE");
gRAA5_data_pt25->Draw("PE");
gRAA5_data_pt58->Draw("PE");
gRAA5_data_pt812->Draw("PE");

gRAA5_datas_pt02->Draw("E2");
gRAA5_datas_pt25->Draw("E2");
gRAA5_datas_pt58->Draw("E2");
gRAA5_datas_pt812->Draw("E2");
box_globRAA_data_pt02->Draw("F");
box_globRAA_data_pt25->Draw("F");
box_globRAA_data_pt58->Draw("F");
box_globRAA_data_pt812->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

c1->cd(2);

hRAA->Draw();
gRAA5_data_pt02_ratio->Draw("PE");
gRAA5_data_pt25_ratio->Draw("PE");
gRAA5_data_pt58_ratio->Draw("PE");

gRAA5_datas_pt02_ratio->Draw("E2");
gRAA5_datas_pt25_ratio->Draw("E2");
gRAA5_datas_pt58_ratio->Draw("E2");

box_globRAA_data_pt02ratio->Draw("F");
box_globRAA_data_pt25ratio->Draw("F");
box_globRAA_data_pt58ratio->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();


//Second Canvas
// TCanvas* c2 = new TCanvas();
// c2->Divide(1,2);
// c2->cd(1);
// hpt->Draw();
// gRAA5_data_cent0020->Draw("PE");
// gRAA5_data_cent2040->Draw("PE");
// gRAA5_data_cent4090->Draw("PE");

// gRAA5_datas_cent0020->Draw("E2");
// gRAA5_datas_cent2040->Draw("E2");
// gRAA5_datas_cent4090->Draw("E2");


// box_globRAA_data_cent0020->Draw("F");
// box_globRAA_data_cent2040->Draw("F");
// box_globRAA_data_cent4090->Draw("F");
// TLine *l2 = new TLine(0.,1.,12.,1.);
// l2->SetLineStyle(2);
// l2->Draw();

// c2->cd(2);

// hpt->Draw();
// gRAA5_data_cent0020_ratio->Draw("PE");
// gRAA5_data_cent2040_ratio->Draw("PE");
// gRAA5_data_cent4090_ratio->Draw("PE");

// gRAA5_datas_cent0020_ratio->Draw("E2");
// gRAA5_datas_cent2040_ratio->Draw("E2");
// gRAA5_datas_cent4090_ratio->Draw("E2");

// box_globRAA_data_cent0020_ratio->Draw("F");
// box_globRAA_data_cent2040_ratio->Draw("F");
// box_globRAA_data_cent4090_ratio->Draw("F");
// l2->Draw();


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