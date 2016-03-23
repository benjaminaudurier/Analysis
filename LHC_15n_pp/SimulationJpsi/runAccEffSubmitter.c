// Macro to configure and run simulation
//
// Created by Benjamin Audurier, 07-22-2015


TString Trigger = "CMUL7-B-NOPF-MUFAST";
TString remoteDir =Form("/alice/cern.ch/user/b/baudurie/Analysis/LHC15n/sim/Jpsi/tune2/%s",Trigger.Data());

void runAccEffSubmitter(const char* mode)
{
	gSystem->Load("libpythia6_4_28");


	AliMuonAccEffSubmitter a("GenParamCustomRadiativeDecay");
	a.SetRemoteDir(remoteDir.Data());
	a.ShouldOverwriteFiles(true);
	// a.MakeNofEventsFixed(10);
	a.MakeNofEventsPropToTriggerCount(Trigger.Data(),3);
	
	//Set parameters
	a.SetVar("VAR_GENLIB_PARNAME","\"pp 5.03\"");
	a.SetVar("VAR_GENPARAMCUSTOM_PT_P0","3783.48"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P1","12.0632"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P2","1.92605"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_PT_P3","3.66485"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P0","1.22478"); 
  	a.SetVar("VAR_GENPARAMCUSTOM_Y_P1","-0.0423893");

  	//Set variables
	a.SetCompactMode(0);
	a.SetRunList("$DATASETDIR/runList_LHC15n.txt");
	a.SetAliPhysicsVersion("VO_ALICE@AliPhysics::vAN-20160310-1");
	a.SetVar("VAR_SIM_ALIGNDATA","\"alien://folder=/alice/simulation/2008/v4-15-Release/Full\"");
 	a.SetVar("VAR_REC_ALIGNDATA","\"raw://\"");

	a.SetCompactMode(0);
	a.Print();
	a.Run(mode);
	a.Submit(false);

}

// pT parameters for J/psi generator:
// Double_t p[4] = {21770.6, 15.2993, 1.97656, 4.01293};

// y parameters for J/psi generator:
// Double_t p[2] = {1.19752, -0.0412364};