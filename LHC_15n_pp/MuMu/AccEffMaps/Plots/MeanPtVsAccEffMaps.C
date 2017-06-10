{
//=========Macro generated from canvas: c1/c1
//=========  (Sat Nov  5 16:04:31 2016) by ROOT version5.34/30
   TCanvas *c1 = new TCanvas("c1", "c1",32,45,700,500);
   c1->Range(-2,2.334739,18,2.429903);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   TH1F *meanpttests__1 = new TH1F("meanpttests__1","Meant pt vs AccEff maps",16,0,16);
   meanpttests__1->SetBinContent(1,2.374606);
   meanpttests__1->SetBinContent(2,2.392193);
   meanpttests__1->SetBinContent(3,2.378038);
   meanpttests__1->SetBinContent(4,2.373433);
   meanpttests__1->SetBinContent(5,2.379731);
   meanpttests__1->SetBinContent(6,2.384928);
   meanpttests__1->SetBinContent(7,2.383007);
   meanpttests__1->SetBinContent(8,2.376805);
   meanpttests__1->SetBinContent(9,2.368117);
   meanpttests__1->SetBinContent(10,2.383816);
   meanpttests__1->SetBinContent(11,2.38267);
   meanpttests__1->SetBinContent(12,2.395971);
   meanpttests__1->SetBinContent(13,2.372041);
   meanpttests__1->SetBinContent(14,2.379883);
   meanpttests__1->SetBinContent(15,2.379677);
   meanpttests__1->SetBinContent(16,2.372557);
   meanpttests__1->SetBinError(1,0.02041647);
   meanpttests__1->SetBinError(2,0.02038402);
   meanpttests__1->SetBinError(3,0.02040538);
   meanpttests__1->SetBinError(4,0.02037188);
   meanpttests__1->SetBinError(5,0.02011998);
   meanpttests__1->SetBinError(6,0.02038442);
   meanpttests__1->SetBinError(7,0.02074076);
   meanpttests__1->SetBinError(8,0.02014105);
   meanpttests__1->SetBinError(9,0.02040924);
   meanpttests__1->SetBinError(10,0.02053074);
   meanpttests__1->SetBinError(11,0.02041717);
   meanpttests__1->SetBinError(12,0.02079035);
   meanpttests__1->SetBinError(13,0.02045342);
   meanpttests__1->SetBinError(14,0.02050348);
   meanpttests__1->SetBinError(15,0.02031585);
   meanpttests__1->SetBinError(16,0.02037574);
   meanpttests__1->SetEntries(16);
   meanpttests__1->SetDirectory(0);
   
   TPaveStats *ptstats = new TPaveStats(0.78,0.775,0.98,0.935,"brNDC");
   ptstats->SetName("stats");
   ptstats->SetBorderSize(1);
   ptstats->SetFillColor(0);
   ptstats->SetTextAlign(12);
   ptstats->SetTextFont(42);
   TText *text = ptstats->AddText("meanpttests");
   text->SetTextSize(0.0368);
   text = ptstats->AddText("Entries = 16     ");
   text = ptstats->AddText("Mean  =  7.999");
   text = ptstats->AddText("RMS   =  4.609");
   ptstats->SetOptStat(1111);
   ptstats->SetOptFit(0);
   ptstats->Draw();
   meanpttests__1->GetListOfFunctions()->Add(ptstats);
   ptstats->SetParent(meanpttests__1);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   meanpttests__1->SetLineColor(ci);
   meanpttests__1->GetXaxis()->SetBinLabel(1,"AccEff_map_0");
   meanpttests__1->GetXaxis()->SetBinLabel(2,"AccEff_map_1");
   meanpttests__1->GetXaxis()->SetBinLabel(3,"AccEff_map_2");
   meanpttests__1->GetXaxis()->SetBinLabel(4,"AccEff_map_3");
   meanpttests__1->GetXaxis()->SetBinLabel(5,"AccEff_map_4");
   meanpttests__1->GetXaxis()->SetBinLabel(6,"AccEff_map_5");
   meanpttests__1->GetXaxis()->SetBinLabel(7,"AccEff_map_6");
   meanpttests__1->GetXaxis()->SetBinLabel(8,"AccEff_map_7");
   meanpttests__1->GetXaxis()->SetBinLabel(9,"AccEff_map_8");
   meanpttests__1->GetXaxis()->SetBinLabel(10,"AccEff_map_9");
   meanpttests__1->GetXaxis()->SetBinLabel(11,"AccEff_map_10");
   meanpttests__1->GetXaxis()->SetBinLabel(12,"AccEff_map_11");
   meanpttests__1->GetXaxis()->SetBinLabel(13,"AccEff_map_12");
   meanpttests__1->GetXaxis()->SetBinLabel(14,"AccEff_map_13");
   meanpttests__1->GetXaxis()->SetBinLabel(15,"AccEff_map_14");
   meanpttests__1->GetXaxis()->SetBinLabel(16,"AccEff_map_15");
   meanpttests__1->GetXaxis()->SetLabelFont(42);
   meanpttests__1->GetXaxis()->SetLabelSize(0.035);
   meanpttests__1->GetXaxis()->SetTitleSize(0.035);
   meanpttests__1->GetXaxis()->SetTitleFont(42);
   meanpttests__1->GetYaxis()->SetLabelFont(42);
   meanpttests__1->GetYaxis()->SetLabelSize(0.035);
   meanpttests__1->GetYaxis()->SetTitleSize(0.035);
   meanpttests__1->GetYaxis()->SetTitleFont(42);
   meanpttests__1->GetZaxis()->SetLabelFont(42);
   meanpttests__1->GetZaxis()->SetLabelSize(0.035);
   meanpttests__1->GetZaxis()->SetTitleSize(0.035);
   meanpttests__1->GetZaxis()->SetTitleFont(42);
   meanpttests__1->Draw("");
   TLine *line = new TLine(0,2.379842,16,2.379842);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineWidth(3);
   line->Draw();
   line = new TLine(0,2.37275,16,2.37275);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(3);
   line->SetLineWidth(3);
   line->Draw();
   line = new TLine(0,2.386934,16,2.386934);

   ci = TColor::GetColor("#0000ff");
   line->SetLineColor(ci);
   line->SetLineStyle(3);
   line->SetLineWidth(3);
   line->Draw();
   
   TPaveText *pt = new TPaveText(0.2846299,0.9360256,0.7153701,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   text = pt->AddText("Meant pt vs AccEff maps");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
