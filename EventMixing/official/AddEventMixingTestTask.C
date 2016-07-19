#ifndef __CINT__
#endif
AliAnalysisTask *AddEventMixingTestTask(TString format = "esd", Bool_t useMC = kFALSE,TString postfix="")
{
   // create manager
   AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) mgr = new AliAnalysisManager("MIX test");



   //MuonTrackCuts
     AliMuonTrackCuts* MuonTrackCuts = new AliMuonTrackCuts("StandardMuonTrackCuts","StandardMuonTrackCuts");
     MuonTrackCuts->SetAllowDefaultParams(kTRUE);
     MuonTrackCuts->SetFilterMask (AliMuonTrackCuts::kMuEta | AliMuonTrackCuts::kMuThetaAbs | AliMuonTrackCuts::kMuPdca);
 //  MuonTrackCuts->SetIsMC();


   std::vector<Double_t> arrayCentralityBins;
   arrayCentralityBins.push_back(0);
   arrayCentralityBins.push_back(10);
   arrayCentralityBins.push_back(20);
   arrayCentralityBins.push_back(30);
   arrayCentralityBins.push_back(40);
   arrayCentralityBins.push_back(50);
   arrayCentralityBins.push_back(60);
   arrayCentralityBins.push_back(70);
   arrayCentralityBins.push_back(80);
   arrayCentralityBins.push_back(90);
   arrayCentralityBins.push_back(-1);

   std::vector<Double_t> arrayRapidityBins;
   arrayRapidityBins.push_back(-4);
   arrayRapidityBins.push_back(-3.75);
   arrayRapidityBins.push_back(-3.5);
   arrayRapidityBins.push_back(-3.25);
   arrayRapidityBins.push_back(-3);
   arrayRapidityBins.push_back(-2.75);
   arrayRapidityBins.push_back(-2.5);
   arrayRapidityBins.push_back(-1);


   std::vector<Double_t> arrayPtBins;
   arrayPtBins.push_back(0);
   arrayPtBins.push_back(1);
   arrayPtBins.push_back(2);
   arrayPtBins.push_back(3);
   arrayPtBins.push_back(4);
   arrayPtBins.push_back(5);
   arrayPtBins.push_back(6);
   arrayPtBins.push_back(8);
   arrayPtBins.push_back(11);
   arrayPtBins.push_back(15);
   arrayPtBins.push_back(-1);



   TaskJpsiPP *task = new TaskJpsiPP("TaskJpsiPP",AliVEvent::kMUS7,244916,246100,arrayCentralityBins,arrayRapidityBins,arrayPtBins);
   task->SetDebugLevel(2);
   // create output container
   AliAnalysisDataContainer *output1 = mgr->CreateContainer("listEvent", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output2 = mgr->CreateContainer("listMuon", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output3 = mgr->CreateContainer("listRawUnlike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output4 = mgr->CreateContainer("listRawPPLike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output5 = mgr->CreateContainer("listRawMMLike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output6 = mgr->CreateContainer("listMixUnlike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output7 = mgr->CreateContainer("listMixPPLike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");
   AliAnalysisDataContainer *output8 = mgr->CreateContainer("listMixMMLike", TList::Class(), AliAnalysisManager::kOutputContainer, "AnalysisResult.root");

  ;

   // add our task to the manager
   mgr->AddTask(task);

   // finaly connect input and output
   mgr->ConnectInput(task, 0,  mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, output1);
   mgr->ConnectOutput(task, 2, output2);
   mgr->ConnectOutput(task, 3, output3);
   mgr->ConnectOutput(task, 4, output4);
   mgr->ConnectOutput(task, 5, output5);
   mgr->ConnectOutput(task, 6, output6);
   mgr->ConnectOutput(task, 7, output7);
   mgr->ConnectOutput(task, 8, output8);
  //  mgr->ConnectOutput(task, 2, output2);


   return task;
}
