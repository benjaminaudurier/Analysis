
///
/// Macro to create new AccEff maps according to bin fluctuation
/// Created by Benjamin Audurier November 4th 2016
///

#include <TH1.h>
#include <TH2.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TSystem.h>
#include <TMath.h>
#include <TRandom1.h>

// number of generated maps
const Int_t nMaps = 16;

// Name of the 2D AccEff Maps
const char* nameOfReferenceMap = "ANY-ALL-pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA-PP";

// gaussian variance (here tracking (1%) + matching (1%) + trigger (1.8%) )
const Double_t sigma = (2.3)/100.;

//_________________________________________________________________________________________
void CreateAccEffMaps(const char* file = "Acceff_reference.root")
{
	// The new maps
	TH2* hNewMaps[nMaps];
	for (int i = 0; i < nMaps; ++i) hNewMaps[i] =0x0;

	// Open File
	TFile* f = TFile::Open(file,"READ");
	if(!f){
		printf("Cannot open file %s\n",file );
		return;
	}

	// Get the reference map
	TH2* AccEffRefMap =0x0;
  	f->GetObject(nameOfReferenceMap,AccEffRefMap);
  	if(!AccEffRefMap){
    	printf("Cannot get AccEff Histogram\n");
    	return;
  	}

  	new TCanvas;
  	AccEffRefMap->DrawCopy("COLZ");

  	// Clone it
  	for (int i = 0; i < nMaps; ++i){
  		hNewMaps[i] = static_cast<TH2*>(AccEffRefMap->Clone());
  		hNewMaps[i]->SetTitle(Form("%s with random fluctuation (n_%d)",AccEffRefMap->GetTitle(),i));
  	}

  	TCanvas* c = new TCanvas();
    c->Divide(2,nMaps/2);
    c->Draw();

  	gRandom->SetSeed(0);

  	// From here we let each points fluctuates
  	for (int i = 0; i < nMaps; ++i)
  	{
  		Int_t nBinsX  = hNewMaps[i]->GetNbinsX();
  		Int_t nBinsY  = hNewMaps[i]->GetNbinsY();

  		// Compute the fluctuation
    	Double_t fluctuations[nBinsX*nBinsY];
    	for (int l = 0; l < nBinsX*nBinsY; ++l) fluctuations[l] = gRandom->Gaus(1,sigma);

  		Int_t g =0;
  		for (int j = 0; j < nBinsX; ++j)
  		{
  			for (int k = 0; k < nBinsY; ++k)
  			{
  				Int_t bin = hNewMaps[i]->GetBin(j+1,k+1);

  				Double_t binContent      = hNewMaps[i]->GetBinContent(bin);
  				Double_t binContentError = hNewMaps[i]->GetBinError(bin);
  				Double_t relatError      = binContentError/binContent;

  				hNewMaps[i]->SetBinContent(bin,binContent*fluctuations[g]);
  				hNewMaps[i]->SetBinError(bin,binContent*fluctuations[g]*relatError);

  				++g;
  			}
  		}

  		c->cd(i+1);
  		hNewMaps[i]->DrawCopy("COLZ");
  	}

  	// Save results
  	for (int i = 0; i < nMaps; ++i)
  	{
  		TFile g(Form("Acceff_Corr_%d.root",i),"RECREATE");
  		hNewMaps[i]->Write();
  		g.Close();
  	}
  	gSystem->Exec("mv Acceff_Corr_* Acceff_Corr");
  	f->Close();



}