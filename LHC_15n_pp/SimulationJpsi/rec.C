void rec()
{
  AliReconstruction reco;
  reco.SetRunQA("MUON:ALL");
  
  reco.SetCleanESD(kFALSE);
  reco.SetStopOnError(kFALSE);
  
  reco.SetDefaultStorage("raw://");

  if ( 0 )
  {
    reco.SetRunReconstruction("VZERO T0 MUON ITS FMD");
  }
  else
  {
    reco.SetRunReconstruction("MUON");
  }
  
  if ( kFALSE )
  {
    reco.SetCDBSnapshotMode("OCDB_rec.root");
  }

  // GRP from local OCDB
  reco.SetSpecificStorage("GRP/GRP/Data",Form("local://%s",gSystem->pwd()));
  
  // MUON Tracker Residual Alignment
  reco.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Residual");
  
  if ( 0 ) {
    // ITS
    reco.SetRunPlaneEff(kTRUE);
    reco.SetUseTrackingErrorsForAlignment("ITS");

    // ITS
    reco.SetSpecificStorage("ITS/Align/Data", "alien://folder=/alice/simulation/2008/v4-15-Release/Residual");
    reco.SetSpecificStorage("ITS/Calib/SPDSparseDead", "alien://folder=/alice/simulation/2008/v4-15-Release/Residual");

  }

  reco.Run();
}
