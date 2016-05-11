// these lines should be in the main() body of your macro, so that you define and initialise just once the TF1 object
TF1 * f = new TF1("",rTVX,0.,4.,2);
Double_t ratioA = 0.438;
Double_t ratioC = 0.381;
f->SetParameter(0,ratioA);
f->SetParameter(1,ratioC);


// this function should be called once per rate measurement, to get the corrected mu value. It takes as argument the total T0 counts divided by the LHC frequency 
// note, this macro also works for V0 if une uses ratioA = 0.079, ratioC = 0.068
Double_t correctT0rate(Double_t ratePerBC){

Float_t true = trova(ratePerBC, f);

return true;
}


// this function defines the relationship between ratePerBC and mu
Double_t * rTVX(Double_t * x, Double_t * par){

//printf("using %f and %f\n",par[0],par[1]);

Double_t eMinMu = TMath::Exp(-x[0]);
Double_t eMinMuA = TMath::Exp(-x[0]*par[0]);
Double_t eMinMuC = TMath::Exp(-x[0]*par[1]);

return (1-eMinMu + eMinMu * (1-eMinMuA) * (1-eMinMuC));
}


// this function inverts numerically the function fun
Double_t trova(Double_t y, TF1* fun)
{
  //
  Double_t xmin=fun->GetXmin();
  Double_t xmax=fun->GetXmax();
  Double_t xmed,val;
  for(Int_t i=0; i<100000; i++){
    xmed=(xmax+xmin)/2.;
    val=fun->Eval(xmed);
    if(val>=y)
      xmax=xmed;
    else
      xmin=xmed;
  }
  xmed=(xmax+xmin)/2.;
  return xmed;
}
