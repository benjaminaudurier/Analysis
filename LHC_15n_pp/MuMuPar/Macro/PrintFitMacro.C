//
//  PrintFitMacro.c
//  
//
//  Created by Benjamin Audurier on 09/06/15.
//
//

// Macro who prints fit results

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliAnalysisMuMuResult.h>
#include <AliAnalysisMuMuBinning.h>
#include <AliMergeableCollection.h>
#include <TF1.h>
#include <TLegend.h>
#include <TFitResultPtr.h>
#include <AliCounterCollection.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TROOT.h>
#include <iostream> 

//Some strings and constants
char                   * sfile="../AnalysisResults-0-8.root";
char                   * sasso="../../AccEff_jpsi/MCPart/AnalysisResults.JPSI.root";
char                   * sasso2="";
char                   * beamYear="mumu.pp2015.config";

TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSALL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETAPDCA";
TString scentrality    ="V0A";

// TString param          =  "";
TString param          =  "sJPsi,mJPsi,NofJPsi,SignalOverBkg3s,FitChi2PerNDF";

// TString subresults     =  "CB2POL1POL2_2.4_4.5_SP1.2,CB2POL1POL2_2.0_5.0_SP1.2";
TString subresults     =  "CB2VWG_2.0_5.0_SP1.2,CB2VWG_2.4_4.5_SP1.2,NA60NEWVWG_2.0_5.0_SP1.2,NA60NEWVWG_2.4_4.5_SP1.2,CB2POL1POL2_2.4_4.5_SP1.2,CB2POL1POL2_2.0_5.0_SP1.2";
                        // ,NA60NEWVWG_2.0_5.0_SP1.2,NA60NEWVWG_2.4_4.5_SP1.2 
                        // "CB2VWG_2.4_4.5_SP1.2,CB2VWG_2.0_5.0_SP1.2,CB2POL1POL2_2.4_4.5_SP1.2,CB2POL1POL2_2.0_5.0_SP1.2"
Double_t lumi                    =111.39; //nb^-1
Double_t lumiTriggerCrossSection = 21.0;
Double_t BR                      =0.0596;

void PrintDist(TObjString* swhat,Bool_t yield,AliAnalysisMuMu &ana);
void PrintCrossSection(AliAnalysisMuMuSpectra *spec,AliAnalysisMuMu &an,char*what);

//_____________________________________________________________________________
void PrintFitMacro(char         * what ="PT",const char * printWhat = "distribution",int debug =0)
{    

    AliLog::SetGlobalDebugLevel(debug);
    
    Bool_t PrintDistribution = kFALSE;
    Bool_t Raa               = kFALSE; 
    Bool_t print             = kFALSE;
    Bool_t yield             = kFALSE;

    TObjArray* sprint = TString(printWhat).Tokenize(",");
    TObjArray* sparam = TString(param).Tokenize(",");
    
    //Set bool
    if(sprint->FindObject("raa")) Raa                        =kTRUE;
    if(sprint->FindObject("distribution")) PrintDistribution =kTRUE;
    if(sprint->FindObject("save")) print                     =kTRUE;
    if(sprint->FindObject("yield")) yield                    =kTRUE;


    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;
    
    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    //_____ Draw 
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    {
        analysis.DrawFitResults("PSI",swhat->String().Data(),"histo",print);
        analysis.PrintNofParticle("PSI","NofJPsi",swhat->String(),kFALSE);
        TIter nextParam(sparam);
        TObjString* sParam;
         if(swhat->String().Contains("PT") || swhat->String().Contains("Y")) while ((sParam=static_cast<TObjString*>(nextParam()))) analysis.PrintFitParam("PSI",sParam->String().Data(),swhat->String().Data(),subresults.Data(),"",kFALSE);
        
        if (Raa) {
            if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",scentrality.Data(),kFALSE);
            else continue;
        }

        PrintDist(swhat,kTRUE,analysis);
    }

    return ;    
} 

//___________________________________________
void PrintDist(TObjString* swhat,Bool_t yield,AliAnalysisMuMu &ana)
{
    

    TObjArray* whatCent= TString(scentrality.Data()).Tokenize(",");
    TIter nextCent(whatCent);
    TObjString* scent;

    while ( ( scent = static_cast<TObjString*>(nextCent()) ) ) 
    {
        //________Get spectra
        TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scent->String().Data(),spairCut.Data(),"PSI",swhat->String().Data());
        printf("--- Centrality = %s\n", scent->String().Data());

        AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(ana.OC()->GetObject(spectraPath.Data()));
        if(!spectra) {
            cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
            return;
        }
        if(swhat->String().Contains("PT") || swhat->String().Contains("Y"))
        {
           new TCanvas;
           spectra->Plot("NofJPsi","",kTRUE)->DrawCopy("");
           if (yield) PrintCrossSection(spectra,ana,Form("%s",swhat->String().Data()));  
        }  
        else if(swhat->String().Contains("INTEGRATED"))
        {

           Double_t jpsi = spectra->GetResultForBin(swhat->String().Data())->GetValue("NofJPsi");
           // Double_t AccEff = spectra->GetResultForBin(swhat->String().Data())->GetValue("AccEffJPsi");
           Double_t corrjpsi = spectra->GetResultForBin(swhat->String().Data())->GetValue("CorrNofJPsi");
           Double_t corrjpsistat = spectra->GetResultForBin(swhat->String().Data())->GetErrorStat("CorrNofJPsi");
           Double_t jpsisys = spectra->GetResultForBin(swhat->String().Data())->GetRMS("NofJPsi");
           Double_t jpsistat = spectra->GetResultForBin(swhat->String().Data())->GetErrorStat("NofJPsi");

           // printf("jpsi         =%f\n",jpsi );
           // // printf("AccEff       =%f\n",AccEff );
           // printf("corrjpsi     =%f\n",corrjpsi );
           // printf("corrjpsistat =%f\n",corrjpsistat );
           // printf("jpsisys      =%f\n",jpsisys );
           // printf("jpsistat     =%f\n",jpsistat );

           Double_t sigma = jpsi/(lumi*BR*1000.*0.240080);
           Double_t sigmastat = sigma*jpsistat/jpsi;
           Double_t sigmasys2 = jpsisys/jpsi*jpsisys/jpsi  +0.01*0.01 +    0.01*0.01 +    0.02*0.02 +    0.02*0.02;
           //                                                   Tracking    Matching       Trigger        tails
           //                                                   
            

            printf("integrated cross section = %f +/- %f +/- %f #mubarn\n",sigma,sigmastat,sigma*TMath::Sqrt(sigmasys2) );
            return;

        }   
        

           
    }
}

//___________________________________________
void PrintCrossSection(AliAnalysisMuMuSpectra *spec,AliAnalysisMuMu &an,char* what)
{
        // Double_t AccEff[7] =  {0.236,0.228,0.229,0.245,0.284,0.329,0.388134};
        Double_t AccEff[6] ={0.100832,0.274249,0.365494,0.368718,0.270976,0.080066};

        // TH1* hCrossSection = static_cast<TH1*>(spec->Plot("NofJPsi","",kTRUE)->Clone());
        // new TCanvas;
        // hCrossSection->DrawCopy();
        // return;

        // TH1* hCrossSection = static_cast<TH1*>(spec->Plot("AccEffJPsi","",kTRUE)->Clone());
        // return;
    
        // hCrossSection->Scale(1./(lumi*BR*1000.*lumiTriggerCrossSection));
        // hCrossSection->SetTitle(Form("J/psi cross section with Lumi. = %f ",lumi));
        // hCrossSection->GetYaxis()->SetTitle(Form("d^{2}#sigma/dp_{T}dy(#muBxGeV^{-1}xc)"));
        // hCrossSection->DrawCopy("");
        // Double_t mul = an.CC()->GetSum(Form("trigger:CMUL7-B-NOPF-MUON/event:PSALL"));
        // Double_t mb = an.CC()->GetSum(Form("trigger:CINT7/event:PSALL/centrality:V0A"));

        AliAnalysisMuMuBinning* binning = spec->Binning();
        TObjArray* bins = binning->CreateBinObjArray();
        TIter nextBin(bins);
        AliAnalysisMuMuBinning::Range* bin;
        Int_t i(0);
        AliAnalysisMuMuResult* r;

        const Double_t * binArrayX = binning->CreateBinArrayX();
        Int_t nBinX = binning->GetNBinsX();

        TGraphErrors * hCrossSection = new TGraphErrors(nBinX);
        TGraphErrors * hRMS = new TGraphErrors(nBinX);

        int j= 1;
        while ( ( bin = static_cast<AliAnalysisMuMuBinning::Range*>(nextBin()) ) )
        {

            r = static_cast<AliAnalysisMuMuResult*>(spec->GetResultForBin(bin->AsString()));

            // printf("AccEffJPsi             =%f\n",AccEff[j-1]);
            // printf("AccEffJPsi framework   =%f\n",r->GetValue("NofJPsi")/r->GetValue("CorrNofJPsi"));
            // printf("\n");
            // printf("NofJPsi                = %f\n", r->GetValue("NofJPsi"));
            // printf("\n");
            // printf("CorrNofJPsi            = %f\n", r->GetValue("NofJPsi")/(AccEff[j-1]));
            // printf("CorrNofJPsi  framework = %f\n", r->GetValue("CorrNofJPsi"));
            // printf("\n");
            // printf("WidthX                 = %f\n", bin->WidthX());


            Double_t sigma =r->GetValue("NofJPsi")/(AccEff[j-1])/(lumi*BR*1000.*bin->WidthX());
            Double_t sigmaerror = sigma*(r->GetErrorStat("NofJPsi")/r->GetValue("NofJPsi"));

             // Get X
            Double_t xmin = bin->Xmin();
            Double_t xmax = bin->Xmax();
            Double_t x = xmin + (xmax-xmin)/2;

            Double_t error2 =(r->GetRMS("NofJPsi")/r->GetValue("NofJPsi"))*(r->GetRMS("NofJPsi")/r->GetValue("NofJPsi")) +0.01*0.01 + 0.01*0.01 + 0.02*0.02;
            printf(" cross section for bin %s = %f +/- %f +/- %f \n",bin->AsString().Data(), sigma, sigmaerror , sigma*TMath::Sqrt(error2));

            hCrossSection->SetPoint(j,x,sigma);
            hCrossSection->SetPointError(j,bin->WidthX()/2,sigmaerror); 

            hRMS->SetPoint(j,x,sigma);
            hRMS->SetPointError(j,bin->WidthX()/2,sigma*TMath::Sqrt(error2));

            j++;
        }

        // TF1* fitpt1 = new TF1("fitpt1","[0]*x*(1/pow((1+pow(x/[1], [2])) ,[3]))",0,12);
        // fitpt1->SetParameters(2424.41, 4.74409,1.70488, 4.42257);

        // TF1* fity1= new TF1("fity1","[0] * TMath::Exp(-0.5*TMath::Power((x-[1])/[2],2.))", -10, 2.5);
        // fity1->SetParameters(2.8,0.00,2.9271); 
        // fity1->FixParameter(1,0.00); 

        new TCanvas;

        gPad->SetLogy();
        hRMS->SetFillColor(2);
        hRMS->SetFillStyle(3001);
        hRMS->SetTitle(Form("J/psi cross section"));
        hRMS->GetYaxis()->SetTitle(Form("d^{2}#sigma/dp_{T}(#muBxGeV^{-1}xc)"));
        hRMS->GetXaxis()->SetTitle(Form("%s",what));
        hRMS->SetMarkerSize(1.7);
        
        TLegend * leg = new TLegend(0.4,0.7,0.70,0.9);
        leg->SetHeader(Form("ALICE, p-p #sqrt{s_{NN}}=5.02 TeV, L_{int}=111.31 #mub^{-1})",what));
        leg->AddEntry(hCrossSection,"Inclusive J/#psi cross-section 2.5 < y < 4","pe"); 
        // leg->AddEntry(hCrossSection,"Inclusive J/#psi cross-section 0 < pT < 8","pe");    
           
        leg->AddEntry(hRMS,"systematic uncertainty ","f");
        // leg->AddEntry((TObject*)0,"global sys.= 2.23 % (Tracking + Trigger)","");
        leg->SetTextSize(0.03);
        hRMS->DrawClone("A2P[]");

        // TFitResultPtr Fitpoint = hCrossSection->Fit("fitpt1","SR");
        // printf("cross section = %f +/- %f \n",fitpt1->Integral(0.,8.),fitpt1->IntegralError(0.,8.));

        // TFitResultPtr Fitpoint = hCrossSection->Fit("fity1","SR");
        // printf("cross section = %f +/- %f \n",fity1->Integral(-4.,-2.5),fity1->IntegralError(-4.,-2.5));

        // TFitResultPtr Fitpoint = hCrossSection->Fit(fity1,"SR");
        // printf("cross section = %f +/- %s \n",fity1->Integral(-4, -2.5),fity1->IntegralError(-4, -2.5));
        

        hCrossSection->DrawClone("Psame");
        leg->DrawClone("same");
    

    
    
    
}

