//
//  PrintFitMacro.c
//  
//
//  Created by Benjamin Audurier on 09/06/15.
//
//

// Macro who prints fit results
TString striggerDimuon ="CMUL7-B-NOPF-MUFAST";
TString striggerMB     ="CINT7-B-NOPF-MUFAST";
TString seventType     ="PSALL";
TString spairCut       ="pALLPAIRYPAIRPTIN0.0-10.0RABSMATCHLOWETAPDCA";
// TString scentrality    ="V0M";
TString scentrality    ="V0M_60.00_90.00";
Double_t FNorm         =15.22;
Double_t BR            =0.005;


//_____________________________________________________________________________
void PrintFitMacro(
char         * what ="PT",
Bool_t PrintDistribution= kTRUE,
Bool_t Raa   = kFALSE, 
Bool_t print = kTRUE,
Bool_t yield = kTRUE,
char         * sfile="../AnalysisResults.root",
char         * sasso="",
char         * sasso2="",
char         * beamYear="mumu.PbPb2015.config")
{    
    
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
        
        if (Raa)
        {
        	if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt",kFALSE);
        	else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",kFALSE);
        	else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",kFALSE);
        	else continue;
        }
        if (PrintDistribution)
        { 
           //________Get spectra
            TString spectraPath= Form("/%s/%s/%s/%s/%s-%s",seventType.Data(),striggerDimuon.Data(),scentrality.Data(),spairCut.Data(),"PSI",swhat->String().Data());

            AliAnalysisMuMuSpectra * spectra = static_cast<AliAnalysisMuMuSpectra*>(analysis.OC()->GetObject(spectraPath.Data()));
            if(!spectra)
            {
                cout << Form("Cannot find spectra with name %s",spectraPath.Data()) <<endl;
                return;
            }

            new TCanvas;
            spectra->Plot("NofJPsi","",kFALSE)->DrawCopy("");
            if (yield)
            {
                TH1* hyield = static_cast<TH1*>(spectra->Plot("NofJPsi","",kTRUE)->Clone());
                Double_t MUL = analysis.CC()->GetSum(Form("trigger:%s/centrality:%s/event:%s",striggerDimuon.Data(),scentrality.Data(),seventType.Data()));
                // printf("Number of MUL = %0.f\n", MUL);
                hyield->Scale(1/(MUL*FNorm*BR));

                hyield->SetTitle(Form("J/psi Yield with MUL = %0.f and FNorm = %0.2f ",MUL,FNorm));
                hyield->GetYaxis()->SetTitle(Form("Yield w/o AccxEff corr."));
                new TCanvas;
                hyield->DrawCopy("e0");
            }
        }
       


    }
    return ;    

} 

