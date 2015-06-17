/*
 *  runGenTunerLoop.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 08/03/13.
 *  Copyright 2013 SUBATECH. All rights reserved.
 *
 */

// Sim :
// Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root
// 
// Data:
// Find;BasePath=/alice/data/2013/LHC13d/000195760/ESDs/muon_pass2/AOD134;FileName=AliAOD.root

//______________________________________________________________________________
void runGenTunerLoop(TString smode = "saf", TString inputFileName = "Find;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/;FileName=AliAOD.Muons.root", Int_t nStep)
{
  /// run the generator tuner in a loop

  if (nStep <= 0) return;
  
  // prepare trending plots
  TGraphErrors *gOldPtParam[3];
  TGraphErrors *gOldPtParamMC[3];
  TGraphErrors *gNewPtParam[3];

  // Fill graph with 0
  for (Int_t i = 0; i < 3; i++) 
  {
    gOldPtParam[i] = new TGraphErrors(nStep);
    gOldPtParam[i]->SetNameTitle(Form("gOldPtParam%d",i), Form("p%d",i));
    gOldPtParamMC[i] = new TGraphErrors(nStep);
    gOldPtParamMC[i]->SetNameTitle(Form("gOldPtParamMC%d",i), Form("p%d",i));
    gNewPtParam[i] = new TGraphErrors(nStep);
    gNewPtParam[i]->SetNameTitle(Form("gNewPtParam%d",i), Form("p%d",i));
  }
  TGraphErrors *gOldYParam[2];
  TGraphErrors *gOldYParamMC[2];
  TGraphErrors *gNewYParam[2];
  for (Int_t i = 0; i < 2; i++) 
  {
    gOldYParam[i] = new TGraphErrors(nStep);
    gOldYParam[i]->SetNameTitle(Form("gOldYParam%d",i), Form("p%d",i));
    gOldYParamMC[i] = new TGraphErrors(nStep);
    gOldYParamMC[i]->SetNameTitle(Form("gOldYParamMC%d",i), Form("p%d",i));
    gNewYParam[i] = new TGraphErrors(nStep);
    gNewYParam[i]->SetNameTitle(Form("gNewYParam%d",i), Form("p%d",i));
  }
  
  TString resume = "";

  // Main loop 
  for (Int_t i = 0; i < nStep; i++) 
  {
    cout << ""<< endl;
    cout << "+++++++++++++++++++++++++" << endl;
    cout << "Loop number " << i << endl;
    cout << "+++++++++++++++++++++++++" << endl;
    cout << ""<< endl;

    // resume or not
    TString inFileName = Form("Results_step%d.root",i);
    if (resume != "n") {
      if (!gSystem->AccessPathName(inFileName.Data())) 
      {
      	if (resume != "y") 
        {
      	  cout<<"Results already exist. Do you want to resume? [y/n] (if not previous results will be deleted) "<<flush;
      	  do {resume.Gets(stdin,kTRUE);} while (resume != "y" && resume != "n");
      	  if (resume == "n") gSystem->Exec("rm -f Results_step*.root");
      	}
      } else resume = "n";
    }
    
    // run the generator tuner
    if (resume != "y") 
    {
      if (i == 0)gSystem->Exec(Form("root -b -q runGenTuner.C\\(\\\"%s\\\",\\\"Find\\;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/\\;FileName=AliAOD.Muons.root\\\",%d\\)",smode.Data(),i));
      else gSystem->Exec(Form("root -b -q runGenTuner.C\\(\\\"%s\\\",\\\"Find\\;BasePath=/alice/cern.ch/user/l/laphecet/Analysis/LHC13d/simjpsi/CynthiaTuneWithRejectList/195760/\\;FileName=AliAOD.Muons.root\\\",%d,\\\'k\\\'\\)",smode.Data(),i));
      // if (i == 0)gSystem->Exec(Form("root -b -q runGenTuner.C\\(\\\"%s\\\",\\\"AliAOD.root\\\",%d,\\\'k\\\'\\)",smode.Data(),i));
      // else gSystem->Exec(Form("root -b -q runGenTuner.C\\(\\\"%s\\\",\\\"AliAOD.root\\\",%d,\\\'k\\\'\\)",smode.Data(),i));
    }
    
    // get the new generator parameters and fill trending plots
    TFile *inFile = TFile::Open(inFileName.Data(),"READ");
    if (inFile && inFile->IsOpen()) 
    {
      cout << "files "<< inFileName.Data() << " opened " <<endl;

      // Get pt fitting functions
      TF1*fOldPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFunc"));
      TF1*fOldPtFuncMC = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncMC"));
      TF1*fNewPtFunc = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncNew"));
      
      // Loop over pt function param. and store them in corresponding graphics.
      for (Int_t j = 0; j < 3; j++) 
      {
        if (fOldPtFunc) 
        {
          gOldPtParam[j]->SetPoint(i, i, fOldPtFunc->GetParameter(j));
          //gOldPtParam[j]->SetPointError(i, 0., fOldPtFunc->GetParError(j));
        }
        if (fOldPtFuncMC) 
        {
          gOldPtParamMC[j]->SetPoint(i, i, fOldPtFuncMC->GetParameter(j));
          //gOldPtParamMC[j]->SetPointError(i, 0., fOldPtFuncMC->GetParError(j));
        }
        if (fNewPtFunc) 
        {
          gNewPtParam[j]->SetPoint(i, i+1, fNewPtFunc->GetParameter(j));
          //gNewPtParam[j]->SetPointError(i, 0., fNewPtFunc->GetParError(j));
        }
      }

      // Get y fitting functions
      TF1*fOldYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFunc"));
      TF1*fOldYFuncMC = static_cast<TF1*>(inFile->FindObjectAny("fYFuncMC"));
      TF1*fNewYFunc = static_cast<TF1*>(inFile->FindObjectAny("fYFuncNew"));

      // Loop over y function param. and store them in corresponding graphics.
      for (Int_t j = 0; j < 2; j++) 
      {
        if (fOldYFunc) 
        {
          gOldYParam[j]->SetPoint(i, i, fOldYFunc->GetParameter(j));
          //gOldYParam[j]->SetPointError(i, 0., fOldYFunc->GetParError(j));
        }
        if (fOldYFuncMC) 
        {
          gOldYParamMC[j]->SetPoint(i, i, fOldYFuncMC->GetParameter(j));
          //gOldYParamMC[j]->SetPointError(i, 0., fOldYFuncMC->GetParError(j));
        }
        if (fNewYFunc) 
        {
          gNewYParam[j]->SetPoint(i, i+1, fNewYFunc->GetParameter(j));
          //gNewYParam[j]->SetPointError(i, 0., fNewYFunc->GetParError(j));
        }
      }
      inFile->Close();
    }
    
  }
  
// display trending plots
  TCanvas *cPtParams = new TCanvas("cPtParams", "cPtParams", 600, 400);
  cPtParams->Divide(3);
  for (Int_t i = 0; i < 3; i++) 
  {
    cPtParams->cd(i+1);
    gOldPtParam[i]->SetMarkerStyle(kFullDotMedium);
    gOldPtParam[i]->SetMarkerColor(4);// blue
    gOldPtParam[i]->SetLineColor(4);// blue
    gOldPtParam[i]->Draw("ap");
    gOldPtParam[i]->GetXaxis()->SetLimits(-1., nStep+1.);
    gOldPtParamMC[i]->SetMarkerStyle(kFullDotMedium);
    gOldPtParamMC[i]->SetMarkerColor(3);// Green
    gOldPtParamMC[i]->SetLineColor(3);// Green
    gOldPtParamMC[i]->Draw("p");
    gNewPtParam[i]->SetMarkerStyle(kFullDotMedium);
    gNewPtParam[i]->SetMarkerColor(2); // red
    gNewPtParam[i]->SetLineColor(2); //red 
    gNewPtParam[i]->Draw("p");
  }
  TCanvas *cYParams = new TCanvas("cYParams", "cYParams", 800, 400);
  cYParams->Divide(2);
  for (Int_t i = 0; i < 2; i++) 
  {
    cYParams->cd(i+1);
    gOldYParam[i]->SetMarkerStyle(kFullDotMedium);
    gOldYParam[i]->SetMarkerColor(4);// blue
    gOldYParam[i]->SetLineColor(4);// blue
    gOldYParam[i]->Draw("ap");
    gOldYParam[i]->GetXaxis()->SetLimits(-1., nStep+1.);
    gOldYParamMC[i]->SetMarkerStyle(kFullDotMedium);
    gOldYParamMC[i]->SetMarkerColor(3);// Green
    gOldYParamMC[i]->SetLineColor(3);// Green
    gOldYParamMC[i]->Draw("p");
    gNewYParam[i]->SetMarkerStyle(kFullDotMedium);
    gNewYParam[i]->SetMarkerColor(2);// Red
    gNewYParam[i]->SetLineColor(2);//Red
    gNewYParam[i]->Draw("p");
  }
  
  // print and plot last results and save trending plots
  TString inFileName = Form("Results_step%d.root",nStep-1);// Previous step
  TFile *inFile = TFile::Open(inFileName.Data(),"UPDATE");
  if (inFile && inFile->IsOpen()) 
  {
    TF1 *fPtFuncMC = static_cast<TF1*>(inFile->FindObjectAny("fPtFuncMC"));
    TF1 *fYFuncMC = static_cast<TF1*>(inFile->FindObjectAny("fYFuncMC"));
    if (fPtFuncMC && fYFuncMC) 
    {
      Double_t *param = fPtFuncMC->GetParameters();
      printf("\npT parameters for single muon generator:\n");
      printf("Double_t p[3] = {%g, %g, %g};\n",
	     param[0], param[1], param[2]);
      param = fYFuncMC->GetParameters();
      printf("\ny parameters for single muon generator:\n");
      printf("Double_t p[2] = {%g, %g};\n\n",
	     param[0], param[1]);
    }
    TCanvas *cRes = static_cast<TCanvas*>(inFile->FindObjectAny("cRes"));
    if (cRes) cRes->DrawClone();
    TCanvas *cRat = static_cast<TCanvas*>(inFile->FindObjectAny("cRat"));
    if (cRat) cRat->DrawClone();
    cPtParams->Write(0x0, TObject::kOverwrite);
    cYParams->Write(0x0, TObject::kOverwrite);
  }
  inFile->Close();
  
}

