//
//  calcPromptRAA.c
//
//
//  Created by Javier Castillo on 23/5/16.
//
//

#include <stdio.h>

Double_t MyPromptRAA(Double_t *x,Double_t *par)
{

	Double_t t = (x[0]-par[0]*x[1])/(1-par[0]);

	return t;
}

void calcPromptRAA() {

	Bool_t bPrintAN = kTRUE;
//	Bool_t bPrintAN = kFALSE; // Print for twiki instead

	Double_t incRAA276 		= 0.58;
	Double_t incRAA502 		= 0.69;
	Double_t Fb276 				= 0.0713;
	Double_t Fb5 					= 0.0889;
	Double_t Fb7 					= 0.1008;
	Double_t minNPRAA276 	= 0.;
	Double_t intNPRAA276 	= 0.5;
	Double_t maxNPRAA276 	= 1.;
	Double_t maxxNPRAA276 = 1.2;
	Double_t minNPRAA 		= 0.;
	Double_t intNPRAA 		= 0.5;
	Double_t maxNPRAA 		= 1.;
	Double_t maxxNPRAA 		= 1.2;

	const Int_t ncent = 9;
	const Int_t npt276 = 7;
	const Int_t npt = 11;
	const Int_t nptlhcb = 14;


	Double_t Fb7l[nptlhcb] = {7.4/100., 8.2/100., 9.7/100.,11.0/100.,11.9/100.,13.6/100.,15.4/100.,17.8/100.,18.6/100.,21.5/100.,20.2/100.,24.7/100.,24.1/100.,32.5/100.};
	Double_t Fb7pt[npt] = {7.4/100., 8.2/100., 9.7/100.,11.0/100.,11.9/100.,13.6/100.,15.4/100.,17.8/100.,18.6/100.,21.5/100.,22.5/100.};

	Double_t pt276pt[npt276] ={0.5,1.5,2.5,3.5,4.5,5.5,7.};
	Double_t pt276lim[npt276+1] ={0.,1.,2.,3.,4.,5.,6.,8.};

	Double_t pt5pt[npt] ={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11.};
	Double_t pt5lim[npt+1] ={0.,1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,12.};

	TF1 *myextrapFunc = new TF1("myextrapFunc","[0]+[1]*x+[2]*x*x",0.,13.);
	myextrapFunc->SetParNames("p0","p1x","p2xx");
//	myextrapFunc->SetParameters(6.28,0.87,0.12);
	myextrapFunc->SetParameters(6.80,0.87,0.12);

//	TF1* extrapFunc = new TF1("extrapFunc","[0]+[1]*x + [2]*x*x",0.,13.,3);
//	extrapFunc->SetParNames("p0", "p1x", "p2xx");
//	extrapFunc->SetParameter(0,6.28,0.87,0.12);

	Double_t Fb5pt[npt];
	for (Int_t ipt = 0; ipt<npt; ipt++) {
//		Fb5pt[ipt] = Fb7pt[ipt]*Fb5/Fb7;
		Double_t fB = myextrapFunc->Eval(pt5pt[ipt])/100.;
		Fb5pt[ipt] = fB/(1.+fB);
	}
	Double_t Fb5pt7[npt276];
	for (Int_t ipt = 0; ipt<npt276; ipt++) {
		//		Fb5pt[ipt] = Fb7pt[ipt]*Fb5/Fb7;
		Double_t fB = myextrapFunc->Eval(pt276pt[ipt])/100.;
		Fb5pt7[ipt] = fB/(1.+fB);
	}

	//
	myextrapFunc->SetParameters(5.28,0.87,0.12);
//	myextrapFunc->SetParameters(4.94,0.87,0.12);
	Double_t Fb276pt[npt276];
	for (Int_t ipt = 0; ipt<npt276; ipt++) {
		Double_t fB = myextrapFunc->Eval(pt276pt[ipt])/100.;
		Fb276pt[ipt] = fB/(1.+fB);
	}
	// 5TeV Raa
	Double_t CentLim[ncent+1]={0.0,10.0,20.0,30.0,40.,50.,60.,70.,80.,90.};
	Double_t RAA5_data[ncent]={0.656,0.664,0.689,0.647,0.725,0.772,0.902,1.093,1.258};
	Double_t ErrStat_RAA5_data[ncent]={0.011,0.011,0.013,0.013,0.016,0.019,0.023,0.036,0.051};
	Double_t ErrSyst_RAA5_data[ncent]={0.027,0.026,0.025,0.027,0.031,0.041,0.055,0.081,0.122};

	// 5TeV Raa pt>0.3 with pp@5TeV
	Double_t RAA5_data_pt3_pp[ncent]={0.652,0.660,0.684,0.640,0.708,0.737,0.834,0.945,0.921,};
	Double_t ErrStat_RAA5_data_pt3_pp[ncent]={0.011,0.012,0.013,0.013,0.016,0.018,0.023,0.034,0.045};
	Double_t ErrSyst_RAA5_data_pt3_pp[ncent]={0.027,0.025,0.024,0.026,0.030,0.039,0.051,0.070,0.089};

	// 5TeV Raa Pt coordinates
	Double_t RAA5_data_PT[npt]={0.771,0.748,0.661,0.523,0.478,0.386,0.332,0.304,0.359,0.237,0.352};
	Double_t ErrStat_RAA5_data_PT[npt]={0.027,0.017,0.016,0.013,0.016,0.015,0.014,0.022,0.027,0.026,0.029};
	Double_t ErrSyst_RAA5_data_PT[npt]={0.069,0.060,0.049,0.039,0.044,0.032,0.034,0.038,0.064,0.050,0.075};

//	====RAA vs pT in 0-20%
 Double_t pT_2015_020[npt276]={0.55,1.55,2.55,3.55,4.55,5.55,7.05};
 Double_t RAA_2015_020[npt276]={0.771,0.748,0.661,0.523,0.478,0.386,0.326};
 Double_t ErrStat_RAA_2015_020[npt276]={0.027,0.017,0.016,0.013,0.016,0.015,0.012};
 Double_t ErrSyst_RAA_2015_020[npt276]={0.069,0.060,0.049,0.039,0.044,0.032,0.033};

	// 2.76TeV Raa
	Double_t RAA276_data[ncent]={0.557,0.573,0.598,0.577,0.609,0.725,0.839,0.849,1.094};
	Double_t ErrStat_RAA276_data[ncent]={0.019,0.020,0.022,0.024,0.028,0.036,0.041,0.063,0.106};
	Double_t ErrSyst_RAA276_data[ncent]={0.024,0.022,0.020,0.025,0.030,0.043,0.058,0.068,0.104};

	// 2.76TeV Raa pt>3
	Double_t RAA276_data_pt3[ncent]={0.545,0.560,0.594,0.570,0.592,0.715,0.805,0.778,0.887};
	Double_t ErrStat_RAA276_data_pt3[ncent]={0.017,0.018,0.020,0.021,0.025,0.033,0.047,0.062,0.064};
	Double_t ErrSyst_RAA276_data_pt3[ncent]={0.016,0.017,0.018,0.023,0.024,0.030,0.044,0.064,0.078};

	// 2.76TeV Raa Pt coordinates
	Double_t RAA276_data_PT[npt276]={0.803,0.690,0.505,0.381,0.355,0.282,0.279};
	Double_t ErrStat_RAA276_data_PT[npt276]={0.084,0.052,0.042,0.037,0.052,0.048,0.064};
	Double_t ErrSyst_RAA276_data_PT[npt276]={0.113,0.084,0.062,0.046,0.041,0.032,0.032};


	// --- Compute inclusive value ---


	TF2* fMyPromptRAA = new TF2("MyPromptRAA",MyPromptRAA,0.,2.,0.,2.,1);
	fMyPromptRAA->SetParameter(0,Fb5);
	TF2* fMyPromptRAA276 = new TF2("MyPromptRAA276",MyPromptRAA,0.,2.,0.,2.,1);
	fMyPromptRAA276->SetParameter(0,Fb276);
	fMyPromptRAA->Draw("colz");
	Double_t maxPRAA = fMyPromptRAA->Eval(incRAA,minNPRAA);
	Double_t minPRAA = fMyPromptRAA->Eval(incRAA,maxNPRAA);
	Double_t intPRAA = fMyPromptRAA->Eval(incRAA,intNPRAA);
	Double_t minnPRAA = fMyPromptRAA->Eval(incRAA,maxxNPRAA);

	printf("Inclusive J/psi R_{AA} 5 TeV = %f\n",incRAA);
	printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb5,minPRAA,(minPRAA-incRAA)/incRAA,maxPRAA,(maxPRAA-incRAA)/incRAA);

	if (bPrintAN) {
		printf(" Centrality & $R_{\\rm AA}^{inc}$ & $F_{\\rm b}$ & $R_{\\rm AA}^{npro} = 1.2$ & $R_{\\rm AA}^{npro} = 1.0$ & $R_{\\rm AA}^{npro} = 0.5$ & $R_{\\rm AA}^{npro} = 0.0$ \\\\ \n");
		printf(" %.0f-%.0f\\%% & %.2f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",CentLim[0],CentLim[ncent],incRAA,Fb5,100.*(minnPRAA-incRAA)/incRAA,100.*(minPRAA-incRAA)/incRAA,100.*(intPRAA-incRAA)/incRAA,100.*(maxPRAA-incRAA)/incRAA);
	} else {
	printf("| Cent | R_AA^incl | F_{b} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
	printf("| %.0f-%.0f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[0],CentLim[ncent],incRAA,Fb5,100.*(minnPRAA-incRAA)/incRAA,100.*(minPRAA-incRAA)/incRAA,100.*(intPRAA-incRAA)/incRAA,100.*(maxPRAA-incRAA)/incRAA);
	}
	for (Int_t icent = 0 ; icent < ncent; icent++) {
		Double_t cincRAA = RAA5_data[icent]*0.975;
		maxPRAA = fMyPromptRAA->Eval(cincRAA,minNPRAA);
		minPRAA = fMyPromptRAA->Eval(cincRAA,maxNPRAA);
		intPRAA = fMyPromptRAA->Eval(cincRAA,intNPRAA);
		minnPRAA = fMyPromptRAA->Eval(cincRAA,maxxNPRAA);
//		printf("Inclusive J/psi R_{AA} = %f\n",RAA5_data[icent]);
//		printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb5,minPRAA,(minPRAA-RAA5_data[icent])/RAA5_data[icent],maxPRAA,(maxPRAA-RAA5_data[icent])/RAA5_data[icent]);
		if (bPrintAN) {
			printf(" %.0f-%.0f\\%% & %.2f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",CentLim[icent],CentLim[icent+1],cincRAA,Fb5,100.*(minnPRAA-cincRAA)/cincRAA,100.*(minPRAA-cincRAA)/cincRAA,100.*(intPRAA-cincRAA)/cincRAA,100.*(maxPRAA-cincRAA)/cincRAA);
		} else {
			printf("| %.0f-%.0f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[icent],CentLim[icent+1],cincRAA,Fb5,100.*(minnPRAA-cincRAA)/cincRAA,100.*(minPRAA-cincRAA)/cincRAA,100.*(intPRAA-cincRAA)/cincRAA,100.*(maxPRAA-cincRAA)/cincRAA);
		}
	}

	if (bPrintAN) {
		printf("\n $\\p_{\\rm T}$ (GeV/$c$)& $R_{\\rm AA}^{inc}$ & $F_{\\rm b}$ & $R_{\\rm AA}^{npro} = 1.2$ & $R_{\\rm AA}^{npro} = 1.0$ & $R_{\\rm AA}^{npro} = 0.5$ & $R_{\\rm AA}^{npro} = 0.0$ \\\\ \n");
	} else {
		printf("\n| pT | R_AA^incl | F_{b} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");

	}

	// ----

	for (Int_t ipt = 0 ; ipt < npt; ipt++) {
		fMyPromptRAA->SetParameter(0,Fb5pt[ipt]);
		Double_t cincRAA = RAA5_data_PT[ipt]*0.975;
		maxPRAA = fMyPromptRAA->Eval(cincRAA,minNPRAA);
		minPRAA = fMyPromptRAA->Eval(cincRAA,maxNPRAA);
		intPRAA = fMyPromptRAA->Eval(cincRAA,intNPRAA);
		minnPRAA = fMyPromptRAA->Eval(cincRAA,maxxNPRAA);
//		printf("Inclusive J/psi R_{AA} = %f\n",RAA5_data_PT[ipt]);
//		printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb5pt[ipt],minPRAA,(minPRAA-RAA5_data_PT[ipt])/RAA5_data_PT[ipt],maxPRAA,(maxPRAA-RAA5_data_PT[ipt])/RAA5_data_PT[ipt]);
		if (bPrintAN) {
			printf(" %.1f-%.1f & %.2f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",pt5lim[ipt],pt5lim[ipt+1],cincRAA,Fb5pt[ipt],100.*(minnPRAA-cincRAA)/cincRAA,100.*(minPRAA-cincRAA)/cincRAA,100.*(intPRAA-cincRAA)/cincRAA,100.*(maxPRAA-cincRAA)/cincRAA);

		} else {
			printf("| %.1f-%.1f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f\% | \*%.2f%%\* |\n",pt5lim[ipt],pt5lim[ipt+1],cincRAA,Fb5pt[ipt],100.*(minnPRAA-cincRAA)/cincRAA,100.*(minPRAA-cincRAA)/cincRAA,100.*(intPRAA-cincRAA)/cincRAA,100.*(maxPRAA-cincRAA)/cincRAA);
		}
	}

	printf("\n| pT | R_AA^incl | F_{b} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
	for (Int_t ipt = 0 ; ipt < npt276; ipt++) {
		fMyPromptRAA->SetParameter(0,Fb5pt7[ipt]);
		Double_t cincRAA = RAA_2015_020[ipt]*0.975;
		maxPRAA = fMyPromptRAA->Eval(cincRAA,minNPRAA);
		minPRAA = fMyPromptRAA->Eval(cincRAA,maxNPRAA);
		intPRAA = fMyPromptRAA->Eval(cincRAA,intNPRAA);
		minnPRAA = fMyPromptRAA->Eval(cincRAA,maxxNPRAA);
		//		printf("Inclusive J/psi R_{AA} = %f\n",RAA5_data_PT[ipt]);
		//		printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb5pt[ipt],minPRAA,(minPRAA-RAA5_data_PT[ipt])/RAA5_data_PT[ipt],maxPRAA,(maxPRAA-RAA5_data_PT[ipt])/RAA5_data_PT[ipt]);
		printf("| %.1f-%.1f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f\% | \*%.2f%%\* |\n",pt5lim[ipt],pt5lim[ipt+1],cincRAA,Fb5pt[ipt],100.*(minnPRAA-cincRAA)/cincRAA,100.*(minPRAA-cincRAA)/cincRAA,100.*(intPRAA-cincRAA)/cincRAA,100.*(maxPRAA-cincRAA)/cincRAA);
	}

	Double_t maxPRAA276 = fMyPromptRAA276->Eval(incRAA276,minNPRAA276);
	Double_t minPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxNPRAA276);
	Double_t intPRAA276 = fMyPromptRAA276->Eval(incRAA276,intNPRAA276);
	Double_t minnPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxxNPRAA276);
	printf("Inclusive J/psi R_{AA} 2.76 TeV = %f\n",incRAA276);
	printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb276,minPRAA276,(minPRAA276-incRAA276)/incRAA276,maxPRAA276,(maxPRAA276-incRAA276)/incRAA276);
	printf("| Cent | R_AA^incl | F_{b} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
	printf("| %.0f-%.0f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[0],CentLim[ncent],incRAA276,Fb276,100.*(minnPRAA276-incRAA276)/incRAA276,100.*(minPRAA276-incRAA276)/incRAA276,100.*(intPRAA276-incRAA276)/incRAA276,100.*(maxPRAA276-incRAA276)/incRAA276);

	for (Int_t icent = 0 ; icent < ncent; icent++) {
		Double_t  cincRAA276 = RAA276_data[icent];
		maxPRAA276 = fMyPromptRAA276->Eval(cincRAA276,minNPRAA276);
		minPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxNPRAA276);
		intPRAA276 = fMyPromptRAA276->Eval(cincRAA276,intNPRAA276);
		minnPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxxNPRAA276);
//		printf("Inclusive J/psi R_{AA} = %f\n",cincRAA276);
//		printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb276,minPRAA276,(minPRAA276-RAA276_data[icent])/RAA276_data[icent],maxPRAA276,(maxPRAA276-RAA276_data[icent])/RAA276_data[icent]);
		printf("| %.0f-%.0f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[icent],CentLim[icent+1],cincRAA276,Fb276,100.*(minnPRAA276-cincRAA276)/cincRAA276,100.*(minPRAA276-cincRAA276)/cincRAA276,100.*(intPRAA276-cincRAA276)/cincRAA276,100.*(maxPRAA276-cincRAA276)/cincRAA276);
	}

	printf("\n| pT | R_AA^incl | F_{b}^{2.76TeV} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
	for (Int_t ipt = 0 ; ipt < npt276; ipt++) {
		fMyPromptRAA276->SetParameter(0,Fb276pt[ipt]);
		Double_t cincRAA276 = RAA276_data_PT[ipt];
		maxPRAA276 = fMyPromptRAA276->Eval(cincRAA276,minNPRAA276);
		minPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxNPRAA276);
		intPRAA276 = fMyPromptRAA276->Eval(cincRAA276,intNPRAA276);
		minnPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxxNPRAA276);
		//		printf("Inclusive J/psi R_{AA} = %f\n",RAA276_data_PT[ipt]);
		//		printf("Assuming F_{b} = %f, the promp J/psi R_{AA} varies from %f(%f\%) to %f(%f\%)\n",Fb276pt[ipt],minPRAA276,(minPRAA276-RAA276_data_PT[ipt])/RAA276_data_PT[ipt],maxPRAA276,(maxPRAA276-RAA276_data_PT[ipt])/RAA276_data_PT[ipt]);
		printf("| %.1f-%.1f | %.2f | %.4f | %.2f%% | \*%.2f%%\* | %.2f\% | \*%.2f%%\* |\n",pt276lim[ipt],pt276lim[ipt+1],cincRAA276,Fb276pt[ipt],100.*(minnPRAA276-cincRAA276)/cincRAA276,100.*(minPRAA276-cincRAA276)/cincRAA276,100.*(intPRAA276-cincRAA276)/cincRAA276,100.*(maxPRAA276-cincRAA276)/cincRAA276);
	}

	fMyPromptRAA->SetParameter(0,Fb5);
	maxPRAA = fMyPromptRAA->Eval(incRAA,minNPRAA);
	minPRAA = fMyPromptRAA->Eval(incRAA,maxNPRAA);
	intPRAA = fMyPromptRAA->Eval(incRAA,intNPRAA);
	minnPRAA = fMyPromptRAA->Eval(incRAA,maxxNPRAA);
	fMyPromptRAA276->SetParameter(0,Fb276);
	maxPRAA276 = fMyPromptRAA276->Eval(incRAA276,minNPRAA276);
	minPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxNPRAA276);
	intPRAA276 = fMyPromptRAA276->Eval(incRAA276,intNPRAA276);
	minnPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxxNPRAA276);
	printf("Inclusive J/psi R_{AA} Ratio 5 TeV over 2.76 TeV = %f\n",incRAA/incRAA276);
	printf("Assuming F_{b}^{5TeV} = %f and F_{b}^{2.76TeV} = %f,\n the promp J/psi R_{AA} ratio varies from %f(%f\%) to %f(%f\%)\n",Fb5,Fb276,minPRAA/minPRAA276,(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),maxPRAA/maxPRAA276,(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	if (bPrintAN) {
		printf("Centrality & $r^{inc}$ & $F_{\\rm b}(5.02 TeV)$ & $F_{\\rm b}(2.76 TeV)$ & $R_{\\rm AA}^{npro} = 1.2$ & $R_{\\rm AA}^{npro} = 1.0$ & $R_{\\rm AA}^{npro} = 0.5$ & $R_{\\rm AA}^{npro} = 0.0$ \\\\ \n");
		printf(" %.0f-%.0f & %.2f & %.4f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",CentLim[0],CentLim[ncent],incRAA/incRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(intPRAA/intPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	} else {
		printf("| Cent | r^incl | F_{b}^{5 TeV} | F_{b}^{2.76 TeV} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
		printf("| %.0f-%.0f | %.2f | %.4f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[0],CentLim[ncent],incRAA/incRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(intPRAA/intPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	}

	for (Int_t icent = 0 ; icent < ncent; icent++) {
		Double_t cincRAA = RAA5_data[icent]*0.975;
		maxPRAA = fMyPromptRAA->Eval(cincRAA,minNPRAA);
		minPRAA = fMyPromptRAA->Eval(cincRAA,maxNPRAA);
		intPRAA = fMyPromptRAA->Eval(cincRAA,intNPRAA);
		minnPRAA = fMyPromptRAA->Eval(cincRAA,maxxNPRAA);
		Double_t  cincRAA276 = RAA276_data[icent];
		maxPRAA276 = fMyPromptRAA276->Eval(cincRAA276,minNPRAA276);
		minPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxNPRAA276);
		intPRAA276 = fMyPromptRAA276->Eval(cincRAA276,intNPRAA276);
		minnPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxxNPRAA276);
//		printf("Inclusive J/psi R_{AA} Ratio 5 TeV over 2.76 TeV = %f\n",RAA5_data[icent]/RAA276_data[icent]);
//		printf("Assuming F_{b}^{5TeV} = %f and F_{b}^{2.76TeV} = %f,\n the promp J/psi R_{AA} ratio varies from %f(%f\%) to %f(%f\%)\n",Fb5,Fb276,minPRAA/minPRAA276,(minPRAA/minPRAA276-RAA5_data[icent]/RAA276_data[icent])/(RAA5_data[icent]/RAA276_data[icent]),maxPRAA/maxPRAA276,(maxPRAA/maxPRAA276-RAA5_data[icent]/RAA276_data[icent])/(RAA5_data[icent]/RAA276_data[icent]));
		if (bPrintAN) {
			printf(" %.0f-%.0f & %.2f & %.4f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",CentLim[icent],CentLim[icent+1],cincRAA/cincRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(minPRAA/minPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(intPRAA/intPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(maxPRAA/maxPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276));
		} else {
			printf("| %.0f-%.0f | %.2f | %.4f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",CentLim[icent],CentLim[icent+1],cincRAA/cincRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(minPRAA/minPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(intPRAA/intPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(maxPRAA/maxPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276));
		}
	}

	fMyPromptRAA->SetParameter(0,Fb5);
	maxPRAA = fMyPromptRAA->Eval(incRAA,minNPRAA);
	minPRAA = fMyPromptRAA->Eval(incRAA,maxNPRAA);
	intPRAA = fMyPromptRAA->Eval(incRAA,intNPRAA);
	minnPRAA = fMyPromptRAA->Eval(incRAA,maxxNPRAA);
	fMyPromptRAA276->SetParameter(0,Fb276);
	maxPRAA276 = fMyPromptRAA276->Eval(incRAA276,minNPRAA276);
	minPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxNPRAA276);
	intPRAA276 = fMyPromptRAA276->Eval(incRAA276,intNPRAA276);
	minnPRAA276 = fMyPromptRAA276->Eval(incRAA276,maxxNPRAA276);
	printf("Inclusive J/psi R_{AA} Ratio 5 TeV over 2.76 TeV = %f\n",incRAA/incRAA276);
	printf("Assuming F_{b}^{5TeV} = %f and F_{b}^{2.76TeV} = %f,\n the promp J/psi R_{AA} ratio varies from %f(%f\%) to %f(%f\%)\n",Fb5,Fb276,minPRAA/minPRAA276,(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),maxPRAA/maxPRAA276,(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	if (bPrintAN) {
		printf("| $\\p_{\\rm T}$ (GeV/$c$)& $r^{inc}$ & $F_{\\rm b}(5.02 TeV)$ & $F_{\\rm b}(2.76 TeV)$ & $R_{\\rm AA}^{npro} = 1.2$ & $R_{\\rm AA}^{npro} = 1.0$ & $R_{\\rm AA}^{npro} = 0.5$ & $R_{\\rm AA}^{npro} = 0.0$ \\\\ \n");
		printf(" %.1f-%.1f & %.2f & %.4f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",pt276lim[0],pt276lim[npt276],incRAA/incRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(intPRAA/intPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	} else {
		printf("| pT | r^incl | F_{b}^{5 TeV} | F_{b}^{2.76 TeV} | R_AA^np = 1.2 | *R_AA^np = 1.0* | R_AA^np = 0.5 | *R_AA^np = 0.0* |\n");
		printf("| %.1f-%.1f | %.2f | %.4f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",pt276lim[0],pt276lim[npt276],incRAA/incRAA276,Fb5,Fb276,100.*(minnPRAA/minnPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(minPRAA/minPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(intPRAA/intPRAA276-incRAA/incRAA276)/(incRAA/incRAA276),100.*(maxPRAA/maxPRAA276-incRAA/incRAA276)/(incRAA/incRAA276));
	}
	for (Int_t ipt = 0 ; ipt < npt276; ipt++) {
		fMyPromptRAA276->SetParameter(0,Fb276pt[ipt]);
		fMyPromptRAA->SetParameter(0,Fb5pt7[ipt]);

		Double_t cincRAA = RAA_2015_020[ipt]*0.975;
		maxPRAA = fMyPromptRAA->Eval(cincRAA,minNPRAA);
		minPRAA = fMyPromptRAA->Eval(cincRAA,maxNPRAA);
		intPRAA = fMyPromptRAA->Eval(cincRAA,intNPRAA);
		minnPRAA = fMyPromptRAA->Eval(cincRAA,maxxNPRAA);
		Double_t  cincRAA276 = RAA276_data_PT[ipt];
		maxPRAA276 = fMyPromptRAA276->Eval(cincRAA276,minNPRAA276);
		minPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxNPRAA276);
		intPRAA276 = fMyPromptRAA276->Eval(cincRAA276,intNPRAA276);
		minnPRAA276 = fMyPromptRAA276->Eval(cincRAA276,maxxNPRAA276);
		//		printf("Inclusive J/psi R_{AA} Ratio 5 TeV over 2.76 TeV = %f\n",RAA5_data[icent]/RAA276_data[icent]);
		//		printf("Assuming F_{b}^{5TeV} = %f and F_{b}^{2.76TeV} = %f,\n the promp J/psi R_{AA} ratio varies from %f(%f\%) to %f(%f\%)\n",Fb5,Fb276,minPRAA/minPRAA276,(minPRAA/minPRAA276-RAA5_data[icent]/RAA276_data[icent])/(RAA5_data[icent]/RAA276_data[icent]),maxPRAA/maxPRAA276,(maxPRAA/maxPRAA276-RAA5_data[icent]/RAA276_data[icent])/(RAA5_data[icent]/RAA276_data[icent]));
		if (bPrintAN) {
			printf(" %.1f-%.1f & %.2f & %.4f & %.4f & %.2f\\%% & %.2f\\%% & %.2f\\%% & %.2f\\%% \\\\ \n",pt276lim[ipt],pt276lim[ipt+1],cincRAA/cincRAA276,Fb5pt7[ipt],Fb276pt[ipt],100.*(minnPRAA/minnPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(minPRAA/minPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(intPRAA/intPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(maxPRAA/maxPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276));
		} else {
			printf("| %.1f-%.1f | %.2f | %.4f | %.4f | %.2f%% | \*%.2f%%\* | %.2f%% | \*%.2f%%\* |\n",pt276lim[ipt],pt276lim[ipt+1],cincRAA/cincRAA276,Fb5pt7[ipt],Fb276pt[ipt],100.*(minnPRAA/minnPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(minPRAA/minPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(intPRAA/intPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276),100.*(maxPRAA/maxPRAA276-cincRAA/cincRAA276)/(cincRAA/cincRAA276));
		}
	}

	return;
}

//// 5TeV Raa
//Double_t CentLim[ncent+1]={0.0,10.0,20.0,30.0,40.,50.,60.,70.,80.,90.};
//Double_t RAA5_data[ncent]={0.662,0.670,0.687,0.656,0.742,0.791,0.923,1.123,1.285};
//Double_t ErrStat_RAA5_data[ncent]={0.016,0.017,0.018,0.018,0.021,0.023,0.029,0.039,0.056};
//Double_t ErrSyst_RAA5_data[ncent]={0.017,0.018,0.019,0.025,0.026,0.033,0.049,0.076,0.109};
//
//// 5TeV Raa pt>0.3 with pp@5TeV
//Double_t RAA5_data_pt3_pp[ncent]={0.647,0.656,0.672,0.640,0.715,0.746,0.843,0.959,0.935};
//Double_t ErrStat_RAA5_data_pt3_pp[ncent]={0.010,0.011,0.012,0.013,0.015,0.017,0.022,0.033,0.045};
//Double_t ErrSyst_RAA5_data_pt3_pp[ncent]={0.017,0.018,0.018,0.024,0.025,0.031,0.045,0.065,0.079};// 5TeV Raa
//
//// 5TeV Raa Pt coordinates
//Double_t RAA5_data_PT[npt]={0.773,0.736,0.696,0.566,0.563,0.451,0.426,0.383,0.441,0.325,0.412};
//Double_t ErrStat_RAA5_data_PT[npt]={0.018,0.012,0.011,0.009,0.013,0.016,0.020,0.017,0.022,0.022,0.054};
//Double_t ErrSyst_RAA5_data_PT[npt]={0.102,0.087,0.078,0.064,0.078,0.054,0.069,0.064,0.112,0.080,0.278};
//
////	====RAA vs pT in 0-20%
//Double_t pT_2015_020[npt276]={0.55,1.55,2.55,3.55,4.55,5.55,7.05};
//Double_t RAA_2015_020[npt276]={0.959,0.825,0.668,0.508,0.466,0.374,0.329};
//Double_t ErrStat_RAA_2015_020[npt276]={0.030,0.018,0.016,0.012,0.016,0.015,0.013};
//Double_t ErrSyst_RAA_2015_020[npt276]={0.060,0.051,0.044,0.037,0.031,0.032,0.041};
//Double_t ErrGlobTotal_020=0.062897;
//
//// 2.76TeV Raa
//Double_t RAA276_data[ncent]={0.557,0.573,0.598,0.577,0.609,0.725,0.839,0.849,1.094};
//Double_t ErrStat_RAA276_data[ncent]={0.019,0.020,0.022,0.024,0.028,0.036,0.041,0.063,0.106};
//Double_t ErrSyst_RAA276_data[ncent]={0.024,0.022,0.020,0.025,0.030,0.043,0.058,0.068,0.104};
//
//// 2.76TeV Raa pt>3
//Double_t RAA276_data_pt3[ncent]={0.545,0.560,0.594,0.570,0.592,0.715,0.805,0.778,0.887};
//Double_t ErrStat_RAA276_data_pt3[ncent]={0.017,0.018,0.020,0.021,0.025,0.033,0.047,0.062,0.064};
//Double_t ErrSyst_RAA276_data_pt3[ncent]={0.016,0.017,0.018,0.023,0.024,0.030,0.044,0.064,0.078};
//
//// 2.76TeV Raa Pt coordinates
//Double_t RAA276_data_PT[npt276]={0.803,0.690,0.505,0.381,0.355,0.282,0.279};
//Double_t ErrStat_RAA276_data_PT[npt276]={0.084,0.052,0.042,0.037,0.052,0.048,0.064};
//Double_t ErrSyst_RAA276_data_PT[npt276]={0.113,0.084,0.062,0.046,0.041,0.032,0.032};
