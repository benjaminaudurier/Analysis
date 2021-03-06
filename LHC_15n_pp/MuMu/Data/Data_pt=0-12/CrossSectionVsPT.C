{
//=========Macro generated from canvas: c/c
//=========  (Sat Jun 25 20:14:16 2016) by ROOT version5.34/30
   TCanvas *c = new TCanvas("c", "c",4,100,1024,768);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   c->Range(-2.314024,-3.510874,13.1128,0.4189239);
   c->SetFillColor(0);
   c->SetBorderMode(0);
   c->SetBorderSize(2);
   c->SetLogy();
   c->SetTickx(1);
   c->SetTicky(1);
   c->SetLeftMargin(0.15);
   c->SetRightMargin(0.03);
   c->SetTopMargin(0.03);
   c->SetBottomMargin(0.13);
   c->SetFrameBorderMode(0);
   c->SetFrameBorderMode(0);
   
   TGraphErrors *gre = new TGraphErrors(12);
   gre->SetName("Graph0");
   gre->SetTitle("J/#psi cross section");
   gre->SetLineColor(2);
   gre->SetLineWidth(2);
   gre->SetFillColor(0);
   gre->SetMarkerColor(2);
   gre->SetMarkerSize(1.7);
   gre->SetPoint(0,0,0);
   gre->SetPointError(0,0,0);
   gre->SetPoint(1,0.5,0.6508897);
   gre->SetPointError(1,0.25,0.03306167);
   gre->SetPoint(2,1.5,1.173068);
   gre->SetPointError(2,0.25,0.05172532);
   gre->SetPoint(3,2.5,0.9032657);
   gre->SetPointError(3,0.25,0.03605123);
   gre->SetPoint(4,3.5,0.5192195);
   gre->SetPointError(4,0.25,0.02125235);
   gre->SetPoint(5,4.5,0.2303686);
   gre->SetPointError(5,0.25,0.0134456);
   gre->SetPoint(6,5.5,0.1267971);
   gre->SetPointError(6,0.25,0.00488173);
   gre->SetPoint(7,6.5,0.06212417);
   gre->SetPointError(7,0.25,0.00216195);
   gre->SetPoint(8,7.5,0.02912176);
   gre->SetPointError(8,0.25,0.0009371625);
   gre->SetPoint(9,8.5,0.01435177);
   gre->SetPointError(9,0.25,0.0007432792);
   gre->SetPoint(10,9.5,0.01115816);
   gre->SetPointError(10,0.25,0.001067213);
   gre->SetPoint(11,11,0.003435803);
   gre->SetPointError(11,0.5,9.350136e-05);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","J/#psi cross section",100,0,12.65);
   Graph_Graph1->SetMinimum(0.001);
   Graph_Graph1->SetMaximum(2);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);
   Graph_Graph1->SetMarkerSize(1.3);
   Graph_Graph1->GetXaxis()->SetTitle("#it{p}_{T}(GeV/#it{c})");
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph1->GetXaxis()->SetTitleOffset(1.1);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetTitle("d^{2}#sigma/d#it{y}d#it{p}_{T} (#mub(GeV/#it{c})^{-1})");
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.06);
   Graph_Graph1->GetYaxis()->SetTickLength(0.02);
   Graph_Graph1->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelOffset(0.01);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1);
   
   gre->Draw("A5.[]");
   
   gre = new TGraphErrors(12);
   gre->SetName("Graph1");
   gre->SetTitle("Graph");
   gre->SetFillColor(1);
   gre->SetLineColor(2);
   gre->SetLineWidth(2);
   gre->SetMarkerColor(2);
   gre->SetMarkerStyle(8);
   gre->SetMarkerSize(1.5);
   gre->SetPoint(0,0,0);
   gre->SetPointError(0,0,0);
   gre->SetPoint(1,0.5,0.6508897);
   gre->SetPointError(1,0.5,0.02380602);
   gre->SetPoint(2,1.5,1.173068);
   gre->SetPointError(2,0.5,0.03109608);
   gre->SetPoint(3,2.5,0.9032657);
   gre->SetPointError(3,0.5,0.02731727);
   gre->SetPoint(4,3.5,0.5192195);
   gre->SetPointError(4,0.5,0.01934671);
   gre->SetPoint(5,4.5,0.2303686);
   gre->SetPointError(5,0.5,0.01257857);
   gre->SetPoint(6,5.5,0.1267971);
   gre->SetPointError(6,0.5,0.007559595);
   gre->SetPoint(7,6.5,0.06212417);
   gre->SetPointError(7,0.5,0.005097989);
   gre->SetPoint(8,7.5,0.02912176);
   gre->SetPointError(8,0.5,0.003306133);
   gre->SetPoint(9,8.5,0.01435177);
   gre->SetPointError(9,0.5,0.002357078);
   gre->SetPoint(10,9.5,0.01115816);
   gre->SetPointError(10,0.5,0.002048506);
   gre->SetPoint(11,11,0.003435803);
   gre->SetPointError(11,1,0.0006906592);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0,13.2);
   Graph_Graph2->SetMinimum(0.001324581);
   Graph_Graph2->SetMaximum(1.324581);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);
   Graph_Graph2->SetMarkerSize(1.3);
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelOffset(0.01);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph2->GetXaxis()->SetTitleOffset(1.1);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelOffset(0.01);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.05);
   Graph_Graph2->GetYaxis()->SetTickLength(0.02);
   Graph_Graph2->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelOffset(0.01);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph2);
   
   gre->Draw("Psame");
   
   TLegend *leg = new TLegend(0.2,0.2,0.5,0.4,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextSize(0.04);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(2);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("NULL","ALICE, pp #sqrt{#it{s}} = 5.02 TeV","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry->SetTextSize(0.055);
   entry=leg->AddEntry("Graph","Inclusive J/#psi , 2.5 < #it{y} < 4 ","pe");
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(2);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1.5);
   entry->SetTextFont(42);
   entry=leg->AddEntry("NULL","L_{int} = 106.3 #pm 2.1% #mub^{-1}","");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   TH1F *Graph = new TH1F("Graph","J/#psi cross section",100,0,12.65);
   Graph->SetMinimum(0.001);
   Graph->SetMaximum(2);
   Graph->SetDirectory(0);
   Graph->SetStats(0);
   Graph->SetMarkerSize(1.3);
   Graph->GetXaxis()->SetTitle("#it{p}_{T}(GeV/#it{c})");
   Graph->GetXaxis()->SetLabelFont(42);
   Graph->GetXaxis()->SetLabelOffset(0.01);
   Graph->GetXaxis()->SetLabelSize(0.05);
   Graph->GetXaxis()->SetTitleSize(0.05);
   Graph->GetXaxis()->SetTitleOffset(1.1);
   Graph->GetXaxis()->SetTitleFont(42);
   Graph->GetYaxis()->SetTitle("d^{2}#sigma/d#it{y}d#it{p}_{T} (#mub(GeV/#it{c})^{-1})");
   Graph->GetYaxis()->SetLabelFont(42);
   Graph->GetYaxis()->SetLabelOffset(0.01);
   Graph->GetYaxis()->SetLabelSize(0.05);
   Graph->GetYaxis()->SetTitleSize(0.06);
   Graph->GetYaxis()->SetTickLength(0.02);
   Graph->GetYaxis()->SetTitleOffset(1.1);
   Graph->GetYaxis()->SetTitleFont(42);
   Graph->GetZaxis()->SetLabelFont(42);
   Graph->GetZaxis()->SetLabelOffset(0.01);
   Graph->GetZaxis()->SetLabelSize(0.05);
   Graph->GetZaxis()->SetTitleSize(0.05);
   Graph->GetZaxis()->SetTitleFont(42);
   Graph->Draw("sameaxis");
   c->Modified();
   c->cd();
   c->SetSelected(c);
}
