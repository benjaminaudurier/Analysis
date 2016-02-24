// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMSL7-B-NOPF-MUFAST";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/singleMuon/tuned/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustomSingleBen");
	a.SetRemoteDir(remoteDir.Data());
	//a.SetMaxEventsPerChunk(10000);
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),1);
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PTMIN","0.8");
  	
  	// LHC15n tuned param
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P0","135.137");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P1","0.555323");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P2","0.578374");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P3","10.1345");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P4","0.000232233");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_PT_P5","-0.924726");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_Y_P0","1.95551");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_Y_P1","-0.104761");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLEBEN_Y_P2","0.00311324");

	a.SetRunList("$DATASETDIR/runList_LHC15n.txt");

	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}
