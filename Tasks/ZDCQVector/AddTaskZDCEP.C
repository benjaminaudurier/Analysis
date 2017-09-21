AliAnalysisTask * AddTaskZDCEP(const char* outputname,
                               const char* ZDCCalibFileName,
                               const char* ZDCTowerEqFileName,
                               const char* Label ="",
                               const char* suffix="")
{
  // the manager is static, so get the existing manager via the static method
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    printf("No analysis manager to connect to!\n");
    return NULL;
  }

  // just to see if all went well, check if the input event handler has been connected
  if (!mgr->GetInputEventHandler()) {
    printf("This task requires an input event handler!\n");
    return NULL;
  }

  // get the default name of the output file ("AnalysisResults.root")
  TString file = "AnalysisResults";

  // create the flow analysis tasks
  TString AnalysisTaskName = "AnalysisTaskZDCEP";
  AnalysisTaskName += Label;
  AnalysisTaskName += suffix;
  AliAnalysisTaskZDCEP *taskZDC = new AliAnalysisTaskZDCEP(AnalysisTaskName);
  taskZDC->SelectCollisionCandidates(AliVEvent::kMuonSingleLowPt7);

  // add list for ZDC towers gain equalization
  TFile* ZDCTowerEqFile = TFile::Open(ZDCTowerEqFileName,"READ");
  if (!ZDCTowerEqFile){
    AliError("Cannot open file ZDC Tower file %s",ZDCTowerEqFileName);
    return;
  }

  TList* ZDCTowerEqList = static_cast<TList*>(ZDCTowerEqFile->FindObjectAny("EZNcalib"));
  if(ZDCTowerEqList) {
    taskZDC->SetTowerEqList(ZDCTowerEqList);
    cout << "ZDCTowerEq set (from " <<  ZDCTowerEqFileName << ")" << endl;
  } else {
    cout << "ERROR: ZDCTowerEqList not found!" << endl;
    return;
  }
  ZDCTowerEqFile->Close();

  // add list for ZDC Q-vector re-centering
  TFile* ZDCCalibFile = TFile::Open(ZDCCalibFileName,"READ");
  if(!ZDCCalibFile) {
    cout << "ERROR: ZDC calibration not found!" << endl;
    exit(1);
  }
  TList* ZDCCalibList = static_cast<TList*>(ZDCCalibFile->FindObjectAny("Q Vectors"));
  if(ZDCCalibList) {
    taskZDC->SetZDCCalibList(ZDCCalibList);
    cout << "ZDC calibration set (from " <<  ZDCCalibFileName << ")" << endl;
  }
  ZDCCalibFile->Close();

  // connect the task to the analysis manager
  mgr->AddTask(taskZDC);

  // get the common input container from the analysis manager
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();

  // create a data container for the output of the flow event task
  // AliAnalysisDataContainer *coutputFE = mgr->CreateContainer("ZDCEPExchangeContainer",AliFlowEventSimple::Class(),AliAnalysisManager::kExchangeContainer,outputname);
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("ZDCEPExchangeContainer",AliFlowEventSimple::Class(),AliAnalysisManager::kExchangeContainer,outputname);
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("OutPutList",TList::Class(),AliAnalysisManager::kOutputContainer,outputname);

  // connect the input data to the flow event task
  mgr->ConnectInput(taskZDC,0,cinput);
  // and connect the output to the flow event task
  mgr->ConnectOutput(taskZDC,1,coutput1);
  mgr->ConnectOutput(taskZDC,2,coutput2);

  return taskZDC;
}

