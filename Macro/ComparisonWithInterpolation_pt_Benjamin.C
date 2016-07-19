void ComparisonWithInterpolation_pt_Benjamin(){

  LoadStyle();

//------------------------------------------------------
// pp values @ 5TeV  
//------------------------------------------------------

const int npt=11;
Double_t pt[npt]       ={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11.};
Double_t ept[npt]      ={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,1.};
Double_t ept_syst[npt] ={0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3,0.3};
Double_t cs[npt]       ={0.6509,1.1731,0.9033,0.5192,0.2304,0.1268,0.0621,0.0291,0.0144,0.0111,0.0034};
Double_t estat_cs[npt] ={ 0.0238,0.0311,0.0273,0.0194,0.0126,0.0076,0.0051,0.0033,0.0024,0.0021,0.0007};
Double_t esyst_cs[npt] ={ 0.033,0.052,0.036,0.021,0.014,0.005,0.0022,0.0009,0.0007,0.0011,0.00009};
Double_t etotglobal_cs[npt];
Double_t etotfull[npt];


for(int i=0;i<npt;i++){
  // global error
  etotglobal_cs[i]=(2.16/100.)*cs[i];   
  // global2 + stat2 + syst2
  etotfull[i]=TMath::Sqrt( esyst_cs[i]*esyst_cs[i]+estat_cs[i]*estat_cs[i]);
}

// Normalize to dy and bin width
Double_t deltay=1.;
Double_t deltapt[npt]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.}; //CHECK

for(int i=0;i<npt;i++){    
  
  cs[i]=cs[i]/deltay/deltapt[i];
  estat_cs[i]=estat_cs[i]/deltay/deltapt[i];
  esyst_cs[i]=esyst_cs[i]/deltay/deltapt[i];
  etotglobal_cs[i]=etotglobal_cs[i]/deltay/deltapt[i];
  
  printf("i=%d cs= %4.3f +- %4.3f (%4.2f) +- %4.3f (%4.2f) etot in perc=%4.2f\n",i,
  cs[i],
  estat_cs[i],
  estat_cs[i]/cs[i]*100.,
  esyst_cs[i],
  esyst_cs[i]/cs[i]*100.,
  etotglobal_cs[i]/cs[i]*100.);
}
// Data points and error stat.
TGraphErrors *gData = new TGraphErrors(npt,pt,cs,ept,estat_cs);
gData->SetMarkerStyle(20);
gData->SetMarkerColor(2);
gData->SetMarkerSize(1.5);

// Data points and error syst.
TGraphErrors *gData_syst = new TGraphErrors(npt,pt,cs,ept_syst,esyst_cs);
gData_syst->SetMarkerStyle(20);
gData_syst->SetMarkerColor(2);
gData_syst->SetMarkerSize(1.5);
gData_syst->SetMarkerSize(1.5);
gData_syst->SetFillStyle(0);
gData_syst->SetLineColor(2);


//---------------------------------
// interpolated values at 5TeV
//--------------------------------- 
 
const int ninterp=7;
Double_t epT_Interp[ninterp]        ={0.5,0.5,0.5,0.5,0.5,0.5,1.}; 
Double_t pT_Interp[ninterp]         ={0.5,1.5,2.5,3.5,4.5,5.5,7.}; 
Double_t CS_Interp[ninterp]         ={0.577,1.077,0.860,0.495,0.246,0.127,0.045};
Double_t StatSystUncInterp[ninterp] ={0.034,0.058,0.045,0.027,0.014,0.009,0.004};
Double_t SystCorInterp[ninterp]     ={0.030,0.056,0.045,0.026,0.013,0.007,0.002};
Double_t SystShapeInterp[ninterp]   ={0.015,0.027,0.025,0.012,0.000,0.001,0.002};
Double_t SystInterp[ninterp];
Double_t ErrTot_CS_Interp[ninterp];

for(int i=0;i<ninterp;i++){     
  SystInterp[i]       =TMath::Sqrt(StatSystUncInterp[i]*StatSystUncInterp[i]+SystShapeInterp[i]*SystShapeInterp[i]);
  ErrTot_CS_Interp[i] =TMath::Sqrt(SystInterp[i]*SystInterp[i]+SystCorInterp[i]*SystCorInterp[i]);
}

// Interpolation points
TGraphErrors *gInterp = new TGraphErrors(ninterp,pT_Interp,CS_Interp,epT_Interp,SystInterp);
gInterp->SetMarkerColor(kBlue);
gInterp->SetFillStyle(0);
gInterp->SetLineColor(kBlue);
gInterp->SetMarkerStyle(20);
gInterp->SetMarkerSize(1.5);

//---------------------------------------------------
// plot
//---------------------------------------------------

// Create canvas
gStyle->SetOptStat(0);
gStyle->SetOptTitle(0);
TCanvas *c = new TCanvas("c","c",20,20,800,800);
c->Divide(1,2);

// Plot Data and interpolated CC
c->cd(1);
gPad->SetLogy(1);
TH2D *hnull = new TH2D("hnull","hnull",100,0.,12.,100,0.001,2.5);
hnull->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
hnull->GetYaxis()->SetTitle("d#sigma/dy/d#it{p}_{T} (#mub/GeV/c)");
hnull->Draw();
gData->Draw("Psame");
gData_syst->Draw("E2Psame");
gInterp->Draw("E2Psame");

// Config. Legend
TLegend*leg0 = new TLegend(0.12,0.15,.4,.4,NULL,"brNDC");
leg0->SetBorderSize(0);
leg0->SetFillColor(10);
leg0->SetFillStyle(1);
leg0->SetLineStyle(0);
leg0->SetLineColor(0);
leg0->SetTextSize(0.06);
leg0->AddEntry(gInterp,"interpolation (+-5.2% global syst.)","P");
leg0->AddEntry(gData,"pp data @ 5TeV (+-2% global syst.)","P");
leg0->Draw();

// //----------------------------------------------
// // ratio
// //----------------------------------------------

Double_t Ratio_Data_Interp[ninterp];
Double_t ErrRatio_Data_Interp[ninterp];

// Compute Ratio points and Errors
for(int i=0;i<ninterp;i++){
  if(i!=6)
  {
    // Points
    Ratio_Data_Interp[i]    =cs[i]/CS_Interp[i];  
    //Error (full)
    ErrRatio_Data_Interp[i] =PropError(cs[i],CS_Interp[i],etotfull[i],SystInterp[i]);  
  } 
  
  // Average Error on the last point
  else if(i==6) 
  {
    Ratio_Data_Interp[i]       =((cs[6]+cs[7])/2.)/CS_Interp[i];  
    
    Double_t ErrSys_CS_Data_67 = TMath::Sqrt(etotfull[6]*etotfull[6]+etotfull[7]*etotfull[7])/2.;
    
    ErrRatio_Data_Interp[i]    =PropError(((cs[6]+cs[7])/2.),CS_Interp[i],ErrSys_CS_Data_67,SystInterp[i]);  
  }  
}

  TGraphErrors *gRatio_Data_Interp = new TGraphErrors(ninterp,pT_Interp,Ratio_Data_Interp,0,ErrRatio_Data_Interp);
  gRatio_Data_Interp->SetMarkerStyle(20);
  gRatio_Data_Interp->SetMarkerColor(kRed);
  gRatio_Data_Interp->SetMarkerSize(1.5);
  
  c->cd(2);
  TH2D *hnull2 = new TH2D("hnull","hnull2",100,0.,8.,100,0.7,1.3);
  hnull2->GetXaxis()->SetTitle("#it{p}_{T} (GeV/c)");
  hnull2->GetYaxis()->SetTitle("Ratio Data/Interpolation");
  hnull2->Draw();
  gRatio_Data_Interp->Draw("P");
  TLine*l = new TLine(0.,1.,8.,1.);
  l->SetLineColor(8);
  l->SetLineWidth(2);
  l->Draw();

  // Config. Legend
  leg = new TLegend(0.2772713,0.6872277,0.5569771,0.8642429,NULL,"brNDC");
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
  TBox *box_glob = new TBox(7.8.,1.-glob,8.,1+glob);
  box_glob->SetFillColor(kRed);
  box_glob->Draw("same");
}

Double_t PropError(Double_t x, Double_t y, Double_t ex, Double_t ey){
  Double_t error = TMath::Power(ex/x,2)+TMath::Power(ey/y,2);    
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