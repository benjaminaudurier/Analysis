void sim(Int_t nev=5000)
{

#if defined(__CINT__)
  gSystem->Load("liblhapdf");      // Parton density functions
  if ( TString("GenParamCustom").Contains("pythia6",TString::kIgnoreCase) )
  {
    std::cout << "Setting up Pythia6 required env. variables" << std::endl;
    gSystem->AddIncludePath("-I$ALICE_ROOT/include");
    
  }
  else  gSystem->Load("libpythia6");     // Pythia 6.2 (for decayer)

  if ( TString("GenParamCustom").Contains("pythia8",TString::kIgnoreCase) )
  {
    std::cout << "Setting up Pythia8 required libraries and env. variables" << std::endl;
    //    gSystem->Load("libpythia8");
    //    gSystem->Load("libAliPythia8");
    gSystem->AddIncludePath("-I$ALICE_ROOT/include");
    
  }
#endif


  if ( 0) {
    TGeoGlobalMagField::Instance()->SetField(new AliMagF("Maps","Maps", -1., -1, AliMagF::k5kG));
  }

  AliSimulation simulator;
  simulator.SetRunQA("MUON:ALL");
  simulator.SetRunHLT("");

  if ( 0 )
  {
    simulator.SetMakeSDigits("MUON T0 VZERO FMD"); // T0 and VZERO for trigger efficiencies, FMD for diffractive studies
    simulator.SetMakeDigitsFromHits("ITS"); // ITS needed to propagate the simulated vertex
    simulator.SetMakeDigits("MUON T0 VZERO FMD");// ITS"); // ITS needed to propagate the simulated vertex
  }
  else
  {
    simulator.SetTriggerConfig("MUON");
    simulator.SetMakeSDigits("MUON");
    simulator.SetMakeDigits("MUON");// ITS"); // ITS needed to propagate the simulated vertex
  }
  

  simulator.SetDefaultStorage("raw://");
  
  if ( kFALSE )
  {
    simulator.SetCDBSnapshotMode("OCDB_sim.root");
  }
  
  if ( ! 0 ) {
    
    // MUON Tracker
    simulator.SetSpecificStorage("MUON/Align/Data","alien://folder=/alice/simulation/2008/v4-15-Release/Ideal");

    // Mag.field from OCDB
    simulator.UseMagFieldFromGRP();

    if ( 0 )
    {
      simulator.SetSpecificStorage("ITS/Align/Data",  "alien://Folder=/alice/simulation/2008/v4-15-Release/Ideal");
      simulator.UseVertexFromCDB();
    }
  }
  
   // The rest
  TStopwatch timer;
  timer.Start();
  simulator.Run(nev);
  timer.Stop();
  timer.Print();
}
