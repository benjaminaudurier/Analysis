//
//  ComputeTriggerSyst.C
//
//
//  Created by Benjamin Audurier on 20/12/17.
//
//

// Compute lpt/apt ratio per group of local board

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliMergeableCollection.h>
#include <AliCounterCollection.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH3.h>
#include <TLine.h>
#include <TF1.h>
#include <TROOT.h>
#include <THnSparse.h>
#include <iostream>

//Some strings and constants
const char* sfileDataWeight      = "LocalBoardGroup/lptapt_data/AnalysisResults.JPSI.root";
const char* configFileDataWeight = "../Lptapt_single_muons/mumu.lptapt.config";

const char* sfileMCWeight        = "LocalBoardGroup/lptapt_mc/AnalysisResults.JPSI.root";
const char* configFileMCWeight   = "../Lptapt_single_muons/mumu.lptapt.config";

int debug=0;


// Double_t ptBins[] = {0.,2.,5.,8.,12.};
// Double_t ptBins[] = {0.,12.};
// Double_t ptBins[] = {0.,1.,2.,3.,4.,5.,12.};
Double_t ptBins[] = {0.,1.,2.,3.,4.,5.,6.,8.,9.,10.,11.,12.};
Double_t rapidityBins[] = {-4.,-3.5,-3,-2.5};
// Double_t rapidityBins[] = {-4.,-3.75,-3.5,-3.25,-3.00,-2.75,-2.5};
// Double_t rapidityBins[] = {-4.,-3.85,-3.70,-3.55,-3.40,-3.25,-3.10,-2.95,-2.80,-2.65,-2.5};
// Double_t rapidityBins[] = {-4.,-3.7,-3.4,-3.1,-2.8,-2.5};
// Double_t rapidityBins[] = {-4,-3.25};
// Double_t rapidityBins[] = {-3.25,-2.5};

Int_t nRapidityBins = sizeof(rapidityBins)/sizeof(rapidityBins[0]) - 1;
Int_t nPtBins = sizeof(ptBins)/sizeof(ptBins[0]) - 1;

const int projection_axis = 1; // 1 = pt, 2 = eta;
// const TString centrality  ="V0M_00.00_90.00";
const TString centrality  ="V0M_00.00_10.00,V0M_10.00_20.00,V0M_20.00_30.00,V0M_30.00_40.00,V0M_40.00_90.00,V0M_00.00_90.00";

TH1* GetHistoFromTH2(TH2* h2=0x0);
void CleanAllHisto(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC);
void ComputeSyst(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC, TString thnsparseName);


//______________________________
void ComputeTriggerSyst( int mode = 3)
{
    /// mode = 0 : all
    /// mode = 1 : eta range method
    /// mode = 2 : local board method
    /// mode = 3 : local board groups method
    /// mode = 4 : clean

    AliLog::SetGlobalDebugLevel(debug);

    AliAnalysisMuMu WeightData(sfileDataWeight,"","",configFileDataWeight);
    AliAnalysisMuMu WeightMC(sfileMCWeight,"","",configFileMCWeight);


    if ( mode == 1 || mode == 0 ) ComputeSyst(&WeightData,&WeightMC,"_y");
    if ( mode == 2 || mode == 0 ) ComputeSyst(&WeightData,&WeightMC,"_lb");
    if ( mode == 3 || mode == 0 ) ComputeSyst(&WeightData,&WeightMC,"_lbg");
    if ( mode == 4) CleanAllHisto(&WeightData,&WeightMC);

    WeightData.Update();
    WeightMC.Update();
}
//______________________________________________
void ComputeSyst(AliAnalysisMuMu* Data,AliAnalysisMuMu* MC, TString thnsparseName)
{
    /// Compute and print results for all local boards

    TObjArray* centralityArray = centrality.Tokenize(",");
    TIter nextCentrality(centralityArray);
    TObjString* scentrality;

    int nCentBin = centralityArray->GetEntries();

    TH2* hsp_Centrality_WeightData=0x0;
    TH2* hsp_Centrality_WeightData_max=0x0;
    TH2* hsp_Centrality_WeightData_min=0x0;
    TH2* hsp_Centrality_WeightMC=0x0;
    TH2* hsp_Centrality_WeightMC_max=0x0;
    TH2* hsp_Centrality_WeightMC_min=0x0;


    // Make sure we have one histo for each centrality
    TH1* h_Centrality_WeightData[nCentBin];
    TH1* h_Centrality_WeightMC[nCentBin];
    TH1* h_Centrality_WeightData_max[nCentBin];
    TH1* h_Centrality_WeightMC_max[nCentBin];
    TH1* h_Centrality_WeightData_min[nCentBin];
    TH1* h_Centrality_WeightMC_min[nCentBin];
    TGraphAsymmErrors* data_MC_ratio[nCentBin];
    for (int i = 0; i < nCentBin; ++i)
    {
        h_Centrality_WeightData[i]     =0x0;
        h_Centrality_WeightData_max[i] =0x0;
        h_Centrality_WeightData_min[i] =0x0;
        h_Centrality_WeightMC[i]       =0x0;
        h_Centrality_WeightMC_max[i]   =0x0;
        h_Centrality_WeightMC_min[i]   =0x0;
        data_MC_ratio[i]   =0x0;
    }

    // --- Print Results for each centrality  ---
    TCanvas* c =new TCanvas("can","can",10,10,600,600);
    TLegend* legend = new TLegend(0.20,0.20,0.60,0.40);
    if(projection_axis == 1 )legend->SetHeader(Form("Centrality, %.2f < y < %.2f ",-rapidityBins[nRapidityBins],-rapidityBins[0]));
    if(projection_axis == 2 )legend->SetHeader(Form("Centrality, %.2f < pt < %.2f ",ptBins[0],ptBins[nPtBins]));

    int i =0;
    nextCentrality.Reset();

    // Loop on each centrality (see MuMuConfig)
    while ( ( scentrality = static_cast<TObjString*>(nextCentrality()) ) )
    {

        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMu%s",thnsparseName.Data())))
            hsp_Centrality_WeightData = static_cast<TH2*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMu%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightData){
            printf("cannot get hsp_Centrality_WeightData histo \n");
            return;
        }

        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMax%s",thnsparseName.Data())))
            hsp_Centrality_WeightData_max = static_cast<TH2*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMax%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightData_max){
            printf("cannot get hsp_Centrality_WeightData_max histo \n");
            return;
        }

        if(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMin%s",thnsparseName.Data())))
            hsp_Centrality_WeightData_min = static_cast<TH2*>(Data->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMin%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightData_min){
            printf("cannot get hsp_Centrality_WeightData_min histo \n");
            return;
        }

        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMu%s",thnsparseName.Data())))
            hsp_Centrality_WeightMC = static_cast<TH2*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMu%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightMC){
            printf("cannot get hsp_Centrality_WeightMC histo \n");
            return;
        }

        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMax%s",thnsparseName.Data())))
            hsp_Centrality_WeightMC_max = static_cast<TH2*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMax%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightMC_max){
            printf("cannot get hsp_Centrality_WeightMC_max histo \n");
            return;
        }

        if(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMin%s",thnsparseName.Data())))
            hsp_Centrality_WeightMC_min = static_cast<TH2*>(MC->OC()->GetObject(Form("/PSINT7inMUON/CINT7-B-NOPF-MUFAST/%s/pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHANYETAPDCA",scentrality->String().Data()),Form("TriggerHitperLocalBoardMuMuErrorMin%s",thnsparseName.Data())));
        if(!hsp_Centrality_WeightMC_min){
            printf("cannot get hsp_Centrality_WeightMC_min histo \n");
            return;
        }

        h_Centrality_WeightData[i]     = GetHistoFromTH2(hsp_Centrality_WeightData);
        h_Centrality_WeightData_max[i] = GetHistoFromTH2(hsp_Centrality_WeightData_max);
        h_Centrality_WeightData_min[i] = GetHistoFromTH2(hsp_Centrality_WeightData_min);

        h_Centrality_WeightMC[i]       = GetHistoFromTH2(hsp_Centrality_WeightMC);
        h_Centrality_WeightMC_max[i]   = GetHistoFromTH2(hsp_Centrality_WeightMC_max);
        h_Centrality_WeightMC_min[i]   = GetHistoFromTH2(hsp_Centrality_WeightMC_min);


        // Create and fill the Histo


        int jmax = projection_axis == 1 ? nPtBins : nRapidityBins;
        // printf("jmax = %d\n",jmax );
        double pt_graph[jmax];
        double pterr_graph[jmax];
        double y_graph[jmax];
        double ymin_graph[jmax];
        double ymax_graph[jmax];

        for (int j = 0; j < jmax; ++j)
        {
            // Get Data Point
            double data     = h_Centrality_WeightData[i]->GetBinContent(j+1);
            double data_max = h_Centrality_WeightData_max[i]->GetBinContent(j+1);
            double data_min = h_Centrality_WeightData_min[i]->GetBinContent(j+1);

            // Get MC Point
            double MC     = h_Centrality_WeightMC[i]->GetBinContent(j+1);
            double MC_max = h_Centrality_WeightMC_max[i]->GetBinContent(j+1);
            double MC_min = h_Centrality_WeightMC_min[i]->GetBinContent(j+1);

            // Fill Graph
            double x = projection_axis == 1 ? ptBins[j]+ (ptBins[j+1]-ptBins[j])/2 : rapidityBins[j]+ (rapidityBins[j+1]-rapidityBins[j])/2;
            double centralValue = MC > 0 ? data/MC : 0.;
            double maxValue     = MC_min > 0 ? data_max/MC_min : 0.;
            double minValue     = MC_max > 0 ? data_min/MC_max : 1.;
            // printf("data = %f MC = %f centralValue = %f , pt %f , centrality %s\n", data,MC,centralValue,x,scentrality->String().Data());
            // printf("maxValue     = %f\n", maxValue);
            // printf("minValue     = %f\n", minValue);

            double epsilon = projection_axis == 1 ? (ptBins[j+1]-ptBins[j])/(nCentBin+1) : (rapidityBins[j+1]-rapidityBins[j])/(nCentBin+1);
            pt_graph[j]    = projection_axis == 1 ? ptBins[j] + (i+1)*epsilon : rapidityBins[j] + (i+1)*epsilon;
            pterr_graph[j] = projection_axis == 1 ? 0.1 : 0.01;
            y_graph[j]     =centralValue;
            ymax_graph[j]  =maxValue-centralValue;
            ymin_graph[j]  =centralValue-minValue;
        }

        data_MC_ratio[i]= new TGraphAsymmErrors(jmax,pt_graph,y_graph,pterr_graph,pterr_graph,ymin_graph,ymax_graph);
        data_MC_ratio[i]->SetTitle("Trigger Response Syst.");
        data_MC_ratio[i]->GetYaxis()->SetRangeUser(0.7,1.1);
        if (projection_axis == 1 )data_MC_ratio[i]->GetXaxis()->SetTitle("#it{p}_{T}");
        if (projection_axis == 1 )data_MC_ratio[i]->GetXaxis()->SetRangeUser(ptBins[0],ptBins[jmax]);
        if (projection_axis == 2 )data_MC_ratio[i]->GetXaxis()->SetTitle("y");
        if (projection_axis == 2 )data_MC_ratio[i]->GetXaxis()->SetRangeUser(rapidityBins[0],rapidityBins[jmax]);
        data_MC_ratio[i]->GetYaxis()->SetTitle("J/#Psi_{lpt/apt_Data}/J/#Psi_{lpt/apt_MC}");
        data_MC_ratio[i]->GetYaxis()->SetTitleOffset(1.5);
        if(i!=4) {
            data_MC_ratio[i]->SetMarkerColor(i+1);
            data_MC_ratio[i]->SetFillColor(i+1);
            data_MC_ratio[i]->SetLineColor(i+1);
        } else {
            data_MC_ratio[i]->SetMarkerColor(i+4);
            data_MC_ratio[i]->SetFillColor(i+4);
            data_MC_ratio[i]->SetLineColor(i+4);
        }
        data_MC_ratio[i]->SetMarkerStyle(20);
        data_MC_ratio[i]->SetFillStyle(3001);;
        legend->AddEntry(data_MC_ratio[i],scentrality->String().Data(),"p");
        if(i==0)data_MC_ratio[i]->Draw("AP2");
        else data_MC_ratio[i]->Draw("P2same");
        i++;
    }

    double tlinemin = projection_axis == 1 ? ptBins[0] : rapidityBins[0];
    double tlinemax = projection_axis == 1 ? ptBins[nPtBins] : rapidityBins[nRapidityBins];
    TLine* l =new TLine(tlinemin,1,tlinemax,1);
    l->Draw("same");
    legend->DrawClone("same");

    for (int i = 0; i < centralityArray->GetEntries(); ++i)
    {

        if(projection_axis == 1)
        {
            for (int j = 0; j < nPtBins; ++j)
            {
                Double_t xy[2] = {0,0};
                data_MC_ratio[i]->GetPoint(j,xy[0],xy[1]);
                printf("[%.0f,%.0f] & %.1f %%\n",ptBins[j],ptBins[j+1],(xy[1]-1)*100);
            }
        }
        if(projection_axis == 2)
        {
            for (int j = 0; j < nRapidityBins; ++j)
            {
                Double_t xy[2] = {0,0};
                data_MC_ratio[i]->GetPoint(j,xy[0],xy[1]);
                printf("[%.2f,%.2f] & %.1f %%\n",rapidityBins[j],rapidityBins[j+1],(xy[1]-1)*100);
            }
        }
    }

}

//______________________________________________
TH1* GetHistoFromTH2(TH2* h2)
{
    /// return a projection of the ThnSparse

    TH2F* h = new TH2F(Form("%s_projection",h2->GetName()),Form("%s_projection",h2->GetName()),nPtBins,ptBins,nRapidityBins,rapidityBins);
    h->Sumw2();

    for (Int_t iybin=1; iybin<=h2->GetYaxis()->GetNbins(); iybin++)
    {
        Double_t ycenter = h2->GetYaxis()->GetBinCenter(iybin);

        for (Int_t ixbin=1; ixbin<=h2->GetXaxis()->GetNbins(); ixbin++)
        {
            Double_t xcenter     = h2->GetXaxis()->GetBinCenter(ixbin);
            Double_t binContent  = h2->GetBinContent(ixbin,iybin);
            Double_t binError    = h2->GetBinError(ixbin,iybin);

            if( ptBins[nPtBins] < xcenter || xcenter < ptBins[0] || rapidityBins[nRapidityBins] < ycenter || ycenter < rapidityBins[0] ) continue;
            // printf("xcenter =%f, ycenter= %f\n",xcenter,ycenter);
            Int_t fillBin        = h->FindBin(xcenter,ycenter);
            // printf("fillBin : %d\n",fillBin);

            Double_t currContent = h->GetBinContent(fillBin);
            Double_t currErr     = h->GetBinError(fillBin);

            Double_t newContent  = binContent+currContent;
            Double_t newError    = TMath::Sqrt(currErr*currErr + binError*binError);
            h->SetBinContent(fillBin,newContent);
            h->SetBinError(fillBin,newError);
        }
    }

    Int_t projBin[2] = {0,0};
    if (projection_axis == 1){
        projBin[0] = h->GetYaxis()->FindBin(rapidityBins[0]);
        projBin[1] = h->GetYaxis()->FindBin(rapidityBins[nRapidityBins]);
    } else {
        projBin[0] = h->GetXaxis()->FindBin(ptBins[0]);
        projBin[1] = h->GetXaxis()->FindBin(ptBins[nPtBins]);
    }
    // printf(" projBin[0] = %d  -- %d  ------  projBin[1] = %d -- %d\n",projBin[0],projBin[1],h->GetYaxis()->GetBinCenter(projBin[0]),h->GetYaxis()->GetBinCenter(projBin[1]));
    // new TCanvas;
    // h->DrawCopy("colz");
    // h->ProjectionX("_px",projBin[0],projBin[1])->DrawCopy();

    if (h && projection_axis ==1) return ((TH1*)h->ProjectionX("_px",projBin[0],projBin[1])->Clone());
    else if (h && projection_axis ==2) return ((TH1*)h->ProjectionY("_px",projBin[0],projBin[1])->Clone());
    else return 0x0;

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




