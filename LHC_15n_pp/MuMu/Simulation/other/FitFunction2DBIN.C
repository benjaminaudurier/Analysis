{
	// Generated Distributions
	TF1* fit = new TF1("fit","[0]*x/TMath::Power([1] + TMath::Power(x,[2]),[3])",0,12);
	fit->SetParameters(5.42*4654.3/1.5,12.8133,1.9647,3.66641);
	fit->SetLineColor(1);


	// 2DBIN1 m04.00_m03.25 fit
	TF1* fit2 = new TF1("fit2","[0]*x/TMath::Power([1] + TMath::Power(x,[2]),[3])",0,12);
	fit2->SetParameters(17884.7,13.9011,2.01721,3.64962);
	fit2->SetLineColor(2);


	// 2DBIN2 m03.25_m02.50 fit
	TF1* fit3 = new TF1("fit3","[0]*x/TMath::Power([1] + TMath::Power(x,[2]),[3])",0,12);
	fit3->SetParameters(449667,13.5231,1.69545,4.78706);
	fit3->SetLineColor(4);


	// Compute inegrale
	Double_t integral  = fit->Integral(0., 12.);
	Double_t integral2 = fit2->Integral(0., 12.);
	Double_t integral3 = fit3->Integral(0., 12.);

	// Normalize
	fit->SetParameter(0, fit->GetParameter(0)/integral);
	fit2->SetParameter(0, fit2->GetParameter(0)/integral2);
	fit3->SetParameter(0, fit3->GetParameter(0)/integral3);

	//legend
	TLegend * leg = new TLegend(0.2,0.2,0.50,0.4);
    leg->AddEntry(fit,"Normalized generated function 2.5 < #it{y} < 4 ","l");
    leg->AddEntry(fit2,"Normalized Fit function  3.25 < #it{y} < 4 ","l");
    leg->AddEntry(fit3,"Normalized Fit functio 2.5 < #it{y} < 3.25 ","l");


	fit->Draw();
	fit2->Draw("same");
	fit3->Draw("same");
	leg->Draw("same");

}