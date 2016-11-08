void PlotAcceff()
{

  const int ndata=9;
  Double_t Centr[ndata]={5.,15.,25.,35.,45.,55.,65.,75.,85.};
  Double_t Acc[ndata]={0.1297,0.1360,0.1411,0.1436,0.1445,0.1455,0.1472,0.1472,0.1472};
  Double_t ErrAcc[ndata]={0.00040,0.0005,0.0005,0.0005,0.0005,0.0005,0.0005,0.0005,0.0003}; 

 TGraphErrors *AccEff = new
TGraphErrors(ndata,Centr,Acc,0,ErrAcc);
AccEff->SetMarkerColor(kBlue+1);
AccEff->SetMarkerStyle(20);
AccEff->SetMarkerSize(1.5);	

AccEff->Draw("AP");

}