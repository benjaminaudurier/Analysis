//
//  PrintFitMacro.c
//  
//
//  Created by Benjamin Audurier on 09/06/15.
//
//

// Macro who prints fit results

TString striggerDimuon  ="CMUL7-B-NOPF-MUFAST";
TString seventType      ="PSALL";
TString spairCut        ="pALLPAIRYPAIRPTIN0.0-10.0RABSMATCHLOWETAPDCA";
TString scentrality     ="V0M_00.00_90.00";


//_____________________________________________________________________________
void PrintFitMacro(
char         * what ="INTEGRATED",
Bool_t PrintDistribution= kFALSE,
Bool_t Raa   = kFALSE,
Bool_t print = kFALSE,
char         * sfile="../AnalysisResults.root",
char         * sasso="",
char         * sasso2="",
char         * beamYear="mumu.PbPb2015.config")
{    
    AliLog::SetGlobalDebugLevel(0);
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
        	if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt","V0M_00.00_90.00",kFALSE);
        	else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt","V0M_00.00_90.00",kFALSE);
        	else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt","V0M_00.00_90.00",kFALSE);
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
            
        }


    }
    return ;    

} 

