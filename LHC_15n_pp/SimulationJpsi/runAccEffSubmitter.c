// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMUL7-B-NOPF-MUFAST";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tuned/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");
	AliMuonAccEffSubmitter a("GenParamCustom");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),3);
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");
	a.SetVar("VAR_GENPARAMCUSTOM_PT_P0","21770.6"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P1","15.2993"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P2","1.97656"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P3","4.01293"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P0","1.19752"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P1","-0.0412364");
	a.SetCompactMode(0);
	a.SetRunList("$DATASETDIR/runList_LHC15n.txt");

	a.Print();
	a.Run(mode);
	a.Submit(false);

}

// pT parameters for J/psi generator:
// Double_t p[4] = {21770.6, 15.2993, 1.97656, 4.01293};

// y parameters for J/psi generator:
// Double_t p[2] = {1.19752, -0.0412364};