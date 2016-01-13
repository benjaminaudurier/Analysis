///
/// Macro to compare 2 histograms in the mergeable collection of the mumu framework$
/// 

// author : Benjamin Audurier 
// 

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliMergeableCollection.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TLine.h>


//Some strings and constants
char                   * sfile="AnalysisResultsNew.root";
char                   * sasso="";
char                   * sasso2="";
char                   * beamYear="mumu.PbPb2015.config";

TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSALL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-10.0RABSMATCHLOWETAPDCA";
TString scentrality    ="V0M_00.00_90.00,V0M_00.00_10.00,V0M_10.00_30.00,V0M_30.00_50.00,V0M_50.00_70.00,V0M_70.00_90.00";

//___________________________________________
void CompareHistoRawCount(char* what ="pt"/*,const char * printWhat = "rawcount"*/)
{    
    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

    TObjArray* whatCent= TString(scentrality.Data()).Tokenize(",");
    TIter nextCent(whatCent);
    TObjString* scent;
	
    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    //_____ Draw 
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) ) 
    {
        while ( ( scent = static_cast<TObjString*>(nextCent()) ) ) 
        {
        	TString histoPath1= Form("/%s/%s/%s/%s/hRawCountVS%s_2.100000-2.800000",seventType.Data(),striggerDimuon.Data(),scent->String().Data(),spairCut.Data(),what);
        	TString histoPath2= Form("/%s/%s/%s/%s/hRawCountVS%s_2.800000-3.400000",seventType.Data(),striggerDimuon.Data(),scent->String().Data(),spairCut.Data(),what);

	    	TH1*h1 = static_cast<TH1*>(analysis.OC()->GetObject(histoPath1.Data())->Clone());
	    	TH1*h2 = static_cast<TH1*>(analysis.OC()->GetObject(histoPath2.Data())->Clone());
	    	h1->Scale(1./h1->GetEntries());
	    	h2->Scale(1./h2->GetEntries());

	    	// h1->Scale(1./h1->GetEntries());
	    	h1->GetYaxis()->SetTitle(Form("normalized raw count of dimuon pairs "));
	    	h1->GetYaxis()->SetRangeUser(1., 10e6);
	    	h2->GetYaxis()->SetRangeUser(1., 10e6);
	    	
	    	// h2->Scale(1./h2->GetEntries());
	    	h1->SetMarkerColor(3);
	    	h1->SetMarkerStyle(20);
	    	h1->SetMarkerStyle(20);
	    	h2->SetMarkerColor(4);
	    	h2->SetMarkerStyle(20);

	    	if(!h1 || !h2) return;
	    	printf("histos found !\n");

	    	TH1* ratio = new TH1F();

	    	TCanvas* c = new TCanvas;
	    	c->Divide(1,2,0.,0.);
	    	
	    	c->cd(1);
	    	TLegend * leg = new TLegend(0.3,0.7,0.70,0.9);
            leg->SetTextSize(0.02);
            leg->SetHeader(Form("ALICE, Pb-Pb #sqrt{s_{NN}}=2.76 TeV, L_{int}=222,4 #mub^{-1}, %s",scent->String().Data()));
            leg->AddEntry(h1," raw count of dimuon pairs in [2.1-4.1] GeV/c ","lpe");
            leg->AddEntry(h2," raw count of dimuon pairs in [2.8-3.4] GeV/c  ","lpe");
	    	gPad->SetLogy();
	    	h1->DrawCopy("e1");
	    	h2->DrawCopy("samee1");
	    	leg->Draw("");
	    	
	    	c->cd(2);	    	
	    	h1->Divide(h2);
	    	h1->GetYaxis()->SetRangeUser(-2,8);
	    	h1->SetMarkerColor(2);
	    	h1->GetYaxis()->SetTitle("Scaled Raw count");
	    	TLine *line = new TLine();
	    	line->SetHorizontal();
	    	line->SetY1(0.);
	    	line->SetLineColor(0);
	    	h1->DrawCopy("e1");
	    	line->Paint("same");

	    	
	    }
    }

    return ;    
} 


