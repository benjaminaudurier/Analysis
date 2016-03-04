MakeDataSet(const char * runlist)
{


	ifstream infile(runlist,std::ios::in);
	TString line;

	gSystem->Exec("mkdir Rep");

	while(infile.eof()!=kTRUE){

	    //read the line
	    line.ReadLine(infile,kFALSE);
	    gSystem->Exec(Form("cp Find_BasePath__alice_data_2015_LHC15o_000%s_muon_calo_pass1_AOD_X_FileName_AliAOD.Muons.root_Tree__aodTree_Filter_AODMUONONLY_PBPB2015_WITHMULTSEL_Aliphysics_vAN-20160113-1.root Rep/",line.Data()));
	}

	gSystem->Exec(Form("hadd ds.%s.root Rep/*",runlist));
	gSystem->Exec(Form("cp ds.%s.root ..",runlist));

	gSystem->Exec("rm -rf Rep");

}