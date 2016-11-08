//
//  PlotMeanPt.C
//
//
//  Created by Benjamin Audurier on 05/11/16.
//
//

// Macro to plot MeanPt in loop

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliAnalysisMuMuResult.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TLine.h>
#include <TFile.h>

#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <AliAnalysisMuMuBinning.h>


// --- Config for the mumu object ---
TString sasso     ="/Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/Simulation/full/AnalysisResults.JPSI.root";
TString sasso2    ="";
TString config    = "mumu.pp2015.meanpt.config";

// --- Directory where the results are
TString dir       = "/Users/audurier/Documents/Analysis/LHC_15n_pp/MuMu/AccEffMaps/results";

// --- Path to the spectra
const char* spectraPath = "/FitResults/PSMUL/CMUL7-B-NOPF-MUFAST/PP/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA/PSI-PT-INT_PUTCUT-AccEffCorr-MeanPtVsMinvUS";


//_____________________________________________________________________________
void PlotMeanPt(const char* bin, int debug =0)
{

    AliLog::SetGlobalDebugLevel(debug);

    // Check directories
    if ( gSystem->AccessPathName(gSystem->ExpandPathName(dir.Data()))) {
        printf("Error : Dir '%s' doesn't exists !!!",dir.Data());
        return ;
    }

    // Get what's inside
    TString file  = gSystem->GetFromPipe(Form("ls %s",dir.Data()));
    TObjArray* files  = file.Tokenize("\n");

    // Create the spectra table
    Double_t meanpt[files->GetEntries()];
    Double_t meanpterrorstat[files->GetEntries()];
    Double_t meanpterrorsyst[files->GetEntries()];

    TIter nextfiles(files);
    TObjString* sfile;
    nextfiles.Reset();

    int i=0;

    // The loop where we fit
    while ( ( sfile = static_cast<TObjString*>(nextfiles()) ) ) {
        printf("\n");
        printf("\n ----> Looking for results in %s/%s\n", dir.Data(),sfile->String().Data() );

        // main object
        AliAnalysisMuMu analysis(Form("%s/%s",dir.Data(),sfile->String().Data()),sasso.Data(),sasso2.Data(),config.Data());
	    if(!analysis.OC()) continue;

        // Get the spectra
        AliAnalysisMuMuSpectra* spectra = analysis.SPECTRA(spectraPath);
        if(!spectra){
            printf("Cannot get spectra \n");
            continue;
        }
        // spectra->Binning()->Print();
        // return;
        // Get the results
        AliAnalysisMuMuResult* r = spectra->GetResultForBin(bin);
        if(!r){
            printf("Cannot get results for bin %s \n",bin);
            continue;
        }

        meanpt[i] = r->GetValue("MeanPtJPsi");
        meanpterrorstat[i] = r->GetErrorStat("MeanPtJPsi");
        meanpterrorsyst[i] = r->GetRMS("MeanPtJPsi");

        ++i;
    }

    // Plot the histograms
    TH1F * h_meanpt = new TH1F("meanpttests",Form("Meant pt vs AccEff maps (%s)",bin),files->GetEntries(),0,files->GetEntries());
    TH1F * h_dummy = new TH1F("dummy","dummy",files->GetEntries(),0.,4.);
    for (int i = 0; i < files->GetEntries(); ++i)
    {
        // the histo we plot
        h_meanpt->SetBinContent(i+1,meanpt[i]);
        h_meanpt->SetBinError(i+1,meanpterrorstat[i]);

        // the the RMS calculation
        Int_t bin = h_dummy->Fill(meanpt[i]);
        h_dummy->SetBinError(bin,meanpterrorstat[i]);

        // Here we change the label names
        h_meanpt->GetXaxis()->SetBinLabel(i+1,Form("AccEff_map_%d",i));
    }

    // Here we draw
    new TCanvas;
    h_meanpt->DrawCopy();
    printf("Mean : %f\n",h_dummy->GetMean() );
    printf("RMS : %f (%f %%)\n",h_dummy->GetRMS(),100*h_dummy->GetRMS()/h_dummy->GetMean() );
    TLine *line1 = new TLine(0,h_dummy->GetMean(),files->GetEntries(),h_dummy->GetMean());
    line1->SetLineColor(kBlue);
    line1->SetLineWidth(3);
    TLine *line2 = new TLine(0,h_dummy->GetMean()-h_dummy->GetRMS(),files->GetEntries(),h_dummy->GetMean()-h_dummy->GetRMS());
    line2->SetLineColor(kBlue);
    line2->SetLineWidth(3);
    line2->SetLineStyle(3);
    TLine *line3 = new TLine(0,h_dummy->GetMean()+h_dummy->GetRMS(),files->GetEntries(),h_dummy->GetMean()+h_dummy->GetRMS());
    line3->SetLineColor(kBlue);
    line3->SetLineWidth(3);
    line3->SetLineStyle(3);
    line1->Draw("same");
    line2->Draw("same");
    line3->Draw("same");
}













