#include <TH1.h>
#include <TString.h>
#include <TFile.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TCanvas.h>
#include <Riostream.h>
#include <TLegend.h>
#include <AliAnalysisMuMu.h>


//Some strings and constants
char                   * sfile="AnalysisResultsNew.root";
char                   * sasso="";
char                   * sasso2="";
char                   * beamYear="mumu.PbPb2015.config";

const char* trigger   ="C0V0M-B-NOPF-CENTNOTRD";
const char* eventType ="PSALL";
const char* run1 = "244980";
const char* run2 = "246994";


void PrintCentralityDistribution(const char * filename ="AnalysisResults_27_20160120-2124.root:MuMuCount")
{
	// Get file
	AliAnalysisMuMu* ana = new AliAnalysisMuMu(filename,"","","PbPb2015");
	if(!ana)return;

	// Create CoutVsCentrality histo
	TH1D* hCountVsCent1 = new TH1D("hCountVsCent1",Form(" Count Vs Centrality for C0V0M and run %s",run1),6,0,60);
	TH1D* hCountVsCent2 = new TH1D("hCountVsCent2",Form(" Count Vs Centrality for C0V0M and run %s",run2),6,0,60);

	// Fill histos
	for (int i = 0; i < 6; ++i)
	{	
		// Get counts
		Double_t count1 = ana->CC()->GetSum(Form("trigger:%s/event:%s/centrality:V0M_%d0.00_%d0.00/run:%s",trigger,eventType,i,i+1,run1));
		printf("count1 for centrality %d0.00_%d0.00 = %.0f\n",i,i+1,count1 );
		Double_t count2 = ana->CC()->GetSum(Form("trigger:%s/event:%s/centrality:V0M_%d0.00_%d0.00/run:%s",trigger,eventType,i,i+1,run2));
		printf("count2 for centrality %d0.00_%d0.00 = %.0f\n",i,i+1,count2 );
		printf("\n");
		// Fill histos
		hCountVsCent1->Fill(0+11*i,count1);
		hCountVsCent2->Fill(0+11*i,count2);
	}
	new TCanvas;
	hCountVsCent1->Draw();
	new TCanvas;
	hCountVsCent2->Draw();

}