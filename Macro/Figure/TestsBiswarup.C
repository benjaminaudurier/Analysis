{
   TString testName = "";
   testName +="VWG2 + CB2 with GEANT3 tails- fit2.2-4.5,";
   testName +="VWG2 + CB2 with GEANT3 tails- fit2.4-4.7,";
   testName +="VWG2 + CB2 with GEANT4 tails- fit2.2-4.5,";
   testName +="VWG2 + CB2 with GEANT4 tails- fit2.4-4.7,";
   testName +="VWG2 + CB2 with pp13TeV tails- fit2.2-4.5,";
   testName +="VWG2 + CB2 with pp13TeV tails- fit2.4-4.7,";
   testName +="Pol2/Pol3 + CB2 with GEANT3 tails- fit2.2-4.5,";
   testName +="Pol2/Pol3 + CB2 with GEANT3 tails- fit2.4-4.7,";
   testName +="Pol2/Pol3 + CB2 with GEANT4 tails- fit2.2-4.5,";
   testName +="Pol2/Pol3 + CB2 with GEANT4 tails- fit2.4-4.7,";
   testName +="Pol2/Pol3 + CB2 with pp13TeV tails- fit2.2-4.5,";
   testName +="Pol2/Pol3 + CB2 with pp13TeV tails- fit2.4-4.7,";
   testName +="Event mixing CB2 with GEANT3 tails- fit2-5,";
   testName +="Event mixing CB2 with GEANT3 tails- fit2.2-4.5,";
   testName +="Event mixing CB2 with GEANT3 tails- fit2.4-4.7,";
   testName +="Event mixing CB2 with GEANT4 tails- fit2-5,";
   testName +="Event mixing CB2 with GEANT4 tails- fit2.2-4.5,";
   testName +="Event mixing CB2 with GEANT4 tails- fit2.4-4.7,";
   testName +="Event mixing CB2 with pp13TeV tails- fit2-5,";
   testName +="Event mixing CB2 with pp13TeV tails- fit2.2-4.5,";
   testName +="Event mixing CB2 with pp13TeV tails- fit2.4-4.7,";
   testName +="VWG2 + NA60 with GEANT3 tails- fit2.2-4.5,";
   testName +="VWG2 + NA60 with GEANT3 tails- fit2.4-4.7,";
   testName +="Pol2/Pol3 + NA60 with GEANT3 tails- fit2.2-4.5,";
   testName +="Event mixing NA60 with GEANT3 tails- fit2-5,";
   testName +="Event mixing NA60 with GEANT3 tails- fit2.2-4.5,";
   testName +="Event mixing NA60 with GEANT3 tails- fit2.4-4.7,";
   testName +="Event mixing NA60 with GEANT4 tails- fit2-5,";
   testName +="Event mixing NA60 with GEANT4 tails- fit2.2-4.5,";
   testName +="Event mixing NA60 with GEANT4 tails- fit2.4-4.7";


   TObjArray* names = testName.Tokenize(",");
   Int_t ntests = names->GetEntries();
   printf("ntests = %d \n",ntests);
   names->At(3)->Print();
   printf("%s\n",names->At(2)->GetName() );
   // return;

   Double_t JPsi[ntests]={48563,48282,48437,48159,46189,46101,48676,47581,48496,47536,45081,45665,45742,46129,46358,45982,46328,46558,44094,44336,44622,48191,47002,48530,46381,46552,46746,46531,46730,46912};
   Double_t ErrJPsi[ntests]={1070,946,1062,920,959,998,834,852,849,868,900,816,1120,1086,1062,1120,1112,1057,1154,1125,1083,924,976,814,1015,1011,1000,1009,1002,1005};

	TH1* tests = new TH1F("tests_Biswarup","tests_Biswarup",ntests,0,ntests);
	for (int i = 0; i < ntests; ++i)
	{
	 	tests->SetBinContent(i+1,JPsi[i]);
	 	tests->SetBinError(i+1,ErrJPsi[i]);
	 	tests->GetXaxis()->SetBinLabel(i+1,Form("%s",names->At(i)->GetName()));
	}

	tests->Draw();



}