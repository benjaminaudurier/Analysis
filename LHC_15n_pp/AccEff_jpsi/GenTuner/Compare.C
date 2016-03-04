/*
 *  Compare.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 22/03/13.
 *  Copyright 2013 SUBATECH. All rights reserved.
 *
 */

//General note : x variable = P_t and p[] variables = parameter array.

//________________________________________________________________________
Double_t Pt(const Double_t *x, const Double_t *p)
{
  /// generated pT fit function
  
  Double_t pT = *x;
  Double_t arg = 0;
  
  arg = p[0]*x[0] / TMath::Power( 1 + p[1]*pT*pT, p[2]);
  
  return arg;
}

//________________________________________________________________________
Double_t Y(const Double_t *x, const Double_t *p)
{
  /// generated y fit function
  Double_t y = *x;
  Double_t arg1 = 0;
    
  arg1 = p[0] * ( 1 + p[1]*y );
  
  
  return arg1;
}

//______________________________________________________________________________
Double_t PtRat(const Double_t *x, const Double_t *p)
{
  /// generated pT fit function ratio
  const Double_t *p1 = &(p[0]);
  const Double_t *p2 = &(p[3]);
  return Pt(x,p1) / Pt(x,p2);
}

//______________________________________________________________________________
Double_t YRat(const Double_t *x, const Double_t *p)
{
  /// generated y fit function ratio
  const Double_t *p1 = &(p[0]);
  const Double_t *p2 = &(p[2]);
  return Y(x,p1) / Y(x,p2);
}

//______________________________________________________________________________
void Compare(TString sfile1, TString sfile2)
{
  /// compare generated histograms and functions
  
  TString sfile[2];
  sfile[0] = sfile1.Data();// Set file 1
  sfile[1] = sfile2.Data(); // Set file 2

  //__________Configure first set of parameter
  TString sRes[4] = {"hPtGen", "hYGen", /*"hPhiGen",*/ "hPtRec", "hYRec"/* "hPhiRec"*/};
  TH1 *hRes[4][3];
  for (Int_t i = 0; i < 4; i++) for (Int_t j = 0; j < 3; j++) hRes[i][j] = 0x0;
  
  TString sfunc[2][2] = {"fPtFuncMC", "fPtFuncMC", "fYFuncMC", "fYFuncMC"};
  void *func[4] = {Pt, Y, PtRat, YRat};
  Int_t nPar[4] = {6, 8, 12, 16};
  TF1 *fRes[2][3];
  for (Int_t i = 0; i < 2; i++) for (Int_t j = 0; j < 3; j++) fRes[i][j] = 0x0;
  //__________


  //============================
  //get results
  //============================

  //__________Loop on files 
  for (Int_t j = 0; j < 2; j++) 
  {
    // Open file
    TFile *file = TFile::Open(sfile[j].Data(),"READ");
    if (!file || !file->IsOpen()) 
    {
      ::Error("cannot open file");
      return;
    }
    
    if (file && file->IsOpen()) 
    { 
      //__________Get histo
      TObjArray *list = static_cast<TObjArray*>(file->FindObjectAny("Histograms")); // Store in array
      if (!list) 
      {
	     ::Error("cannot find histograms");
	     return;
      }
      
      for (Int_t i = 0; i < 6; i++) 
      {
        // Clone histo in hRes
        hRes[i][j] = static_cast<TH1*>(list->FindObject(sRes[i].Data())->Clone(Form("%s%d",sRes[i].Data(),j+1)));
        if (hRes[i][j]) hRes[i][j]->SetDirectory(0);
      }
      //__________

      //__________Get function
      for (Int_t i = 0; i < 2; i++) 
      {
	      TF1 *f = static_cast<TF1*>(file->FindObjectAny(sfunc[i][j].Data()));
      	
        if (f) 
        {
      	  fRes[i][j] = new TF1(Form("%s%d",sfunc[i][j].Data(),j+1), func[i], f->GetXmin(), f->GetXmax(), nPar[i]);
      	  fRes[i][j]->SetParameters(f->GetParameters());
      	}
      }
      //__________
    }

    file->Close();
  }
  //__________
  

  //__________normalize histograms
  for (Int_t i = 0; i < 6 && hRes[i][0] && hRes[i][1]; i++) 
  {
    for (Int_t j = 0; j < 2; j++) 
    {
      Double_t integral;
    
      if (i == 0) integral = hRes[i][j]->Integral(hRes[i][j]->FindBin(0.8), hRes[i][j]->FindBin(30.), "width");
    
      else if (i == 3) integral = hRes[i][j]->Integral(hRes[i][j]->FindBin(4.), hRes[i][j]->FindBin(30.), "width");
    
      else integral = hRes[i][j]->Integral("width");
      
      Double_t norm = (integral != 0.) ? 1./integral : 1.;
      hRes[i][j]->Scale(norm);
    }
  }
  //__________
  
  //__________compute ratios
  for (Int_t i = 0; i < 6 && hRes[i][0] && hRes[i][1]; i++) 
  {
    hRes[i][2] = static_cast<TH1*>(hRes[i][1]->Clone(Form("%sOver1",hRes[i][1]->GetName())));
    hRes[i][2]->Divide(hRes[i][0]);
  }
  for (Int_t i = 0; i < 2 && fRes[i][0] && fRes[i][1]; i++) 
  {
    fRes[i][2] = new TF1(Form("%sOver1",fRes[i][1]->GetName()), func[i+2], fRes[i][1]->GetXmin(), fRes[i][1]->GetXmax(), nPar[i+2]);
    Double_t *p = new Double_t[nPar[i+2]];
    fRes[i][1]->GetParameters(p);
    fRes[i][0]->GetParameters(&(p[nPar[i]]));
    fRes[i][2]->SetParameters(p);
    delete[] p;
  }
  //__________
  
  //__________print results
  TString var[2] = {"pT", "y"};
  for (Int_t i = 0; i < 2; i++) 
  {
    for (Int_t j = 0; j < 2 && fRes[i][j]; j++) 
    {
      Double_t *param = fRes[i][j]->GetParameters();
      printf("\n%s parameters for single muon generator in file %d:\n", var[i].Data(), j+1);
      printf("Double_t p[%d] = {", nPar[i]);
      for (Int_t k = 0; k < nPar[i]-1; k++) printf("%g, ", param[k]);
      printf("%g};\n", param[nPar[i]-1]);
    }
  }
  //__________
  
  //__________display results at the generation level
  TCanvas *cGen = new TCanvas("cGen", "cGen", 1200, 800);
  cGen->Divide(3,2);
  for (Int_t i = 0; i < 3; i++) 
  {
    cGen->cd(i+1);
    if (i == 0) gPad->SetLogy();
    for (Int_t j = 0; j < 2 && hRes[i][j]; j++) 
    {
      hRes[i][j]->SetLineColor(2*j+2);
      hRes[i][j]->Draw((j == 0) ? "" : "sames");
      if (i < 2 && fRes[i][j]) 
      {
        fRes[i][j]->SetLineColor(2*j+2);
        fRes[i][j]->Draw("sames");
      }
    }
    cGen->cd(i+4);
    if (hRes[i][2]) 
    {
      hRes[i][2]->Draw();
      hRes[i][2]->SetMinimum(0.8);
      hRes[i][2]->SetMaximum(1.2);
    }
    if (i < 2 && fRes[i][2]) fRes[i][2]->Draw("sames");
  }
  
  //__________display results at the reconstruction level
  TCanvas *cRec = new TCanvas("cRec", "cRec", 1200, 800);
  cRec->Divide(3,2);
  cRec->cd(1);
  for (Int_t i = 3; i < 6; i++) 
  {
    cRec->cd(i-2);
    if (i == 3) gPad->SetLogy();
    for (Int_t j = 0; j < 2 && hRes[i][j]; j++) 
    {
      hRes[i][j]->SetLineColor(2*j+2);
      hRes[i][j]->Draw((j == 0) ? "" : "sames");
    }
    cRec->cd(i+1);
    if (hRes[i][2]) 
    {
      hRes[i][2]->Draw();
      hRes[i][2]->SetMinimum(0.8);
      hRes[i][2]->SetMaximum(1.2);
    }
  }
  //__________
  
}

