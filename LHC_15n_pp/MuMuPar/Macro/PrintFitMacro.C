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
           Double_t jpsisys = spectra->GetResultForBin(swhat->String().Data())->GetRMS("NofJPsi");
           Double_t jpsistat = spectra->GetResultForBin(swhat->String().Data())->GetErrorStat("NofJPsi");

           Double_t sigma = corrjpsi/(lumi*BR*1000.);
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
        // Double_t AccEff[6] ={0.100832,0.274249,0.365494,0.368718,0.270976,0.080066};

        AliAnalysisMuMuBinning* binning = spec->Binning();
        TObjArray* bins = binning->CreateBinObjArray();
        TIter nextBin(bins);
        AliAnalysisMuMuBinning::Range* bin;
        Int_t i(0);
        AliAnalysisMuMuResult* r;

        const Double_t * binArrayX = binning->CreateBinArrayX();
        Int_t nBinX = binning->GetNBinsX();

        TGraphErrors * gCrossSection = new TGraphErrors(nBinX);
        TGraphErrors * gSys = new TGraphErrors(nBinX);
        TString sbin;

        int j= 1;
        while ( ( bin = static_cast<AliAnalysisMuMuBinning::Range*>(nextBin()) ) )
        {

            r = static_cast<AliAnalysisMuMuResult*>(spec->GetResultForBin(bin->AsString()));
            if(j==1 && bin->AsString().Contains("PT"))sbin ="PT";
            else if(j==1 && bin->AsString().Contains("Y"))sbin ="Y";
            printf("sbin = %s\n",sbin.Data() );

            Double_t sigma =r->GetValue("CorrNofJPsi")/(lumi*BR*1000.*bin->WidthX());
            Double_t sigmaerror = sigma*(r->GetErrorStat("NofJPsi")/r->GetValue("NofJPsi"));

             // Get X
            Double_t xmin = bin->Xmin();
            Double_t xmax = bin->Xmax();
            Double_t x = xmin + (xmax-xmin)/2;

            Double_t error2 =(r->GetRMS("NofJPsi")/r->GetValue("NofJPsi"))*(r->GetRMS("NofJPsi")/r->GetValue("NofJPsi")) +0.01*0.01 + 0.01*0.01 + 0.02*0.02;
            printf(" cross section for bin %s = %f +/- %f +/- %f \n",bin->AsString().Data(), sigma, sigmaerror , sigma*TMath::Sqrt(error2));

            gCrossSection->SetPoint(j,x,sigma);
            gCrossSection->SetPointError(j,bin->WidthX()/2,sigmaerror); 

            gSys->SetPoint(j,x,sigma);
            gSys->SetPointError(j,bin->WidthX()/2,sigma*TMath::Sqrt(error2));

            j++;
        }
        TF1* fit=0x0;
        TFitResultPtr Fitpoint=0x0;

        if(sbin.Contains("PT")){
            fit = new TF1("fit","[0]*x*(1/pow((1+pow(x/[1], [2])) ,[3]))",0,12);
            fit->SetParameters(2424.41, 4.74409,1.70488, 4.42257);
        }

        if(sbin.Contains("Y")){
            fit= new TF1("fit","[0] * TMath::Exp(-0.5*TMath::Power((x-[1])/[2],2.))", -10, 2.5);
            fit->SetParameters(2.8,0.00,2.9271); 
            fit->FixParameter(1,0.00);
        }
        new TCanvas;

        if(sbin.Contains("PT")) gPad->SetLogy();
        gSys->SetFillColor(2);
        gSys->SetFillStyle(3001);
        gSys->SetTitle(Form("J/psi cross section"));
        if(sbin.Contains("PT")) gSys->GetYaxis()->SetTitle(Form("d^{2}#sigma/dp_{T}(#muBxGeV^{-1}xc)"));
        if(sbin.Contains("Y")) gSys->GetYaxis()->SetTitle(Form("d^{2}#sigma/dY(#muB)"));
        gSys->GetXaxis()->SetTitle(Form("%s",what));
        gSys->SetMarkerSize(1.7);
        
        TLegend * leg = new TLegend(0.4,0.7,0.70,0.9);
        leg->SetHeader(Form("ALICE, p-p #sqrt{s_{NN}}=5.02 TeV, L_{int}=111.31 #mub^{-1})",what));
        if(sbin.Contains("PT")) leg->AddEntry(gCrossSection,"Inclusive J/#psi cross-section 2.5 < y < 4","pe"); 
        if(sbin.Contains("Y")) leg->AddEntry(gCrossSection,"Inclusive J/#psi cross-section 0 < pT < 8","pe");               
        leg->AddEntry(gSys,"systematic uncertainty ","f");
        leg->SetTextSize(0.03);

        gSys->DrawClone("A2P[]");

        if(fit) Fitpoint = gCrossSection->Fit("fit","SR");
        if(sbin.Contains("PT") && static_cast<int>(Fitpoint)==0) printf("cross section = %f +/- %f \n",fit->Integral(0.,8.),fit->IntegralError(0.,8.));
        if(sbin.Contains("Y")  && static_cast<int>(Fitpoint)==0)  printf("cross section = %f +/- %f \n",fit->Integral(-4.,-2.5),fit->IntegralError(-4.,-2.5));
        
        gCrossSection->DrawClone("Psame");
        leg->DrawClone("same");
}

