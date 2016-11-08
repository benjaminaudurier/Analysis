void PlotRaa(){

// Npart
const int n=9;
Double_t NPart_data[9]={356.0,260.1,185.8,128.5,94.7,52.4,29.77,15.27,7.49};
Double_t ErrNPart_data[9]={8.,8.,8.,8.,8.,8.,8.,8.,8.};

//Pt
const int npt=11;
Double_t Pt_data[11]={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11};
Double_t ErrPt_data[11]={0.5.,0.5.,0.5.,0.5.,0.5.,0.5.,0.5.,0.5.,0.5.,0.5.,1.};


// 5TeV Raa pt>3 with pp@5TeV
Double_t RAA5_data_pt3_pp[n]={0.633,0.642,0.658,0.626,0.699,0.729,0.824,0.938,0.915};
Double_t ErrStat_RAA5_data_pt3_pp[n]={0.017,0.018,0.019,0.018,0.021,0.023,0.028,0.038,0.049};
Double_t ErrSyst_RAA5_data_pt3_pp[n]={0.016,0.017,0.018,0.023,0.024,0.030,0.044,0.064,0.078};// 5TeV Raa


// 5 TeV with pp@5TeVRef
Double_t RAA5_data_pp[n]={0.662,0.670,0.687,0.656,0.742,0.791,0.923,1.123,1.285};
Double_t ErrStat_RAA5_data_pp[n]={0.016,0.017,0.018,0.018,0.021,0.023,0.029,0.039,0.056};
Double_t ErrSyst_RAA5_data_pp[n]={0.017,0.018,0.019,0.025,0.026,0.033,0.049,0.076,0.109};


// Pt coordinates
Double_t RAA5_data_PT[npt]={0.785,0.748,0.704,0.577,0.578,0.454,0.428,0.397,0.445,0.329,0.431};
Double_t ErrStat_RAA5_data_PT[npt]={0.063,0.043,0.045,0.035,0.052,0.037,0.045,0.061,0.084,0.104,0.211};
Double_t ErrSyst_RAA5_data_PT[npt]={0.066,0.062,0.056,0.044,0.051,0.034,0.034,0.031,0.036,0.046,0.071};

// Raa vs centrality for PbPb@5TeV + pp@5TeV reference
//____________________________
TGraphErrors *gRAA5_data_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pp,0,ErrStat_RAA5_data_pp);
gRAA5_data_pp->SetMarkerColor(kBlue+1);
gRAA5_data_pp->SetMarkerStyle(20);
gRAA5_data_pp->SetMarkerSize(1.5);

TGraphErrors *gRAA5_datas_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pp,ErrNPart_data,ErrSyst_RAA5_data_pp);
gRAA5_datas_pp->SetMarkerColor(kBlue+1);
gRAA5_datas_pp->SetMarkerStyle(20);
gRAA5_datas_pp->SetMarkerSize(1.5);
//gRAA5_datas_pp->SetLineColor(kGreen+1);
gRAA5_datas_pp->SetFillStyle(0);

Double_t globRAA_data_pt = 0.081332;
TBox *box_globRAA_data_pt = new TBox(435.,1.-globRAA_data_pt,450,1+globRAA_data_pt);
box_globRAA_data_pt->SetFillColor(kBlue+1);

//____________________________


// Raa vs centrality for PbPb@5TeV + pp@5TeV reference for pt>0.3
//____________________________
TGraphErrors *gRAA5_data_pt3_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pt3_pp,0,ErrStat_RAA5_data_pt3_pp);
gRAA5_data_pt3_pp->SetMarkerColor(kBlue+1);
gRAA5_data_pt3_pp->SetMarkerStyle(20);
gRAA5_data_pt3_pp->SetMarkerSize(1.5);

TGraphErrors *gRAA5_datas_pt3_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pt3_pp,ErrNPart_data,ErrSyst_RAA5_data_pt3_pp);
gRAA5_datas_pt3_pp->SetMarkerColor(kBlue+1);
gRAA5_datas_pt3_pp->SetMarkerStyle(20);
gRAA5_datas_pt3_pp->SetMarkerSize(1.5);
//gRAA5_datas_pt3_pp->SetLineColor(kGreen+1);
gRAA5_datas_pt3_pp->SetFillStyle(0);

Double_t globRAA_data_pt3 = 0.079166;
TBox *box_globRAA_data_pt3_pp = new TBox(435.,1.-globRAA_data_pt3,450,1+globRAA_data_pt3);
box_globRAA_data_pt3_pp->SetFillColor(kBlue+1);



//____________________________


// Raa vs pt for PbPb@5TeV + pp@5TeV reference
//____________________________
TGraphErrors *gRAA5_data_pt = new
TGraphErrors(npt,Pt_data,RAA5_data_PT,ErrPt_data,ErrStat_RAA5_data_PT);
gRAA5_data_pt->SetMarkerColor(kRed+1);
gRAA5_data_pt->SetMarkerStyle(20);
gRAA5_data_pt->SetMarkerSize(1.5);


TGraphErrors *gRAA5_datas_pt = new
TGraphErrors(npt,Pt_data,RAA5_data_PT,ErrPt_data,ErrSyst_RAA5_data_PT);
gRAA5_datas_pt->SetMarkerStyle(20);
gRAA5_datas_pt->SetMarkerSize(1.5);
gRAA5_datas_pt->SetLineColor(kRed+1);
gRAA5_datas_pt->SetFillStyle(0);

Double_t glob2015_pt_12 = 0.066818;
TBox *box_glob2015_pt_12 = new TBox(11.5,1.-glob2015_pt_12,12,1+glob2015_pt_12);
box_glob2015_pt_12->SetFillColor(kRed+1);
//____________________________


TH2D *hRAA = new TH2D("hRAA","",100,0.,450.,100,0.,1.4);
hRAA->GetXaxis()->SetTitle("N_{part}");
hRAA->GetYaxis()->SetTitle("R_{AA}");

TH2D *hpt = new TH2D("h","",100,0.,12.,100,0.,1.4);
hpt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
hpt->GetYaxis()->SetTitle("R_{AA}(5.02 TeV)");


//First Canvas
new TCanvas();
hRAA->Draw();
gRAA5_data_pp->Draw("P");
gRAA5_datas_pp->Draw("E2");
box_globRAA_data_pt->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

//Second Canvas
new TCanvas();
hRAA->Draw();
gRAA5_data_pt3_pp->Draw("P");
gRAA5_datas_pt3_pp->Draw("E2");
box_globRAA_data_pt3_pp->Draw("F");
l1->Draw();


//Third Canvas
new TCanvas();
hpt->Draw();
gRAA5_data_pt->Draw("P");
gRAA5_datas_pt->Draw("E2");
box_glob2015_pt_12->Draw("F");
TLine *l2 = new TLine(0.,1.,12.,1.);
l2->SetLineStyle(2);
l2->Draw();


}