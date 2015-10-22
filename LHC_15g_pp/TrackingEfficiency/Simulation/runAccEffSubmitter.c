// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMSL7-B-NOPF-MUON";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/sim/singleMuon/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustomSingle");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data());
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P0","0.675825");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P1","0.297392");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P2","0.853814");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P3","5.20967");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P0","1.17771");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P1","-0.591384");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P2","-0.418129");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P3","-0.0554779");

	a.SetRunList("runList.txt");
	// a.SetAliPhysicsVersion("VO_ALICE@AliPhysics::vAN-20151015-1");
	// a.SetAliRootVersion("VO_ALICE@AliROOT::v5-07-01-3");
	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}

// Double_t newPtParam[4] = {0.675825, 0.297392, 0.853814, 5.20967};
// Double_t newYParam[4] = {1.17771, -0.591384, -0.418129, -0.0554779};