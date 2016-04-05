// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMSL7-B-NOPF-MUFAST";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15o/sim/singleMuontuned/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustomSingle");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	a.SetMaxEventsPerChunk(10000);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),0.5);//factuer pour avoir 25 millions d'events
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PTMIN","0.8");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P0","349.454");
  	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P1","0.965971");
  	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P2","0.83717");
  	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P3","7.82193");


	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P0","1.77115");
  	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P1","-0.0966005");
  	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P2","0.00260707");

	a.SetRunList("runList_LHC15o_AOD.txt");
	a.SetAliPhysicsVersion("vAN-20160301-1");
	//a.SetAliRootVersion("v5-08-01-1");

	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}

// Double_t newPtParam[6] = {349.454, 0.965971, 0.83717, 7.82193, -0.00325109, -1.79551};
// Double_t newYParam[3] = {1.77115, -0.0966005, 0.00260707};

