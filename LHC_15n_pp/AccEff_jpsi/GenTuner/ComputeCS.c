//__________Setting for spectra path
TString striggerDimuon  ="CMUL7-B-NOPF-MUFAST";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETAPDCA";
TString scentrality     ="V0A";
TString subResultName   = "";/*"YVSPT_BENJ_02.00_04.00_m03.50_m03.00";*/
//__________


void ComputeCS(const char* file,const char* file2,Double_t lum = 110.12 )
{
	TFile *f = TFile::Open(file,"READ");

	TObjArray* o = static_cast<TObjArray*>(f->FindObjectAny("Histograms;1"));

	TH1D *fhptacc = static_cast<TH1D*>(o->FindObject("hPtRecOverGen")->Clone());
	new TCanvas;
	fhptacc->DrawCopy("");
	
	TFile *f2 = TFile::Open(file2,"READ");


	// Get HistoCollection
	  AliMergeableCollection * oc = 0x0;
	  f2->GetObject("OC",oc);
	  if (!oc) return;

	   // Get spectras
	  AliAnalysisMuMuSpectra * spectraPT = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
	  if(!spectraPT)
	  {
	      cout << Form("Cannot find PT spectra in /%s/%s/%s/%s/PSI-PT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
	      return;
	  }
	

	  //___________Set ref.Spectra
	  // Pt spectra
	TH1* hpt= spectraPT->Plot("NofJPsi",subResultName,kTRUE);// new
	
	hpt->Divide(fhptacc);

	hpt->Scale(1./((5.93/100)*lum*1000*1.5)); // Lum in nm^-1
	hpt->SetTitle("J/#psi cross section");
	hpt->GetYaxis()->SetTitle("d#sigma / dpt (mb/Gev)");
	hpt->GetYaxis()->SetTitle("d#sigma / dpt (mb/Gev)");
	hpt->Print("range");
	// hpt->GetXaxis()->SetTitle("pt");
	new TCanvas;
	hpt->DrawCopy("");	
	return;


		  f->Close();
		  	f2->Close();


	
}