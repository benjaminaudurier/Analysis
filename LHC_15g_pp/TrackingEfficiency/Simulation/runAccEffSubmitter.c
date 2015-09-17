// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMSL7-B-NOPF-MUON";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/simsinglemuon/pp/new/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustomSingle");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data());
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P0","0.805901");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P1","0.508179");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P2","5.05173");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_PT_P3","0.840121");

	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P0","0.00497794");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P1","-139.038");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P2","-51.4033");
	a.SetVar("VAR_GENPARAMCUSTOMSINGLE_Y_P3","-5.66992");

	a.SetRunList("runlist_15-07-2015-ESD.txt");
	a.SetPackages("VO_ALICE@AliPhysics::vAN-20150902");
	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}

