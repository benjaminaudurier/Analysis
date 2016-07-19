void ComparisonWithInterpolation_y_Benjamin(){

  LoadStyle();
//------------------------------------------------------
// pp values@5TeV  
//------------------------------------------------------
const int ny          =6;
Double_t y[ny]        ={2.625,2.875,3.125,3.375,3.625,3.875};
Double_t ey[ny]       ={0.,0.,0.,0.,0.,0.};
Double_t ey_syst[ny]  ={0.03,0.03,0.03,0.03,0.03,0.03};
Double_t cs[ny]       ={4.594,4.346,3.989,3.643,3.097,2.519};
Double_t estat_cs[ny] ={0.264,0.136,0.112,0.104,0.107,0.165};
Double_t esyst_cs[ny] ={0.293,0.190,0.155,0.132,0.109,0.133};
Double_t etotglobal_cs[ny];
Double_t etotfull[ny];

for(int i=0;i<ny;i++){
  // global error
  etotglobal_cs[i]=(2.16/100)*cs[i];   
  // global2 + stat2 + syst2
  etotfull[i]=TMath::Sqrt( esyst_cs[i]*esyst_cs[i]+estat_cs[i]*estat_cs[i]);
}


Double_t deltay=1;
for(int i=0;i<ny;i++){
   printf("i=%d cs= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,cs[i],estat_cs[i],estat_cs[i]/cs[i]*100.,esyst_cs[i],
   esyst_cs[i]/cs[i]*100.,etotglobal_cs[i]/cs[i]*100.);
}

TGraphErrors *gData = new TGraphErrors(ny,y,cs,ey,estat_cs);
gData->SetMarkerStyle(20);
gData->SetMarkerColor(2);
gData->SetMarkerSize(1.5);

TGraphErrors *gData_syst = new TGraphErrors(ny,y,cs,ey_syst,esyst_cs);
gData_syst->SetMarkerStyle(20);
gData_syst->SetMarkerColor(2);
gData_syst->SetMarkerSize(1.5);
gData_syst->SetMarkerSize(1.5);
gData_syst->SetFillStyle(0);
gData_syst->SetLineColor(2);


//---------------------------------
// interpolated values at 5TeV
//---------------------------------  
//from public note

  const int ninterp=6;
  Double_t y_Interp[ninterp]          ={2.625,2.875,3.125,3.375,3.625,3.875}; 
  Double_t CS_Interp[ninterp]         ={4.65,4.01,3.64,3.29,3.08,2.68};
  Double_t StatSystUncInterp[ninterp] ={0.43,0.36,0.33,0.30,0.28,0.24};
  Double_t SystShapeInterp[ninterp]   ={0.12,0.04,0.06,0.05,0.07,0.05};
  Double_t SystInterp[ninterp];
  Double_t ErrTot_CS_Interp[ninterp];
  
  for(int i=0;i<ninterp;i++)
  {     
    SystInterp[i]       =TMath::Sqrt(StatSystUncInterp[i]*StatSystUncInterp[i]+SystShapeInterp[i]*SystShapeInterp[i]);
    ErrTot_CS_Interp[i] =SystInterp[i];
  }
  
  TGraphErrors *gInterp = new TGraphErrors(ninterp,y_Interp,CS_Interp,ey_syst,SystInterp);
  gInterp->SetMarkerColor(kBlue);
  gInterp->SetFillStyle(0);
  gInterp->SetLineColor(kBlue);
  gInterp->SetMarkerStyle(20);
  gInterp->SetMarkerSize(1.5);


//---------------------------------------------------
// plot
//---------------------------------------------------

gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
TCanvas *c = new TCanvas("c","c",20,20,800,800);
c->Divide(1,2);

c->cd(1);
//gPad->SetLogy(1);
TH2D *hnull = new TH2D("hnull","hnull",100,2.5,4,100,0,6);
hnull->GetXaxis()->SetTitle("#it{y}");
hnull->GetYaxis()->SetTitle("d#sigma/d#it{y} (#mub)");
hnull->Draw();
gData->Draw("Psame");
gData_syst->Draw("E2Psame");
gInterp->Draw("E2Psame");

TLegend *leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gInterp,"interpolation (+-5.2% global syst.)","P");
leg0->AddEntry(gData,"pp data @ 5TeV (+-2% global syst.)","P");
leg0->Draw();

//----------------------------------------------
// ratio
//----------------------------------------------
Double_t Ratio_Data_Interp[ninterp];
Double_t ErrRatio_Data_Interp[ninterp];

for(int i=0;i<ninterp;i++){
  if(i!=6){
    Ratio_Data_Interp[i]=cs[i]/CS_Interp[i];  
    ErrRatio_Data_Interp[i]=PropError(cs[i],CS_Interp[i],etotfull[i],SystInterp[i]);  
  } 
  // last point 
  else if(i==6) {
    Ratio_Data_Interp[i]=((cs[6]+cs[7])/2.)/CS_Interp[i];  
    
    Double_t ErrSyst_CS_Data_67= TMath::Sqrt(etotfull[6]*etotfull[6]+etotfull[7]*etotfull[7])/2.;
    
    ErrRatio_Data_Interp[i]=PropError(((cs[6]+cs[7])/2.),CS_Interp[i],ErrSyst_CS_Data_67,SystInterp[i]);  
  }  
}

  TGraphErrors *gRatio_Data_Interp = new TGraphErrors(ninterp,y_Interp,Ratio_Data_Interp,0,ErrRatio_Data_Interp);
  gRatio_Data_Interp->SetMarkerStyle(20);
  gRatio_Data_Interp->SetMarkerColor(kRed);
  gRatio_Data_Interp->SetMarkerSize(1.5);
  
  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,2.5,4,100,0.7,1.3);
  hnull2->GetXaxis()->SetTitle("#it{y}");
  hnull2->GetYaxis()->SetTitle("Ratio Data/Interpolation");
  hnull2->Draw();
  gRatio_Data_Interp->Draw("P");
  TLine *l = new TLine(2.5,1.,4.,1.);
  l->SetLineColor(8);
  l->SetLineWidth(2);
  l->Draw();

   // Config. Legend
   leg = new TLegend(0.2166957,0.1273987,0.4961542,0.3780455,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.06);
   leg->SetLineColor(0);
   leg->SetLineStyle(0);
   leg->SetLineWidth(2);
   leg->SetFillColor(10);
   leg->SetFillStyle(1);
   entry=leg->AddEntry("NULL","interpolation (+-5.2% global syst.)","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","pp data @ 5TeV (+-2% global syst.)","P");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();

  Double_t glob = TMath::Sqrt(2.16*2.16 + 5.2*5.2)/100; ;
  TBox *box_glob = new TBox(3.95.,1.-glob,4.,1+glob);
  box_glob->SetFillColor(kRed);
  box_glob->Draw("same");

}

Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey){
  Double_t error = TMath::Power((ex/x),2)+TMath::Power((ey/y),2);    
  return TMath::Sqrt(error);
}

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