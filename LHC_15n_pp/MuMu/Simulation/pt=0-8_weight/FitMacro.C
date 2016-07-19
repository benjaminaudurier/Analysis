//
//  Fit.c
//
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro to fit Minv Spectra and draw J/psi distribution vs pt and y

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <TROOT.h>
#include <TString.h>

char* sfile="AnalysisResults.JPSI.root";
char* sasso="";
char* sasso2="";
char* beamYear="mumu.pp2015.simu.config";


//_____________________________________________________________________________
void FitMacro( char* what ="pt",const char* printWhat = "", int debug =0 )
{

    AliLog::SetGlobalDebugLevel(debug);

    Bool_t rawcount = kFALSE;
    Bool_t clean = kFALSE;
    Bool_t print = kFALSE;

    TString sprint (printWhat);
    TString What (what);

    //Set bool
    if(sprint.Contains("rawcount")) rawcount =kTRUE;
    if(sprint.Contains("clean")) clean       =kTRUE;
    if(sprint.Contains("print")) print       =kTRUE;

   //General conf.
    TObjArray* whatArray= TString(what).Tokenize(",");
    TIter nextWhat(whatArray);
    TObjString* swhat;

    // main object
    AliAnalysisMuMu analysis(sfile,sasso,sasso2,beamYear);

    // Clean
    if(clean) analysis.CleanAllSpectra();

    //_____ Fit
    while ( ( swhat = static_cast<TObjString*>(nextWhat()) ) ){
        if(swhat->String().Contains("integrated")) analysis.Jpsi(swhat->String().Data(),"",kFALSE,kFALSE);
        else analysis.Jpsi(swhat->String().Data(),"BENJ",kFALSE,kFALSE);
    }

    // analysis.PrintNofParticle("PSI","NofJPsi","YVSPT",kFALSE);
    // analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    if( What.Contains("pt"))         analysis.PrintNofParticle("PSI","NofJPsi","PT",kFALSE);
    if( What.Contains("y"))          analysis.PrintNofParticle("PSI","NofJPsi","Y",kFALSE);
    if( What.Contains("integrated")) analysis.PrintNofParticle("PSI","NofJPsi","INTEGRATED",kFALSE);
    if( What.Contains("yvspt"))      analysis.PrintNofParticle("PSI","NofJPsi","YVSPT",kFALSE);

    if(rawcount){
        analysis.ComputeDimuonRawCount(2.8,3.4);
        analysis.ComputeDimuonRawCount(2.1,2.8);
    }

}
