//
//  PrintFitMacro.c
//  
//
//  Created by Benjamin Audurier on 09/06/15.
//
//

// Macro who prints fit results


//_____________________________________________________________________________
void PrintFitMacro(
char         * what ="INTEGRATED",
char         * sfile="../datasetfull.txt.saf.root",
char         * sasso="",
char         * sasso2="",
char         * beamYear="PbPb2011",
Bool_t Raa   = kTRUE, 
Bool_t print = kFALSE)
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
        analysis.DrawFitResults("PSI",swhat->String().Data(),"../histo",print);
        if (Raa)
        {
        	if(swhat->String().Contains("INTEGRATED")) analysis.RAAasGraphic("PSI",swhat->String().Data(),"externFile_PT.txt","externFile_CENT.txt",kFALSE);
        	else if(swhat->String().Contains("Y")) analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",kFALSE);
        	else if(swhat->String().Contains("PT")) analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",kFALSE);
        	else continue;
        }
    }
    return ;    

} 

