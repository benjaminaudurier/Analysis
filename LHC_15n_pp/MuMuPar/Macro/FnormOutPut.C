#include <TH1.h>
#include <TGraphErrors.h>
#include <TString.h>
#include <TFile.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TCanvas.h>
#include <Riostream.h>
#include <TLegend.h>
#include <AliAnalysisMuMu.h>

std::set<int> RunNumbers(AliAnalysisMuMu* a);
// void ComputeCentEffFactor(AliAnalysisMuMu* a);
// void ComputeFnormScalersForC0V0M(AliAnalysisMuMu* a);
Double_t* ComputeSumOfMB(AliAnalysisMuMu* a1, TGraphErrors* h1  );
TH1* GetGraphAsHisto(AliAnalysisMuMu* a1, TGraphErrors* h1 );
void GetValueAndErrorFromGraph(TGraphErrors* graph,Int_t runNumber,Double_t& value,Double_t& error);


void FnormOutPut(const char * filename ="../AnalysisResults_grid.root" ,
	const char * filename2 ="../AnalysisResults_grid_C0TVX-B-NOPF-CENTNOTRD.root")
{
	// Offline
	TH1*FnormOffline1PUPS =0x0;
	TH1*FnormOffline2PUPS =0x0;
	TGraphErrors*FnormOffline2PUPSGraph =0x0;
	//Scaler
	TH1*FnormScalers =0x0;
	TH1*FnormScalersPUPS =0x0;
	TH1*FnormScalersPUPS2 =0x0;
	//Purity for CINT7
	TH1*CINTPurity =0x0;
	TH1*CINTPurityMUL =0x0;
	//Factor for C0V0M
	TH1*CentEffC0 =0x0;
	TH1*CentEffFactC0 =0x0;

	TH1*f4 =0x0;

	//Get files
	AliAnalysisMuMu ana(filename,"","","mumu.pp2015.config");
	AliAnalysisMuMu ana2(filename2,"","","PbPb2015");

	// ComputeFnormScalersForC0V0M(&ana2);

	//Get Graph
	// FnormOffline1PUPS = static_cast<TGraphErrors*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/FnormOffline1PUPS")->Clone());
	// FnormOffline2PUPSGraph = static_cast<TGraphErrors*>(ana.OC()->GetObject("/FNORM/Offline/GRAPHS/FnormOffline2PUPS")->Clone());
	// FnormOffline2PUPS = GetGraphAsHisto(&ana,FnormOffline2PUPSGraph);

	// new TCanvas;
	// FnormOffline2PUPS->DrawCopy();
	// return;
	FnormScalersPUPS = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());
	
	FnormScalersPUPS2 = static_cast<TH1*>(ana2.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPU_AsHisto")->Clone());
	TH1* Purityfactor2 = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/PurityFactorForScalerPSMUL_PP_AsHisto")->Clone());
	
	if(!FnormScalersPUPS || !FnormScalersPUPS2 || !Purityfactor2) {
		printf("cannot get all the histo\n");
		return;
	}
    
    Double_t MUL = ana.CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL"));
    printf("MUL =%f\n", MUL);

    // FnormScalersPUPS2->Divide(Purityfactor2);
    FnormScalersPUPS2->Scale(MUL/22.29/TMath::Power(10,6));
    FnormScalersPUPS->Scale(MUL/52.62/TMath::Power(10,6));

    new TCanvas;
    FnormScalersPUPS2->DrawCopy();
    new TCanvas;
    FnormScalersPUPS->DrawCopy();


	// FnormScalersPUPS = static_cast<TH1*>(ana.OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPUPS_AsHisto")->Clone());

	// TFile  foutput("FnormScalersOutput.root","NEW");
	// FnormOffline1PUPS->Write();
	// FnormOffline2PUPS->Write();
	// FnormScalersPUPS->Write();
	// return;

	// Double_t* F1 = ComputeSumOfMB(&ana,FnormOffline1PUPS);
	// Double_t* F2 = ComputeSumOfMB(&ana,FnormOffline2PUPS);
	// Double_t* F3 = ComputeSumOfMB(&ana,FnormScalersPUPS);

	// //Compute Mean FNorm
	// Double_t MeanMB = (F1[0]/F1[1] + F2[0]/F2[1] + F3[0]/F3[1]) / ( 1./F1[1] + 1./F2[1] + 1./F3[1]) ;
	// Double_t MeanMBError = TMath::Sqrt(        1./ (   1./F1[1]+1./F2[1]+1./F3[1] )   ) ;
	// Double_t MUL = ana.CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL"));
	// printf("MUL = %.0f\n", MUL);
	// printf("Mean MBeq = %.0f +/- %.0f ( %f percent ) \n",MeanMB , MeanMBError,100.* MeanMBError/MeanMB );

	// ______Projection histograms
	// New Histo to plot purposes
	TH1* h1 =0x0;
	printf("FnormScalersPUPS2->GetNbinsX() = %d\n", FnormScalersPUPS2->GetNbinsX());
	printf("FnormScalersPUPS2->GetXaxis()->GetXmin() = %f\n", FnormScalersPUPS2->GetXaxis()->GetXmin());
	printf("FnormScalersPUPS2->GetXaxis()->GetXmax() = %f\n", FnormScalersPUPS2->GetXaxis()->GetXmax());
	TH1F* h2 = new TH1F("V0_scalers","V0_scalers",100,70.,120.);// Total histo
	TH1F* h3 = new TH1F("T0_scalers","T0_scalers",100,70.,120.);// Total histo
	TH1F* h4 = new TH1F("Sum","Sum",100,70.,120.);// Total histo

	// //Fill with Fnorm run by run divided by squared of the error
	// for ( Int_t ipoint = 0; ipoint < FnormOffline1PUPS->GetEntries(); ++ipoint ){
	// 	Double_t y  = FnormOffline1PUPS->GetY(ipoint+1);
	// 	Double_t e2 = (FnormOffline1PUPS->GetErrorY(ipoint+1)* FnormOffline1PUPS->GetErrorY(ipoint+1));
	// 	h1->Fill(y,1./e2);

	// 	Double_t y
	// 	Double_t e2
	// }
	for ( Int_t ipoint = 0; ipoint < FnormScalersPUPS2->GetEntries(); ++ipoint ){
		Double_t y  = FnormScalersPUPS2->GetBinContent(ipoint+1);
		printf("y = %f \n",y);
		Double_t e2 = (FnormScalersPUPS2->GetBinError(ipoint+1)* FnormScalersPUPS2->GetBinError(ipoint+1));

		h2->Fill(y,1./e2);
	}

	for ( Int_t ipoint = 0; ipoint < FnormScalersPUPS2->GetEntries(); ++ipoint ){
		Double_t y  = FnormScalersPUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormScalersPUPS->GetBinError(ipoint+1)* FnormScalersPUPS->GetBinError(ipoint+1));
		h3->Fill(y,1./e2);
	}

	for ( Int_t ipoint = 0; ipoint < FnormScalersPUPS2->GetEntries(); ++ipoint ){
		Double_t y  = FnormScalersPUPS->GetBinContent(ipoint+1);
		Double_t e2 = (FnormScalersPUPS->GetBinError(ipoint+1)* FnormScalersPUPS->GetBinError(ipoint+1));

		Double_t y2  = FnormScalersPUPS2->GetBinContent(ipoint+1);
		Double_t e3 = (FnormScalersPUPS2->GetBinError(ipoint+1)* FnormScalersPUPS2->GetBinError(ipoint+1));

		// Double_t y3  = FnormOffline1PUPS->GetBinContent(ipoint);
		// Double_t e4 = (FnormOffline1PUPS->GetBinError(ipoint)* FnormOffline1PUPS->GetBinError(ipoint));
		h4->Fill(y,1/e2);
		h4->Fill(y2,1/e3);
	}

	// h1->Scale(1./h1->Integral());
	h2->Scale(1./h2->Integral());
	h3->Scale(1./h3->Integral());
	h4->Scale(1./h4->Integral());
	// new TCanvas;
	// h2->DrawCopy();
	// new TCanvas;
	// h3->DrawCopy();
	new TCanvas;
	h4->DrawCopy();

	//______________Plot
	// h1->SetLineColor(2);
	h2->SetLineColor(2);
	h3->SetLineColor(3);
	h4->SetLineColor(1);

	// h1->SetLineStyle(9);
	h2->SetLineStyle(1);
	h3->SetLineStyle(1);
	h4->SetLineStyle(1);

	h4->SetLineWidth(2);
	// h1->SetLineWidth(1.2);
	h2->SetLineWidth(0.8);
	h3->SetLineWidth(0.8);


	new TCanvas;
	TLegend * leg = new TLegend(0.52,0.4,0.89,0.68);
	leg->SetHeader("Distribution of FNorm for different methods");
	// leg->AddEntry(h1,"FNormOffline1","le");
	// leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h1->GetMean(),h1->GetRMS()),"");
	leg->AddEntry(h2,"Scalers method C0TVX","le");
	leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h2->GetMean(),h2->GetRMS()),"");
	leg->AddEntry(h3,"Scalers method CINT7","le");
	leg->AddEntry((TObject*)0,Form("mean = %.3f ; RMS = %.3f",h3->GetMean(),h3->GetRMS()),"");
	leg->AddEntry(h4,"Sum","le");
	leg->AddEntry((TObject*)0,Form("mean = %.3f; RMS =  %.3f",h4->GetMean(),h4->GetRMS()),"");


	h4->DrawCopy("");
	// h1->DrawCopy("same");
	h2->DrawCopy("same");
	h3->DrawCopy("same");
	leg->Draw();



	// TCanvas* c3 = new TCanvas();
	// c3->Divide(1,1,0,0);

	// c3->cd(1);
	// FnormOffline1PUPS->SetMarkerColor(2);
	// FnormOffline1PUPS->SetMarkerSize(0.9);
	// FnormOffline1PUPS->SetMarkerStyle(8);
	// FnormOffline2PUPS->SetMarkerColor(3);
	// FnormOffline2PUPS->SetMarkerSize(0.9);
	// FnormOffline2PUPS->SetMarkerStyle(8);
	// FnormScalersPUPS->SetMarkerColor(4);
	// FnormScalersPUPS->SetMarkerSize(0.9);
	// FnormScalersPUPS->SetMarkerStyle(8);
	// FnormOffline1PUPS->SetTitle("FNorm");
	// // FnormOffline1PUPS->GetYaxis()->SetRangeUser(8,15);
	// FnormOffline1PUPS->DrawClone("AP");
	// FnormOffline2PUPS->DrawClone("Psame");
	// FnormScalersPUPS->DrawClone("Psame");
	// TLegend * leg3 = new TLegend(0.4,0.2,0.90,0.4);
	// leg3->SetHeader("FNorm");
	// leg3->AddEntry(FnormOffline1PUPS,"FNormOffline1 = CINT7/CINT7&0MUL","pe");
	// leg3->AddEntry(FnormOffline2PUPS,"FNormOffline2 = CMSL/CMSL&0MUL x CINT7/CINT7&0MSL","pe");
	// leg3->AddEntry(FnormScalersPUPS,"Scaler Fnorm with purity and pile-up correction","pe");
	// leg3->DrawClone("same");

	// c3->cd(2);
	// FnormOffline1PUPS->SetTitle("Ratio");
	// TH1* FnormOffline1PUPSclone = static_cast<TH1*>(FnormOffline1PUPS->Clone());
	// TLegend * leg4 = new TLegend(0.4,0.2,0.90,0.4);
	// leg4->SetHeader("FNorm");
	// leg4->AddEntry(FnormOffline1PUPS,"FNormOffline1/FNormOffline2","pe");
	// leg4->AddEntry(FnormOffline1PUPSclone,"FNormOffline1/Scaler","pe");
	// leg4->AddEntry(FnormOffline2PUPS,"FNormOffline2/Scaler","pe");
	// FnormOffline1PUPS->Divide(FnormOffline2PUPS);
	// FnormOffline1PUPS->GetYaxis()->SetRangeUser(0,1.5);
	// FnormOffline1PUPS->DrawCopy();
	// FnormOffline1PUPSclone->Divide(FnormScalersPUPS);
	// FnormOffline1PUPSclone->SetMarkerColor(4);
	// FnormOffline1PUPSclone->SetMarkerSize(0.55);
	// FnormOffline1PUPSclone->SetMarkerStyle(8);
	// FnormOffline1PUPSclone->DrawCopy("same");
	// FnormOffline2PUPS->Divide(FnormScalersPUPS);
	// FnormOffline2PUPS->DrawCopy("same");
	// leg4->Draw("same");

// 	return;
}

//______________________________________________
std::set<int> RunNumbers(AliAnalysisMuMu* a)
{
  // Extract the run numbers from our counter collection

  std::set<int> runset;

  TString sruns = a->CC()->GetKeyWords("run");
  TObjArray* runs = sruns.Tokenize(",");

  TIter next(runs);
  TObjString* s;

  while ( ( s = static_cast<TObjString*>(next())) )
  {
    runset.insert(s->String().Atoi());
  }
  delete runs;

  return runset;
}


// //______________________________________________
// void ComputeCentEffFactor(AliAnalysisMuMu* a)
// {
// 	std::set<int> runs= RunNumbers(a);

// 	TGraphErrors* CentEff = new TGraphErrors(Form("Centrality_Efficiency_60-70_AsHisto"),Form("Centrality_Efficiency_60-70_AsHisto"),1.,0.,1.);
// 	TGraphErrors* CentEffFact = new TGraphErrors(Form("CentralityCorrection_C0V0M_AsHisto"),Form("CentralityCorrection_C0V0M_AsHisto"),1.,0.,1.);
// 	for ( std::set<int>::const_iterator it = runs.begin(); it != runs.end(); ++it )
// 	{
// 		Int_t runNumber = *it;

// 		Double_t ZeroToNinety = a->CC()->GetSum(Form("run:%d/trigger:C0V0M-B-NOPF-CENTNOTRD/centrality:V0M_00.00_90.00",runNumber));
// 		Double_t SixtyToSeventy = a->CC()->GetSum(Form("run:%d/trigger:C0V0M-B-NOPF-CENTNOTRD/centrality:V0M_60.00_70.00",runNumber));
// 		// printf("60-70 = %f\n",SixtyToSeventy );
// 		// printf("00-90 = %f\n",ZeroToNinety );

// 		Double_t a =SixtyToSeventy/ZeroToNinety;
// 		Int_t bin = CentEff->Fill(TString::Format("%d",runNumber).Data(),a);
// 		Int_t binbis = CentEffFact->Fill(TString::Format("%d",runNumber).Data(),9.*(1-a)/6.);
// 		CentEffFact->SetBinError(binbis,0.);;
// 	}

// 	int j=0;
//   	for (int i = 1; i < CentEff->GetEntries()+1; i++)
//   	{
//     	if(TString(CentEff->GetXaxis()->GetBinLabel(i)).IsNull()) continue;
//     	j++;
//   	}
//   	if(j == runs.size()) CentEff->GetXaxis()->SetRange(1,j);
//   	if(j == runs.size()) CentEffFact->GetXaxis()->SetRange(1,j);

//   	TH1* o1 = a->OC()->Histo("/FNORM/Scaler/GRAPHS",CentEff->GetName());
//   	TH1* o2 = a->OC()->Histo("/FNORM/Scaler/GRAPHS",CentEffFact->GetName());

//     if (o1)
//     {
//         printf(Form("Replacing /FNORM/Scaler/GRAPHS/%s",CentEff->GetName()));
//         a->OC()->Remove(Form("/FNORM/Scaler/GRAPHS/%s",CentEff->GetName()));
//     }

//     //Adopt
//     Bool_t adoptOK = a->OC()->Adopt("/FNORM/Scaler/GRAPHS",CentEff);
//     if ( adoptOK ) std::cout << "+++raw histo " << CentEff->GetName() << " adopted" << std::endl;

//     if (o2)
//     {
//         printf(Form("Replacing /FNORM/Scaler/GRAPHS/%s",CentEffFact->GetName()));
//         a->OC()->Remove(Form("/FNORM/Scaler/GRAPHS/%s",CentEffFact->GetName()));
//     }
//     //Adopt
//     adoptOK = a->OC()->Adopt("/FNORM/Scaler/GRAPHS",CentEffFact);
//     if ( adoptOK ) std::cout << "+++raw histo " << CentEffFact->GetName() << " adopted" << std::endl;
// }

// //______________________________________________
// void ComputeFnormScalersForC0V0M(AliAnalysisMuMu* a)
// {

// 	//Compute Centrality correction for C0V0M
// 	ComputeCentEffFactor(a);

// 	//Compute FNorm
// 	TH1* FnormScalersPUPS = static_cast<TH1*>(a->OC()->GetObject("/FNORM/Scaler/GRAPHS/FnormScalersPU_AsHisto")->Clone());
// 	TH1* CentEffFactC0 =  static_cast<TH1*>(a->OC()->GetObject("/FNORM/Scaler/GRAPHS/CentralityCorrection_C0V0M_AsHisto")->Clone());
//   	FnormScalersPUPS->Multiply(CentEffFactC0);

// 	//Change Names
// 	FnormScalersPUPS->SetName("FnormScalersPUPSFinal_AsHisto");
// 	FnormScalersPUPS->SetTitle("Computed using OCDB scalers assuming PU = 1 ");

// 	TH1* o1 = a->OC()->Histo("/FNORM/Scaler/GRAPHS",FnormScalersPUPS->GetName());

//     if (o1)
//     {
//         printf(Form("Replacing /FNORM/Scaler/GRAPHS/%s",FnormScalersPUPS->GetName()));
//         a->OC()->Remove(Form("/FNORM/Scaler/GRAPHS/%s",FnormScalersPUPS->GetName()));
//     }

//     //Adopt
//     Bool_t adoptOK = a->OC()->Adopt("/FNORM/Scaler/GRAPHS",FnormScalersPUPS);
//     if ( adoptOK ) std::cout << "+++raw histo " << FnormScalersPUPS->GetName() << " adopted" << std::endl;

// }
//______________________________________________
Double_t* ComputeSumOfMB(AliAnalysisMuMu* a1, TGraphErrors* h1 )
{
	std::set<int> runs= RunNumbers(a1);
	Double_t sum =0.;
	Double_t sumerr2 =0.;

	int i= 1;
	for ( std::set<int>::const_iterator it = runs.begin(); it != runs.end(); ++it )
	{
		Double_t FNorm;
		Double_t FNormerr;
		Double_t x;
		Double_t errx;

		Int_t runNumber = *it;
		Double_t nMUL = a1->CC()->GetSum(Form("run:%d/trigger:CMUL7-B-NOPF-MUFAST/centrality:PP/event:PSMUL",runNumber));

		h1->GetPoint(i,x,FNorm);

		errx     = h1->GetErrorX(i);
		FNormerr = h1->GetErrorY(i);

		// printf("FNorm for run %d = %f\n",runNumber, FNorm );
		// printf("FNormerr for run %d = %f\n",runNumber, FNormerr );
		sum = sum + FNorm*nMUL;
		sumerr2 = sumerr2 + FNormerr*FNormerr*nMUL*nMUL;
		i++;
	}

	printf("NMBeq for %s   : %.0f +/- %0.f ( %f percent ) \n",h1->GetName(), sum, TMath::Sqrt(sumerr2), 100*TMath::Sqrt(sumerr2)/sum );
	Double_t* F  = new Double_t[2];
	F[0]= sum;
	F[1] =sumerr2;
	return F;
}

//_____________________________________________________________________________
TH1* GetGraphAsHisto(AliAnalysisMuMu* a1, TGraphErrors* h1 )
{

  std::set<int> runs= RunNumbers(a1);

  TGraphErrors *g = static_cast<TGraphErrors*>(h1->Clone());
  TH1F * h =new TH1F(Form("%s_AsHisto",g->GetName()),Form("%s_AsHisto",g->GetName()),1,0.,1.);

  Double_t y = 0.;
  Double_t dy = 0.;

  //Fill
  for ( std::set<int>::const_iterator it = runs.begin(); it != runs.end(); ++it )
  {
    Int_t runNumber = *it;

    GetValueAndErrorFromGraph(g,runNumber,y,dy);
    if(y==0.)
    {
      printf("Error : no values for run %d\n",runNumber );
      continue;
    }

    Int_t bin = h->Fill(TString::Format("%d",runNumber).Data(),y);
    h->SetBinError(bin,dy);
    printf("F!lled !\n");
  }
  new TCanvas;
  h->DrawCopy();
  // Set range
  int j=0;
  for (int i = 1; i < h->GetEntries()+1; i++)
  {
    if(TString(h->GetXaxis()->GetBinLabel(i)).IsNull()) continue;
    j++;
  }
  if(j == runs.size()) h->GetXaxis()->SetRange(1,j);

  delete g;
  
  return h;

}

//_____________________________________________________________________________
void GetValueAndErrorFromGraph(TGraphErrors* graph,
                                                     Int_t runNumber,
                                                     Double_t& value,
                                                     Double_t& error)
{
  /// get (value,error) corresponding to run=runNumber.
  /// Works both for compact and non-compact graphs
  
  value = TMath::Limits<Double_t>::Max();
  error = 0.0;
  
  if (!graph) return;
  
  TAxis* axis = graph->GetXaxis();
  
  for ( Int_t i = 0; i < graph->GetN(); ++i )
  {
    Int_t rbin = TMath::Nint(graph->GetX()[i]);
    Int_t rlabel = TString(axis->GetBinLabel(i+1)).Atoi();
    if ( rbin == runNumber || rlabel == runNumber )
    {
      value = graph->GetY()[i];
      error = graph->GetEY()[i];
    }
  }
}
