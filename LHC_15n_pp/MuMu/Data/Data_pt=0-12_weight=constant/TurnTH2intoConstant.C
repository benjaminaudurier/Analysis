#include "TH2.h"
#include "TFile.h"
#include "TCanvas.h"


void TurnTH2intoConstant()
{
	TFile *f = TFile::Open("AccEffCorr.root","UPDATE");
	TH2* h(0x0);

	f->GetObject("ANY-ALL-pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA-PP",h);

	Int_t nBinsX  = h->GetNbinsX();
  	Int_t nBinsY  = h->GetNbinsY();

	Int_t g =0;
	for (int j = 0; j < nBinsX; ++j)
	{
		for (int k = 0; k < nBinsY; ++k)
		{
			Int_t bin = h->GetBin(j+1,k+1);

			h->SetBinContent(bin,0.1);
			h->SetBinError(bin,0.000001);
			++g;
		}
	}
	new TCanvas;
	h->DrawCopy("colz");
	h->Write();

	f->Close();
}