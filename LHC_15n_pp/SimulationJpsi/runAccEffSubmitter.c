// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMUL7-B-NOPF-MUFAST";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/untuned/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustom");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),3);
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");
	a.SetVar("VAR_GENPARAMCUSTOM_PT_P0","0.32231"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P1","0.95996"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P2","0.0665176"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P3","3.70602"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P0","0.998251"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P1","-0.0308991");
	a.SetCompactMode(0);
	a.SetRunList("$DATASETDIR/runList_LHC15n.txt");

	a.Print();
	a.Run(mode);
	a.Submit(false);

}

// Double_t ptparam[4] = {0.32231, 0.95996, 0.0665176, 3.70602};
// Double_t newYParam[4] = {1.17771, -0.591384, -0.418129, -0.0554779};