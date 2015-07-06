//
//  MyMacro.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro loadded by runMuMuResult.sh

TString striggerDimuon  ="CMUL7-B-NOPF-MUON";
TString seventType      ="PSALL";
TString spairCut        ="pPAIRYPAIRPTIN0.0-18.0RABSMATCHLOWETAPDCAPSALL";
TString sbinType        ="Y";
TString scentrality     ="V0A";
TString sResName        ="";
Bool_t divideByBinWidth =kTRUE; 
Double_t parPOWLAW[3] = {80.,1.,1.};
Double_t Pol12Par[2] = {1.,1.};


//_____________________________________________________________________________
void FitData(
char           * sfile="AnalysisResultsReference.root",
char           * beamYear="PbPb2011",
char           * what ="pt,y",
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
    AliAnalysisMuMu analysis(sfile,"","",beamYear);



    // // Clean   
    // analysis.CleanAllSpectra();    



    // //_____ Fit 
    // while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) )
    // {
    //     if(swhat->String().Contains("integrated")) analysis.Jpsi(swhat->String().Data(),"",kFALSE,kFALSE);

    //     else analysis.Jpsi(swhat->String().Data(),"BENJ",kFALSE,kFALSE);
    // }
    
    // analysis.DrawMinv();
    // return;


    
    // analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
 
    //_______Fit  J/psi vs pt or J/psi vs y 
    if(FitDist)
    {   
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
            spectra->Print("-");

            TH1* h= static_cast<TH1*>(spectra->Plot("NofJPsi",sResName,divideByBinWidth)->Clone());
            cout << h<< endl;
            TCanvas *c = new TCanvas;
            c->SetLogy();

            //________
            
            //________Fit function
            gROOT->LoadMacro("../../My_First_Task/FittingFunctions.C");

            TF1 * f =0x0;
            if (sWHAT->String().Contains("PT"))
            {
                f = new TF1("Fit",powerLaw3Par,0.,8.,3);
                f->SetParameters(&parPOWLAW[0]);
                f->SetParNames("C","p_0","n");
                f->SetLineColor(36);
                f->SetLineStyle(5);

                h->Fit(f);
            
                h->DrawCopy();
                f->Draw("same");
                delete h;
                delete f;
            }
            else if (sWHAT->String().Contains("Y"))
            {
                f = new TF1("Fit",normPol12Par,0.,8.,2);
                f->SetParameters(&Pol12Par[0]);
                f->SetParNames("p_0","p_1");
                f->SetLineColor(36);
                f->SetLineStyle(5);

                h->Fit(f);
            
            h->DrawCopy();
            f->Draw("same");
            delete h;
            delete f;
            }
            //________
        }    
    }
        
    return ;    
}













