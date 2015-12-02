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
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-8.0RABSMATCHLOWETA";
TString sbinType        ="YVSPT";
TString scentrality     ="V0A";
TString sResName        ="";
Bool_t divideByBinWidth =kTRUE; 
Double_t parPOWLAW[3] = {80.,1.,1.};
Double_t Pol12Par[3] = {80.,1.,1.};


//_____________________________________________________________________________
void FitData(
char           * sfile="AnalysisResultsReference.root",
char           * sasso="",
char           * sasso2="",
char           * beamYear="mumu.pp2015.config",
char           * what ="yvspt",
Bool_t FitDist = kTRUE)
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

    // //_____ Fit 
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    {
        if(swhat->String().Contains("integrated")) analysis.Jpsi(swhat->String().Data(),"",kFALSE,kFALSE);

        else analysis.Jpsi(swhat->String().Data(),"BENJ",kFALSE,kFALSE);
    }

    // analysis.PrintNofParticle("PSI","NofJPsi","yvspt",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
 
    //_______Fit  J/psi vs pt or J/psi vs y 
    if(FitDist)
    {   
        TFile* dataFile = TFile::Open("AnalysisResultsReference.root","READ");
        if (!dataFile || !dataFile->IsOpen()) return;

        // Get HistoCollection
        AliMergeableCollection * oc = 0x0;
        dataFile->GetObject("OC",oc);
        if (!oc) return;

        // Get spectras
        AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(oc->GetObject(Form("/%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data())));
        if(!spectra)
        {
          cout << Form("Cannot find PT spectra in /%s/%s/%s/%s/PSI-YVSPT",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data()) << endl;
          return;
        }


        Double_t* ptbin= spectra->Binning()->CreateBinArrayX();
        Double_t* ybin= spectra->Binning()->CreateBinArrayY();

        Int_t ptnofbin= spectra->Binning()->GetNBinsX();
        Int_t ynofbin= spectra->Binning()->GetNBinsY();

        //___________Set ref.Spectra
        // Pt spectra
        TH2* hpt= spectra->Plot("NofJPsi","",kTRUE);// new
        //Y spectra
        TH2* hy= spectra->Plot("NofJPsi","",kTRUE);// new

        dataFile->Close();

        new TCanvas;
        if(hpt)hpt->ProjectionX()->Draw("");
        new TCanvas;
        if(hy)hy->ProjectionY()->Draw("");

        //________Fit function
        gROOT->LoadMacro("../../../My_First_Task/FittingFunctions.C");

        if (hpt)
        {
            TF1 * f = new TF1("Fit",powerLaw3Par,0.,8.,3);
            f->SetParameters(&parPOWLAW[0]);
            f->SetParNames("C","p_0","n");
            f->SetLineColor(36);
            f->SetLineStyle(5);

            hpt->ProjectionX()->Fit(f);

            // hpt->ProjectionX()->DrawCopy("same");
            // f->DrawCopy("same");
            delete hpt;
            delete f;
        }
        if (hy)
        {
            TF1 *f = new TF1("Fit",normPol12Par,-4.,-2.5,3);
            f->SetParameters(&Pol12Par[0]);
            // f->SetParNames("p_0","p_1");
            f->SetLineColor(36);
            f->SetLineStyle(5);

            hy->ProjectionY()->Fit(f);

            // hy->ProjectionY()->DrawCopy("same");
            // f->DrawCopy("same");
            delete hy;
            delete f;
        }
 //       ________
          
    }
        
    return ;      
}













