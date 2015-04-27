//
//  MyMacro.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro loadded by runMuMUuResult.sh

void MyMacro(const char* filename="datasetfull.txt.saf.root",
                  const char* associatedSimFileName="",
                  const char* associatedSimFileName2="",
                  const char* beamYear="PbPb2011",const int DebugLevel =0)
{    
	// the analysis
    AliAnalysisMuMu analysis(filename,associatedSimFileName,associatedSimFileName2,beamYear);

    //Set debug level 
    AliLog::SetGlobalDebugLevel(DebugLevel);

    // Perform fits and draw them   
    analysis.CleanAllSpectra();
    // analysis.Config()->Print();
    
    //_____ FNorm
    // analysis.CC()->Print("run/trigger/centrality","trigger:CCENT_R2-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_00.00_07.50/event:PSALL/");
    // analysis.CC()->PrintSum("trigger:CCENT_R2-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_00.00_07.50/event:PSALL/run");

    // analysis.CC()->Print("run/trigger/centrality","trigger:CSEMI_R1-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_10.00_50.00/event:PSALL");
   
    // analysis.CC()->Print("run/trigger/centrality/event","trigger:CPBI2_B1-B-NOPF-ALLNOTRD&0MUL/centrality:V0M_00.00_90.00/event:PSALL");

    analysis.ComputeIntFnormFromCounters("","","","CPBI2_B1-B-NOPF-ALLNOTRD&0MUL",kFALSE);
    // analysis.ComputeIntFnormFromCounters("","","","CCENT_R2-B-NOPF-ALLNOTRD&0MUL",kFALSE);
    // analysis.ComputeIntFnormFromCounters("","","","CSEMI_R1-B-NOPF-ALLNOTRD&0MUL",kFALSE);
      
      // analysis.ComputeMeanIntFnormFromHisto(kFALSE);

      //_____ 
    
    //_____ Plot ALl Fnorm
    // TH1* h = analysis.OC()->Histo("/FNORM-CPBI1MUL-B-NOPF-MUON/PSALL/V0M_00.00_10.00/hFNormIntVSrun_CCENT_R2-B-NOPF-ALLNOTRD");
    // if (!h) return;
    // h->Scale()
    // h->Draw("Clone");
     
    //_____ Fit 
    // analysis.Jpsi("integrated","");
    // analysis.Jpsi("y","BENJ");
    // analysis.Jpsi("pt","BENJ")
    
    // analysis.DrawJPsiResults("PSI","INTEGRATED","histo",kFALSE);
    // analysis.DrawJPsiResults("PSI","Y","histo",kTRUE);
    
    // analysis.PrintNofJpsi("PSI","INTEGRATED");
    // analysis.PrintNofJpsi("PSI","Y");
    // analysis.PrintNofJpsi("PSI","PT");
    //_____ 

    //_____ Raa
    // analysis.RAAasGraphic("PSI","Y","externFile_Y.txt","externFile_CENT.txt",kFALSE);
    // analysis.RAAasGraphic("PSI","INTEGRATED","externFile_PT.txt","externFile_CENT.txt",kTRUE);
    // analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",kFALSE);
    // analysis.RAAasGraphic("PSI","PT","externFile_PT.txt","externFile_CENT.txt",kFALSE);
    //_____ 


    return ;    
}