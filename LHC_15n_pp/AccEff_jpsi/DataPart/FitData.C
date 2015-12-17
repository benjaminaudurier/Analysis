//
//  Fit.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//


// Macro to fit Minv Spectra and draw J/psi distribution vs pt and y

TString striggerDimuon  ="CMUL7-B-NOPF-MUFAST";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETAPDCA";
TString sbinType        ="Y,PT";
TString scentrality     ="V0A";
TString sResName        ="";
Bool_t divideByBinWidth =kTRUE; 
Double_t parPOWLAW[3] = {80.,1.,1.};
Double_t Pol12Par[3] = {2.812e-07, 559030, -29460.9};


//_____________________________________________________________________________
void FitData(
char           * sfile="AnalysisResults.root",
char           * sasso="",
char           * sasso2="",
char           * beamYear="mumu.pp2015.config",
char           * what ="pt,y",
Bool_t PrintDistribution = kTRUE)
{    
    
    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

    TObjArray* WHATArray= TString(sbinType).Tokenize(",");
    TIter nextWHAT(WHATArray);
    TObjString* sWHAT;
    
    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    // // Clean   
    // analysis.CleanAllSpectra();    

    // // //_____ Fit 
    // while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    // {
    //     if(swhat->String().Contains("integrated")) analysis.Jpsi(swhat->String().Data(),"",kFALSE,kFALSE);

    //     else analysis.Jpsi(swhat->String().Data(),"BENJ",kFALSE,kFALSE);
    // }

    // analysis.PrintNofParticle("PSI","NofJPsi","yvspt",kFALSE);
    analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    analysis.PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
 
    //_______Fit  J/psi vs pt or J/psi vs y 
    if(PrintDistribution)
    {   
        while ( ( sWHAT = static_cast<TObjString*>(nextWHAT()) ) )
        {
            TFile* dataFile = TFile::Open(sfile,"READ");
            if (!dataFile || !dataFile->IsOpen()) return;

            // Get HistoCollection
            AliMergeableCollection * oc = 0x0;
            dataFile->GetObject("OC",oc);
            if (!oc) return;


           //________Get spectra
            TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data(),"PSI",sWHAT->String().Data());

            AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(analysis.OC()->GetObject(spectraPath.Data()));
            if(!spectra)
            {
                cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
                return;
            }

            new TCanvas;
            spectra->Plot("NofJPsi","",kFALSE)->DrawCopy("");


            dataFile->Close();
        }
            
        // //________Fit function
        // gROOT->LoadMacro("../../../My_First_Task/FittingFunctions.C");

        // if (hpt)
        // {
        //     TF1 * f = new TF1("Fit",powerLaw3Par,0.,8.,3);
        //     f->SetParameters(&parPOWLAW[0]);
        //     f->SetParNames("C","p_0","n");
        //     f->SetLineColor(36);
        //     f->SetLineStyle(5);

        //     hpt->ProjectionX()->Fit(f);

        //     // hpt->ProjectionX()->DrawCopy("same");
        //     // f->DrawCopy("same");
        //     delete hpt;
        //     delete f;
        // }
        // if (hy)
        // {
        //     TF1 *f = new TF1("Fit","[0] * ( 1 + [1]*x*x + [2]*x*x*x*x )",-3.9,-2.51);
        //     // f->SetParameters(&Pol12Par[0]);
        //     // f->SetParNames("p_0","p_1");
        //     f->SetLineColor(36);
        //     f->SetLineStyle(5);

        //     hy->ProjectionY()->Fit(f);

        //     // hy->ProjectionY()->DrawCopy("same");
        //     // f->DrawCopy("same");
        //     delete hy;
        //     delete f;
        // }
 //       ________
          
    }
        
    return ;      
}













