#include <Riostream.h>
#include <TFile.h>
#include <TObject.h>
#include <TArrayD.h>
#include <TObjString.h>
#include <TList.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TRandom3.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TArrayD.h>
#include <TMath.h>
#include <TGraph.h>
#include <AliAnalysisMuMu.h>
#include <AliMergeableCollection.h>
#include <TPaveText.h>
#include <TText.h>
#include <AliAnalysisMuMuSpectra.h>
#include <AliAnalysisMuMuResult.h>
#include <AliAnalysisMuMuBinning.h>
#include <TParameter.h>
#include <AliCounterCollection.h>


//---------------------------------------------------------------------------
void plotRelJpsiSystAccxEff(const char* variable="NofJPsi",
                            const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                            const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                            const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                            const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                            const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                            const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                            const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                            const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                            const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                            const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                            const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                            const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                            const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString svariable(variable);
  TString saxis("");
  TString saxisI("");
  TString saxisN("");

  if ( svariable.Contains("NofJPsi") )
  {
    saxis += "N_{Jpsi} / N_{Jpsi}^{int}";
    saxisI += "N_{Jpsi}^{int}";
    saxisN += "N_{Jpsi}^{bin}";
  }
  else if ( svariable.Contains("JPsiMPt") )
  {
    saxis += "#LT #it{p}_{T}^{Jpsi} #GT / #LT #it{p}_{T}^{Jpsi} #GT^{int}";
    saxisI += "#LT #it{p}_{T}^{Jpsi} #GT^{int}";
    saxisN += "#LT #it{p}_{T}^{Jpsi} #GT^{bin}";
  }
  else
  {
    std::cout << "Error: Unrecognized variable " << svariable.Data() << ", maybe you meant NofJPsi or JPsiMPt" << std::endl;
    return;
  }
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  Int_t nbins(0);
  
  TObjArray* res = new TObjArray();
  res->SetOwner(kTRUE);
  TIter next(arr);
  AliAnalysisMuMu* r;
  Int_t j(0);
  TH1* hNJpsi(0x0);
  
  while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
  {
    
    hNJpsi = r->OC()->Histo(Form("/RESULTS-MUON/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/CMUL7-B-NOPF-MUON/V0A/pRABSETAPDCAMATCHLOWPAIRYPAIRPTIN0.0-15.0/hMeanRel%sVSntrcorr",svariable.Data()));
    
    if ( hNJpsi ) res->Add(hNJpsi->Clone());
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No "<< svariable.Data() << " histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( j == 0 )
    {
      nbins = hNJpsi->GetNbinsX();
    }
    
    j++;
}

  //===== Bin by bim systematics
  
  TCanvas* c = new TCanvas(Form("Rel%sAccEffVariation",svariable.Data()),Form("%s  Acc#times#varepsilon variation",saxis.Data()),2,44,1597,980);
//  c->SetBottomMargin(0.01);
  
  if ( nbins == 2 ) c->Divide(2,0,0,0);
  else if ( nbins <= 4 ) c->Divide(2,2,0,0);
  else if ( nbins <= 6 ) c->Divide(3,2,0,0);
  else if ( nbins <= 8 ) c->Divide(4,2,0,0);
  else if ( nbins <= 9 ) c->Divide(3,3,0,0);
  else if ( nbins <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" <<std::endl;
    return;
  }
  
  TIter nextHisto(res);
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  
  Double_t mean(0.),errorMean(0.),sigma(0.);
  std::vector<Double_t> diffs;
  for ( Int_t i = 1 ; i <= nbins ; i++ )
  {
    mean = 0.;
    errorMean = 0.;
    sigma = 0.;
    diffs.clear();
    nextHisto.Reset();
    TH1* hBin = new TH1D(Form("Rel%sAccefSystBin%d",svariable.Data(),i),"",j,0,j);
    TH1* h(0x0);
    Int_t k = 0;
    TString sBinName("");
    while ( (h = static_cast<TH1*>(nextHisto())) )
    {
      Double_t val = h->GetBinContent(i);
      Double_t err = h->GetBinError(i);

      mean += val;
      errorMean += err*err;
      diffs.push_back(val);
      
      if ( k == 0)
      {
        sBinName += h->GetXaxis()->GetBinLabel(i); //Get once the name of the bin to use it later to extract the bin range
        
        //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
        hBin->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
        hBin->GetYaxis()->SetTitleOffset(1.6);
      }
      
      hBin->SetBinContent(k+1,val);
      hBin->SetBinError(k+1,err);
      hBin->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
      
      aBins->Add(hBin);
      k++;
    }

    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    errorMean = TMath::Sqrt(errorMean/k);
    mean = mean/k;
    for (k =0 ; k < j ; k++)
    {
      sigma += (diffs[k]-mean)*(diffs[k]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " : "  << std::endl;
    std::cout << "Mean = " << mean << " +- " << errorMean << " (" << errorMean*100./mean << "%) " << " +- " << sigma << " ("<< sigma*100./mean << "%)" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    TF1* meanF = new TF1("mean","[0]",0,j);
    meanF->SetParameter(0,mean);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,j);
    meanFPS->SetParameter(0,mean+errorMean);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,j);
    meanFMS->SetParameter(0,mean-errorMean);
    meanFMS->SetLineStyle(2);
    
    hBin->GetListOfFunctions()->Add(meanF);
    hBin->GetListOfFunctions()->Add(meanFPS);
    hBin->GetListOfFunctions()->Add(meanFMS);
  
    c->cd(i);
    c->cd(i)->SetRightMargin(0.02325581);
    c->cd(i)->SetLeftMargin(0.1721507);
    c->cd(i)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    Int_t min(0),max(0);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin += sBinName.Data();
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinName.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    ptb->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
  }
  
  TFile* f = new TFile(Form("Rel%sAccxEffSystematics.root",svariable.Data()),"recreate");
  c->Write(Form("Rel%sAccEffVariation",svariable.Data()), TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write(Form("Rel%sAccEffVariationBins",svariable.Data()), TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
}

//---------------------------------------------------------------------------
void plotRelYieldSyst(const char* variable="NofJPsi",
                      const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                      const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                      const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                      const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                      const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                      const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                      const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                      const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                      const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                      const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                      const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                      const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                      const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)

  TString svariable(variable);
  TString saxis("");
  TString saxisI("");
  TString saxisN("");
  
  if ( svariable.Contains("NofJPsi") )
  {
    saxis += "Y_{R}";
//    saxis += "N_{Jpsi} / N_{Jpsi}^{int}";
    saxisI += "Yield";
    saxisN += "N_{Jpsi}^{bin}";
  }
  else if ( svariable.Contains("JPsiMPt") )
  {
    saxis += "#LT #it{p}_{T}^{Jpsi} #GT / #LT #it{p}_{T}^{Jpsi} #GT^{int}";
    saxisI += "MeanPt";
    saxisN += "#LT #it{p}_{T}^{Jpsi} #GT^{bin}";
  }
  else
  {
    std::cout << "Error: Unrecognized variable " << svariable.Data() << ", maybe you meant NofJPsi or JPsiMPt" << std::endl;
    return;
  }
//  TString saxis("Y_{R}");
//  TString saxisI("");
//  TString saxisN("");
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  Int_t nbins(0);
  
  TObjArray* resNJpsi = new TObjArray();
  TObjArray* resNEqMBint = new TObjArray();
  TObjArray* resNEqMBbins = new TObjArray();
  resNJpsi->SetOwner(kTRUE);
  resNEqMBint->SetOwner(kTRUE);
  resNEqMBbins->SetOwner(kTRUE);
  
  TIter next(arr);
  AliAnalysisMuMu* r;
  Int_t j(0);
  TH1* hNJpsi(0x0);
  TH1* hNEqMBint(0x0);
  TH1* hNEqMBbins(0x0);
  
  TH1* hJpsiYield(0x0);
  
  while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
  {
    
    hNJpsi = r->OC()->Histo(Form("/RESULTS-MUON/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/CMUL7-B-NOPF-MUON/V0A/pRABSETAPDCAMATCHLOWPAIRYPAIRPTIN0.0-15.0/hMeanRel%sVSntrcorr",svariable.Data()));
    
    if ( j ==0 ) hJpsiYield = new TH1D("RelJpsiYieldVsNtrCorr","Relative J/#psi yield",hNJpsi->GetNbinsX(),static_cast<const TArrayD*>(hNJpsi->GetXaxis()->GetXbins())->GetArray());
    
    hNEqMBbins = r->OC()->Histo("/FNORM-MUON/PSALL/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/V0A/hNofEqMBVSntrcorrFromMean");
    
    hNEqMBint = r->OC()->Histo("/FNORM-MUON/PSALL/V0A/hNEqMB");
    
    if ( hNJpsi ) resNJpsi->Add(hNJpsi->Clone());
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No NofJpsi histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( svariable.Contains("NofJPsi") )
    {
      if ( hNEqMBbins ) resNEqMBbins->Add(hNEqMBbins->Clone());
      else
      {
        const char* msg = Form("input%d",j);
        std::cout << "Error: No Eq MB bins histo in " << msg << std::endl;
        
        delete names;
        delete n;
        delete o;
        delete p;
        delete q;
        
        return;
      }
      
      if ( hNEqMBint ) resNEqMBint->Add(hNEqMBint->Clone());
      else
      {
        const char* msg = Form("input%d",j);
        std::cout << "Error: No eq MB integrated histo in " << msg << std::endl;
        
        delete names;
        delete n;
        delete o;
        delete p;
        delete q;
        
        return;
      }
    }
    
    if ( j == 0 )
    {
      nbins = hNJpsi->GetNbinsX();
    }
    
    j++;
  }
  
  //===== Bin by bim systematics
  
  TCanvas* c = new TCanvas(Form("CombRel%sSyst",saxisI.Data()),Form("Combined relative %s systematic uncertainty",saxisI.Data()),2,44,1597,980);
  //  c->SetBottomMargin(0.01);
  
  if ( nbins == 2 ) c->Divide(2,0,0,0);
  else if ( nbins <= 4 ) c->Divide(2,2,0,0);
  else if ( nbins <= 6 ) c->Divide(3,2,0,0);
  else if ( nbins <= 8 ) c->Divide(4,2,0,0);
  else if ( nbins <= 9 ) c->Divide(3,3,0,0);
  else if ( nbins <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" <<std::endl;
    return;
  }
  
  TIter nextHistoJpsi(resNJpsi);
  TIter nextHistoMBInt(resNEqMBint);
  TIter nextHistoMBBins(resNEqMBbins);
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  
  Double_t mean(0.),errorMean(0.),sigma(0.);
  std::vector<Double_t> diffs;
  for ( Int_t i = 1 ; i <= nbins ; i++ )
  {
    mean = 0.;
    errorMean = 0.;
    sigma = 0.;
    diffs.clear();
    nextHistoJpsi.Reset();
    nextHistoMBInt.Reset();
    nextHistoMBBins.Reset();
    TH1* hBin = new TH1D(Form("Rel%sCombSystBin%d",saxisI.Data(),i),"",j,0,j);
    TH1* hJ(0x0);
    TH1* hMBI(0x0);
    TH1* hMBB(0x0);

    Int_t k = 0;
    TString sBinName("");
    if ( svariable.Contains("NofJPsi") )
    {
      while ( (hJ = static_cast<TH1*>(nextHistoJpsi())) && (hMBI = static_cast<TH1*>(nextHistoMBInt())) && (hMBB = static_cast<TH1*>(nextHistoMBBins())) )
      {
        Double_t valJpsi = hJ->GetBinContent(i);
        Double_t errJpsi = hJ->GetBinError(i);
        
        Double_t valMBI = hMBI->GetBinContent(1);
        Double_t errMBI = hMBI->GetBinError(1);
        
        Double_t valMBB = hMBB->GetBinContent(i+2);
        Double_t errMBB = hMBB->GetBinError(i+2);
        
        Double_t yield = valJpsi*(valMBI/valMBB);
        Double_t yieldErr = yield*TMath::Sqrt(TMath::Power(errJpsi/valJpsi,2.) +TMath::Power(errMBI/valMBI,2.) +TMath::Power(errMBB/valMBB,2.));
        
        mean += yield;
        errorMean += yieldErr*yieldErr;
        diffs.push_back(yield);
        
        if ( k == 0)
        {
          sBinName += hJ->GetXaxis()->GetBinLabel(i); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hBin->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hBin->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hBin->SetBinContent(k+1,yield);
        hBin->SetBinError(k+1,yieldErr);
        hBin->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aBins->Add(hBin);
        k++;
      }
    }
    else
    {
      while ( (hJ = static_cast<TH1*>(nextHistoJpsi())) )
      {
        Double_t valJpsi = hJ->GetBinContent(i);
        Double_t errJpsi = hJ->GetBinError(i);
        
        mean += valJpsi;
        errorMean += errJpsi*errJpsi;
        diffs.push_back(valJpsi);
        
        if ( k == 0)
        {
          sBinName += hJ->GetXaxis()->GetBinLabel(i); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hBin->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hBin->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hBin->SetBinContent(k+1,valJpsi);
        hBin->SetBinError(k+1,errJpsi);
        hBin->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aBins->Add(hBin);
        k++;
      }
    }
    
    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    errorMean = TMath::Sqrt(errorMean/k);
    mean = mean/k;
    for (k =0 ; k < j ; k++)
    {
      sigma += (diffs[k]-mean)*(diffs[k]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " : "  << std::endl;
    std::cout << "Mean = " << mean << " +- " << errorMean << " (" << errorMean*100./mean << "%) " << " +- " << sigma << " ("<< sigma*100./mean << "%)" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    hJpsiYield->SetBinContent(i,mean);
    hJpsiYield->SetBinError(i,errorMean);
    hJpsiYield->GetXaxis()->SetBinLabel(i,sBinName.Data());
    
    TF1* meanF = new TF1("mean","[0]",0,j);
    meanF->SetParameter(0,mean);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,j);
    meanFPS->SetParameter(0,mean+errorMean);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,j);
    meanFMS->SetParameter(0,mean-errorMean);
    meanFMS->SetLineStyle(2);
    
    hBin->GetListOfFunctions()->Add(meanF);
    hBin->GetListOfFunctions()->Add(meanFPS);
    hBin->GetListOfFunctions()->Add(meanFMS);
    
    c->cd(i);
    c->cd(i)->SetRightMargin(0.02325581);
    c->cd(i)->SetLeftMargin(0.1721507);
    c->cd(i)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    Int_t min(0),max(0);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin += sBinName.Data();
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinName.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s combined variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    ptb->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
  }
  
  TFile* f = new TFile(Form("Rel%sSystematics.root",saxisI.Data()),"recreate");
  c->Write(Form("Rel%sVariation",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write(Form("Rel%sVariationBins",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  TFile* fy = new TFile(Form("Rel%sVsNtrCorr.root",saxisI.Data()),"recreate");
  hJpsiYield->Write(Form("Rel%sVsNtrCorr",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  fy->Close();
  
}


//---------------------------------------------------------------------------
void plotAbsYieldSyst(const char* variable="NofJPsi",
                      const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                      const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                      const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                      const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                      const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                      const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                      const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                      const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                      const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                      const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                      const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                      const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                      const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString svariable(variable);
  TString saxis("");
  TString saxisI("");
  TString saxisN("");
  TString sspectraName("");
  TString sspectraIntName("");
  
  if ( svariable.Contains("NofJPsi") )
  {
    saxis += "Y";
    //    saxis += "N_{Jpsi} / N_{Jpsi}^{int}";
    saxisI += "Yield";
    saxisN += "N_{Jpsi}^{bin}";
    sspectraName += "PSI-NTRCORR-AccEffCorr";
    sspectraIntName += "PSI-INTEGRATED-AccEffCorr";
  }
  else if ( svariable.Contains("MeanPtJPsi") )
  {
    saxis += "#LT #it{p}_{T}^{Jpsi} #GT";
    saxisI += "MeanPt";
    saxisN += "#LT #it{p}_{T}^{Jpsi} #GT^{bin}";
    sspectraName += "PSI-NTRCORR-AccEffCorr-MeanPtVsMinvUS";
    sspectraIntName += "PSI-INTEGRATED-AccEffCorr-MeanPtVsMinvUS";
  }
  else
  {
    std::cout << "Error: Unrecognized variable " << svariable.Data() << ", maybe you meant NofJPsi or MeanPtJPsi" << std::endl;
    return;
  }
  //  TString saxis("Y_{R}");
  //  TString saxisI("");
  //  TString saxisN("");
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  Int_t nOfTests(5); //Number of Accef test plus nominal value
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  Int_t nbins(0);
  
  TObjArray* resNJpsi = new TObjArray();
  TObjArray* resNJpsiInt = new TObjArray();
  TObjArray* resNEqMBint = new TObjArray();
  TObjArray* resNEqMBbins = new TObjArray();
  resNJpsi->SetOwner(kTRUE);
  resNJpsiInt->SetOwner(kTRUE);
  resNEqMBint->SetOwner(kTRUE);
  resNEqMBbins->SetOwner(kTRUE);

  
  TIter next(arr);
  AliAnalysisMuMu* r;
  Int_t j(0);
  TH1* hNJpsi(0x0);
  TH1* hNJpsiInt(0x0);
  TH1* hNEqMBint(0x0);
  TH1* hNEqMBbins(0x0);
  
  TH1* hJpsiYield(0x0);
  TH1* hJpsiYieldInt(0x0);
  
  while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
  {
    
    AliAnalysisMuMuSpectra* s = static_cast<AliAnalysisMuMuSpectra*>(r->OC()->GetObject(Form("/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/CMUL7-B-NOPF-MUON/V0A/pRABSETAPDCAMATCHLOWPAIRYPAIRPTIN0.0-15.0/%s",sspectraName.Data())));
    
    hNJpsi = s->Plot(svariable.Data(),"",kFALSE);
    
    AliAnalysisMuMuSpectra* sint = static_cast<AliAnalysisMuMuSpectra*>(r->OC()->GetObject(Form("/PSALL/CMUL7-B-NOPF-MUON/V0A/pRABSETAPDCAMATCHLOWPAIRYPAIRPTIN0.0-15.0/%s",sspectraIntName.Data())));
    
    hNJpsiInt = sint->Plot(svariable.Data(),"",kFALSE);
    
    if ( j ==0 )
    {
      hJpsiYieldInt = new TH1D("JpsiYieldInt","Integrated J/#psi yield",1,0.,1.);
      
      const TArrayD* binArray = hNJpsi->GetXaxis()->GetXbins(); //Just get the axis to make the plots for the different MU's
      Int_t size = binArray->GetSize()-1;
      Double_t* axis = new Double_t[size];
      for ( Int_t k = 0 ; k < size ; k++ )
      {
        axis[k] = binArray->At(k+1);
      }
      
      //      hJpsiYield = new TH1D("JpsiYieldVsNtrCorr","J/#psi yield",hNJpsi->GetNbinsX(),static_cast<const TArrayD*>(hNJpsi->GetXaxis()->GetXbins())->GetArray());
      hJpsiYield = new TH1D("JpsiYieldVsNtrCorr","J/#psi yield vs N_{tr}^{corr}",size-1,axis);
    }
    
    hNEqMBbins = r->OC()->Histo("/FNORM-MUON/PSALL/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/V0A/hNofEqMBVSntrcorrFromMean");
    
    hNEqMBint = r->OC()->Histo("/FNORM-MUON/PSALL/V0A/hNEqMB");
    
    if ( hNJpsi ) resNJpsi->Add(hNJpsi);
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No NofJpsi histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( hNJpsiInt ) resNJpsiInt->Add(hNJpsiInt);
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No integrated NofJpsi histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( svariable.Contains("NofJPsi") )
    {
      if ( hNEqMBbins ) resNEqMBbins->Add(hNEqMBbins->Clone());
      else
      {
        const char* msg = Form("input%d",j);
        std::cout << "Error: No Eq MB bins histo in " << msg << std::endl;
        
        delete names;
        delete n;
        delete o;
        delete p;
        delete q;
        
        return;
      }
      
      if ( hNEqMBint ) resNEqMBint->Add(hNEqMBint->Clone());
      else
      {
        const char* msg = Form("input%d",j);
        std::cout << "Error: No eq MB integrated histo in " << msg << std::endl;
        
        delete names;
        delete n;
        delete o;
        delete p;
        delete q;
        
        return;
      }
    }
    
    if ( j == 0 )
    {
      nbins = hNJpsi->GetNbinsX()-1;
    }
    
    j++;
  }
  
  
  
  TIter nextHistoJpsi(resNJpsi);
  TIter nextHistoJpsiInt(resNJpsiInt);
  TIter nextHistoMBInt(resNEqMBint);
  TIter nextHistoMBBins(resNEqMBbins);
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  TObjArray* aInt = new TObjArray();
  aInt->SetOwner(kTRUE);
  
  
  Double_t bR = 0.05961; // BR(JPsi->mu+mu-)
  Double_t bRerror = 0.00033 ;
  
  
  //====== Integrated systematics computation
  TCanvas* cInt = new TCanvas(Form("CombInt%sSyst",saxisI.Data()),Form("Combined integrated %s systematic uncertainty",saxisI.Data()),2,44,1597,980);
  
  Double_t mean(0.),errorMean(0.),sigma(0.);
  std::vector<Double_t> diffs;
  for ( Int_t i = 0 ; i < 1 ; i++ )
  {
    mean = 0.;
    errorMean = 0.;
    sigma = 0.;
    diffs.clear();
    nextHistoJpsiInt.Reset();
    nextHistoMBInt.Reset();
    TH1* hInt = new TH1D(Form("%sCombSystInt",saxisI.Data()),"",nOfTests,0,nOfTests);
    TH1* hJInt(0x0);
    TH1* hMBI(0x0);
    
    Int_t k = 0;
    TString sBinName("");
    if ( svariable.Contains("NofJPsi") )
    {
      while ( (hJInt = static_cast<TH1*>(nextHistoJpsiInt())) && (hMBI = static_cast<TH1*>(nextHistoMBInt())) )
      {
        if ( static_cast<TObjString*>(names->At(k))->String().Contains("Test") ) continue;
        Double_t valJpsi = hJInt->GetBinContent(i+1);
        Double_t errJpsi = hJInt->GetBinError(i+1);
        
        Double_t valMBI = hMBI->GetBinContent(1);
        Double_t errMBI = hMBI->GetBinError(1);
        
        Double_t yield = valJpsi/(bR*valMBI);
        Double_t yieldErr = yield*TMath::Sqrt(TMath::Power(errJpsi/valJpsi,2.) + TMath::Power(bRerror/bR,2.) + TMath::Power(errMBI/valMBI,2.));
        
        mean += yield;
        errorMean += yieldErr*yieldErr;
        diffs.push_back(yield);
        
        if ( k == 0)
        {
          sBinName += hJInt->GetXaxis()->GetBinLabel(i+1); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hInt->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hInt->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hInt->SetBinContent(k+1,yield);
        hInt->SetBinError(k+1,yieldErr);
        hInt->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aInt->Add(hInt);
        k++;
      }
    }
    else
    {
      while ( (hJInt = static_cast<TH1*>(nextHistoJpsiInt())) )
      {
        if ( static_cast<TObjString*>(names->At(k))->String().Contains("Test") ) continue;
        
        Double_t valJpsi = hJInt->GetBinContent(i+1);
        Double_t errJpsi = hJInt->GetBinError(i+1);
        
        mean += valJpsi;
        errorMean += errJpsi*errJpsi;
        diffs.push_back(valJpsi);
        
        if ( k == 0)
        {
          sBinName += hJInt->GetXaxis()->GetBinLabel(i+1); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hInt->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hInt->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hInt->SetBinContent(k+1,valJpsi);
        hInt->SetBinError(k+1,errJpsi);
        hInt->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aInt->Add(hInt);
        k++;
      }
    }
    
    Double_t minimum = hInt->GetMinimum();
    Double_t maximum = hInt->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    errorMean = TMath::Sqrt(errorMean/k);
    mean = mean/k;
    for (k =0 ; k < nOfTests ; k++)
    {
      sigma += (diffs[k]-mean)*(diffs[k]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i+1 << " : "  << std::endl;
    std::cout << "Mean = " << mean << " +- " << errorMean << " (" << errorMean*100./mean << "%) " << " +- " << sigma << " ("<< sigma*100./mean << "%)" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    hJpsiYieldInt->SetBinContent(i+1,mean);
    hJpsiYieldInt->SetBinError(i+1,errorMean);
    hJpsiYieldInt->GetXaxis()->SetBinLabel(i+1,sBinName.Data());
    
    TF1* meanF = new TF1("mean","[0]",0,j);
    meanF->SetParameter(0,mean);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,j);
    meanFPS->SetParameter(0,mean+errorMean);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,j);
    meanFMS->SetParameter(0,mean-errorMean);
    meanFMS->SetLineStyle(2);
    
    hInt->GetListOfFunctions()->Add(meanF);
    hInt->GetListOfFunctions()->Add(meanFPS);
    hInt->GetListOfFunctions()->Add(meanFMS);
    
    cInt->cd(i+1);
    cInt->cd(i+1)->SetRightMargin(0.02325581);
    cInt->cd(i+1)->SetLeftMargin(0.1721507);
    cInt->cd(i+1)->SetBottomMargin(0.15);
    
    
    hInt->SetStats(0);
    
    hInt->GetYaxis()->SetLabelSize(0.04);
    hInt->GetYaxis()->SetTitleSize(0.04);
    hInt->GetXaxis()->SetLabelSize(0.05);
    
    hInt->Draw();
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("Integrated %s combined variation",saxis.Data()));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    ptb->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
  }
  
  
  //====== Bin by bin computation
  
  TCanvas* c = new TCanvas(Form("Comb%sSyst",saxisI.Data()),Form("Combined %s systematic uncertainty",saxisI.Data()),2,44,1597,980);
  //  c->SetBottomMargin(0.01);
  
  if ( nbins == 2 ) c->Divide(2,0,0,0);
  else if ( nbins <= 4 ) c->Divide(2,2,0,0);
  else if ( nbins <= 6 ) c->Divide(3,2,0,0);
  else if ( nbins <= 8 ) c->Divide(4,2,0,0);
  else if ( nbins <= 9 ) c->Divide(3,3,0,0);
  else if ( nbins <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" <<std::endl;
    return;
  }
  

  for ( Int_t i = 1 ; i <= nbins ; i++ )
  {
    mean = 0.;
    errorMean = 0.;
    sigma = 0.;
    diffs.clear();
    nextHistoJpsi.Reset();
    nextHistoMBInt.Reset();
    nextHistoMBBins.Reset();
    TH1* hBin = new TH1D(Form("%sCombSystBin%d",saxisI.Data(),i),"",j,0,j);
    TH1* hJ(0x0);
    TH1* hMBI(0x0);
    TH1* hMBB(0x0);
    
    Int_t k = 0;
    TString sBinName("");
    if ( svariable.Contains("NofJPsi") )
    {
      while ( (hJ = static_cast<TH1*>(nextHistoJpsi())) && (hMBI = static_cast<TH1*>(nextHistoMBInt())) && (hMBB = static_cast<TH1*>(nextHistoMBBins())) )
      {
        Double_t valJpsi = hJ->GetBinContent(i+1);
        Double_t errJpsi = hJ->GetBinError(i+1);
        
        Double_t valMBI = hMBI->GetBinContent(1);
        Double_t errMBI = hMBI->GetBinError(1);
        
        Double_t valMBB = hMBB->GetBinContent(i+2);
        Double_t errMBB = hMBB->GetBinError(i+2);
        
        Double_t yield = valJpsi/(bR*valMBB);
        Double_t yieldErr = yield*TMath::Sqrt(TMath::Power(errJpsi/valJpsi,2.) + TMath::Power(bRerror/bR,2.) + TMath::Power(errMBB/valMBB,2.));
        
        mean += yield;
        errorMean += yieldErr*yieldErr;
        diffs.push_back(yield);
        
        if ( k == 0)
        {
          sBinName += hJ->GetXaxis()->GetBinLabel(i+1); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hBin->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hBin->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hBin->SetBinContent(k+1,yield);
        hBin->SetBinError(k+1,yieldErr);
        hBin->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aBins->Add(hBin);
        k++;
      }
    }
    else
    {
      while ( (hJ = static_cast<TH1*>(nextHistoJpsi())) )
      {
        Double_t valJpsi = hJ->GetBinContent(i+1);
        Double_t errJpsi = hJ->GetBinError(i+1);
        
        mean += valJpsi;
        errorMean += errJpsi*errJpsi;
        diffs.push_back(valJpsi);
        
        if ( k == 0)
        {
          sBinName += hJ->GetXaxis()->GetBinLabel(i+1); //Get once the name of the bin to use it later to extract the bin range
          
          //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
          hBin->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
          hBin->GetYaxis()->SetTitleOffset(1.6);
        }
        
        hBin->SetBinContent(k+1,valJpsi);
        hBin->SetBinError(k+1,errJpsi);
        hBin->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
        
        aBins->Add(hBin);
        k++;
      }
    }
    std::cout << sBinName.Data() << std::endl;
    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    errorMean = TMath::Sqrt(errorMean/k);
    mean = mean/k;
    for (k =0 ; k < j ; k++)
    {
      sigma += (diffs[k]-mean)*(diffs[k]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " : "  << std::endl;
    std::cout << "Mean = " << mean << " +- " << errorMean << " (" << errorMean*100./mean << "%) " << " +- " << sigma << " ("<< sigma*100./mean << "%)" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    hJpsiYield->SetBinContent(i,mean);
    hJpsiYield->SetBinError(i,errorMean);
    hJpsiYield->GetXaxis()->SetBinLabel(i,sBinName.Data());
    
    TF1* meanF = new TF1("mean","[0]",0,j);
    meanF->SetParameter(0,mean);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,j);
    meanFPS->SetParameter(0,mean+errorMean);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,j);
    meanFMS->SetParameter(0,mean-errorMean);
    meanFMS->SetLineStyle(2);
    
    hBin->GetListOfFunctions()->Add(meanF);
    hBin->GetListOfFunctions()->Add(meanFPS);
    hBin->GetListOfFunctions()->Add(meanFMS);
    
    c->cd(i);
    c->cd(i)->SetRightMargin(0.02325581);
    c->cd(i)->SetLeftMargin(0.1721507);
    c->cd(i)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    Int_t min(0),max(0);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin += sBinName.Data();
    sBinName.Remove(0,sBinName.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinName.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s combined variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    ptb->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
  }
  
  TFile* f = new TFile(Form("%sSystematics.root",saxisI.Data()),"recreate");
  c->Write(Form("%sVariation",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write(Form("%sVariationBins",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
  TFile* fInt = new TFile(Form("%sIntegratedSystematics.root",saxisI.Data()),"recreate");
  cInt->Write(Form("%sVariation",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  aInt->Write(Form("%sVariationInt",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  fInt->Close();
  
  TFile* fy = new TFile(Form("%sVsNtrCorr.root",saxisI.Data()),"recreate");
  hJpsiYield->Write(Form("%sVsNtrCorr",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  fy->Close();
  
  TFile* fyInt = new TFile(Form("%sIntegrated.root",saxisI.Data()),"recreate");
  hJpsiYieldInt->Write(Form("%sIntegrated",saxisI.Data()), TObject::kOverwrite | TObject::kSingleKey);
  fyInt->Close();
  
}


//---------------------------------------------------------------------------
void plotFNormSyst(const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                   const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                   const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                   const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                   const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                   const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                   const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                   const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                   const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                   const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                   const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                   const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                   const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString saxis("F_{Norm}");
  TString saxisI("N_{MB eq.}");
  TString saxisN("N_{CMUL}");
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  Int_t nbins(0);
  
  TObjArray* resFN = new TObjArray();
  TObjArray* resMB = new TObjArray();
  TObjArray* resCC = new TObjArray();
  resCC->SetOwner(kTRUE);
  resFN->SetOwner(kTRUE);
  resMB->SetOwner(kTRUE);
  TIter next(arr);
  AliAnalysisMuMu* r;
  Int_t j(0);
  TH1* hMFNorm(0x0);
  TH1* hMEqMB(0x0);
  
  while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
  {
    hMFNorm = r->OC()->Histo("/FNORM-MUON/PSALL/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/V0A/hFNormVSntrcorrFromMean");
    
    hMEqMB = r->OC()->Histo("/FNORM-MUON/PSALL/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/V0A/hNofEqMBVSntrcorrFromMean");
    
    resCC->Add(r->CC()->Clone());
    
    if ( hMFNorm ) resFN->Add(hMFNorm->Clone());
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No Fnorm histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( hMEqMB ) resMB->Add(hMEqMB->Clone());
    else
    {
      const char* msg = Form("input%d",j);
      std::cout << "Error: No MB histo in " << msg << std::endl;
      
      delete names;
      delete n;
      delete o;
      delete p;
      delete q;
      
      return;
    }
    
    if ( j == 0 )
    {
      nbins = hMFNorm->GetNbinsX();
    }
    
    j++;
  }
  
  //===== Bin by bim systematics
  
  TCanvas* cFn = new TCanvas("FNormVariation","F_{N} variation",2,44,1597,980);
  TCanvas* cMB = new TCanvas("EqNMBVariation","Nof eq. MB variation",2,44,1597,980);
  TCanvas* cMUL = new TCanvas("CMULVariation","Nof CMUL variation",2,44,1597,980);

  //  c->SetBottomMargin(0.01);
  
  if ( (nbins-3) == 2 )
  {
    cFn->Divide(2,0,0,0);
    cMB->Divide(2,0,0,0);
    cMUL->Divide(2,0,0,0);
    
  }
  else if ( (nbins-3) <= 4 )
  {
    cFn->Divide(2,2,0,0);
    cMB->Divide(2,2,0,0);
    cMUL->Divide(2,2,0,0);
  }
  else if ( (nbins-3) <= 6 )
  {
    cFn->Divide(3,2,0,0);
    cMB->Divide(3,2,0,0);
    cMUL->Divide(3,2,0,0);
  }
  else if ( (nbins-3) <= 8 )
  {
    cFn->Divide(4,2,0,0);
    cMB->Divide(4,2,0,0);
    cMUL->Divide(4,2,0,0);
  }
  else if ( (nbins-3) <= 9 )
  {
    cFn->Divide(3,3,0,0);
    cMB->Divide(3,3,0,0);
    cMUL->Divide(3,3,0,0);
  }
  else if ( (nbins-3) <= 12 )
  {
    cFn->Divide(4,3,0,0);
    cMB->Divide(4,3,0,0);
    cMUL->Divide(4,3,0,0);
  }
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" <<std::endl;
    return;
  }
  
  TIter nextHistoFn(resFN);
  TIter nextHistoMB(resMB);
  TIter nextCC(resCC);
  TObjArray* aBinsFn = new TObjArray();
  aBinsFn->SetOwner(kTRUE);
  TObjArray* aBinsMB = new TObjArray();
  aBinsMB->SetOwner(kTRUE);
  TObjArray* aBinsMUL = new TObjArray();
  aBinsMUL->SetOwner(kTRUE);
  
  AliCounterCollection* cc(0x0);
  
  Double_t meanFn(0.),errorMeanFn(0.),sigmaFn(0.);
  Double_t meanMB(0.),errorMeanMB(0.),sigmaMB(0.);
  Double_t meanCMUL(0.),errorMeanCMUL(0.),sigmaCMUL(0.);
  std::vector<Double_t> diffsFn;
  std::vector<Double_t> diffsMB;
  std::vector<Double_t> diffsCMUL;
  for ( Int_t i = 3 ; i <= nbins-1 ; i++ )
  {
    meanFn = 0.;
    errorMeanFn = 0.;
    sigmaFn = 0.;
    diffsFn.clear();
    nextHistoFn.Reset();
    TH1* hBinFn = new TH1D(Form("FNormSystBin%d",i),"",j,0,j);
    TH1* h(0x0);
    Int_t k = 0;
    TString sBinName("");
    while ( (h = static_cast<TH1*>(nextHistoFn())) )
    {
      Double_t valFn = h->GetBinContent(i);
      Double_t errFn = h->GetBinError(i);
      
      meanFn += valFn;
      errorMeanFn += errFn*errFn;
      diffsFn.push_back(valFn);
      
      if ( k == 0)
      {
        sBinName += h->GetXaxis()->GetBinLabel(i); //Get once the name of the bin to use it later to extract the bin range
        
        //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
        hBinFn->GetYaxis()->SetTitle(Form("#LT %s #GT",saxis.Data()));
        hBinFn->GetYaxis()->SetTitleOffset(1.6);

      }
      
      hBinFn->SetBinContent(k+1,valFn);
      hBinFn->SetBinError(k+1,errFn);
      hBinFn->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
      
      aBinsFn->Add(hBinFn);
      k++;
    }
    
    Double_t minimumFn = hBinFn->GetMinimum();
    Double_t maximumFn = hBinFn->GetMaximum();
    
    Double_t systFn = (maximumFn-minimumFn)*100./minimumFn;
    
    errorMeanFn = TMath::Sqrt(errorMeanFn/k);
    meanFn = meanFn/k;
    for (k =0 ; k < j ; k++)
    {
      sigmaFn += (diffsFn[k]-meanFn)*(diffsFn[k]-meanFn);
    }
    
    sigmaFn = TMath::Sqrt(sigmaFn/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " FNorm : "  << std::endl;
    std::cout << "Mean = " << meanFn << " +- " << errorMeanFn << " (" << errorMeanFn*100./meanFn << "%) " << " +- " << sigmaFn << " ("<< sigmaFn*100./meanFn << "%)" << std::endl;
    std::cout << "Syst max = " << systFn << "% " << std::endl;
    
    TF1* meanF = new TF1("mean","[0]",0,j);
    meanF->SetParameter(0,meanFn);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,j);
    meanFPS->SetParameter(0,meanFn+errorMeanFn);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,j);
    meanFMS->SetParameter(0,meanFn-errorMeanFn);
    meanFMS->SetLineStyle(2);
    
    hBinFn->GetListOfFunctions()->Add(meanF);
    hBinFn->GetListOfFunctions()->Add(meanFPS);
    hBinFn->GetListOfFunctions()->Add(meanFMS);
    
    cFn->cd(i-2);
    cFn->cd(i-2)->SetRightMargin(0.02325581);
    cFn->cd(i-2)->SetLeftMargin(0.1721507);
    cFn->cd(i-2)->SetBottomMargin(0.15);
    
    
    hBinFn->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBinFn->GetYaxis()->SetLabelSize(0.04);
      hBinFn->GetYaxis()->SetTitleSize(0.04);
      hBinFn->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBinFn->GetYaxis()->SetLabelSize(0.05);
      hBinFn->GetYaxis()->SetTitleSize(0.05);
      hBinFn->GetXaxis()->SetLabelSize(0.07);
    }
    hBinFn->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    TString sBinNameMax(sBinName.Data());
    Int_t min(0),max(0);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin += sBinNameMax.Data();
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinNameMax.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    pt1b->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",systFn));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigmaFn*100./meanFn));
    pt1b->Draw("same");
    
    
    //==================
    
    meanCMUL = 0.;
    errorMeanCMUL = 0.;
    sigmaCMUL = 0.;
    diffsCMUL.clear();
    nextCC.Reset();
    TH1* hBinCMUL = new TH1D(Form("CMULVarBin%d",i),"",j,0,j);
    k = 0;
    while ( (cc = static_cast<AliCounterCollection*>(nextCC())) )
    {
      Double_t NofEvents = cc->GetSum(Form("/event:PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/trigger:CMUL7-B-NOPF-MUON/centrality:V0A/bin:%s",sBinName.Data()));
      
      meanCMUL += NofEvents;
      errorMeanCMUL += TMath::Sqrt(NofEvents)*TMath::Sqrt(NofEvents);
      diffsCMUL.push_back(NofEvents);
      
      if ( k == 0)
      {
        //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
        hBinCMUL->GetYaxis()->SetTitle(Form("%s",saxisN.Data()));
        hBinCMUL->GetYaxis()->SetTitleOffset(1.6);
        hBinCMUL->GetYaxis()->SetTitleOffset(1.6);
        
      }
      
      hBinCMUL->SetBinContent(k+1,NofEvents);
      hBinCMUL->SetBinError(k+1,TMath::Sqrt(NofEvents));
      hBinCMUL->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
      
      
      //      std::cout << "NofEvents = " << NofEvents << std::endl;
      
      aBinsMUL->Add(hBinCMUL);
      k++;
    }
    
    Double_t minimumMUL = hBinCMUL->GetMinimum();
    Double_t maximumMUL = hBinCMUL->GetMaximum();
    
    Double_t systMUL = (maximumMUL-minimumMUL)*100./minimumMUL;
    
    errorMeanCMUL = TMath::Sqrt(errorMeanCMUL/k);
    meanCMUL = meanCMUL/k;
    for (k =0 ; k < j ; k++)
    {
      sigmaCMUL += (diffsCMUL[k]-meanCMUL)*(diffsCMUL[k]-meanCMUL);
    }
    
    sigmaCMUL = TMath::Sqrt(sigmaCMUL/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " Nof CMUL : "  << std::endl;
    std::cout << "Mean = " << meanCMUL << " +- " << errorMeanCMUL << " (" << errorMeanCMUL*100./meanCMUL << "%) " << " +- " << sigmaCMUL << " ("<< sigmaCMUL*100./meanCMUL << "%)" << std::endl;
    std::cout << "Syst max = " << systMUL << "% " << std::endl;
    
    TF1* meanFMUL = new TF1("mean","[0]",0,j);
    meanFMUL->SetParameter(0,meanCMUL);
    
    TF1* meanFPSMUL = new TF1("meanPS","[0]",0,j);
    meanFPSMUL->SetParameter(0,meanCMUL+errorMeanCMUL);
    meanFPSMUL->SetLineStyle(2);
    
    TF1* meanFMSMUL = new TF1("meanMS","[0]",0,j);
    meanFMSMUL->SetParameter(0,meanCMUL-errorMeanCMUL);
    meanFMSMUL->SetLineStyle(2);
    
    hBinCMUL->GetListOfFunctions()->Add(meanFMUL);
    hBinCMUL->GetListOfFunctions()->Add(meanFPSMUL);
    hBinCMUL->GetListOfFunctions()->Add(meanFMSMUL);
    
    cMUL->cd(i-2);
    cMUL->cd(i-2)->SetRightMargin(0.02325581);
    cMUL->cd(i-2)->SetLeftMargin(0.1721507);
    cMUL->cd(i-2)->SetBottomMargin(0.15);
    cMUL->cd(i-2)->SetTopMargin(0.10);
    
    
    hBinCMUL->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBinCMUL->GetYaxis()->SetLabelSize(0.04);
      hBinCMUL->GetYaxis()->SetTitleSize(0.04);
      hBinCMUL->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBinCMUL->GetYaxis()->SetLabelSize(0.05);
      hBinCMUL->GetYaxis()->SetTitleSize(0.05);
      hBinCMUL->GetXaxis()->SetLabelSize(0.07);
    }
    hBinCMUL->Draw();
    
    
    TPaveText *ptbN = new TPaveText(0.2516049,0.8514274,0.9375592,0.9952622,"blNDC");
    ptbN->SetName("binName");
    ptbN->SetBorderSize(0);
    ptbN->SetFillColor(17);
    ptbN->SetFillStyle(3001);
    ptbN->SetTextFont(42);
    TText *textbN = ptbN->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxisN.Data(),min,max));
    
    ptbN->Draw("same");
    
    TPaveText* pt1bN = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    pt1bN->SetName("syst");
    pt1bN->SetBorderSize(0);
    pt1bN->SetFillColor(5);
    pt1bN->SetFillStyle(3001);
    pt1bN->SetTextFont(42);
    TText* text1bN = pt1bN->AddText(Form("#Delta_{max} = %1.2f %%",systMUL));
    TText* text3bN = pt1bN->AddText(Form("#sigma = %1.2f %%",sigmaCMUL*100./meanCMUL));
    pt1bN->Draw("same");
    
    
    //==================
    
    meanMB = 0.;
    errorMeanMB = 0.;
    sigmaMB = 0.;
    diffsMB.clear();
    nextHistoMB.Reset();
    TH1* hBinMB = new TH1D(Form("NEqMBSystBin%d",i),"",j,0,j);
    h = 0x0;
    k = 0;
    while ( (h = static_cast<TH1*>(nextHistoMB())) )
    {
      Double_t valMB = h->GetBinContent(i);
      Double_t errMB = h->GetBinError(i);
      
      meanMB += valMB;
      errorMeanMB += errMB*errMB;
      diffsMB.push_back(valMB);
      
      if ( k == 0)
      {
        //        sBinName += h->GetXaxis()->GetBinLabel(i); //Get once the name of the bin to use it later to extract the bin range
        
        //        hBin->SetTitle(Form("%s  Acc#times#varepsilon variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
        hBinMB->GetYaxis()->SetTitle(Form("#LT %s #GT",saxisI.Data()));
        hBinMB->GetYaxis()->SetTitleOffset(1.6);
      }
      
      hBinMB->SetBinContent(k+1,valMB);
      hBinMB->SetBinError(k+1,errMB);
      hBinMB->GetXaxis()->SetBinLabel(k+1,static_cast<TObjString*>(names->At(k))->GetName());
      
      aBinsMB->Add(hBinMB);
      k++;
    }
    
    
    Double_t minimumMB = hBinMB->GetMinimum();
    Double_t maximumMB = hBinMB->GetMaximum();
    
    Double_t systMB = (maximumMB-minimumMB)*100./minimumMB;
    
    errorMeanMB = TMath::Sqrt(errorMeanMB/k);
    meanMB = meanMB/k;
    for (k =0 ; k < j ; k++)
    {
      sigmaMB += (diffsMB[k]-meanMB)*(diffsMB[k]-meanMB);
    }
    
    sigmaMB = TMath::Sqrt(sigmaMB/(k-1.));
    std::cout << std::endl;
    std::cout << "Bin " << i << " Nof MB eq. : "  << std::endl;
    std::cout << "Mean = " << meanMB << " +- " << errorMeanMB << " (" << errorMeanMB*100./meanMB << "%) " << " +- " << sigmaMB << " ("<< sigmaMB*100./meanMB << "%)" << std::endl;
    std::cout << "Syst max = " << systMB << "% " << std::endl;
    
    TF1* meanFMB = new TF1("mean","[0]",0,j);
    meanFMB->SetParameter(0,meanMB);
    
    TF1* meanFPSMB = new TF1("meanPS","[0]",0,j);
    meanFPSMB->SetParameter(0,meanMB+errorMeanMB);
    meanFPSMB->SetLineStyle(2);
    
    TF1* meanFMSMB = new TF1("meanMS","[0]",0,j);
    meanFMSMB->SetParameter(0,meanMB-errorMeanMB);
    meanFMSMB->SetLineStyle(2);
    
    hBinMB->GetListOfFunctions()->Add(meanFMB);
    hBinMB->GetListOfFunctions()->Add(meanFPSMB);
    hBinMB->GetListOfFunctions()->Add(meanFMSMB);
    
    cMB->cd(i-2);
    cMB->cd(i-2)->SetRightMargin(0.02325581);
    cMB->cd(i-2)->SetLeftMargin(0.1721507);
    cMB->cd(i-2)->SetBottomMargin(0.15);
    cMB->cd(i-2)->SetTopMargin(0.10);
    
    
    hBinMB->SetStats(0);
    if ( i == 9 || i == 10 || i == 11 || i == 12)
    {
      hBinMB->GetYaxis()->SetLabelSize(0.04);
      hBinMB->GetYaxis()->SetTitleSize(0.04);
      hBinMB->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBinMB->GetYaxis()->SetLabelSize(0.05);
      hBinMB->GetYaxis()->SetTitleSize(0.05);
      hBinMB->GetXaxis()->SetLabelSize(0.07);
    }
    hBinMB->Draw();
    
    //__
    
    TPaveText *ptbMB = new TPaveText(0.2516049,0.8514274,0.9375592,0.9952622,"blNDC");
    ptbMB->SetName("binName");
    ptbMB->SetBorderSize(0);
    ptbMB->SetFillColor(17);
    ptbMB->SetFillStyle(3001);
    ptbMB->SetTextFont(42);
    TText *textbMB = ptbMB->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxisI.Data(),min,max));
    
    ptbMB->Draw("same");
    
    TPaveText* pt1bMB = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    ptbMB->SetName("syst");
    pt1bMB->SetBorderSize(0);
    pt1bMB->SetFillColor(5);
    pt1bMB->SetFillStyle(3001);
    pt1bMB->SetTextFont(42);
    TText* text1bMB = pt1bMB->AddText(Form("#Delta_{max} = %1.2f %%",systMB));
    TText* text3bMB = pt1bMB->AddText(Form("#sigma = %1.2f %%",sigmaMB*100./meanMB));
    pt1bMB->Draw("same");
    
    
  }
  
  TFile* f = new TFile("FNormSystematics.root","recreate");
  cFn->Write("FNormSystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  aBinsFn->Write("FNormSystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
}

//---------------------------------------------------------------------------
void plotMeanNtrCorrSyst(const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                         const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                         const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                         const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                         const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                         const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                         const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                         const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                         const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                         const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                         const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                         const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                         const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString saxis("#LT N_{tr}^{corr} #GT");
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  Int_t nOfTests = names->GetEntries();
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  TIter next(arr);
  
  Double_t  NtrToNchCorr[12] = {2.10,1.99,1.96,1.94,1.92,1.90,1.88,1.86,1.83,1.81,1.77,1.73};
  
  TH1* hNtrCorr(0x0);
  
  TObjArray* bin = static_cast<AliAnalysisMuMuBinning*>(m->BIN())->CreateBinObjArray();
  Int_t nbins = bin->GetEntries();
  
  TIter nextBin(bin);
  AliAnalysisMuMuBinning::Range* range(0x0);
  AliAnalysisMuMu* r(0x0);
  
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  
  
  TCanvas* c = new TCanvas("MeanNtrCorrVariation","#LT N_{tr}^{corr} #GT variation",2,44,1597,980);
  
  if ( (nbins-5) == 2 )c->Divide(2,0,0,0);
  else if ( (nbins-5) <= 4 ) c->Divide(2,2,0,0);
  else if ( (nbins-5) <= 6 ) c->Divide(3,2,0,0);
  else if ( (nbins-5) <= 8 ) c->Divide(4,2,0,0);
  else if ( (nbins-5) <= 9 ) c->Divide(3,3,0,0);
  else if ( (nbins-5) <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" << std::endl;
    return;
  }
  
  Int_t k = 0; // Multiplity bin index
  while ( (range = static_cast<AliAnalysisMuMuBinning::Range*>(nextBin())) )
  {
    next.Reset();
    Double_t xmin= range->Xmin();
    Double_t xmax= range->Xmax();
    
    if ( xmax < 1. || xmax > 140.5 ) continue; // To avoid the bin INTEGRATED, [-1.5,-0.5), [-0.5,0.5) and [140.5,200.5)
    Double_t mean(0.),errorMean(0.),sigma(0.);
    std::vector<Double_t> diffs;
    
    TH1* hBin = new TH1D(Form("MeanNtrCorrSystBin%d",k+1),"",nOfTests,0,nOfTests);
    TString sBinName("");
    
    Int_t p(0); // Test bin index
    while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
    {
      hNtrCorr = r->OC()->Histo("/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/CINT7-B-NOPF-ALLNOTRD/V0A/Nch");
      
      if ( !hNtrCorr )
      {
        std::cout << "Error:: No corrected tracklets histo found" << std::endl;
        return;
      }
      
      Int_t binMin = hNtrCorr->FindBin(xmin); //Get the corresponding bins in the NtrCorr CINT distribution
      Int_t binMax = hNtrCorr->FindBin(xmax - 1.E-6);
      
      Double_t meanX1(0.),meanX1Error2(0.);
      
      for ( Int_t j = binMin ; j <= binMax ; j++ ) //Compute mean value of NtrCorr in the bin
      {
        meanX1 += hNtrCorr->GetBinContent(j)*hNtrCorr->GetBinCenter(j);
        
        meanX1Error2 += TMath::Power(TMath::Sqrt(hNtrCorr->GetBinContent(j))*hNtrCorr->GetBinCenter(j),2.);
      }
      
      Double_t Integral1Bin = hNtrCorr->Integral(binMin,binMax);
      
      Double_t meanX1Error = TMath::Sqrt(TMath::Power(TMath::Sqrt(meanX1Error2)/Integral1Bin,2.) +
                                         TMath::Power(TMath::Sqrt(Integral1Bin)*meanX1/TMath::Power(Integral1Bin,2.),2.));
      meanX1 /= Integral1Bin;
      
      meanX1 = meanX1*NtrToNchCorr[k]/2.; //Apply the correction to Nch
      
      
      mean += meanX1;
      errorMean += meanX1Error*meanX1Error;
      diffs.push_back(meanX1);
      
      if ( p == 0)
      {
        sBinName += range->AsString().Data(); //Get once the name of the bin to use it later to extract the bin range
        hBin->GetYaxis()->SetTitle(Form("%s",saxis.Data()));
        hBin->GetYaxis()->SetTitleOffset(1.6);
        
      }
      
      hBin->SetBinContent(p+1,meanX1);
      hBin->SetBinError(p+1,meanX1Error);
      hBin->GetXaxis()->SetBinLabel(p+1,static_cast<TObjString*>(names->At(p))->GetName());
      
      aBins->Add(hBin);
      p++;
    }
 
    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    errorMean = TMath::Sqrt(errorMean/p);
    mean = mean/p;
    for (Int_t j = 0 ; j < p ; j++)
    {
      sigma += (diffs[j]-mean)*(diffs[j]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(p-1.));
    std::cout << std::endl;
    std::cout << "Bin " << k+1 << " <NtrCorr> : "  << std::endl;
    std::cout << "Mean = " << mean << " +- " << errorMean << " (" << errorMean*100./mean << "%) " << " +- " << sigma << " ("<< sigma*100./mean << "%)" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    TF1* meanF = new TF1("mean","[0]",0,nOfTests);
    meanF->SetParameter(0,mean);
    
    TF1* meanFPS = new TF1("meanPS","[0]",0,nOfTests);
    meanFPS->SetParameter(0,mean+errorMean);
    meanFPS->SetLineStyle(2);
    
    TF1* meanFMS = new TF1("meanMS","[0]",0,nOfTests);
    meanFMS->SetParameter(0,mean-errorMean);
    meanFMS->SetLineStyle(2);
    
    hBin->GetListOfFunctions()->Add(meanF);
    hBin->GetListOfFunctions()->Add(meanFPS);
    hBin->GetListOfFunctions()->Add(meanFMS);
    
    c->cd(k+1);
    c->cd(k+1)->SetRightMargin(0.02325581);
    c->cd(k+1)->SetLeftMargin(0.1721507);
    c->cd(k+1)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( k+1 == 9 || k+1 == 10 || k+1 == 11 || k+1 == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    TString sBinNameMax(sBinName.Data());
    Int_t min(0),max(0);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin += sBinNameMax.Data();
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinNameMax.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    pt1b->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
    
    k++;
  }
  
 
  
  TFile* f = new TFile("MeanNtrCorrSystematics.root","recreate");
  c->Write("MeanNtrCorrSystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write("MeanNtrCorrSystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
}

//---------------------------------------------------------------------------
void plotMeanSESyst(const char* variable="NofJPsi",
                    const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                    const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                    const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                    const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                    const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                    const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                    const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                    const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                    const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                    const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                    const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                    const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                    const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString svariable(variable);
  
  TString saxis("");
  TString sSys("");
  
  if ( svariable.Contains("NofJPsi") )
  {
    sSys += "yield_Systematics";
    saxis += "Average sig. extr. unc.";
  }
  else if ( svariable.Contains("MeanPtJPsi") )
  {
    sSys += "mpt_Systematics";
    saxis += "Average #LT #it{p_{T}} #GT extr. unc.";
  }
  else
  {
    std::cout << "Error: Unrecognized variable " << svariable.Data() << ", maybe you meant NofJPsi or MeanPtJPsi" << std::endl;
    return;
  }
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  Int_t nOfTests = names->GetEntries();
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  TIter next(arr);
  
  Double_t  NtrToNchCorr[12] = {2.10,1.99,1.96,1.94,1.92,1.90,1.88,1.86,1.83,1.81,1.77,1.73};
  
  TH1* hNtrCorr(0x0);
  
  TObjArray* bin = static_cast<AliAnalysisMuMuBinning*>(m->BIN())->CreateBinObjArray();
  Int_t nbins = bin->GetEntries();
  
  TIter nextBin(bin);
  AliAnalysisMuMuBinning::Range* range(0x0);
  AliAnalysisMuMu* r(0x0);
  
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  
  
  TCanvas* c = new TCanvas("MeanSESystematicsVariation","SE systematics variation",2,44,1597,980);
  
  if ( (nbins-5) == 2 )c->Divide(2,0,0,0);
  else if ( (nbins-5) <= 4 ) c->Divide(2,2,0,0);
  else if ( (nbins-5) <= 6 ) c->Divide(3,2,0,0);
  else if ( (nbins-5) <= 8 ) c->Divide(4,2,0,0);
  else if ( (nbins-5) <= 9 ) c->Divide(3,3,0,0);
  else if ( (nbins-5) <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" << std::endl;
    return;
  }
  
  Int_t k = 0; // Multiplity bin index
  while ( (range = static_cast<AliAnalysisMuMuBinning::Range*>(nextBin())) )
  {
    next.Reset();
    Double_t xmin= range->Xmin();
    Double_t xmax= range->Xmax();
    
    if ( xmax < 1. || xmax > 140.5 ) continue; // To avoid the bin INTEGRATED, [-1.5,-0.5), [-0.5,0.5) and [140.5,200.5)
    Double_t mean(0.),sigma(0.);
    std::vector<Double_t> diffs;
    
    TH1* hBin = new TH1D(Form("MeanNtrCorrSystBin%d",k+1),"",nOfTests,0,nOfTests);
    TString sBinName("");
    
    Int_t p1(0); // Test bin index
    while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
    {
      hNtrCorr = r->OC()->Histo(Form("/TESTSYST/PSALL/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/%s",sSys.Data()));
      
      if ( !hNtrCorr )
      {
        std::cout << "Error:: No SE systematics histo found" << std::endl;
        return;
      }
      
      Double_t meanX1 = hNtrCorr->GetBinContent(k+1); //Syst value
      
      mean += meanX1;
      diffs.push_back(meanX1);
      
      if ( p1 == 0)
      {
        sBinName += range->AsString().Data(); //Get once the name of the bin to use it later to extract the bin range
        hBin->GetYaxis()->SetTitle(Form("%s",saxis.Data()));
        hBin->GetYaxis()->SetTitleOffset(1.6);
        
      }
      
      hBin->SetBinContent(p1+1,meanX1);
      hBin->GetXaxis()->SetBinLabel(p1+1,static_cast<TObjString*>(names->At(p1))->GetName());
      
      aBins->Add(hBin);
      p1++;
    }
    
    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    mean = mean/p1;
    for (Int_t j = 0 ; j < p1 ; j++)
    {
      sigma += (diffs[j]-mean)*(diffs[j]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(p1-1.));
    std::cout << std::endl;
    std::cout << "Bin " << k+1 << " <NtrCorr> : "  << std::endl;
    std::cout << "Mean syst= " << mean << " +- " << sigma << " ("<< sigma*100./mean << "% (dispersion))" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    TF1* meanF = new TF1("mean","[0]",0,nOfTests);
    meanF->SetParameter(0,mean);
    
    
    hBin->GetListOfFunctions()->Add(meanF);
    
    c->cd(k+1);
    c->cd(k+1)->SetRightMargin(0.02325581);
    c->cd(k+1)->SetLeftMargin(0.1721507);
    c->cd(k+1)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( k+1 == 9 || k+1 == 10 || k+1 == 11 || k+1 == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    TString sBinNameMax(sBinName.Data());
    Int_t min(0),max(0);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin += sBinNameMax.Data();
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinNameMax.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    pt1b->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
    
    k++;
  }
  
  
  
  TFile* f = new TFile("MeanSESystematics.root","recreate");
  c->Write("MeanSESystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write("MeanSESystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
}

//---------------------------------------------------------------------------
void plotMeanSESystAbs(const char* variable="NofJPsi",
                       const char* input0="../Nominal/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening.txt.saf.root",
                       const char* input1="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys1.txt.saf.root",
                       const char* input2="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys2.txt.saf.root",
                       const char* input3="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys3.txt.saf.root",
                       const char* input4="../AccefSyst/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Sys4.txt.saf.root",
                       const char* input5="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test1.txt.saf.root",
                       const char* input6="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test2.txt.saf.root",
                       const char* input7="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test3.txt.saf.root",
                       const char* input8="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test4.txt.saf.root",
                       const char* input9="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test5.txt.saf.root",
                       const char* input10="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test6.txt.saf.root",
                       const char* input11="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test7.txt.saf.root",
                       const char* input12="../RunTests/filesLHC13defullAOD_CorrTracklets_CMUL_Minv_Pt_Broadening_Test8.txt.saf.root")
{
  
  /// Variable: "NofJPsi" or "JPsiMPt" (respecting capital letters)
  
  TString svariable(variable);
  
  TString saxis("");
  TString sSys("");
  TString specName("");

  if ( svariable.Contains("NofJPsi") )
  {
    sSys += "yield_Systematics";
    saxis += "Average sig. extr. unc.";
    specName+= "PSI-NTRCORR-AccEffCorr";
  }
  else if ( svariable.Contains("MeanPtJPsi") )
  {
    sSys += "mpt_Systematics";
    saxis += "Average #LT #it{p_{T}} #GT extr. unc.";
    specName+= "PSI-NTRCORR-AccEffCorr-MeanPtVsMinvUS";

  }
  else
  {
    std::cout << "Error: Unrecognized variable " << svariable.Data() << ", maybe you meant NofJPsi or MeanPtJPsi" << std::endl;
    return;
  }
  
  TObjArray* names = new TObjArray();
  names->Add(new TObjString("Nominal"));
  names->Add(new TObjString("hard #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("hard #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},soft #it{p}_{T}"));
  names->Add(new TObjString("soft #it{y},hard #it{p}_{T}"));
  names->Add(new TObjString("Test 1"));
  names->Add(new TObjString("Test 2"));
  names->Add(new TObjString("Test 3"));
  names->Add(new TObjString("Test 4"));
  names->Add(new TObjString("Test 5"));
  names->Add(new TObjString("Test 6"));
  names->Add(new TObjString("Test 7"));
  names->Add(new TObjString("Test 8"));
  
  Int_t nOfTests = names->GetEntries();
  
  TObjArray* arr = new TObjArray();
  
  AliAnalysisMuMu* m = new AliAnalysisMuMu(input0);
  arr->Add(m);
  AliAnalysisMuMu* n = new AliAnalysisMuMu(input1);
  arr->Add(n);
  AliAnalysisMuMu* o = new AliAnalysisMuMu(input2);
  arr->Add(o);
  AliAnalysisMuMu* p = new AliAnalysisMuMu(input3);
  arr->Add(p);
  AliAnalysisMuMu* q = new AliAnalysisMuMu(input4);
  arr->Add(q);
  AliAnalysisMuMu* rr = new AliAnalysisMuMu(input5);
  arr->Add(rr);
  AliAnalysisMuMu* ss = new AliAnalysisMuMu(input6);
  arr->Add(ss);
  AliAnalysisMuMu* t = new AliAnalysisMuMu(input7);
  arr->Add(t);
  AliAnalysisMuMu* u = new AliAnalysisMuMu(input8);
  arr->Add(u);
  AliAnalysisMuMu* u1 = new AliAnalysisMuMu(input9);
  arr->Add(u1);
  AliAnalysisMuMu* u2 = new AliAnalysisMuMu(input10);
  arr->Add(u2);
  AliAnalysisMuMu* u3 = new AliAnalysisMuMu(input11);
  arr->Add(u3);
  AliAnalysisMuMu* u4 = new AliAnalysisMuMu(input12);
  arr->Add(u4);
  
  TIter next(arr);
  
  Double_t  NtrToNchCorr[12] = {2.10,1.99,1.96,1.94,1.92,1.90,1.88,1.86,1.83,1.81,1.77,1.73};
  
  TH1* hNtrCorr(0x0);
  
  TObjArray* bin = static_cast<AliAnalysisMuMuBinning*>(m->BIN())->CreateBinObjArray();
  Int_t nbins = bin->GetEntries();
  
  TIter nextBin(bin);
  AliAnalysisMuMuBinning::Range* range(0x0);
  AliAnalysisMuMu* r(0x0);
  
  TObjArray* aBins = new TObjArray();
  aBins->SetOwner(kTRUE);
  
  
  TCanvas* c = new TCanvas("MeanSESystematicsVariation","SE systematics variation",2,44,1597,980);
  
  if ( (nbins-5) == 2 )c->Divide(2,0,0,0);
  else if ( (nbins-5) <= 4 ) c->Divide(2,2,0,0);
  else if ( (nbins-5) <= 6 ) c->Divide(3,2,0,0);
  else if ( (nbins-5) <= 8 ) c->Divide(4,2,0,0);
  else if ( (nbins-5) <= 9 ) c->Divide(3,3,0,0);
  else if ( (nbins-5) <= 12 ) c->Divide(4,3,0,0);
  else
  {
    std::cout << "Error: Implement what to do with nbins > 12" << std::endl;
    return;
  }
  
  Int_t k = 0; // Multiplity bin index
  AliAnalysisMuMuSpectra* s(0x0);
  AliAnalysisMuMuResult* result(0x0);
  
  while ( (range = static_cast<AliAnalysisMuMuBinning::Range*>(nextBin())) )
  {
    next.Reset();
    Double_t xmin= range->Xmin();
    Double_t xmax= range->Xmax();
    
    if ( xmax < 1. || xmax > 140.5 ) continue; // To avoid the bin INTEGRATED, [-1.5,-0.5), [-0.5,0.5) and [140.5,200.5)
    Double_t mean(0.),sigma(0.);
    std::vector<Double_t> diffs;
    
    TH1* hBin = new TH1D(Form("MeanNtrCorrSystBin%d",k+1),"",nOfTests,0,nOfTests);
    TString sBinName("");
    
    Int_t p1(0); // Test bin index
    while ( (r = static_cast<AliAnalysisMuMu*>(next())) )
    {
      s = static_cast<AliAnalysisMuMuSpectra*>(r->OC()->GetObject(Form("/PSALLHASSPDSPDZQA_RES0.25_ZDIF0.50SPDABSZLT10.00/CMUL7-B-NOPF-MUON/V0A/pRABSETAPDCAMATCHLOWPAIRYPAIRPTIN0.0-15.0/%s",specName.Data())));
      
      if ( !s )
      {
        std::cout << "Error:: No spectra found" << std::endl;
        return;
      }
      
      result = static_cast<AliAnalysisMuMuResult*>(s->GetResultForBin(*range));
      
      if ( !result )
      {
        std::cout << "Error:: No subresult found" << std::endl;
        return;
      }
      
      Double_t meanX1 = result->GetRMS(svariable.Data())*100./result->GetValue(svariable.Data()); //Syst value
      
      mean += meanX1;
      diffs.push_back(meanX1);
      
      if ( p1 == 0)
      {
        sBinName += range->AsString().Data(); //Get once the name of the bin to use it later to extract the bin range
        hBin->GetYaxis()->SetTitle(Form("%s",saxis.Data()));
        hBin->GetYaxis()->SetTitleOffset(1.6);
        
      }
      
      hBin->SetBinContent(p1+1,meanX1);
      hBin->GetXaxis()->SetBinLabel(p1+1,static_cast<TObjString*>(names->At(p1))->GetName());
      
      aBins->Add(hBin);
      p1++;
    }
    
    Double_t minimum = hBin->GetMinimum();
    Double_t maximum = hBin->GetMaximum();
    
    Double_t syst = (maximum-minimum)*100./minimum;
    
    mean = mean/p1;
    for (Int_t j = 0 ; j < p1 ; j++)
    {
      sigma += (diffs[j]-mean)*(diffs[j]-mean);
    }
    
    sigma = TMath::Sqrt(sigma/(p1-1.));
    std::cout << std::endl;
    std::cout << "Bin " << k+1 << " <NtrCorr> : "  << std::endl;
    std::cout << "Mean syst= " << mean << " +- " << sigma << " ("<< sigma*100./mean << "% (dispersion))" << std::endl;
    std::cout << "Syst max = " << syst << "% " << std::endl;
    
    TF1* meanF = new TF1("mean","[0]",0,nOfTests);
    meanF->SetParameter(0,mean);
    
    
    hBin->GetListOfFunctions()->Add(meanF);
    
    c->cd(k+1);
    c->cd(k+1)->SetRightMargin(0.02325581);
    c->cd(k+1)->SetLeftMargin(0.1721507);
    c->cd(k+1)->SetBottomMargin(0.15);
    
    
    hBin->SetStats(0);
    if ( k+1 == 9 || k+1 == 10 || k+1 == 11 || k+1 == 12)
    {
      hBin->GetYaxis()->SetLabelSize(0.04);
      hBin->GetYaxis()->SetTitleSize(0.04);
      hBin->GetXaxis()->SetLabelSize(0.06);
    }
    else
    {
      hBin->GetYaxis()->SetLabelSize(0.05);
      hBin->GetYaxis()->SetTitleSize(0.05);
      hBin->GetXaxis()->SetLabelSize(0.07);
    }
    hBin->Draw();
    
    //__ Extract the bin range
    TString sBinNameMin("");
    TString sBinNameMax(sBinName.Data());
    Int_t min(0),max(0);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin += sBinNameMax.Data();
    sBinNameMax.Remove(0,sBinNameMax.First("_")+1);
    sBinNameMin.Remove(sBinNameMin.First("_"),sBinNameMin.Sizeof());
    
    min = sBinNameMin.Atoi()+1;
    max = sBinNameMax.Atoi();
    //__
    
    TPaveText *ptb = new TPaveText(0.2277686,0.8508228,0.9375592,0.9950071,"blNDC");
    ptb->SetName("binName");
    ptb->SetBorderSize(0);
    ptb->SetFillColor(17);
    ptb->SetFillStyle(3001);
    ptb->SetTextFont(42);
    TText *textb = ptb->AddText(Form("%s variation. N_{tr}^{corr} %d-%d",saxis.Data(),min,max));
    
    ptb->Draw("same");
    
    TPaveText* pt1b = new TPaveText(0.2077081,0.2016887,0.4965894,0.4372594,"blNDC");//small:0.2277686,0.1983397,0.431701,0.3616117
    pt1b->SetName("syst");
    pt1b->SetBorderSize(0);
    pt1b->SetFillColor(5);
    pt1b->SetFillStyle(3001);
    pt1b->SetTextFont(42);
    TText* text1b = pt1b->AddText(Form("#Delta_{max} = %1.2f %%",syst));
    TText* text3b = pt1b->AddText(Form("#sigma = %1.2f %%",sigma*100./mean));
    pt1b->Draw("same");
    
    k++;
  }
  
  
  
  TFile* f = new TFile("MeanAbsSESystematics.root","recreate");
  c->Write("MeanAbsSESystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  aBins->Write("MeanAbsSESystematicsVariation", TObject::kOverwrite | TObject::kSingleKey);
  f->Close();
  
}

