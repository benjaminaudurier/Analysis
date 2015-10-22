//
//  Fit.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro to fit Minv Spectra and draw J/psi distribution vs pt and y

TString striggerDimuon  ="CPBI1MUL-B-NOPF-MUON";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-18.0RABSMATCHLOWETAPDCA";
TString sbinType        ="Y";
TString scentrality     ="V0M_00.00_90.00";
TString sResName        ="";
Bool_t divideByBinWidth =kTRUE; 
Double_t parPOWLAW[3] = {80.,1.,1.};


//_____________________________________________________________________________
void FitMacro(
char           * what ="integrated",
char           * sfile="../datasetfull.txt.saf.root",
char           * sasso="",
char           * sasso2="",
char           * beamYear="PbPb2011",
Bool_t FitDist = kFALSE)
{    
    
    //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

	
    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    // // Clean   
    // analysis.CleanAllSpectra();    

    //_____ Fit 
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    {
        if(swhat->String().Contains("integrated")) analysis.Jpsi(swhat->String().Data(),"",kFALSE,kFALSE);

        else analysis.Jpsi(swhat->String().Data(),"BENJ",kFALSE,kFALSE);
    }

    // analysis.PrintNofParticle("PSI","NofJPsi","INTEGRATED",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
 
    //_______Fit  J/psi vs pt or J/psi vs y 
    if(FitDist)
    {   
        TObjArray* WHATArray= TString(sbinType).Tokenize(",");
        TIter nextWHAT(WHATArray);
        TObjString* sWHAT;

        // analysis.PrintDistribution(sbinType);
        while ( ( sWHAT = static_cast<TObjString*>(nextWHAT()) ) )
        {
           //________Get spectra
            TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data(),"PSI",sWHAT->String().Data());

            AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(analysis.OC()->GetObject(spectraPath.Data()));
            if(!spectra)
            {
                cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
                return;
            }
            TObject* o= static_cast<TObject*>(spectra->Plot("NofJPsi",sResName,divideByBinWidth));
            cout << o << endl;
            TH1* h= static_cast<TH1*>(o->Clone());
            cout << h<< endl;
            TCanvas *c = new TCanvas;
            c->SetLogy();
            h->DrawCopy();

            //________
            
            // //________Fit function
            // gROOT->LoadMacro("../../My_First_Task/FittingFunctions.C");

            // TF1 * f =0x0;
            // if (sWHAT->String().Contains("PT"))
            // {
            //     f = new TF1("Fit",powerLaw3Par,0.,8.,3);
            //     f->SetParameters(&parPOWLAW[0]);
            //     f->SetParNames("C","p_0","n");
            //     f->SetLineColor(36);
            //     f->SetLineStyle(5);

            //     h->Fit(f);
            
            //     h->DrawCopy();
            //     f->Draw("same");
            //     delete h;
            //     delete f;
            // }
            // else if (sWHAT->String().Contains("Y"))
            // {
            //     h->Fit("pol4");
            //     h->DrawCopy();
            //     delete h;
            // }
            // //________
        }    
    }
        
    return ;    
}













