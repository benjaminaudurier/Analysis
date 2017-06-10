//
//  ComputeLptAptRatio.C
//
//
//  Created by Benjamin Audurier on 18/12/17.
//
//

// Compute lpt/apt ratio per group of local board
// Macro to be used with the output of the AliAnalysisMuMuTriggerResponse() task belonging to the AliAnalysisMuMu Framework.
// With this macro, the lpt/apt distributions are computed and stored in the same files as the ones plugged in the macro.
// Once this step done, one could use those files as a reference for the AliAnalysisMuMuTriggerResponse() task and reweight the MC (see AliAnalysisMuMuTriggerResponse for more details)
//
//
#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TLegend.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TF1.h>
#include <TROOT.h>
#include <THnSparse.h>
#include <iostream>

// Files and config files mendatory to the AliAnalysisMuMu framework
const char* sfileData="Data/AnalysisResults.root";
const char* configFileData="mumu.lptapt.config";

const char* sfileMc="MC/AnalysisResults.JPSI.root";
const char* configFileMc="mumu.lptapt.config";

// Debug
int debug=0;

// ranges
// Double_t ptrange[] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.};
Double_t ptrange[] = {0.,12.};
Double_t etarange[] = {-4.,-3.5,-3.0,-2.5};
// Double_t etarange[] = {-4.,-3.70,-3.40,-3.10,-2.80,-2.5};
// Double_t etarange[] = {-4.,-3.85,-3.70,-3.55,-3.40,-3.25,-3.10,-2.95,-2.80,-2.65,-2.5};
// Double_t etarange[] = {-3.25,-2.5};
// Double_t etarange[] = {-4.,-3.};
Int_t nEtaBins = sizeof(etarange)/sizeof(etarange[0]) - 1;
Int_t nPtBins = sizeof(ptrange)/sizeof(ptrange[0]) - 1;

const int projection_axis = 1; // 0 = local board, 1 = pt, 2 = eta;

// The centrality for which we want to compute lpt/apt
// const TString centrality ="V0M_00.00_10.00,V0M_10.00_20.00,V0M_20.00_30.00,V0M_30.00_40.00,V0M_40.00_90.00,V0M_00.00_90.00";
// const TString centrality ="V0M_00.00_10.00,V0M_40.00_90.00";
const TString centrality ="V0M_00.00_90.00";

void CleanAllHisto(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void LptAptPerLocalBoardGroups(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void LptAptForAllLocalBoard(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void LptAptForDistributionRangeVersusRange(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void LptAptForDistributionRangeVersusCentrality(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void LptAptForSingleBoard(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC,int localBoard = 45);

TH1* GetLocalBoardResponseFromThnSparse(THnSparse* hnsparse=0x0,int localboard=1 );
TH1* GetRangeResponseFromThnSparse(THnSparse* hnsparse=0x0, int bin=0);
void SaveObject(AliAnalysisMuMu* ali, TObject* obj, char* path);

Double_t FitFuncErfFixed ( Double_t*, Double_t* );
Double_t FitFuncErf ( Double_t*, Double_t* );
Double_t FitFuncInt ( Double_t*, Double_t* );

//______________________________________________
void ComputeLptAptRatio( const char* mode ="" , int localBoard = 1)
{
    /// mode = centrality : Print canvas for each centrality bins
    ///     - localBoard = 0 : results are computed for each local board
    ///     - localBoard = <num> : results are computed for local board <num>
    ///     - localBoard < 0 : results are computed versus each eta/pt interval according to projection_axis
    /// mode = group : Print canvas for each local board group bins
    /// mode = y     : Print canvas for each centrality with all the ranges

    AliLog::SetGlobalDebugLevel(debug);

    // Get Data and MC files
    AliAnalysisMuMu Data(sfileData,"","",configFileData);
    AliAnalysisMuMu MC(sfileMc,"","",configFileMc);

    // List with results to be stored

    TString smode(mode);

    if ( smode.Contains("cent") )
    {
        if ( localBoard == 0 )
            for (int i = 0; i < 234; ++i)
                LptAptForSingleBoard(&Data,&MC,i+1);
        else if ( localBoard < 0 ) LptAptForDistributionRangeVersusRange(&Data,&MC);
        else                       LptAptForSingleBoard(&Data,&MC,localBoard);
    }
    if ( smode.Contains("group") ) LptAptPerLocalBoardGroups(&Data,&MC);
    if ( smode.Contains("y") ) LptAptForDistributionRangeVersusCentrality(&Data,&MC);
    if ( smode.Contains("clean") ) CleanAllHisto(&Data,&MC);

    Data.Update();
    MC.Update();

}


//______________________________________________
void LptAptForSingleBoard(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC,int localBoard)
{
    /// Compute and print results for specific local boards

    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;

    // pointers
    TH1* h_LocalBoard_Data_lpt[centralityArray->GetEntries()];
    TH1* h_LocalBoard_Data_apt[centralityArray->GetEntries()];
    TGraphAsymmErrors* h_LocalBoard_Data_lpt_apt[centralityArray->GetEntries()];
    TH1* h_LocalBoard_MC_lpt[centralityArray->GetEntries()];
    TH1* h_LocalBoard_MC_apt[centralityArray->GetEntries()];
    TGraphAsymmErrors* h_LocalBoard_MC_lpt_apt[centralityArray->GetEntries()];
    TH1* h_LocalBoard_Data_over_MC[centralityArray->GetEntries()];

    for (int i = 0; i < centralityArray->GetEntries(); ++i)
    {
        h_LocalBoard_Data_lpt[i]     =0x0;
        h_LocalBoard_Data_apt[i]     =0x0;
        h_LocalBoard_Data_lpt_apt[i] =0x0;
        h_LocalBoard_MC_lpt[i]       =0x0;
        h_LocalBoard_MC_apt[i]       =0x0;
        h_LocalBoard_MC_lpt_apt[i]   =0x0;
        h_LocalBoard_Data_over_MC[i] =0x0;
    }

    TCanvas* c =new TCanvas(Form("can_%d",localBoard),Form("can_%d",localBoard),200,10,600,600);
    c->Divide(2,2);
    int ncent =0;
    TLegend* legend = new TLegend(0.20,0.20,0.40,0.40);

    // --- Print Results for each centrality  ---
    nextCentrality.Reset();
    while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
    {
        // --- Get lpt and apt ThnSparse from data ---

        THnSparse* h_Data_apt=0x0;
        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_Data_apt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_Data_apt){
            printf("cannot get h_Data_apt histo %s \n",scentrality->String().Data());
            return;
        }

        THnSparse* h_Data_lpt=0x0;
        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_Data_lpt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_Data_lpt){
            printf("cannot get h_Data_lpt histo %s \n",scentrality->String().Data());
            return;
        }

        // --- Get lpt and apt ThnSparse from MC ---

        THnSparse* h_MC_apt=0x0;
        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_MC_apt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_MC_apt){
            printf("cannot get h_MC_apt histo %s \n",scentrality->String().Data());
            return;
        }

        THnSparse* h_MC_lpt=0x0;
        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_MC_lpt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_MC_lpt){
            printf("cannot get h_MC_lpt histo %s \n",scentrality->String().Data());
            return;
        }

        // --- Get TH1 ---

        h_LocalBoard_Data_lpt[ncent] = GetLocalBoardResponseFromThnSparse(h_Data_lpt,localBoard);
        // h_LocalBoard_Data_lpt[ncent]->Rebin(2);
        h_LocalBoard_Data_apt[ncent] = GetLocalBoardResponseFromThnSparse(h_Data_apt,localBoard);
        // h_LocalBoard_Data_apt[ncent]->Rebin(2);

        h_LocalBoard_MC_lpt[ncent]   = GetLocalBoardResponseFromThnSparse(h_MC_lpt,localBoard);
        // h_LocalBoard_MC_lpt[ncent]  ->Rebin(2);
        h_LocalBoard_MC_apt[ncent]   = GetLocalBoardResponseFromThnSparse(h_MC_apt,localBoard);
        // h_LocalBoard_MC_apt[ncent]  ->Rebin(2);

        // --- Compute and save lpt/apt form data ---

        h_LocalBoard_Data_lpt_apt[ncent] = new TGraphAsymmErrors(h_LocalBoard_Data_lpt[ncent],h_LocalBoard_Data_apt[ncent],"cpe0");
        TAxis * axis = ((TAxis*)h_LocalBoard_Data_lpt[ncent]->GetXaxis()->Clone());
        h_LocalBoard_Data_lpt[ncent]->Divide(h_LocalBoard_Data_apt[ncent]);
        if(projection_axis==1)h_LocalBoard_Data_lpt_apt[ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_localboard_%d",etarange[0],etarange[nEtaBins],localBoard));
        if(projection_axis==2)h_LocalBoard_Data_lpt_apt[ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_localboard_%d",ptrange[0],ptrange[nPtBins],localBoard));
        axis->SetName(Form("%s_axis",h_LocalBoard_Data_lpt_apt[ncent]->GetName()));
        SaveObject(Data,((TObject*)axis),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));
        SaveObject(Data,((TObject*)h_LocalBoard_Data_lpt_apt[ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));

        // Draw
        c->cd(1);
        legend->AddEntry(h_LocalBoard_Data_lpt_apt[ncent],scentrality->String().Data());
        gPad->SetPad(0., 0.5, 0.5, 1);
        h_LocalBoard_Data_lpt_apt[ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
        h_LocalBoard_Data_lpt_apt[ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
        int color = ncent+1;
        color= color - (color/49)*49;
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_Data_lpt_apt[ncent]->SetMarkerColor(color+1);
        else h_LocalBoard_Data_lpt_apt[ncent]->SetMarkerColor(color);
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_Data_lpt_apt[ncent]->SetLineColor(color+1);
        else h_LocalBoard_Data_lpt_apt[ncent]->SetLineColor(color);
        if(ncent==0)h_LocalBoard_Data_lpt_apt[ncent]->Draw("AP");
        else h_LocalBoard_Data_lpt_apt[ncent]->Draw("Psame");

        // --- Compute and save lpt/apt form MC ---

        h_LocalBoard_MC_lpt_apt[ncent]= new TGraphAsymmErrors(h_LocalBoard_MC_lpt[ncent],h_LocalBoard_MC_apt[ncent],"cpe0");
        TAxis * axis2 = ((TAxis*)h_LocalBoard_MC_lpt[ncent]->GetXaxis()->Clone());
        h_LocalBoard_MC_lpt[ncent]->Divide(h_LocalBoard_MC_apt[ncent]);
        if(projection_axis==1)h_LocalBoard_MC_lpt_apt[ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_localboard_%d",etarange[0],etarange[nEtaBins],localBoard));
        if(projection_axis==2)h_LocalBoard_MC_lpt_apt[ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_localboard_%d",ptrange[0],ptrange[nPtBins],localBoard));
        axis2->SetName(Form("%s_axis",h_LocalBoard_MC_lpt_apt[ncent]->GetName()));
        SaveObject(MC,((TObject*)axis2),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));
        SaveObject(MC,((TObject*)h_LocalBoard_MC_lpt_apt[ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));

        // Draw
        c->cd(2);
        gPad->SetPad(0.5, 0.5, 1, 1);
        h_LocalBoard_MC_lpt_apt[ncent]->GetYaxis()->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
        h_LocalBoard_MC_lpt_apt[ncent]->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_MC_lpt_apt[ncent]->SetMarkerColor(color+1);
        else h_LocalBoard_MC_lpt_apt[ncent]->SetMarkerColor(color);
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_MC_lpt_apt[ncent]->SetLineColor(color+1);
        else h_LocalBoard_MC_lpt_apt[ncent]->SetLineColor(color);
        if(ncent==0)h_LocalBoard_MC_lpt_apt[ncent]->Draw("AP");
        else h_LocalBoard_MC_lpt_apt[ncent]->Draw("Psame");

        // --- Compute and Print lpt_data/apt_data  / lpt_mc/apt_mc

        h_LocalBoard_Data_over_MC[ncent]= ((TH1*)h_LocalBoard_Data_lpt[ncent]->Clone());
        h_LocalBoard_Data_over_MC[ncent]->Divide(h_LocalBoard_MC_lpt[ncent]);
        if(projection_axis ==1) h_LocalBoard_Data_over_MC[ncent]->SetName(Form("lpt_apt_DataOversMC_pt_eta_%.2f_%.2f",etarange[0],etarange[nEtaBins]));
        if(projection_axis ==2) h_LocalBoard_Data_over_MC[ncent]->SetName(Form("lpt_apt_DataOversMC_eta_pt_%.2f_%.2f",ptrange[0],ptrange[nPtBins]));

        c->cd(4);
        gPad->SetPad(0., 0., 1., 0.5);
        h_LocalBoard_Data_over_MC[ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
        h_LocalBoard_Data_over_MC[ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_Data_over_MC[ncent]->SetMarkerColor(color+1);
        else h_LocalBoard_Data_over_MC[ncent]->SetMarkerColor(color);
        if(color==0 || color ==10|| color ==19 || color ==5 ) h_LocalBoard_Data_over_MC[ncent]->SetLineColor(color+1);
        else h_LocalBoard_Data_over_MC[ncent]->SetLineColor(color);

        h_LocalBoard_Data_over_MC[ncent]->SetMarkerStyle(8);
        h_LocalBoard_Data_over_MC[ncent]->SetMarkerSize(0.5);
        if(ncent==0)h_LocalBoard_Data_over_MC[ncent]->DrawCopy("");
        else h_LocalBoard_Data_over_MC[ncent]->DrawCopy("same");


        ++ncent;
    }

    c->cd(4);
    legend->Draw("Same");

}


//______________________________________________
void LptAptForAllLocalBoard(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC)
{
    /// Compute and print results for all local boards

    TH1* h_per_local_board_Data_lpt[234]     = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_local_board_Data_apt[234]     = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TGraphAsymmErrors* h_per_local_board_Data_lpt_apt[234]     = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_local_board_MC_lpt[234]       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_local_board_MC_apt[234]       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TGraphAsymmErrors* h_per_local_board_MC_lpt_apt[234]       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_local_board_Data_over_MC[234] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;


    // --- Print Results for each centrality  ---

    nextCentrality.Reset();
    while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
    {
        TCanvas* c =new TCanvas(Form("can_%s",scentrality->String().Data()),Form("can_%s",scentrality->String().Data()),200,10,600,600);
        c->Divide(2,2);
        TLegend* legend = new TLegend(0.20,0.20,0.40,0.40);
        legend->SetHeader(scentrality->String().Data());

        // --- Get lpt and apt ThnSparse from data ---

        THnSparse* h_Data_apt=0x0;
        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_Data_apt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_Data_apt){
            printf("cannot get h_Data_apt histo %s \n",scentrality->String().Data());
            return;
        }

        THnSparse* h_Data_lpt=0x0;
        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_Data_lpt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_Data_lpt){
            printf("cannot get h_Data_lpt histo %s \n",scentrality->String().Data());
            return;
        }

        // --- Get lpt and apt ThnSparse from MC ---

        THnSparse* h_MC_apt=0x0;
        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_MC_apt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_MC_apt){
            printf("cannot get h_MC_apt histo %s \n",scentrality->String().Data());
            return;
        }

        THnSparse* h_MC_lpt=0x0;
        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
            h_MC_lpt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
        if(!h_MC_lpt){
            printf("cannot get h_MC_lpt histo %s \n",scentrality->String().Data());
            return;
        }

        for (int i = 0; i < 234; ++i)
        {
            // --- Get TH1 ---

            h_per_local_board_Data_lpt[i] = GetLocalBoardResponseFromThnSparse(h_Data_lpt,i+1);
            h_per_local_board_Data_lpt[i]->Rebin(2);
            h_per_local_board_Data_apt[i] = GetLocalBoardResponseFromThnSparse(h_Data_apt,i+1);
            h_per_local_board_Data_apt[i]->Rebin(2);

            h_per_local_board_MC_lpt[i] = GetLocalBoardResponseFromThnSparse(h_MC_lpt,i+1);
            h_per_local_board_MC_lpt[i]->Rebin(2);
            h_per_local_board_MC_apt[i] = GetLocalBoardResponseFromThnSparse(h_MC_apt,i+1);
            h_per_local_board_MC_apt[i]->Rebin(2);

            // --- Compute and save lpt/apt form data ---

            h_per_local_board_Data_lpt_apt[i] = new TGraphAsymmErrors(h_per_local_board_Data_lpt[i],h_per_local_board_Data_apt[i],"cpe0");
            TAxis * axis = ((TAxis*)h_per_local_board_Data_apt[i]->GetXaxis()->Clone());
            h_per_local_board_Data_lpt[i]->Divide(h_per_local_board_Data_apt[i]);
            if(projection_axis ==1) h_per_local_board_Data_lpt_apt[i]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_localboard_%d",etarange[0],etarange[nEtaBins],i+1));
            if(projection_axis ==2) h_per_local_board_Data_lpt_apt[i]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_localboard_%d",ptrange[0],ptrange[nEtaBins],i+1));
            axis->SetName(Form("%s_axis",h_per_local_board_Data_lpt_apt[i]->GetName()));
            SaveObject(Data,((TObject*)axis),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));
            SaveObject(Data,((TObject*)h_per_local_board_Data_lpt_apt[i]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));

            c->cd(1);
            gPad->SetPad(0., 0.5, 0.5, 1);
            h_per_local_board_Data_lpt_apt[i]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            h_per_local_board_Data_lpt_apt[i]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            int color = i+1;
            color= color - (color/49)*49;

            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_Data_lpt_apt[i]->SetMarkerColor(color+1);
            else h_per_local_board_Data_lpt_apt[i]->SetMarkerColor(color);
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_Data_lpt_apt[i]->SetLineColor(color+1);
            else h_per_local_board_Data_lpt_apt[i]->SetLineColor(color);
            if(i ==0 )h_per_local_board_Data_lpt_apt[i]->Draw("AP");
            else h_per_local_board_Data_lpt_apt[i]->Draw("PSame");

            // --- Compute and save lpt/apt form MC ---

            h_per_local_board_MC_lpt_apt[i]= new TGraphAsymmErrors(h_per_local_board_MC_lpt[i],h_per_local_board_MC_apt[i],"cpe0");
            TAxis * axis2 = ((TAxis*)h_per_local_board_MC_apt[i]->GetXaxis()->Clone());
            h_per_local_board_MC_lpt[i]->Divide(h_per_local_board_MC_apt[i]);
            if(projection_axis ==1) h_per_local_board_MC_lpt_apt[i]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_localboard_%d",etarange[0],etarange[nEtaBins],i+1));
            if(projection_axis ==2) h_per_local_board_MC_lpt_apt[i]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_localboard_%d",ptrange[0],ptrange[nEtaBins],i+1));
            axis2->SetName(Form("%s_axis",h_per_local_board_MC_lpt[i]->GetName()));
            SaveObject(MC,((TObject*)axis2),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));

            c->cd(2);
            gPad->SetPad(0.5, 0.5, 1, 1);
            h_per_local_board_MC_lpt_apt[i]->GetYaxis()->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_local_board_MC_lpt_apt[i]->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_MC_lpt_apt[i]->SetMarkerColor(color+1);
            else h_per_local_board_MC_lpt_apt[i]->SetMarkerColor(color);
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_MC_lpt_apt[i]->SetLineColor(color+1);
            else h_per_local_board_MC_lpt_apt[i]->SetLineColor(color);
            if(i ==0 )h_per_local_board_MC_lpt_apt[i]->Draw("AP");
            else h_per_local_board_MC_lpt_apt[i]->Draw("PSame");

            SaveObject(MC,((TObject*)h_per_local_board_Data_lpt_apt[i]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoard",scentrality->String().Data()));

            // --- Compute and Print lpt_data/apt_data  /  lpt_mc/apt_mc

            h_per_local_board_Data_over_MC[i]= ((TH1*)h_per_local_board_Data_lpt[i]->Clone());
            h_per_local_board_Data_over_MC[i]->Divide(h_per_local_board_MC_lpt[i]);
            if(projection_axis ==1) h_per_local_board_Data_over_MC[i]->SetName(Form("lpt_apt_DataOversMC_pt_eta_%.2f_%.2f",etarange[0],etarange[nEtaBins]));
            if(projection_axis ==2) h_per_local_board_Data_over_MC[i]->SetName(Form("lpt_apt_DataOversMC_eta_pt_%.2f_%.2f",ptrange[0],ptrange[nPtBins]));

            c->cd(4);
            gPad->SetPad(0., 0., 1., 0.5);
            h_per_local_board_Data_over_MC[i]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_local_board_Data_over_MC[i]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_Data_over_MC[i]->SetMarkerColor(color+1);
            else h_per_local_board_Data_over_MC[i]->SetMarkerColor(color);
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_per_local_board_Data_over_MC[i]->SetLineColor(color+1);
            else h_per_local_board_Data_over_MC[i]->SetLineColor(color);

            h_per_local_board_Data_over_MC[i]->SetMarkerStyle(8);
            h_per_local_board_Data_over_MC[i]->SetMarkerSize(0.5);
            if(i ==0 )h_per_local_board_Data_over_MC[i]->DrawCopy("");
            else h_per_local_board_Data_over_MC[i]->DrawCopy("Same");
        }

        c->cd(4);
        legend->Draw("Same");
    }

}

//______________________________________________
void LptAptForDistributionRangeVersusRange(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC)
{
    /// Compute and print results according to the range
    /// if projection_axis == 1, we compute results for each eta intervals
    /// if projection_axis == 2, we compute results for each pt intervals

    // if we project on pt (1) , nbin = nEtaBins and vice versa
    int nbin = projection_axis == 1 ? nEtaBins : nPtBins;

    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;

    TH1* h_proj_Data_lpt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_Data_apt[nbin][centralityArray->GetEntries()];
    TGraphAsymmErrors* h_proj_Data_lpt_apt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_MC_lpt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_MC_apt[nbin][centralityArray->GetEntries()];
    TGraphAsymmErrors* h_proj_MC_lpt_apt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_Data_over_MC[nbin][centralityArray->GetEntries()];

    for (int i = 0; i < nbin; ++i)
    {
        for (int j = 0; j < centralityArray->GetEntries(); ++j)
        {
            h_proj_Data_lpt[i][j]     =0x0;
            h_proj_Data_apt[i][j]     =0x0;
            h_proj_Data_lpt_apt[i][j] =0x0;
            h_proj_MC_lpt[i][j]       =0x0;
            h_proj_MC_apt[i][j]       =0x0;
            h_proj_MC_lpt_apt[i][j]   =0x0;
            h_proj_Data_over_MC[i][j] =0x0;
        }
    }

    for (int i = 0; i < nbin; ++i)
    {

        TCanvas* c =new TCanvas(Form("can_%d",i),Form("can_%d",i),200,10,600,600);
        c->Divide(2,2);
        TLegend* legend = new TLegend(0.20,0.20,0.40,0.40);
        if( projection_axis == 1 ) legend->SetHeader(Form("%.1f < y < %.1f",etarange[i],etarange[i+1]));
        else legend->SetHeader(Form("%f < y < %f",ptrange[i],ptrange[i+1]));


        // --- Print Results for each centrality  ---

        int ncent =0;
        nextCentrality.Reset();
        while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
        {

            // --- Get lpt and apt ThnSparse from data ---

            THnSparse* h_Data_apt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_apt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_apt){
                printf("cannot get h_Data_apt histo %s \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_Data_lpt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_lpt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_lpt){
                printf("cannot get h_Data_lpt histo %s \n",scentrality->String().Data());
                return;
            }

            // --- Get lpt and apt ThnSparse from MC ---

            THnSparse* h_MC_apt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_apt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_apt){
                printf("cannot get h_MC_apt histo %s \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_MC_lpt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_lpt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_lpt){
                printf("cannot get h_MC_lpt histo %s \n",scentrality->String().Data());
                return;
            }

            // --- Get TH1 ---

            h_proj_Data_lpt[i][ncent] = GetRangeResponseFromThnSparse(h_Data_lpt,i);
            h_proj_Data_lpt[i][ncent]->Rebin(2);
            h_proj_Data_apt[i][ncent] = GetRangeResponseFromThnSparse(h_Data_apt,i);
            h_proj_Data_apt[i][ncent]->Rebin(2);

            // --- Get TH1 ---

            h_proj_MC_lpt[i][ncent] = GetRangeResponseFromThnSparse(h_MC_lpt,i);
            h_proj_MC_lpt[i][ncent]->Rebin(2);
            h_proj_MC_apt[i][ncent] = GetRangeResponseFromThnSparse(h_MC_apt,i);
            h_proj_MC_apt[i][ncent]->Rebin(2);

            // --- Compute and save lpt/apt form data ---

            h_proj_Data_lpt_apt[i][ncent] = new TGraphAsymmErrors(h_proj_Data_lpt[i][ncent],h_proj_Data_apt[i][ncent],"cpe0");
            TAxis * axis = ((TAxis*)h_proj_Data_apt[i][ncent]->GetXaxis()->Clone());
            h_proj_Data_lpt[i][ncent]->Divide(h_proj_Data_apt[i][ncent]);
            if(projection_axis ==1) h_proj_Data_lpt_apt[i][ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f",etarange[i],etarange[i+1]));
            if(projection_axis ==2) h_proj_Data_lpt_apt[i][ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f",ptrange[i],ptrange[i+1]));
            axis->SetName(Form("%s_axis",h_proj_Data_lpt_apt[i][ncent]->GetName()));
            SaveObject(Data,((TObject*)axis),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            c->cd(1);
            gPad->SetPad(0., 0.5, 0.5, 1);
            h_proj_Data_lpt_apt[i][ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_Data_lpt_apt[i][ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            int color = ncent+1;
            color= color - (color/49)*49;

            if(color==0 || color ==10|| color ==19 || color ==5 ) h_proj_Data_lpt_apt[i][ncent]->SetMarkerColor(color+10);
            else h_proj_Data_lpt_apt[i][ncent]->SetMarkerColor(color);
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_proj_Data_lpt_apt[i][ncent]->SetLineColor(color+10);
            else h_proj_Data_lpt_apt[i][ncent]->SetLineColor(color);
            legend->AddEntry(h_proj_Data_lpt_apt[i][ncent],scentrality->String().Data(),"pe");
            if(ncent ==0 )h_proj_Data_lpt_apt[i][ncent]->Draw("AP");
            else h_proj_Data_lpt_apt[i][ncent]->Draw("PSame");

            SaveObject(Data,((TObject*)h_proj_Data_lpt_apt[i][ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            // --- Compute and save lpt/apt form MC ---

            h_proj_MC_lpt_apt[i][ncent] = new TGraphAsymmErrors(h_proj_MC_lpt[i][ncent],h_proj_MC_apt[i][ncent],"cpe0");
            TAxis * axis2 = ((TAxis*)h_proj_MC_apt[i][ncent]->GetXaxis()->Clone());
            h_proj_MC_lpt[i][ncent]->Divide(h_proj_MC_apt[i][ncent]);
            if(projection_axis ==1) h_proj_MC_lpt_apt[i][ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f",etarange[i],etarange[i+1]));
            if(projection_axis ==2) h_proj_MC_lpt_apt[i][ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f",ptrange[i],ptrange[i+1]));
            axis2->SetName(Form("%s_axis",h_proj_MC_lpt_apt[i][ncent]->GetName()));
            SaveObject(MC,((TObject*)axis2),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            c->cd(2);
            gPad->SetPad(0.5, 0.5, 1, 1);
            h_proj_MC_lpt_apt[i][ncent]->GetYaxis()->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_MC_lpt_apt[i][ncent]->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_MC_lpt_apt[i][ncent]->SetMarkerColor(color+10);
            else h_proj_MC_lpt_apt[i][ncent]->SetMarkerColor(color);
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_MC_lpt_apt[i][ncent]->SetLineColor(color+10);
            else h_proj_MC_lpt_apt[i][ncent]->SetLineColor(color);
            if(ncent ==0 )h_proj_MC_lpt_apt[i][ncent]->Draw("AP");
            else h_proj_MC_lpt_apt[i][ncent]->Draw("PSame");

            SaveObject(MC,((TObject*)h_proj_MC_lpt_apt[i][ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            // --- Compute and Print lpt_data/apt_data  / lpt_mc/apt_mc

            // h_proj_Data_over_MC[i][ncent]= new TGraphAsymmErrors(h_proj_Data_lpt[i][ncent],h_proj_MC_lpt[i][ncent],"cpe0");
            h_proj_Data_over_MC[i][ncent]= ((TH1*)h_proj_Data_lpt[i][ncent]->Clone());
            h_proj_Data_over_MC[i][ncent]->Divide(h_proj_MC_lpt[i][ncent]);
            if( projection_axis == 1 ) h_proj_Data_over_MC[i][ncent]->SetName(Form("lpt_apt_DataOverMC_pt_%.2f_%.2f_eta_%.2f_%.2f",ptrange[0],ptrange[nPtBins],etarange[ncent],etarange[i+1]));
            if( projection_axis == 2 ) h_proj_Data_over_MC[i][ncent]->SetName(Form("lpt_apt_DataOverMC_pt_%.2f_%.2f_eta_%.2f_%.2f",ptrange[ncent],ptrange[i+1],etarange[0],etarange[nEtaBins]));

            c->cd(4);
            gPad->SetPad(0., 0., 1., 0.5);
            h_proj_Data_over_MC[i][ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_Data_over_MC[i][ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_Data_over_MC[i][ncent]->SetMarkerColor(color+10);
            else h_proj_Data_over_MC[i][ncent]->SetMarkerColor(color);
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_Data_over_MC[i][ncent]->SetLineColor(color+10);
            else h_proj_Data_over_MC[i][ncent]->SetLineColor(color);

            h_proj_Data_over_MC[i][ncent]->SetMarkerStyle(8);
            h_proj_Data_over_MC[i][ncent]->SetMarkerSize(0.5);
            if( ncent == 0 )h_proj_Data_over_MC[i][ncent]->DrawCopy("");
            else h_proj_Data_over_MC[i][ncent]->DrawCopy("Same");
            ++ncent;

        }

        c->cd(4);
        legend->Draw("AP");
    }

    TFile* f = new TFile("lpt_apt_range_results.root","RECREATE");
    for (int i = 0; i < nbin; ++i)
    {
        for (int j = 0; j < centralityArray->GetEntries(); ++j)
        {
            h_proj_Data_lpt_apt[i][j]->Write(Form("%s_Data_%s",h_proj_Data_lpt_apt[i][j]->GetName(),centralityArray->At(j)->GetName()));
            h_proj_MC_lpt_apt[i][j]->Write(Form("%s_MC_%s",h_proj_Data_lpt_apt[i][j]->GetName(),centralityArray->At(j)->GetName()));
        }
    }
    f->Close();
}
//______________________________________________
void LptAptForDistributionRangeVersusCentrality(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC)
{
    /// Compute and print results according to the range
    /// if projection_axis == 1, we compute results for each eta intervals
    /// if projection_axis == 2, we compute results for each pt intervals

    // if we project on pt (1) , nbin = nEtaBins and vice versa
    int nbin = projection_axis == 1 ? nEtaBins : nPtBins;

    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;

    TH1* h_proj_Data_lpt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_Data_apt[nbin][centralityArray->GetEntries()];
    TGraphAsymmErrors* h_proj_Data_lpt_apt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_MC_lpt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_MC_apt[nbin][centralityArray->GetEntries()];
    TGraphAsymmErrors* h_proj_MC_lpt_apt[nbin][centralityArray->GetEntries()];
    TH1* h_proj_Data_over_MC[nbin][centralityArray->GetEntries()];

    for (int i = 0; i < nbin; ++i)
    {
        for (int j = 0; j < centralityArray->GetEntries(); ++j)
        {
            h_proj_Data_lpt[i][j]     =0x0;
            h_proj_Data_apt[i][j]     =0x0;
            h_proj_Data_lpt_apt[i][j] =0x0;
            h_proj_MC_lpt[i][j]       =0x0;
            h_proj_MC_apt[i][j]       =0x0;
            h_proj_MC_lpt_apt[i][j]   =0x0;
            h_proj_Data_over_MC[i][j] =0x0;
        }
    }

    int ncent =0;
    nextCentrality.Reset();
    while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
    {

        TCanvas* c =new TCanvas(Form("can_%s",scentrality->String().Data()),Form("can_%s",scentrality->String().Data()),200,10,600,600);
        c->Divide(2,2);
        TLegend* legend = new TLegend(0.20,0.20,0.40,0.40);
        legend->SetHeader(Form("%s",scentrality->String().Data()));


        // --- Print Results for each centrality  ---

        for (int i = 0; i < nbin; ++i)
        {

            // --- Get lpt and apt ThnSparse from data ---

            THnSparse* h_Data_apt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_apt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_apt){
                printf("cannot get h_Data_apt histo %s \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_Data_lpt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_lpt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_lpt){
                printf("cannot get h_Data_lpt histo %s \n",scentrality->String().Data());
                return;
            }

            // --- Get lpt and apt ThnSparse from MC ---

            THnSparse* h_MC_apt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_apt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_apt){
                printf("cannot get h_MC_apt histo %s \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_MC_lpt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_lpt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_lpt){
                printf("cannot get h_MC_lpt histo %s \n",scentrality->String().Data());
                return;
            }

            // --- Get TH1 ---

            h_proj_Data_lpt[i][ncent] = GetRangeResponseFromThnSparse(h_Data_lpt,i);
            h_proj_Data_lpt[i][ncent]->Rebin(2);
            h_proj_Data_apt[i][ncent] = GetRangeResponseFromThnSparse(h_Data_apt,i);
            h_proj_Data_apt[i][ncent]->Rebin(2);

            // --- Get TH1 ---

            h_proj_MC_lpt[i][ncent] = GetRangeResponseFromThnSparse(h_MC_lpt,i);
            h_proj_MC_lpt[i][ncent]->Rebin(2);
            h_proj_MC_apt[i][ncent] = GetRangeResponseFromThnSparse(h_MC_apt,i);
            h_proj_MC_apt[i][ncent]->Rebin(2);

            // --- Compute and save lpt/apt form data ---

            h_proj_Data_lpt_apt[i][ncent] = new TGraphAsymmErrors(h_proj_Data_lpt[i][ncent],h_proj_Data_apt[i][ncent],"cpe0");
            TAxis * axis = ((TAxis*)h_proj_Data_apt[i][ncent]->GetXaxis()->Clone());
            h_proj_Data_lpt[i][ncent]->Divide(h_proj_Data_apt[i][ncent]);
            if(projection_axis ==1) h_proj_Data_lpt_apt[i][ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f",etarange[i],etarange[i+1]));
            if(projection_axis ==2) h_proj_Data_lpt_apt[i][ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f",ptrange[i],ptrange[i+1]));
            axis->SetName(Form("%s_axis",h_proj_Data_lpt_apt[i][ncent]->GetName()));
            SaveObject(Data,((TObject*)axis),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            c->cd(1);
            gPad->SetPad(0., 0.5, 0.5, 1);
            h_proj_Data_lpt_apt[i][ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_Data_lpt_apt[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_Data_lpt_apt[i][ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            int color = i+1;
            color= color - (color/49)*49;

            if(color==0 || color ==10|| color ==19 || color ==5 ) h_proj_Data_lpt_apt[i][ncent]->SetMarkerColor(color+10);
            else h_proj_Data_lpt_apt[i][ncent]->SetMarkerColor(color);
            if(color==0 || color ==10|| color ==19 || color ==5 ) h_proj_Data_lpt_apt[i][ncent]->SetLineColor(color+10);
            else h_proj_Data_lpt_apt[i][ncent]->SetLineColor(color);

            if(projection_axis==1)legend->AddEntry(h_proj_Data_lpt_apt[i][ncent],Form("%.2f < y < %.2f",etarange[i],etarange[i+1]),"pe");
            if(projection_axis==2)legend->AddEntry(h_proj_Data_lpt_apt[i][ncent],Form("%.2f < y < %.2f",ptrange[i],ptrange[i+1]),"pe");

            if(i ==0 )h_proj_Data_lpt_apt[i][ncent]->Draw("AP");
            else h_proj_Data_lpt_apt[i][ncent]->Draw("PSame");

            SaveObject(Data,((TObject*)h_proj_Data_lpt_apt[i][ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            // --- Compute and save lpt/apt form MC ---

            h_proj_MC_lpt_apt[i][ncent] = new TGraphAsymmErrors(h_proj_MC_lpt[i][ncent],h_proj_MC_apt[i][ncent],"cpe0");
            TAxis * axis2 = ((TAxis*)h_proj_MC_apt[i][ncent]->GetXaxis()->Clone());
            h_proj_MC_lpt[i][ncent]->Divide(h_proj_MC_apt[i][ncent]);
            if(projection_axis ==1) h_proj_MC_lpt_apt[i][ncent]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f",etarange[i],etarange[i+1]));
            if(projection_axis ==2) h_proj_MC_lpt_apt[i][ncent]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f",ptrange[i],ptrange[i+1]));
            axis2->SetName(Form("%s_axis",h_proj_MC_lpt_apt[i][ncent]->GetName()));
            SaveObject(MC,((TObject*)axis2),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            c->cd(2);
            gPad->SetPad(0.5, 0.5, 1, 1);
            h_proj_MC_lpt_apt[i][ncent]->GetYaxis()->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_MC_lpt_apt[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_MC_lpt_apt[i][ncent]->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_MC_lpt_apt[i][ncent]->SetMarkerColor(color+10);
            else h_proj_MC_lpt_apt[i][ncent]->SetMarkerColor(color);
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_MC_lpt_apt[i][ncent]->SetLineColor(color+10);
            else h_proj_MC_lpt_apt[i][ncent]->SetLineColor(color);
            if(i ==0 )h_proj_MC_lpt_apt[i][ncent]->Draw("AP");
            else h_proj_MC_lpt_apt[i][ncent]->Draw("PSame");

            SaveObject(MC,((TObject*)h_proj_MC_lpt_apt[i][ncent]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/Range",scentrality->String().Data()));

            // --- Compute and Print lpt_data/apt_data  / lpt_mc/apt_mc

            // h_proj_Data_over_MC[i][ncent]= new TGraphAsymmErrors(h_proj_Data_lpt[i][ncent],h_proj_MC_lpt[i][ncent],"cpe0");
            h_proj_Data_over_MC[i][ncent]= ((TH1*)h_proj_Data_lpt[i][ncent]->Clone());
            h_proj_Data_over_MC[i][ncent]->Divide(h_proj_MC_lpt[i][ncent]);
            if( projection_axis == 1 ) h_proj_Data_over_MC[i][ncent]->SetName(Form("lpt_apt_DataOverMC_pt_%.2f_%.2f_eta_%.2f_%.2f",ptrange[0],ptrange[nPtBins],etarange[ncent],etarange[i+1]));
            if( projection_axis == 2 ) h_proj_Data_over_MC[i][ncent]->SetName(Form("lpt_apt_DataOverMC_pt_%.2f_%.2f_eta_%.2f_%.2f",ptrange[ncent],ptrange[i+1],etarange[0],etarange[nEtaBins]));

            c->cd(4);
            gPad->SetPad(0., 0., 1., 0.5);
            h_proj_Data_over_MC[i][ncent]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetRangeUser(0,4.);
            if(projection_axis==1) h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetTitle("p_{T}");
            if(projection_axis==2) h_proj_Data_over_MC[i][ncent]->GetXaxis()->SetTitle("y");
            h_proj_Data_over_MC[i][ncent]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_Data_over_MC[i][ncent]->SetMarkerColor(color+10);
            else h_proj_Data_over_MC[i][ncent]->SetMarkerColor(color);
            if( color == 0 || color == 10 || color == 19 || color == 5 ) h_proj_Data_over_MC[i][ncent]->SetLineColor(color+10);
            else h_proj_Data_over_MC[i][ncent]->SetLineColor(color);

            h_proj_Data_over_MC[i][ncent]->SetMarkerStyle(8);
            h_proj_Data_over_MC[i][ncent]->SetMarkerSize(0.5);
            if( i == 0 )h_proj_Data_over_MC[i][ncent]->DrawCopy("");
            else h_proj_Data_over_MC[i][ncent]->DrawCopy("Same");
        }

        ++ncent;
        c->cd(4);
        legend->Draw("AP");
    }

    TFile* f = new TFile("lpt_apt_range_results.root","RECREATE");
    for (int i = 0; i < nbin; ++i)
    {
        for (int j = 0; j < centralityArray->GetEntries(); ++j)
        {
            h_proj_Data_lpt_apt[i][j]->Write(Form("%s_Data_%s",h_proj_Data_lpt_apt[i][j]->GetName(),centralityArray->At(j)->GetName()));
            h_proj_MC_lpt_apt[i][j]->Write(Form("%s_MC_%s",h_proj_Data_lpt_apt[i][j]->GetName(),centralityArray->At(j)->GetName()));
        }
    }
    f->Close();
}

//______________________________________________
void LptAptPerLocalBoardGroups(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC)
{
    /// Compute and print results for all local board groups


    // From 2015 pp@13TeV analysis, to be adapted
//   double group[17][34] = 
//   {
//     {1,2,3,4,5,6,17,18,19,20,21,22,23,26,39,40,41,42,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {121,122,123,124,134,135,136,137,138,139,140,156,157,158,159,160,161,162,178,179,180,181,182,0,0,0,0,0,0,0,0,0,0,0},
//     {163,212,228,229,230,231,232,233,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {145,148,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {7,8,9,24,27,28,29,30,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {10,11,12,13,14,15,16,25,32,33,34,35,36,37,38,49,50,51,52,53,54,55,56,0,0,0,0,0,0,0,0,0,0,0},
//     {66,67,68,69,71,76,103,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {111,112,125,127,128,129,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {133,141,142,143,149,150,151,152,153,154,155,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {165,166,167,168,169,170,171,172,173,174,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {176,177,184,185,186,187,188,189,199,200,201,202,203,204,205,206,207,208,209,216,217,218,219,220,221,227,0,0,0,0,0,0,0,0},
//     {43,44,45,46,47,48,57,58,59,60,61,62,63,64,65,70,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {72,73,74,75,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,94,95,96,97,98,99,100,101,102,104,105,106,107,110},
//     {113,114,115,116,118,119,126,222,223,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {131,132,144,146,147,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//     {164,175,183,190,191,192,193,194,195,196,197,198,213,214,215,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//         {93,108,109,117,210,211,224,225,226,234,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
// };

   int group[6][48] = {
    {26,27,28,29,48,49,50,51,165,166,167,168,143,144,145,146,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {6,7,8,22,23,24,25,44,45,46,47,9,10,11,30,31,32,33,52,53,54,55,123,124,125,139,140,141,142,161,162,163,164,169,170,171,172,147,148,149,150,126,127,128,0,0,0,0},
    {215,216,217,218,219,220,199,200,201,202,203,204,183,184,185,186,187,188,66,67,68,69,70,82,83,84,85,86,87,98,99,100,101,102,103,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {213,214,197,198,181,182,159,160,137,138,121,122,4,5,20,21,42,43,64,65,80,81,96,97,221,222,205,206,189,190,173,174,151,152,129,130,12,13,34,35,56,57,72,73,88,89,104,105},
    {211,212,195,196,179,180,157,158,135,136,119,120,2,3,18,19,40,41,62,63,78,79,94,95,223,224,207,208,191,192,175,176,153,154,131,132,14,15,36,37,58,59,74,75,90,91,106,107},
    {1,17,39,61,77,93,109,109,110,111,112,113,114,115,116,117,108,92,76,60,38,16,133,155,177,193,209,225,234,233,232,231,230,229,228,227,226,0,0,0,0,0,0}
  };

    AliLog::SetGlobalDebugLevel(debug);

    // TH1* h_per_group_Data_lpt[17]                       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TH1* h_per_group_Data_apt[17]                       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TGraphAsymmErrors* h_per_group_Data_lpt_apt[17]     = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TH1* h_per_group_MC_lpt[17]                         = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TH1* h_per_group_MC_apt[17]                         = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TGraphAsymmErrors* h_per_group_MC_lpt_apt[17]       = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    // TH1* h_per_group_Data_over_MC[17]                   = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

    TH1* h_per_group_Data_lpt[6]                       = {0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_group_Data_apt[6]                       = {0x0,0x0,0x0,0x0,0x0,0x0};
    TGraphAsymmErrors* h_per_group_Data_lpt_apt[6]     = {0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_group_MC_lpt[6]                         = {0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_group_MC_apt[6]                         = {0x0,0x0,0x0,0x0,0x0,0x0};
    TGraphAsymmErrors* h_per_group_MC_lpt_apt[6]       = {0x0,0x0,0x0,0x0,0x0,0x0};
    TH1* h_per_group_Data_over_MC[6]                   = {0x0,0x0,0x0,0x0,0x0,0x0};


    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;

    // --- Print Results for each local board groups ---

    for (int i = 0; i < 6; ++i)
    {
        TCanvas* c =new TCanvas(Form("can%d",i),Form("can%d",i),200,10,600,600);
        c->Divide(2,2);
        TLegend* legend = new TLegend(0.20,0.20,0.40,0.40);
        legend->SetHeader(Form("Group n %d",i+1));

        int ncent = 0;
        nextCentrality.Reset();

        // Loop on each centrality
        while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
        {
            // --- Get lpt and apt ThnSparse  ---

            THnSparse* h_Data_apt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_apt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_apt){
                printf("cannot get h_Data_apt ThnSparse in /PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_Data_lpt=0x0;
            if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_Data_lpt = static_cast<THnSparse*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_Data_lpt){
                printf("cannot get h_Data_lpt ThnSparse in /PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_MC_apt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_apt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_apt){
                printf("cannot get h_MC_apt ThnSparse in /PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHANYETAPDCA \n",scentrality->String().Data());
                return;
            }

            THnSparse* h_MC_lpt=0x0;
            if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"))
                h_MC_lpt = static_cast<THnSparse*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA",scentrality->String().Data()),"HitperTriggerLocalBoardMu"));
            if(!h_MC_lpt){
                printf("cannot get h_MC_lpt ThnSparse in /PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/sALLRABSMATCHLOWETAPDCA \n",scentrality->String().Data());
                return;
            }

            // loop over the max number of local board per group
            for (int j = 0; j < 48; ++j)
            {
                // Check if we have a local board
                if(group[i][j]==0) continue;
                // printf("group[%d][%d] = %d\n",i,j,group[i][j]);
                TH1* lpt_Data = GetLocalBoardResponseFromThnSparse(h_Data_lpt,group[i][j]);
                TH1* Apt_Data = GetLocalBoardResponseFromThnSparse(h_Data_apt,group[i][j]);

                TH1* lpt_MC = GetLocalBoardResponseFromThnSparse(h_MC_lpt,group[i][j]);
                TH1* Apt_MC = GetLocalBoardResponseFromThnSparse(h_MC_apt,group[i][j]);

                // double nlptData = lpt_Data->GetEntries();
                // double nlptMC   = lpt_MC->GetEntries();
                // Printf("nlptData : %f",nlptData);
                // Printf("nlptMC : %f",nlptMC);

                // if (nlptData > 0.)
                // {
                //     lpt_MC->Scale(nlptData/nlptMC);
                //     Apt_MC->Scale(nlptData/nlptMC);
                // }

                if (j==0) h_per_group_Data_lpt[i] = lpt_Data;
                else h_per_group_Data_lpt[i]->Add(lpt_Data);
                // h_per_group_Data_lpt[i]->Rebin(2);
                if (j==0) h_per_group_Data_apt[i] = Apt_Data;
                else h_per_group_Data_apt[i]->Add(Apt_Data);
                // h_per_group_Data_apt[i]->Rebin(2);


                // --- Get TH1 ---

                if (j==0) h_per_group_MC_lpt[i] = lpt_MC;
                else h_per_group_MC_lpt[i]->Add(lpt_MC);
                // h_per_group_MC_lpt[i]->Rebin(2);
                if (j==0) h_per_group_MC_apt[i] = Apt_MC;
                else h_per_group_MC_apt[i]->Add(Apt_MC);
                // h_per_group_MC_apt[i]->Rebin(2);
            }

            // --- Compute and save lpt/apt form data ---

            h_per_group_Data_lpt_apt[i] =new TGraphAsymmErrors(h_per_group_Data_lpt[i],h_per_group_Data_apt[i],"cpe0");
            TAxis * axis = ((TAxis*)h_per_group_Data_apt[i]->GetXaxis()->Clone());
            h_per_group_Data_lpt[i]->Divide(h_per_group_Data_apt[i]);
            if(projection_axis ==1) h_per_group_Data_lpt_apt[i]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_group_%d",etarange[0],etarange[nEtaBins],i+1));
            if(projection_axis ==2) h_per_group_Data_lpt_apt[i]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_group_%d",ptrange[0],ptrange[nPtBins],i+1));
            axis->SetName(Form("%s_axis",h_per_group_Data_lpt_apt[i]->GetName()));
            SaveObject(Data,((TObject*)axis),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoardGroup",scentrality->String().Data()));
            SaveObject(Data,((TObject*)h_per_group_Data_lpt_apt[i]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoardGroup",scentrality->String().Data()));

            c->cd(1);
            h_per_group_Data_lpt_apt[i]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            h_per_group_Data_lpt_apt[i]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T}");
            h_per_group_Data_lpt_apt[i]->SetMarkerColor(ncent+1);
            h_per_group_Data_lpt_apt[i]->SetLineColor(ncent+1);
            legend->AddEntry(h_per_group_Data_lpt_apt[i],scentrality->String().Data(),"pe");
            if(ncent ==0 )h_per_group_Data_lpt_apt[i]->Draw("AP");
            else h_per_group_Data_lpt_apt[i]->Draw("PSame");

            // --- Compute and save lpt/apt form MC ---

            h_per_group_MC_lpt_apt[i] =new TGraphAsymmErrors(h_per_group_MC_lpt[i],h_per_group_MC_apt[i],"cpe0");
            TAxis * axis2 = ((TAxis*)h_per_group_MC_apt[i]->GetXaxis()->Clone());
            h_per_group_MC_lpt[i]->Divide(h_per_group_MC_apt[i]);

            if(projection_axis ==1) h_per_group_MC_lpt_apt[i]->SetName(Form("lpt_apt_pt_eta_%.2f_%.2f_group_%d",etarange[0],etarange[nEtaBins],i+1));
            if(projection_axis ==2) h_per_group_MC_lpt_apt[i]->SetName(Form("lpt_apt_eta_pt_%.2f_%.2f_group_%d",ptrange[0],ptrange[nPtBins],i+1));
            axis2->SetName(Form("%s_axis",h_per_group_MC_lpt_apt[i]->GetName()));
            SaveObject(MC,((TObject*)axis2),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoardGroup",scentrality->String().Data()));
            SaveObject(MC,((TObject*)h_per_group_MC_lpt_apt[i]),Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/LocalBoardGroup",scentrality->String().Data()));

            c->cd(2);
            h_per_group_MC_lpt_apt[i]->GetYaxis()->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_group_MC_lpt_apt[i]->SetTitle("L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_group_MC_lpt_apt[i]->SetMarkerColor(ncent+1);
            h_per_group_MC_lpt_apt[i]->SetLineColor(ncent+1);
            if(ncent ==0 )h_per_group_MC_lpt_apt[i]->Draw("AP");
            else h_per_group_MC_lpt_apt[i]->Draw("PSame");


            // --- Compute and Print lpt_data/apt_data  / lpt_mc/apt_mc

            h_per_group_Data_over_MC[i]= ((TH1*)h_per_group_Data_lpt[i]->Clone());
            h_per_group_Data_over_MC[i]->Divide(h_per_group_MC_lpt[i]);
            if(projection_axis ==1) h_per_group_Data_over_MC[i]->SetName(Form("lpt_apt_DataOversMC_pt_eta_%.2f_%.2f",etarange[0],etarange[nEtaBins]));
            if(projection_axis ==2) h_per_group_Data_over_MC[i]->SetName(Form("lpt_apt_DataOversMC_eta_pt_%.2f_%.2f",ptrange[0],ptrange[nPtBins]));

            c->cd(4);
            gPad->SetPad(0., 0., 1., 0.5);
            h_per_group_Data_over_MC[i]->GetYaxis()->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_group_Data_over_MC[i]->SetTitle("L^{Data}_{p_T}/A^{Data}_{p_T} / L^{MC}_{p_T}/A^{MC}_{p_T}");
            h_per_group_Data_over_MC[i]->SetMarkerColor(ncent+1);
            h_per_group_Data_over_MC[i]->SetLineColor(ncent+1);
            h_per_group_Data_over_MC[i]->SetMarkerStyle(8);
            h_per_group_Data_over_MC[i]->SetMarkerSize(0.5);
            if(ncent ==0 )h_per_group_Data_over_MC[i]->DrawCopy("");
            else h_per_group_Data_over_MC[i]->DrawCopy("Same");
            ++ncent;
        }

        c->cd(4);
        legend->Draw("Same");
    }

}

//______________________________________________
void CleanAllHisto(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC)
{
    /// Since all stored results are TGraphAsymmErrors...
    Data->OC()->RemoveByType("TGraphAsymmErrors");
    Data->OC()->RemoveByType("TAxis");
    MC->OC()->RemoveByType("TGraphAsymmErrors");
    MC->OC()->RemoveByType("TAxis");
}


//______________________________________________
TH1* GetLocalBoardResponseFromThnSparse(THnSparse* hnsparse, int localboard )
{
    /// return a projection of the ThnSparse

    //Pt projection
    Int_t binminpt = hnsparse->GetAxis(1)->FindBin(ptrange[0]);
    Int_t binmaxpt = hnsparse->GetAxis(1)->FindBin(ptrange[nPtBins]);
    hnsparse->GetAxis(1)->SetRange(binminpt,binmaxpt);

    //eta projection
    Int_t binmineta = hnsparse->GetAxis(2)->FindBin(etarange[0]);
    Int_t binmaxeta = hnsparse->GetAxis(2)->FindBin(etarange[nEtaBins]);
    hnsparse->GetAxis(2)->SetRange(binmineta,binmaxeta);

    TH2D* h2 = hnsparse->Projection(0,projection_axis);
    if (h2) return ((TH1*)h2->ProjectionX("_px",localboard,localboard)->Clone());
    else return 0x0;

}

//______________________________________________
TH1* GetRangeResponseFromThnSparse(THnSparse* hnsparse, int bin)
{
    /// return a projection of the ThnSparse

    if ( projection_axis < 1 || projection_axis > 2)
    {
        printf("projection_axis should be 1 or 2\n");
        return 0x0;
    }

    // projection on the non-selected axis
    Int_t binmin = projection_axis == 1 ? hnsparse->GetAxis(2)->FindBin(etarange[bin])   : hnsparse->GetAxis(1)->FindBin(ptrange[bin]);
    // printf("binmin 1  = %d\n",binmin );
    Int_t binmax = projection_axis == 1 ? hnsparse->GetAxis(2)->FindBin(etarange[bin+1]) : hnsparse->GetAxis(1)->FindBin(ptrange[bin+1]);
    // printf("binmax 1  = %d\n",binmax );
    if ( projection_axis == 1 ) hnsparse->GetAxis(2)->SetRange(binmin,binmax);
    else                        hnsparse->GetAxis(1)->SetRange(binmin,binmax);

    // projection on the selected axis
    binmin = projection_axis == 1 ? hnsparse->GetAxis(1)->FindBin(ptrange[0])   : hnsparse->GetAxis(1)->FindBin(etarange[0]);
    // printf("binmin 2  = %d\n",binmin );
    binmax = projection_axis == 1 ? hnsparse->GetAxis(1)->FindBin(ptrange[nPtBins]) : hnsparse->GetAxis(1)->FindBin(etarange[nEtaBins]);
    // printf("binmax 2  = %d\n",binmax );
    if ( projection_axis == 1 ) hnsparse->GetAxis(1)->SetRange(binmin,binmax);
    else                        hnsparse->GetAxis(2)->SetRange(binmin,binmax);

    TH2D* h2 = hnsparse->Projection(0,projection_axis);
    if (h2) return ((TH1*)h2->ProjectionX()->Clone());
    else return 0x0;

}

//______________________________________________
void SaveObject(AliAnalysisMuMu* ali, TObject* obj, char* path)
{
    /// save Object in the mergeable collection of ali

    TObject* o = ali->OC()->GetObject(Form("%s/%s",path,obj->GetName()));
    if (o) ali->OC()->Remove(Form("%s/%s",path,obj->GetName()));
    Bool_t adoptOK = ali->OC()->Adopt(Form("%s",path),obj);

    if ( adoptOK ) std::cout << "\n" << "+++ " << obj->GetName() << " adopted in " << path << std::endl;
    else printf("Could not adopt  %s\n",obj->GetName());
}

//______________________________________________
Double_t FitFuncErfFixed ( Double_t* xVal, Double_t* par )
{
  Double_t xx = xVal[0];
  Double_t currX = TMath::Max(xx,par[6]);
  Double_t sqrtTwo = TMath::Sqrt(2.);
  Double_t yVal = 1.+par[0]*(TMath::Erf((currX-par[1])/par[2]/sqrtTwo)-1.);
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