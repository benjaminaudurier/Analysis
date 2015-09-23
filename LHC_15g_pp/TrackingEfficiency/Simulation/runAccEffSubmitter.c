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

	a.SetRunList("runlist_15-07-2015-ESD.txt");
	a.SetPackages("VO_ALICE@AliPhysics::vAN-20150902");
	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}

