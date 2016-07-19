#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TPaveText.h"
#include "TF1.h"
#include "TList.h"
#include "TVirtualPad.h"
#include "TPaveStats.h"
#include "AliAnalysisMuMu.h"
#include "AliMergeableCollection.h"
#endif



TH1* GetHistoInCanvas(TVirtualPad*, TString );
Double_t FitFuncErfFixed ( Double_t*, Double_t* );
Double_t FitFuncErfFixedPlus ( Double_t*, Double_t* );
Double_t FitFuncErf ( Double_t*, Double_t* );
Double_t FitFuncInt ( Double_t*, Double_t* );
void fitHptLpt ( TString filename, TString histoName,TF1* fitFunc );

/// Histo name from Astrid's file : hDataIntegrated, 
//
//

//_______________________________________________________________________
void ExtractRelativeParamError(/*TString file, TString histo */ )
{

	// Preparing function
	TF1* fitFuncData13 = new TF1("fitFuncData13",FitFuncErfFixedPlus,0.,10.,8);
	fitFuncData13->SetParameters(0.5, 1., 0.3, 1., 0.2, 0.1, 0.35,1.);
	fitFuncData13->FixParameter(0,4.77114e-01);
	fitFuncData13->FixParameter(7,1.);
	fitHptLpt("TriggerTurnOnCurves13TeVData.root","hDataIntegrated",fitFuncData13);

	TF1* fitFuncMC13 = new TF1("fitFuncMC13",FitFuncErfFixedPlus,0.,10.,8);
	fitFuncMC13->SetParameters(0.5, 1., 0.3, 1., 0.2, 0.1, 0.35,1.);
	fitFuncMC13->FixParameter(0,fitFuncData13->GetParameter(0));
	fitFuncMC13->FixParameter(7,1.);
	fitHptLpt("TriggerTurnOnCurves13TeVData.root","hMCIntegrated",fitFuncMC13);

	TF1* fitFuncMC5 = new TF1("fitFuncMC5",FitFuncErfFixedPlus,0.,10.,8);
	fitFuncMC5->SetParameters(0.5, 1., 0.3, 1., 1., 1., 0.35,1.);
	// fitFuncMC5->FixParameter(0,0.5);
	fitFuncMC5->FixParameter(3,1.);
	fitFuncMC5->FixParameter(4,1.);
	fitFuncMC5->FixParameter(5,1.);
	fitFuncMC5->FixParameter(6,-100);
	// fitFuncMC5->FixParameter(7,1);
	fitHptLpt("AnalysisResults.JPSI.root","",fitFuncMC5);

	TF1* fitFuncMC5Corr = new TF1("fitFuncMC5Corr",FitFuncErfFixedPlus,0.,10.,8);
	fitFuncMC5Corr->SetParameters(0.5, 1., 0.3, 1., 1., 1., 0.35,1.);
	// return;

	//Param from Data 13 TeV
	Double_t meanData13     = fitFuncData13->GetParameter(1); 
	// Double_t errmeanData13  = fitFuncData13->GetParError(1);
	
	Double_t sigmaData13    = fitFuncData13->GetParameter(2); 
	// Double_t errsigmaData13 = fitFuncData13->GetParError(2);

	//Param from MC 13 TeV
	Double_t meanMC13       = fitFuncMC13->GetParameter(1); 
	// Double_t errmeanMC13    = fitFuncMC13->GetParError(1);
	
	Double_t sigmaMC13    = fitFuncMC13->GetParameter(2); 
	// Double_t errsigmaMC13 = fitFuncMC13->GetParError(2);

	//Param from MC 5 TeV
	Double_t meanMC5       = fitFuncMC5->GetParameter(1); 
	// Double_t errmeanMC5    = fitFuncMC5->GetParError(1);
	
	Double_t sigmaMC5    = fitFuncMC5->GetParameter(2); 
	// Double_t errsigmaMC5 = fitFuncMC5->GetParError(2); 

	printf("\n");
	printf("mean pt error for pp@5TeV        : %f  \n",meanMC5 );
	printf("sigma pt error for pp@5TeV       : %f  \n",sigmaMC5 );
	printf("\n");
	printf("Delta_mean pt error for pp@13TeV : %f %% \n",100*( meanData13-meanMC13)/meanMC13 );
	printf("Delta_sigma pt error for pp@13TeV: %f %% \n",100*( sigmaData13-sigmaMC13)/sigmaMC13 );
	printf("\n");
	printf("new mean pt error for pp@5TeV    : %f  \n", meanMC5*(1. + (meanData13-meanMC13)/meanMC13) );
	printf("new sigma pt error for pp@5TeV    : %f  \n",sigmaMC5*( 1. + (sigmaData13-sigmaMC13)/sigmaMC13) );
	
	fitFuncMC5Corr->SetParameters(fitFuncMC5->GetParameters());
	fitFuncMC5Corr->SetParameter(1,meanMC5*(1. + (meanData13-meanMC13)/meanMC13));
	fitFuncMC5Corr->SetParameter(2,sigmaMC5*( 1. + (sigmaData13-sigmaMC13)/sigmaMC13));
	fitFuncMC5Corr->SetLineColor(kBlue);
	fitFuncData13->SetLineColor(kBlue);
	
	new TCanvas;
	TLegend * leg = new TLegend(0.4,0.7,0.70,0.9);
    leg->SetHeader(Form("Weight functions for pp@5TeV"));
    leg->AddEntry(fitFuncMC5,"lpt/allpt MC ","l");
    leg->AddEntry(fitFuncMC5Corr,"lpt/allpt Data","l");
	fitFuncMC5->Draw();
	fitFuncMC5Corr->Draw("same");
	leg->Draw("same");

	new TCanvas;
	TLegend * leg2 = new TLegend(0.4,0.7,0.70,0.9);
    leg2->SetHeader(Form("Weight functions for pp@13TeV"));
    leg2->AddEntry(fitFuncMC13,"lpt/allpt MC ","l");
    leg2->AddEntry(fitFuncData13,"lpt/allpt Data","l");
	fitFuncMC13->Draw();
	fitFuncData13->Draw("same");
	leg2->Draw("same");


	return;

}

//_______________________________________________________________________
void fitHptLpt ( TString filename, TString histoName, TF1* fitFunc  )
{
	//Open file
	TFile* file = 0x0;
	AliAnalysisMuMu* a=0x0;
	
	// Select how to open files depending on the name
	if( ! filename.Contains("JPSI") ) {
		file = TFile::Open(filename.Data());
		if ( ! file ) {
			printf("Fatal: cannot find %s\n", filename.Data());
			return;
		}
	} 
	else {
		a =new AliAnalysisMuMu(filename,"","","mumu.pp2015.simu.config");
		if ( ! a ) {
			printf("Fatal: cannot find %s\n", filename.Data());
			return;
		}
	}

	Bool_t isInCanvas = kFALSE;
	TString canName = "";
	// If we want to fit more than on histo
	if ( histoName.Contains(":") ) {
		isInCanvas = kTRUE;
		TObjArray  * auxNames = histoName.Tokenize(":");
		histoName  = auxNames->At(1)->GetName();
		canName    = auxNames->At(0)->GetName();
		delete auxNames;
	}
	
	TH1* histo = 0x0;
	if ( isInCanvas ) {
		TVirtualPad* pad = dynamic_cast<TVirtualPad*>(file->FindObjectAny(canName.Data()));
		histo = GetHistoInCanvas(pad, histoName);
	} 
	else if (! filename.Contains("JPSI") ) {
    	histo = dynamic_cast<TH1*>(file->FindObjectAny(histoName.Data())); 
  	}
	else if (! filename.Contains("W1") ){
		histo = dynamic_cast<TH1*>(a->OC()->H2("/ALL/ANY/PP/sALLMATCHLOWRABSETAPDCA/PtRapidityMu")->ProjectionY("_py",0,-1,"e")->Clone());  
		if( ! histo) {
			printf("Fatal: cannot access to histo /ALL/ANY/PP/sALLMATCHLOWRABSETAPDCA/PtRapidityMu\n");
			return;
		}  
		TH1* histoAll = dynamic_cast<TH1*>(a->OC()->H2("/ALL/ANY/PP/sALLRABSETAPDCA/PtRapidityMu")->ProjectionY("_py",0,-1,"e")->Clone());
		if( ! histoAll) {
			printf("Fatal: cannot access to histo /ALL/ANY/PP/sALLABSETAPDCA/PtRapidityMu\n");
			return;
		} 
		histo->Divide(histoAll);  
	}
	else {
		histo = dynamic_cast<TH1*>(a->OC()->H2("/ALL/ANY/PP/sALL/PtRapidityMu")->ProjectionY("_py",0,-1,"e")->Clone());  
		if( ! histo) {
			printf("Fatal: cannot access to histo /ALL/ANY/PP/sALL/PtRapidityMu\n");
			return;
		}  
		TH1* histoAll = dynamic_cast<TH1*>(a->OC()->H2("/ALL/ANY/PP/sALL/PtRapidityMu")->ProjectionY("_py",0,-1,"e")->Clone());
		if( ! histoAll) {
			printf("Fatal: cannot access to histo /ALL/ANY/PP/sALL/PtRapidityMu\n");
			return;
		} 
		histo->Divide(histoAll);  
	}
	
	if ( ! histo ) {
		printf("Fatal: histo %s not found in file %s\n", histoName.Data(), filename.Data());
		return;
	}

	new TCanvas();
	
	// fitFunc->FixParameter(6,3.66659e-01);

	//histo->GetListOfFunctions()->Clear();
	histo->Fit(fitFunc,"RM");
	histo->Draw("e");

}

//_______________________________________________________________________
TH1* GetHistoInCanvas(TVirtualPad* pad, TString histoName)
{
  if ( ! pad ) return 0x0;
  TList* list = pad->GetListOfPrimitives();
  TH1* histo = (TH1*)list->FindObject(histoName.Data());
  if ( histo ) return histo;
  
  for ( Int_t ientry=0; ientry<list->GetEntries(); ientry++ ) {
    TVirtualPad* subPad = dynamic_cast<TVirtualPad*>(list->At(ientry));
    if ( subPad ) {
      histo = GetHistoInCanvas(subPad, histoName);
      if ( histo ) break;
    }
  }
  
  return histo;
}

//______________________________________________
Double_t FitFuncErfFixed ( Double_t* xVal, Double_t* par )
{
	Double_t xx             = xVal[0];
	Double_t currX          = TMath::Max(xx,par[6]);
	Double_t sqrtTwo        = TMath::Sqrt(2.);
	Double_t yVal           = 1.+par[0]*(TMath::Erf((currX-par[1])/par[2]/sqrtTwo)-1.);
	if ( xx < par[6] ) yVal += par[3]*(TMath::Erf((-xx-par[4])/par[5]/sqrtTwo) - TMath::Erf((-par[6]-par[4])/par[5]/sqrtTwo));
	
	return yVal;
}

//______________________________________________
Double_t FitFuncErfFixedPlus ( Double_t* xVal, Double_t* par )
{
	Double_t xx             = xVal[0];
	Double_t currX          = TMath::Max(xx,par[6]);
	Double_t sqrtTwo        = TMath::Sqrt(2.);
	Double_t yVal           = par[7]+par[0]*(TMath::Erf((currX-par[1])/par[2]/sqrtTwo)-1.);
	if ( xx < par[6] ) yVal += par[3]*(TMath::Erf((-xx-par[4])/par[5]/sqrtTwo) - TMath::Erf((-par[6]-par[4])/par[5]/sqrtTwo));
	
	return yVal;
}


//______________________________________________
Double_t FitFuncErf ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  Double_t yVal = 0.;
  if ( xx > par[8] )
    yVal = par[3]+par[0]*(1.+TMath::Erf((xx-par[1])/par[2]/TMath::Sqrt(2.)));
  else
    yVal = par[7]+par[4]*(1.+TMath::Erf((-xx-par[5])/par[6]/TMath::Sqrt(2.)));
  
  return yVal;
}

//______________________________________________
Double_t FitFuncInt ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  
  Double_t yVal = 0.;
  Double_t sigma = par[2] + par[4] * ( xx - par[1] ) / par[2];
  yVal = par[3]+par[0]*(1.+TMath::Erf((xx-par[1])/sigma/TMath::Sqrt(2.)));  
  return yVal;
}