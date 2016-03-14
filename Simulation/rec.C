void rec() {
  
  AliReconstruction reco;
//  reco.SetRunReconstruction("MUON");
  reco.SetRunReconstruction("MUON ITS");
  reco.SetRunQA("MUON:ALL");
  reco.SetQAWriteExpert(AliQAv1::kMUON);
  
// switch off cleanESD
  reco.SetCleanESD(kFALSE);
  reco.SetStopOnError(kFALSE); // important to run the vertexerMC without simulating the ITS digits
  
  // Raw OCDB
//  reco.SetDefaultStorage("alien://folder=/alice/data/2015/OCDB");
//  reco.SetDefaultStorage("alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal");
  
  // GRP from local OCDB
//  reco.SetSpecificStorage("GRP/GRP/Data",Form("local://%s",gSystem->pwd()));
  
  // MUON Tracker
//  reco.SetSpecificStorage("MUON/Calib/RejectList","alien://folder=/alice/cern.ch/user/l/laphecet/OCDB");
//  reco.SetSpecificStorage("MUON/Calib/RecoParam","alien://folder=/alice/cern.ch/user/p/ppillot/OCDB2012_newReco");
//  reco.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Residual");
//  reco.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Ideal");
//  reco.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/data/2011/OCDB");
//  reco.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/cern.ch/user/p/ppillot/OCDB_PbPbSim");
  
  AliCDBManager* man = AliCDBManager::Instance();
  man->SetDefaultStorage("alien://folder=/alice/data/2015/OCDB");
  man->SetSpecificStorage("GRP/GRP/Data",Form("local://%s",gSystem->pwd()));
//  man->SetSpecificStorage("MUON/Align/Data", "alien://folder=/alice/data/2015/OCDB",5);
  man->SetSpecificStorage("ITS/Calib/RecoParam","alien://folder=/alice/cern.ch/user/p/ppillot/OCDB2015");
  /*
  AliMUONRecoParam *muonRecoParam = AliMUONRecoParam::GetLowFluxParam();
  for (Int_t iCh=0; iCh<10; iCh++) {
    muonRecoParam->SetDefaultNonBendingReso(iCh,0.2);
    muonRecoParam->SetDefaultBendingReso(iCh,0.2);
  }
  reco.SetRecoParam("MUON",muonRecoParam);
  */
  // RecoParam for vertexerMC
  //reco.SetSpecificStorage("ITS/Calib/RecoParam","alien://folder=/alice/cern.ch/user/p/ppillot/OCDB_PbPbSim");
  
  TStopwatch timer;
  timer.Start();
  reco.Run();
  timer.Stop();
  timer.Print();
}
