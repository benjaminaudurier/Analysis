// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMUL7-B-NOPF-MUON";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15g/sim/Jpsi/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustom");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),2);
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");
	a.SetRunList("runList.txt");
	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);
}

// Double_t newPtParam[4] = {0.675825, 0.297392, 0.853814, 5.20967};
// Double_t newYParam[4] = {1.17771, -0.591384, -0.418129, -0.0554779};