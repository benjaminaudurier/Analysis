

void RAAtheo_may2016(){

//Theory predictions for 
// 1) inclusive J/psi R_AA at sqrt(sNN)=5.02 TeV
// 2) double ratio R_AA(5.02 TeV)/R_AA(2.76 TeV)
// 3) R_AA vs pT
// Some predictions include (see comments) cuts such as pT>0.3 GeV/c
// (remove e.m. contribution) or 2<pT<5 GeV/c (enhances effect on double ratio)
// In some cases predictions for central rapidity are also available
// E.S. April 2016

gStyle->SetOptStat(0);

//====================================================================================================
//====================================================================================================
//====================================================================================================
//====================================================================================================
//										MODELS
//====================================================================================================
//====================================================================================================
//====================================================================================================


// From R. Rapp
//==============

Double_t NPart[27]={
406.2,405.1,401.5,395.3,386.5,375.1,361.4,345.6,328.2,309.4,289.7,269.2,248.3,227.1,206.0,185.1,164.6,144.7,125.5,107.3,90.1,74.1,59.6,46.5,35.1,25.4,17.6};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, EPS NLO based

Double_t RAAfwymin[27]={
4.45E-001,4.42E-001,4.43E-001,4.39E-001,4.39E-001,4.36E-001,4.35E-001,4.34E-001
,4.33E-001,4.32E-001,4.30E-001,4.30E-001,4.29E-001,4.30E-001,4.29E-001,4.33E-001
,4.35E-001,4.55E-001,4.73E-001,5.04E-001,5.33E-001,5.82E-001,6.29E-001,7.05E-001
,8.10E-001,9.37E-001,1.01E+000};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, no shad

Double_t RAAfwymax[27]={
7.24E-001,7.18E-001,7.19E-001,7.11E-001,7.08E-001,7.00E-001,6.95E-001,6.87E-001
,6.81E-001,6.73E-001,6.64E-001,6.57E-001,6.47E-001,6.40E-001,6.28E-001,6.25E-001
,6.14E-001,6.20E-001,6.14E-001,6.48E-001,6.76E-001,7.28E-001,7.72E-001,8.42E-001
,8.86E-001,9.71E-001,1.01E+000};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, EPS NLO based, pT>0.3 GeV/c

Double_t RAAfwyminptcut[27]={4.45E-001,4.42E-001,4.43E-001,4.39E-001,4.38E-001,
4.35E-001,4.35E-001,4.33E-001,4.33E-001,4.31E-001,4.29E-001.
,4.29E-001,4.28E-001,4.29E-001,4.27E-001,4.32E-001,4.33E-001,4.52E-001,4.70E-001,
5.01E-001,5.30E-001,5.78E-001,6.24E-001,6.99E-001,8.03E-001,9.29E-001,1.01E+000};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, no shad, pT>0.3 GeV/c

Double_t RAAfwymaxptcut[27]={7.24E-001,7.18E-001,7.19E-001,7.10E-001,7.07E-001
,6.99E-001,6.94E-001,6.86E-001,6.80E-001,6.71E-001,6.62E-001,6.55E-001,6.45E-001
,6.38E-001,6.25E-001,6.22E-001,6.11E-001,6.16E-001,6.10E-001,6.44E-001,6.71E-001
,7.22E-001,7.65E-001,8.35E-001,8.79E-001,9.63E-001,1.01E+000};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, no shad, 2<pT<5 GeV/c

Double_t RAAfwymaxpt25[27]={7.24E-001,7.17E-001,7.16E-001,7.04E-001,
,6.97E-001,6.82E-001,6.71E-001,6.55E-001,6.40E-001,6.23E-001,6.05E-001,
5.89E-001,5.70E-001,5.55E-001,5.36E-001,5.25E-001,5.11E-001,5.09E-001,5.04E-001
,5.25E-001,5.46E-001,5.84E-001,6.24E-001,6.83E-001,7.44E-001,8.24E-001,9.06E-001};

// 2.5<y<4 sigma_J/psi = 3.14 ub, sigma_cc = 0.57 mb, EPS NLO based, 2<pT<5 GeV/c

Double_t RAAfwyminpt25[27]={
4.45E-001,4.42E-001,4.41E-001,4.35E-001,4.32E-001,4.26E-001,4.21E-001,4.14E-001
,4.09E-001,4.02E-001,3.95E-001,3.89E-001,3.82E-001,3.78E-001,3.72E-001,3.71E-001
,3.69E-001,3.80E-001,3.91E-001,4.13E-001,4.35E-001,4.72E-001,5.13E-001,5.75E-001
,6.81E-001,8.00E-001,9.06E-001};

// sqrt(s)= 2.76 TeV,2.5<y<4 sigma_J/psi = 2.20 ub, sigma_cc = 0.40 mb, EPS NLO based, pT>0.3 GeV/c

Double_t RAA276fwyminptcut[27]={
3.98E-001,3.95E-001,3.96E-001,3.94E-001,3.94E-001,3.93E-001,3.94E-001,3.95E-001
,3.96E-001,3.98E-001,3.99E-001,4.02E-001,4.05E-001,4.16E-001,4.28E-001,4.42E-001
,4.57E-001,4.78E-001,5.00E-001,5.39E-001,5.78E-001,6.24E-001,6.64E-001,7.38E-001
,8.42E-001,9.59E-001,1.02E+000};

// sqrt(s)= 2.76 TeV,2.5<y<4 sigma_J/psi = 2.20 ub, sigma_cc = 0.40 mb, no shad, pT>0.3 GeV/c

Double_t RAA276fwymaxptcut[27]={
6.29E-001,6.25E-001,6.26E-001,6.20E-001,6.18E-001,6.14E-001,6.11E-001,6.08E-001
,6.05E-001,6.02E-001,5.98E-001,5.96E-001,5.92E-001,5.92E-001,5.89E-001,5.99E-001
,6.07E-001,6.28E-001,6.46E-001,6.85E-001,7.19E-001,7.69E-001,8.09E-001,8.77E-001
,9.21E-001,9.92E-001,1.02E+000};

// |y|<0.9 sigma_J/psi = 5.34 ub, sigma_cc = 0.92 mb, no shad

Double_t RAAcentymax[27]={
8.94E-001,8.87E-001,8.87E-001,8.75E-001,8.70E-001,8.57E-001,8.49E-001,8.36E-001
,8.24E-001,8.14E-001,8.04E-001,7.88E-001,7.67E-001,7.50E-001,7.24E-001,7.12E-001
,6.86E-001,6.80E-001,6.55E-001,6.64E-001,6.49E-001,6.87E-001,7.07E-001,7.81E-001
,8.35E-001,9.28E-001,9.79E-001};

// |y|<0.9 sigma_J/psi = 5.34 ub, sigma_cc = 0.92 mb, EPS NLO based

Double_t RAAcentymin[27]={
5.35E-001,5.31E-001,5.31E-001,5.27E-001,5.27E-001,5.23E-001,5.22E-001,5.18E-001
,5.15E-001,5.10E-001,5.05E-001,5.00E-001,4.92E-001,4.88E-001,4.79E-001,4.78E-001
,4.70E-001,4.74E-001,4.70E-001,4.87E-001,4.95E-001,5.37E-001,5.74E-001,6.52E-001
,7.61E-001,8.94E-001,9.80E-001};

// sqrt(s)= 2.76 TeV,2.5<y<4 sigma_J/psi = 2.20 ub, sigma_cc = 0.40 mb, no shad, 2<pt5  GeV/c

Double_t RAA276fwymaxpt25[27]={
5.92E-001,5.87E-001,5.86E-001,5.78E-001,5.73E-001,5.64E-001,5.57E-001,5.48E-001
,5.39E-001,5.30E-001,5.20E-001,5.13E-001,5.03E-001,4.99E-001,4.93E-001,4.96E-001
,5.00E-001,5.14E-001,5.28E-001,5.56E-001,5.86E-001,6.26E-001,6.69E-001,7.30E-001
,7.92E-001,8.65E-001,9.36E-001};

// sqrt(s)= 2.76 TeV,2.5<y<4 sigma_J/psi = 2.20 ub, sigma_cc = 0.40 mb, EPS NLO based, 2<pt5  GeV/c

Double_t RAA276fwyminpt25[27]={
3.76E-001,3.73E-001,3.73E-001,3.69E-001,3.68E-001,3.64E-001,3.62E-001,3.59E-001
,3.57E-001,3.55E-001,3.52E-001,3.51E-001,3.50E-001,3.56E-001,3.62E-001,3.71E-001
,3.80E-001,3.95E-001,4.12E-001,4.40E-001,4.72E-001,5.10E-001,5.52E-001,6.17E-001
,7.25E-001,8.41E-001,9.36E-001};


// RAA(5.02)/RAA(2.76), 2.5<y<4, sigma_cc(5.02 TeV)= 0.54 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwymin[27]={
1.061843562 ,1.060688180 ,1.061034831 ,1.0586493663,1.0577985468,1.0546026329
,1.0520082305,1.0480439945,1.0442859824,1.0389629519,1.032392263 ,1.0268130643
,1.021080794 ,1.0125608569,1.0021230956,0.9830738432,0.9593715655,0.9412647865
,0.9163476252,0.9142291809,0.9113748327,0.9189175592,0.9295518924,0.9368469618
,0.9443150677,0.960879,1.00553};

// RAA(5.02)/RAA(2.76), 2.5<y<4, sigma_cc(5.02 TeV)= 0.60 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwymax[27]={
1.2384291947,1.2364518497,1.2364430348,1.2322333495,1.2298283975,1.223920039
,1.2185330477,1.2108421681,1.2029313291,1.1928651225,1.1806611964,1.1692571558
,1.1566220313,1.1407581506,1.120827281,1.0922371885,1.055734907,1.0237121793
,0.9794717533,0.9693045543,0.9547200795,0.9577681914,0.9606864596,0.9651351082
,0.9661707005,0.980593,1.018244};

// RAA(5.02)/RAA(2.76), 2.5<y<4, pT>0.3 GeV/c, sigma_cc(5.02 TeV)= 0.54 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwyminptcut[27]={
1.063704456,1.0630053522,1.0631541668,1.0603321606,1.060216346,1.0567025516
,1.054003048,1.0498335875,1.0458954244,1.0399666936,1.0342361845,1.0287022064
,1.0233583733,1.014070688,1.0023519203,0.9845483801,0.9600370737,0.9427226762
,0.9172640766,0.9150211691,0.9123019712,0.9201851724,0.929495468,0.9370489606
,0.9442521585,0.9605411414,0.9781858434};

// RAA(5.02)/RAA(2.76), 2.5<y<4, pT>0.3 GeV/c, sigma_cc(5.02 TeV)= 0.60 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwymaxptcut[27]={
1.2405921837,1.2390972585,1.2388978538,1.2341898153,1.2325662454,1.2262628864
,1.2207527445,1.2128099225,1.2046844775,1.193937742,1.1825682989,1.171214297
,1.1589064748,1.1421970058,1.1207996937,1.0934752077,1.0560167616,1.0248077543
,0.9799755328,0.9696082947,0.9551658935,0.9584726988,0.9600324935,0.9646631699
,0.9654376111,0.9795238393,0.9900120482};

// RAA(5.02)/RAA(2.76), 2.5<y<4, 2<pT<5 GeV/c, sigma_cc(5.02 TeV)= 0.54 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwyminpt25[27]={
1.1311852969,1.1303878549,1.1297739203,1.127993604,1.126061896,1.1224374552
,1.1189508578,1.1131426145,1.1068040145,1.0993886871,1.0895552306,1.0801360823
,1.0689653322,1.0539505897,1.0349949839,1.0118176893,0.9826693896,0.9612342929
,0.9356946133,0.9290371366,0.9205660024,0.9222713768,0.9248045034,0.9293873903
,0.9363667484,0.9506186248,0.9669748231};

// RAA(5.02)/RAA(2.76), 2.5<y<4, 2<pT<5 GeV/c, sigma_cc(5.02 TeV)= 0.60 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiofwymaxpt25[27]={
1.3192942246,1.3175778134,1.3164305132,1.3125045829,1.3084348496,1.3013470935
,1.2940278158,1.2830612182,1.2707882514,1.2566502665,1.2388225323,1.2208633846
,1.1997647764,1.1741115353,1.1421183385,1.106399702,1.061588637,1.024848694
,0.9808609303,0.9646896145,0.9452465235,0.9415505832,0.9378349231,0.9390773706
,0.9421813203,0.9546663373,0.968931527};

// RAA(5.02)/RAA(2.76), |y|<0.8, sigma_cc(5.02 TeV)= 0.88 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiocentymin[27]={
1.0861174531,1.0831950548,1.0802874247,1.0776079085,1.0758018318,1.0740153033
,1.07189591,1.07524983,1.0782062979,1.0757790247,1.0742048038,1.0677291945
,1.059129813,1.0520697523,1.0432795101,1.0358014203,1.0247149165,1.0133388747
,0.9980100189,0.9717747996,0.9321979012,0.9264180707,0.9162936573,0.9260833232
,0.9387562538,0.9533880017,0.972103    };

// RAA(5.02)/RAA(2.76), |y|<0.8, sigma_cc(5.02 TeV)= 0.975 mub, sigma_cc(2.76 TeV)= ?

Double_t DoubleRatiocentymax[27]={
1.2896469569,1.2855949044,1.2818777954,1.2776291216,1.2744458448,1.2705971762
,1.2663018677,1.2614020588,1.2546959066,1.2545972428,1.2562095419,1.2443213759
,1.2290285816,1.2154150973,1.1977695179,1.1821688419,1.1592956209,1.1375748248
,1.1067053224,1.0671220817,1.007371902,0.9885986297,0.956914938,0.9626147541
,0.9666033441,0.9778275814,0.986654    
};

Double_t pt[51]={
0,0.2,0.4,0.6,0.8,1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4,3.6,3.8,4
,4.2,4.4,4.6,4.8,5,5.2,5.4,5.6,5.8,6,6.2,6.4,6.6,6.8,7,7.2,7.4,7.6,7.8,8
,8.2,8.4,8.6,8.8,9,9.2,9.4,9.6,9.8,10};

// sqrt(s)=5.02 TeV, 2.5<y<4, sigma_Jpsi = 3.14 ub, sigma_cc = 0.57 mub, Npart=400, EPS09NLO based shad

Double_t RAAptfwymin400[51]={
6.202621e-01,6.200681e-01,6.188614e-01,6.159691e-01,6.110831e-01,6.029112e-01
,5.913391e-01,5.758219e-01,5.558223e-01,5.317594e-01,5.049655e-01,4.754014e-01
,4.448088e-01,4.130383e-01,3.832282e-01,3.548252e-01,3.280327e-01,3.034181e-01
,2.807556e-01,2.622345e-01,2.447933e-01,2.300184e-01,2.162692e-01,2.058103e-01
,1.952445e-01,1.877500e-01,1.801014e-01,1.736013e-01,1.686021e-01,1.634058e-01
,1.587734e-01,1.554909e-01,1.517151e-01,1.484641e-01,1.467548e-01,1.446783e-01
,1.430557e-01,1.424488e-01,1.404362e-01,1.394692e-01,1.392066e-01,1.380566e-01
,1.376310e-01,1.370639e-01,1.369521e-01,1.374287e-01,1.379883e-01,1.377707e-01
,1.381017e-01,1.386785e-01,1.387089e-01};

// sqrt(s)=5.02 TeV, 2.5<y<4, sigma_Jpsi = 3.14 ub, sigma_cc = 0.57 mb, Npart=400, no shad

Double_t RAAptfwymax400[51]={
1.031506e+00,1.031112e+00,1.029013e+00,1.024083e+00,1.015580e+00,1.001421e+00
,9.811386e-01,9.538103e-01,9.186824e-01,8.764488e-01,8.291708e-01,7.771887e-01
,7.232683e-01,6.676523e-01,6.149219e-01,5.646643e-01,5.173900e-01,4.740087e-01
,4.342950e-01,4.011791e-01,3.705433e-01,3.444330e-01,3.205722e-01,3.018816e-01
,2.837343e-01,2.702732e-01,2.571134e-01,2.459156e-01,2.370844e-01,2.283010e-01
,2.205335e-01,2.147906e-01,2.085836e-01,2.032425e-01,2.001006e-01,1.965940e-01
,1.938011e-01,1.924567e-01,1.893208e-01,1.876498e-01,1.869795e-01,1.851792e-01
,1.843900e-01,1.834511e-01,1.831518e-01,1.836644e-01,1.843108e-01,1.839403e-01
,1.843171e-01,1.850338e-01,1.850348e-01};

// sqrt(s)=5.02 TeV, |y|<0.8, sigma_Jpsi = 5.34 ub, sigma_cc = 0.92 mb, Npart=400, EPS09NLO based shad

Double_t RAAptcentymin400[51]={
6.711359e-01,6.719058e-01,6.731650e-01,6.748442e-01,6.761807e-01,6.760690e-01
,6.733733e-01,6.668263e-01,6.563629e-01,6.404832e-01,6.207227e-01,5.958343e-01
,5.676942e-01,5.359239e-01,5.033706e-01,4.699883e-01,4.377018e-01,4.057257e-01
,3.743797e-01,3.450208e-01,3.177625e-01,2.927090e-01,2.700472e-01,2.497114e-01
,2.318223e-01,2.154305e-01,2.013189e-01,1.891889e-01,1.782158e-01,1.688431e-01
,1.603610e-01,1.529284e-01,1.462679e-01,1.406290e-01,1.358560e-01,1.310377e-01
,1.269752e-01,1.238079e-01,1.209586e-01,1.184693e-01,1.163349e-01,1.144713e-01
,1.128784e-01,1.115490e-01,1.102684e-01,1.093337e-01,1.086545e-01,1.078974e-01
,1.075860e-01,1.066523e-01,1.064523e-01};

// sqrt(s)=5.02 TeV, |y|<0.8, sigma_Jpsi = 5.34 ub, sigma_cc = 0.92 mb, Npart=400, no shad

Double_t RAAptcentymax400[51]={
1.136110e+00,1.137358e+00,1.139622e+00,1.142582e+00,1.144863e+00,1.144546e+00
,1.139638e+00,1.128008e+00,1.109235e+00,1.081137e+00,1.045761e+00,1.001620e+00
,9.515300e-01,8.954005e-01,8.374850e-01,7.781760e-01,7.204521e-01,6.636147e-01
,6.083575e-01,5.566778e-01,5.088385e-01,4.650267e-01,4.254876e-01,3.900910e-01
,3.589295e-01,3.306537e-01,3.062164e-01,2.851292e-01,2.662058e-01,2.499579e-01
,2.353682e-01,2.225868e-01,2.111901e-01,2.014651e-01,1.931658e-01,1.850448e-01
,1.781419e-01,1.726193e-01,1.676866e-01,1.633816e-01,1.596812e-01,1.564590e-01
,1.537011e-01,1.513862e-01,1.492172e-01,1.475800e-01,1.463466e-01,1.450693e-01
,1.444409e-01,1.427840e-01,1.424238e-01};

// sqrt(s)=5.02 TeV, 2.5<y<4, sigma_Jpsi = 3.14 ub, sigma_cc = 0.57 mub, Npart=324, EPS09NLO based shad

Double_t RAAptfwymin324[51]={
6.619402e-01,6.604524e-01,6.558669e-01,6.476949e-01,6.354792e-01,6.185282e-01
,5.970990e-01,5.710844e-01,5.413481e-01,5.090101e-01,4.746558e-01,4.406697e-01
,4.060579e-01,3.745474e-01,3.445145e-01,3.178751e-01,2.938873e-01,2.723685e-01
,2.546110e-01,2.403428e-01,2.264637e-01,2.164344e-01,2.080329e-01,2.011319e-01
,1.955278e-01,1.907646e-01,1.880894e-01,1.837586e-01,1.809835e-01,1.809915e-01
,1.793709e-01,1.773721e-01,1.771875e-01,1.774305e-01,1.780987e-01,1.792025e-01
,1.782688e-01,1.780212e-01,1.775469e-01,1.789275e-01,1.805625e-01,1.817661e-01
,1.837278e-01,1.842223e-01,1.856465e-01,1.873069e-01,1.865748e-01,1.882649e-01
,1.884211e-01,1.884393e-01,1.903850e-01};

// sqrt(s)=5.02 TeV, 2.5<y<4, sigma_Jpsi = 3.14 ub, sigma_cc = 0.57 mub, Npart=324, no shad

Double_t RAAptfwymax324[51]={
1.072181e+00,1.069665e+00,1.061885e+00,1.048014e+00,1.027216e+00,9.983808e-01
,9.617640e-01,9.173020e-01,8.663827e-01,8.108722e-01,7.520171e-01,6.934582e-01
,6.343425e-01,5.799683e-01,5.284746e-01,4.825670e-01,4.413496e-01,4.045871e-01
,3.739330e-01,3.489388e-01,3.254031e-01,3.077740e-01,2.930038e-01,2.808164e-01
,2.708467e-01,2.624093e-01,2.570791e-01,2.498570e-01,2.449317e-01,2.438824e-01
,2.408554e-01,2.374736e-01,2.366071e-01,2.364081e-01,2.368574e-01,2.379556e-01
,2.364280e-01,2.358585e-01,2.350346e-01,2.366952e-01,2.387223e-01,2.402061e-01
,2.427109e-01,2.432972e-01,2.451242e-01,2.472740e-01,2.462761e-01,2.484813e-01
,2.486684e-01,2.486778e-01,2.512338e-01};

// sqrt(s)=5.02 TeV, |y|<0.8, sigma_Jpsi = 5.34 ub, sigma_cc = 0.92 mb, Npart=324, EPS09NLO based shad

Double_t RAAptcentymin324[51]={
7.140247e-01,7.135390e-01,7.123656e-01,7.099676e-01,7.052375e-01,6.970351e-01
,6.846198e-01,6.676249e-01,6.453373e-01,6.176086e-01,5.862099e-01,5.518647e-01
,5.148782e-01,4.786710e-01,4.423435e-01,4.068920e-01,3.758740e-01,3.457188e-01
,3.186207e-01,2.954398e-01,2.733410e-01,2.559912e-01,2.385282e-01,2.269008e-01
,2.167375e-01,2.048018e-01,1.980479e-01,1.912593e-01,1.848234e-01,1.794295e-01
,1.758107e-01,1.723596e-01,1.705945e-01,1.664897e-01,1.648917e-01,1.620627e-01
,1.618131e-01,1.617070e-01,1.611183e-01,1.613230e-01,1.603861e-01,1.602923e-01
,1.588184e-01,1.594606e-01,1.585929e-01,1.589869e-01,1.602995e-01,1.615431e-01
,1.625346e-01,1.638275e-01,1.637999e-01};

// sqrt(s)=5.02 TeV, |y|<0.8, sigma_Jpsi = 5.34 ub, sigma_cc = 0.92 mb, Npart=324, no shad

Double_t RAAptcentymax324[51]={
1.164240e+00,1.163504e+00,1.161580e+00,1.157511e+00,1.149474e+00,1.135562e+00
,1.114435e+00,1.085362e+00,1.047322e+00,1.000225e+00,9.466813e-01,8.880433e-01
,8.251799e-01,7.630813e-01,7.010003e-01,6.406192e-01,5.869123e-01,5.353575e-01
,4.889538e-01,4.488690e-01,4.113390e-01,3.811397e-01,3.517686e-01,3.309747e-01
,3.128866e-01,2.931337e-01,2.807986e-01,2.689148e-01,2.579203e-01,2.486622e-01
,2.420541e-01,2.359305e-01,2.322571e-01,2.256908e-01,2.225995e-01,2.180287e-01
,2.169867e-01,2.162426e-01,2.149560e-01,2.147975e-01,2.132026e-01,2.127801e-01
,2.105880e-01,2.112317e-01,2.099198e-01,2.103033e-01,2.119249e-01,2.134767e-01
,2.147137e-01,2.163625e-01,2.162830e-01};

// Kai and Peifeng
//=================

Double_t Npart_Kai[26]={
407.19,405.6,400.66,392.07,379.71,363.85,345.01,323.79,300.79,276.53,251.5,226.13,
200.78,175.82,151.56,128.3,106.33,85.92,67.34,50.83,36.62,24.91,15.82,9.29,5.03,2.52};

// sqrt(s)=5.02 TeV, 2.5<y<4, dsigma_Jpsi/dy(y=3.25)=3.5ub, dsigma_charm/dy(y=3.25)=0.82mb(FONLL up limit), without Shadowing

Double_t RAA_Kai_max[26]={
0.7777,0.7775,0.7762,0.7742,0.7714,0.7684,0.7636,0.7584,0.7528,0.7468,0.7399,0.7326,
0.7251,0.7183,0.7127,0.7105,0.7149,0.7321,0.7144,0.6904,0.6724,0.6703,0.6934,0.7578,
0.8949,0.9983};

// sqrt(s)=5.02 TeV, 2.5<y<4, dsigma_Jpsi/dy(y=3.25)=3.5ub, dsigma_charm/dy(y=3.25)=0.82mb(FONLL up limit), EPS09NLO shadowing

Double_t RAA_Kai_min[26]={
0.5392,0.5327,0.5262,0.52,0.5143,0.5096,0.505,0.5014,0.4988,0.4968,0.496,0.4962,
0.4983,0.5018,0.5083,0.5193,0.5377,0.5693,0.5736,0.5726,0.5758,0.591,0.626,0.6966,
0.8343,0.9431};

// Anton 
//=========

Double_t Npart_Anton[18]={
20,40,60,80,100,120,140,160,180,200,220,240,260,280,300,320,340,360};

//sqrt(s)=2.76 TeV, 2.5<y<4, dsigmacc/dy = 0.21 mb

Double_t RAA276_Anton[18]={
1.000,1.000,0.918,0.758,0.664,0.613,0.589,0.573,0.567,0.563,0.561,0.562,0.567,
0.572,0.575,0.582,0.586,0.593};

//sqrt(s)=5.02 TeV, 2.5<y<4, dsigmacc/dy = 1.5 * 0.21 mb
// (factor 1.5 accounts for 1.59 from FONLL, reduced to account for extra shad)
// volume factor = 1.25 from 2.76 to 5 TeV

Double_t RAA502_Anton[18]={
1.000,1.062,0.795,0.680,0.620,0.593,0.585,0.583,0.587,0.591,0.596,0.602,0.611,
0.619,0.626,0.636,0.643,0.653};

//Band variation corresponds to +/-10% variation on sigma_cc (optimistic?)

Double_t DoubleRatiocentral_Anton[18]={
1.000,1.062,0.866,0.897,0.934,0.967,0.993,1.017,1.035,1.050,1.062,1.071,1.078,
1.082,1.089,1.093,1.097,1.101};

Double_t DoubleRatiomin_Anton[18]={
1.000,0.974,0.785,0.806,0.825,0.845,0.861,0.874,0.885,0.892,0.898,0.902,0.907,
0.907,0.910,0.912,0.915,0.916};

Double_t DoubleRatiomax_Anton[18]={
1.000,1.154,0.952,0.999,1.053,1.103,1.143,1.180,1.205,1.227,1.248,1.263,1.273,
1.281,1.292,1.296,1.305,1.310};


Double_t RAA502_minAnton[18];
Double_t RAA502_maxAnton[18];
for(Int_t j=0;j<18;j++){
RAA502_minAnton[j]=RAA502_Anton[j]*DoubleRatiomin_Anton[j]/DoubleRatiocentral_Anton[j];
RAA502_maxAnton[j]=RAA502_Anton[j]*DoubleRatiomax_Anton[j]/DoubleRatiocentral_Anton[j];
}

//////////////////////////////////////////////////////////
//Elena
//////////////////////////////////////////////////////////

// Inclusive J/psi 2.5<y<4
const int nb=14;
Double_t NPart_5_div2[nb]={
   201.65876887559463,199.16890389991912,191.44575718808002,178.66014611797439,     161.95338651499563,142.65554931535954,122.01919990493811,101.02635431106988,     80.503204449490454,61.180511296973719,43.720379178757184,28.734594741258359, 
   16.783255066604244,8.3083608186690672};       
Double_t NPart_5[nb];
for(int i=0;i<nb;i++) NPart_5[i] = NPart_5_div2[i]*2.;

//R_AA(5.02) min recombination (c=1.5, sigcc=0.6 mb)
Double_t RAA_5_min_Elena[nb]={   0.29703969307441075,0.29781295422184634,0.29989539862232545,0.30318266192917981,
0.30755304350454205,0.31340866341914658,0.32154111930646762,0.33312944151559348,
0.34996369457514265,0.37634710219441531,0.42040061972373510,0.49953068307005677,
0.64158432813914434,0.84379446792477575};

//R_AA(5.02) max recombination (c=3.3, sigcc=0.85 mb)
Double_t RAA_5_max_Elena[nb]={
0.61816573342012593,0.61844250018926816,0.61932137412365862,0.62139116025080843,
0.62445981769603531,0.62869419680691085,0.63459542433352290,0.64272792493180086,
0.65323430200455945,0.66787462899180106,0.68931230831615209,0.72436090075959514,
0.78824646308461954,0.89257500125404221};

//R_AA(5.02)/R_AA(2.76) min sigcc=0.6 mb at 5.02 TeV and sigcc=0.4 mb at 2.76 TeV
Double_t RAA_5_276_min_Elena[nb]={
0.8040310,0.8042830,0.8048560,0.8055382,
0.8062242,0.8068541,0.8075189,0.8080198,
0.8084049,0.8088541,0.8117998,0.8239846,
0.8592570,0.9217196};

//R_AA(5.02)/R_AA(2.76) max (sigcc=0.85 mb at 5.02 TeV and sigcc=0.6 mb at 2.76 TeV)
Double_t RAA_5_276_max_Elena[nb]={
1.059305,1.059006,1.058304,1.057489,
1.056663,1.055644,1.054269,1.051746,
1.047251,1.038422,1.022686,0.9968488,
0.9669523,0.9570791};


//====================================================================================================
//====================================================================================================
//====================================================================================================
//====================================================================================================
//										Graphics
//====================================================================================================
//====================================================================================================
//====================================================================================================


// Data (just as example, not up to date!!!)
//===========================================

// Npart
const int n=9;
Double_t NPart_data[9]={356.0,260.1,185.8,128.5,94.7,52.4,29.77,15.27,7.49};
Double_t ErrNPart_data[9]={8.,8.,8.,8.,8.,8.,8.,8.,8.};

//Pt
const int npt=11;
Double_t Pt_data[11]={0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,11};
Double_t ErrPt_data[11]={0.25.,0.25.,0.25.,0.25.,0.25.,0.25.,0.25.,0.25.,0.25.,0.25.,0.5};

// 5 to 2 TeV Ratio
Double_t Ratio_5_276[n]={1.197,1.179,1.157,1.146,1.227,1.099,1.109,1.333,1.184};
Double_t ErrStat_Ratio_5_276[n]={0.045,0.046,0.047,0.053,0.062,0.060,0.061,0.106,0.124};
Double_t ErrSyst_Ratio_5_276[n]={0.053,0.047,0.040,0.055,0.066,0.077,0.093,0.131,0.140};

Double_t glob20152011 = 0.176; //quadratic sum of 2011 and 20155 global uncertainties 

//Double_t scale_DR=1-0.176;
Double_t scale_DR=1.;
for(Int_t jj=0;jj<n;jj++){
  Ratio_5_276[jj]=Ratio_5_276[jj]*scale_DR;
  ErrStat_Ratio_5_276[jj]=ErrStat_Ratio_5_276[jj]*scale_DR;
  ErrSyst_Ratio_5_276[jj]=ErrSyst_Ratio_5_276[jj]*scale_DR;
}

// 5TeV Raa
Double_t RAA5_data[n]={0.656,0.664,0.689,0.647,0.725,0.772,0.902,1.093,1.258};
Double_t ErrStat_RAA5_data[n]={0.011,0.011,0.013,0.013,0.016,0.019,0.023,0.036,0.051};
Double_t ErrSyst_RAA5_data[n]={0.025,0.024,0.024,0.027,0.031,0.041,0.055,0.079,0.122};
 
// 5TeV Raa 0.3<pt<8 with pp@5TeV
Double_t RAA5_data_pt3_pp[n]={0.635,0.644,0.667,0.624,0.690,0.718,0.813,0.920,0.897};
Double_t ErrStat_RAA5_data_pt3_pp[n]={0.011,0.012,0.012,0.013,0.015,0.018,0.022,0.033,0.044};
Double_t ErrSyst_RAA5_data_pt3_pp[n]={0.025,0.023,0.024,0.026,0.030,0.039,0.051,0.066,0.076};// 5TeV Raa

 // 5TeV Raa pt>0.3 with interpolation
// Double_t RAA5_data_pt3_pp[n]={0.663,0.671,0.688,0.655,0.731,0.763,0.862,0.981,0.957};
// Double_t ErrStat_RAA5_data_pt3_pp[n]={0.011,0.012,0.012,0.013,0.016,0.018,0.023,0.034,0.046};
// Double_t ErrSyst_RAA5_data_pt3_pp[n]={0.031,0.031,0.031,0.033,0.037,0.045,0.057,0.071,0.080};// 5TeV Raa


// 2.76eV Raa pt>3 
Double_t RAA276_data_pt3[n]={0.545,0.560,0.594,0.570,0.592,0.715,0.805,0.778,0.887};
Double_t ErrStat_RAA276_data_pt3[n]={0.017,0.018,0.020,0.021,0.025,0.033,0.047,0.062,0.064};
Double_t ErrSyst_RAA276_data_pt3[n]={0.016,0.017,0.018,0.023,0.024,0.030,0.044,0.064,0.078};// 5TeV Raa

// 5TeV Raa 0<pt<8 with pp@5TeV
Double_t RAA5_data_pp[n]={0.632,0.640,0.664,0.624,0.699,0.745,0.870,1.054,1.213};
Double_t ErrStat_RAA5_data_pp[n]={0.011,0.011,0.012,0.013,0.015,0.018,0.022,0.035,0.049};
Double_t ErrSyst_RAA5_data_pp[n]={0.024,0.024,0.023,0.026,0.030,0.039,0.053,0.076,0.118};

// Pt coordinates
Double_t RAA5_data_PT[npt]={0.751,0.729,0.644,0.509,0.466,0.376,0.324,0.296,0.349,0.233,0.352};
Double_t ErrStat_RAA5_data_PT[npt]={0.027,0.017,0.016,0.013,0.016,0.015,0.014,0.021,0.026,0.025,0.032};
Double_t ErrSyst_RAA5_data_PT[npt]={0.067,0.058,0.047,0.037,0.043,0.031,0.032,0.037,0.063,0.051,0.075}; 


// TGraphs data
//================


// RAA_5/RAA_2.76 + interpolation reference
//____________________________
TGraphErrors *gRatio_5_276 = new
TGraphErrors(n,NPart_data,Ratio_5_276,0,ErrStat_Ratio_5_276);
gRatio_5_276->SetMarkerColor(kBlue+1);
gRatio_5_276->SetMarkerStyle(20);
gRatio_5_276->SetMarkerSize(1.5);

TGraphErrors *gRatio_5_276s = new
TGraphErrors(n,NPart_data,Ratio_5_276,ErrNPart_data,ErrSyst_Ratio_5_276);
gRatio_5_276s->SetMarkerColor(kBlue+1);
gRatio_5_276s->SetMarkerStyle(20);
gRatio_5_276s->SetMarkerSize(1.5);
//gRatio_5_276s->SetLineColor(kBlue+1);
gRatio_5_276s->SetFillStyle(0);

Double_t glob20152011 = 0.176;
TBox *box_glob20152011 = new TBox(435.,1.-glob20152011,450,1+glob20152011);
box_glob20152011->SetFillColor(kBlue+1);
//____________________________


// RAA_2.76 pt>0.3 + interpolation reference
//____________________________
TGraphErrors *gRAA276_data_pt3 = new
TGraphErrors(n,NPart_data,RAA276_data_pt3,0,ErrStat_RAA276_data_pt3);
gRAA276_data_pt3->SetMarkerColor(kGreen+1);
gRAA276_data_pt3->SetMarkerStyle(20);
gRAA276_data_pt3->SetMarkerSize(1.5);

TGraphErrors *gRAA276_datas_pt3 = new
TGraphErrors(n,NPart_data,RAA276_data_pt3,ErrNPart_data,ErrSyst_RAA276_data_pt3);
gRAA276_datas_pt3->SetMarkerColor(kGreen+1);
gRAA276_datas_pt3->SetMarkerStyle(20);
gRAA276_datas_pt3->SetMarkerSize(1.5);
//gRAA276_datas_pt3->SetLineColor(kGreen+1);
gRAA276_datas_pt3->SetFillStyle(0);

Double_t glob2011_pt3 = 0.15;
TBox *box_glob2011_pt3 = new TBox(420.,1.-glob2011_pt3,435,1+glob2011_pt3);
box_glob2011_pt3->SetFillColor(kGreen+1);
//____________________________


// Raa vs centrality for PbPb@5TeV + interpolation reference
//____________________________
TGraphErrors *gRAA5_data = new
TGraphErrors(n,NPart_data,RAA5_data,0,ErrStat_RAA5_data);
//gRAA5_data->SetMarkerColor(kGreen+1);
gRAA5_data->SetMarkerStyle(20);
gRAA5_data->SetMarkerSize(1.5);

TGraphErrors *gRAA5_datas = new
TGraphErrors(n,NPart_data,RAA5_data,ErrNPart_data,ErrSyst_RAA5_data);
//gRAA5_datas->SetMarkerColor(kGreen+1);
gRAA5_datas->SetMarkerStyle(20);
gRAA5_datas->SetMarkerSize(1.5);
//gRAA5_datas->SetLineColor(kGreen+1);
gRAA5_datas->SetFillStyle(0);

Double_t globRAA_data = 0.077496;
TBox *box_globRAA_data = new TBox(435.,1.-globRAA_data,450,1+globRAA_data);
box_globRAA_data->SetFillColor(1);
//____________________________


// Raa vs centrality for PbPb@5TeV + pp@5TeV reference
//____________________________
TGraphErrors *gRAA5_data_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pp,0,ErrStat_RAA5_data_pp);
gRAA5_data_pp->SetMarkerColor(kBlue+1);
gRAA5_data_pp->SetMarkerStyle(20);
gRAA5_data_pp->SetMarkerSize(1.5);

TGraphErrors *gRAA5_datas_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pp,ErrNPart_data,ErrSyst_RAA5_data_pp);
gRAA5_datas_pp->SetMarkerColor(kBlue+1);
gRAA5_datas_pp->SetMarkerStyle(20);
gRAA5_datas_pp->SetMarkerSize(1.5);
//gRAA5_datas_pp->SetLineColor(kGreen+1);
gRAA5_datas_pp->SetFillStyle(0);

Double_t globRAA_data_pt = 0.076086;
TBox *box_globRAA_data_pt = new TBox(420.,1.-globRAA_data_pt,435,1+globRAA_data_pt);
box_globRAA_data_pt->SetFillColor(kBlue+1);

//____________________________


// Raa vs centrality for PbPb@5TeV + pp@5TeV reference for pt>0.3
//____________________________
TGraphErrors *gRAA5_data_pt3_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pt3_pp,0,ErrStat_RAA5_data_pt3_pp);
gRAA5_data_pt3_pp->SetMarkerColor(kRed+1);
gRAA5_data_pt3_pp->SetMarkerStyle(20);
gRAA5_data_pt3_pp->SetMarkerSize(1.5);

TGraphErrors *gRAA5_datas_pt3_pp = new
TGraphErrors(n,NPart_data,RAA5_data_pt3_pp,ErrNPart_data,ErrSyst_RAA5_data_pt3_pp);
gRAA5_datas_pt3_pp->SetMarkerColor(kRed+1);
gRAA5_datas_pt3_pp->SetMarkerStyle(20);
gRAA5_datas_pt3_pp->SetMarkerSize(1.5);
//gRAA5_datas_pt3_pp->SetLineColor(kGreen+1);
gRAA5_datas_pt3_pp->SetFillStyle(0);

// Double_t globRAA_data_pt3 = 0.09213; // Interpolation
Double_t globRAA_data_pt3 = 0.074591; // Data
TBox *box_globRAA_data_pt3_pp = new TBox(435.,1.-globRAA_data_pt3,450,1+globRAA_data_pt3);
box_globRAA_data_pt3_pp->SetFillColor(kRed+1);

//____________________________


// Raa vs pt for PbPb@5TeV + pp@5TeV reference
//____________________________
TGraphErrors *gRAA5_datas_pt = new
TGraphErrors(npt,Pt_data,RAA5_data_PT,ErrPt_data,ErrStat_RAA5_data_PT);
gRAA5_datas_pt->SetMarkerColor(kRed+1);
gRAA5_datas_pt->SetMarkerStyle(20);
gRAA5_datas_pt->SetMarkerSize(1.5);


TGraphErrors *gRAA5_datas_pt_sys = new
TGraphErrors(npt,Pt_data,RAA5_data_PT,ErrPt_data,ErrSyst_RAA5_data_PT);
gRAA5_datas_pt_sys->SetMarkerStyle(20);
gRAA5_datas_pt_sys->SetMarkerSize(1.5);
gRAA5_datas_pt_sys->SetLineColor(kRed+1);
gRAA5_datas_pt_sys->SetFillStyle(0);

Double_t glob2015_pt_12 = 0.040247;
TBox *box_glob2015_pt_12 = new TBox(11.5,1.-glob2015_pt_12,12,1+glob2015_pt_12);
box_glob2015_pt_12->SetFillColor(kRed+1);
//____________________________




//====================================================================================================
//====================================================================================================
//====================================================================================================
//										Plots
//====================================================================================================
//====================================================================================================
//====================================================================================================


// TGraphs calculation
//=====================

TGraph *gRAAfwymin  = new TGraph(27,NPart,RAAfwymin);
TGraph *gRAAfwymax  = new TGraph(27,NPart,RAAfwymax);
TGraph *gRAAfwyminptcut  = new TGraph(27,NPart,RAAfwyminptcut);
TGraph *gRAAfwymaxptcut  = new TGraph(27,NPart,RAAfwymaxptcut);
TGraph *gRAA276fwyminptcut  = new TGraph(27,NPart,RAA276fwyminptcut);
TGraph *gRAA276fwymaxptcut  = new TGraph(27,NPart,RAA276fwymaxptcut);
TGraph *gRAAfwyminpt25  = new TGraph(27,NPart,RAAfwyminpt25);
TGraph *gRAAfwymaxpt25  = new TGraph(27,NPart,RAAfwymaxpt25);
TGraph *gRAA276fwyminpt25  = new TGraph(27,NPart,RAA276fwyminpt25);
TGraph *gRAA276fwymaxpt25  = new TGraph(27,NPart,RAA276fwymaxpt25);
TGraph *gRAAcentymin  = new TGraph(27,NPart,RAAcentymin);
TGraph *gRAAcentymax  = new TGraph(27,NPart,RAAcentymax);

TGraph *gRAAfwyminKai  = new TGraph(26,Npart_Kai,RAA_Kai_min);
TGraph *gRAAfwymaxKai  = new TGraph(26,Npart_Kai,RAA_Kai_max);

TGraph *gRAAfwyAnton  = new TGraph(18,Npart_Anton,RAA502_Anton);
TGraph *gRAAfwyminAnton  = new TGraph(18,Npart_Anton,RAA502_minAnton);
TGraph *gRAAfwymaxAnton  = new TGraph(18,Npart_Anton,RAA502_maxAnton);

TGraph *gRAAfwyminElena = new TGraph(14,NPart_5,RAA_5_min_Elena);
TGraph *gRAAfwymaxElena = new TGraph(14,NPart_5,RAA_5_max_Elena);

//TGraph *gRAAminforw276  = new TGraph(15,NPart_shad,Total_shad);
//TGraph *gRAAmaxforw276  = new TGraph(15,NPart_shad,Total_noshad);

TGraph *gRAAratiominforw  = new TGraph(27,NPart,DoubleRatiofwymin);
TGraph *gRAAratiomaxforw  = new TGraph(27,NPart,DoubleRatiofwymax);
TGraph *gRAAratiominforwptcut  = new TGraph(27,NPart,DoubleRatiofwyminptcut);
TGraph *gRAAratiomaxforwptcut  = new TGraph(27,NPart,DoubleRatiofwymaxptcut);
TGraph *gRAAratiominforwpt25  = new TGraph(27,NPart,DoubleRatiofwyminpt25);
TGraph *gRAAratiomaxforwpt25  = new TGraph(27,NPart,DoubleRatiofwymaxpt25);
TGraph *gRAAratiomincent  = new TGraph(27,NPart,DoubleRatiocentymin);
TGraph *gRAAratiomaxcent  = new TGraph(27,NPart,DoubleRatiocentymax);

TGraph *gRAAratiominforwAnton  = new TGraph(18,Npart_Anton,DoubleRatiomin_Anton);
TGraph *gRAAratiocenforwAnton  = new TGraph(18,Npart_Anton,DoubleRatiocentral_Anton);
TGraph *gRAAratiomaxforwAnton  = new TGraph(18,Npart_Anton,DoubleRatiomax_Anton);

TGraph *gRAAratiominforwElena  = new TGraph(14,NPart_5,RAA_5_276_min_Elena);
TGraph *gRAAratiomaxforwElena  = new TGraph(14,NPart_5,RAA_5_276_max_Elena);

TGraph *gRAAptminforw324  = new TGraph(51,pt,RAAptfwymin324);
TGraph *gRAAptmaxforw324  = new TGraph(51,pt,RAAptfwymax324);
TGraph *gRAAptminforw400  = new TGraph(51,pt,RAAptfwymin400);
TGraph *gRAAptmaxforw400  = new TGraph(51,pt,RAAptfwymax400);
TGraph *gRAAptmincent324  = new TGraph(51,pt,RAAptcentymin324);
TGraph *gRAAptmaxcent324  = new TGraph(51,pt,RAAptcentymax324);
TGraph *gRAAptmincent400  = new TGraph(51,pt,RAAptcentymin400);
TGraph *gRAAptmaxcent400  = new TGraph(51,pt,RAAptcentymax400);

TH2D *hRAA = new TH2D("hRAA","",100,0.,450.,100,0.,1.5);
hRAA->GetXaxis()->SetTitle("N_{part}");
hRAA->GetYaxis()->SetTitle("R_{AA}");

TH2D *hdr = new TH2D("h","",100,0.,450.,100,0.5,1.6);
hdr->GetXaxis()->SetTitle("N_{part}");
hdr->GetYaxis()->SetTitle("R_{AA}(5.02 TeV)/R_{AA}(2.76 TeV)");

TH2D *hpt = new TH2D("h","",100,0.,12.,100,0.,1.5);
hpt->GetXaxis()->SetTitle("p_{T} (GeV/c)");
hpt->GetYaxis()->SetTitle("R_{AA}(5.02 TeV)");

gRAAfwymin->SetLineColor(2);
gRAAfwymax->SetLineColor(2);
gRAAfwyminptcut->SetLineColor(3);
gRAAfwymaxptcut->SetLineColor(3);
gRAA276fwyminptcut->SetLineColor(6);
gRAA276fwymaxptcut->SetLineColor(6);
gRAA276fwyminpt25->SetLineColor(6);
gRAA276fwymaxpt25->SetLineColor(6);
gRAAfwyminpt25->SetLineColor(2);
gRAAfwymaxpt25->SetLineColor(2);
gRAAfwyAnton->SetLineColor(4);
gRAAfwyminAnton->SetLineColor(4);
gRAAfwymaxAnton->SetLineColor(4);
gRAAfwyminElena->SetLineColor(6);
gRAAfwymaxElena->SetLineColor(6);
gRAAfwyminKai->SetLineColor(2);
gRAAfwymaxKai->SetLineColor(2);
gRAAcentymin->SetLineColor(4);
gRAAcentymax->SetLineColor(4);
gRAAratiominforw->SetLineColor(2);
gRAAratiomaxforw->SetLineColor(2);
gRAAratiominforwptcut->SetLineColor(3);
gRAAratiomaxforwptcut->SetLineColor(3);
gRAAratiominforwpt25->SetLineColor(2);
gRAAratiomaxforwpt25->SetLineColor(2);
gRAAratiomincent->SetLineColor(4);
gRAAratiomaxcent->SetLineColor(4);
gRAAratiominforwAnton->SetLineColor(4);
gRAAratiocenforwAnton->SetLineColor(4);
gRAAratiomaxforwAnton->SetLineColor(4);
gRAAratiomaxforwElena->SetLineColor(6);
gRAAratiominforwElena->SetLineColor(6);
gRAAptminforw324->SetLineColor(4);
gRAAptmaxforw324->SetLineColor(4);
gRAAptminforw400->SetLineColor(2);
gRAAptmaxforw400->SetLineColor(2);
gRAAptmincent324->SetLineColor(4);
gRAAptmaxcent324->SetLineColor(4);
gRAAptmincent400->SetLineColor(2);
gRAAptmaxcent400->SetLineColor(2);


//____________________________
// raa vs centrality with and without pt cut
//____________________________

new TCanvas();
hRAA->Draw();
gRAA5_data_pp->Draw("P");
gRAA5_datas_pp->Draw("E2");
gRAA5_data_pt3_pp->Draw("sameP");
gRAA5_datas_pt3_pp->Draw("sameE2");

TLegend *legRAAvsCent = new TLegend(0.5,0.7,0.9,0.9,"R_{AA} #sqrt{s}=5.02 TeV with pp@5TeV ref.");
legRAAvsCent->AddEntry(gRAA5_data_pp,"0<pt<8, ","pe");
legRAAvsCent->AddEntry(gRAA5_data_pt3_pp,"0.3<pt<8","pe");

legRAAvsCent->Draw("same");
box_globRAA_data_pt->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

box_globRAA_data_pt3_pp->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();
// ____________________________


//____________________________
// The Two raa vs centrality 2015-2011 pt>0.3
//____________________________

new TCanvas();
hRAA->Draw();
gRAA276_data_pt3->Draw("P");
gRAA276_datas_pt3->Draw("E2");
gRAA5_data_pt3_pp->Draw("sameP");
gRAA5_datas_pt3_pp->Draw("sameE2");

TLegend *legRAAvsCent = new TLegend(0.5,0.7,0.9,0.9,"");
legRAAvsCent->AddEntry(gRAA276_data_pt3,"R_{AA} #sqrt{s}=2.76, 0.3<pt<8, ","pe");
legRAAvsCent->AddEntry(gRAA5_data_pt3_pp,"R_{AA} #sqrt{s}=5.02, 0.3<pt<8","pe");

legRAAvsCent->Draw("same");
box_globRAA_data_pt->Draw("F");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

box_globRAA_data_pt3_pp->Draw("F");
box_glob2011_pt3->Draw("F");
// ____________________________


//____________________________
// Rapp and Du pt 0-20% forward rapidity
//____________________________

TCanvas *c2 = new TCanvas("c2","c2");
hpt->Draw();
gRAA5_datas_pt->Draw("P");
gRAA5_datas_pt_sys->Draw("E2");
gRAAptminforw324->Draw("L");
gRAAptmaxforw324->Draw("L");
// gRAAptminforw400->Draw("L");
// gRAAptmaxforw400->Draw("L");
TLine *l = new TLine(0.,1.,12.,1.);
l->SetLineStyle(2);
l->Draw();
box_glob2015_pt_12->Draw("F");

TLegend *legpt = new TLegend(0.65,0.7,0.9,0.9,"  R_{AA} #sqrt{s}=5.02 TeV with pp@5TeV ref.");
legpt->AddEntry(gRAAptminforw324,"Rapp and Du 2.5<y<4, N_{part}=324","l");
// legpt->AddEntry(gRAAptminforw400,"2.5<y<4, N_{part}=400","l");
legpt->Draw();
//____________________________



// ____________________________
// RAA vs Centrality Transport model + pp reference (pt>0.3GeV)
// ____________________________

TCanvas *c3compareRAA1 = new TCanvas("c3compareRAA1","c3compareRAA1");
hRAA->Draw();
gRAAfwyminptcut->Draw("L");
gRAAfwymaxptcut->Draw("L");
gRAAfwyminKai->Draw("L");
gRAAfwymaxKai->Draw("L");
gRAA5_data_pt3_pp->Draw("P");
gRAA5_datas_pt3_pp->Draw("E2");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

TLegend *legRAAcompare_pp1 = new TLegend(0.5,0.7,0.9,0.9,"#sqrt{s}=5.02 TeV, 2.5<y<4");
legRAAcompare_pp1->AddEntry(gRAAfwyminKai,"Kai and Pengfei","l");
legRAAcompare_pp1->AddEntry(gRAAfwyminptcut,"Rapp, p_{T}>0.3 GeV/c","l");
legRAAcompare_pp1->AddEntry(gRAA5_data_pt3_pp,"ALICE, p_{T}>0.3 GeV/c,","P");
legRAAcompare_pp1->Draw();
box_globRAA_data_pt3_pp->Draw("F");
l1->Draw();
//____________________________



// ____________________________
// RAA vs Centrality Hadronization + pp reference (pt>0.3GeV) 
// ____________________________

TCanvas *c3compareRAA2 = new TCanvas("c3compareRAA2","c3compareRAA2");
hRAA->Draw();
gRAAfwyAnton->Draw("L");
gRAAfwyminAnton->Draw("L");
gRAAfwymaxAnton->Draw("L");
gRAA5_data_pt3_pp->Draw("P");
gRAA5_datas_pt3_pp->Draw("E2");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

TLegend *legRAAcompare_pp2 = new TLegend(0.5,0.7,0.9,0.9,"#sqrt{s}=5.02 TeV, 2.5<y<4");
legRAAcompare_pp2->AddEntry(gRAAfwyAnton,"Anton","l");
legRAAcompare_pp2->AddEntry(gRAA5_data_pt3_pp,"ALICE, p_{T}>0.3 GeV/c,","P");
legRAAcompare_pp2->Draw();
box_globRAA_data_pt3_pp->Draw("F");
l1->Draw();
//____________________________



// ____________________________
// RAA vs Centrality Comover + pp reference (pt>0.3GeV)
// ____________________________

TCanvas *c3compareRAA3 = new TCanvas("c3compareRAA3","c3compareRAA3");
hRAA->Draw();
gRAAfwyminElena->Draw("L");
gRAAfwymaxElena->Draw("L");
gRAA5_data_pt3_pp->Draw("P");
gRAA5_datas_pt3_pp->Draw("E2");
TLine *l1 = new TLine(0.,1.,450.,1.);
l1->SetLineStyle(2);
l1->Draw();

TLegend *legRAAcompare_pp1 = new TLegend(0.5,0.7,0.9,0.9,"#sqrt{s}=5.02 TeV, 2.5<y<4");
legRAAcompare_pp1->AddEntry(gRAAfwyminElena,"Elena","l");
legRAAcompare_pp1->AddEntry(gRAA5_data_pt3_pp,"ALICE, p_{T}>0.3 GeV/c,","P");
legRAAcompare_pp1->Draw();
box_globRAA_data_pt3_pp->Draw("F");
l1->Draw();
//____________________________



// ____________________________
// RAA vs Centrality Ratio Transport + interpolation (pt>0.3GeV) 
// ____________________________

TCanvas *c3comparedoubleRatio1 = new TCanvas("c3comparedoubleRatio1","c3comparedoubleRatio1");
hdr->Draw();
gRAAratiominforwptcut->Draw("L");
gRAAratiomaxforwptcut->Draw("L");
gRatio_5_276->Draw("P");
gRatio_5_276s->Draw("E2");
box_glob20152011->Draw("F");
l1->Draw();

TLegend *legDoubleRatiocompare1 = new TLegend(0.6,0.75,0.9,0.9,"2.5<y<4");
legDoubleRatiocompare1->AddEntry(gRAAratiominforwptcut,"Rapp, p_{T}>0.3 GeV/c","l");
legDoubleRatiocompare1->AddEntry(gRatio_5_276,"ALICE, p_{T}>0.3 GeV/c","P");
legDoubleRatiocompare1->Draw();



// ____________________________
// RAA vs Centrality Ratio Hadronization + interpolation (pt>0.3GeV) 
// ____________________________

TCanvas *c3comparedoubleRatio2 = new TCanvas("c3comparedoubleRatio2","c3comparedoubleRatio2");
hdr->Draw();
gRAAratiominforwAnton->Draw("L");
gRAAratiocenforwAnton->Draw("L");
gRAAratiomaxforwAnton->Draw("L");
gRatio_5_276->Draw("P");
gRatio_5_276s->Draw("E2");
box_glob20152011->Draw("F");
l1->Draw();

TLegend *legDoubleRatiocompare2 = new TLegend(0.6,0.75,0.9,0.9,"2.5<y<4");
legDoubleRatiocompare2->AddEntry(gRAAratiominforwAnton,"Anton","l");
legDoubleRatiocompare2->AddEntry(gRatio_5_276,"ALICE, p_{T}>0.3 GeV/c","P");
legDoubleRatiocompare2->Draw();


// ____________________________
// RAA vs Centrality Ratio Comover + interpolation (pt>0.3GeV) 
// ____________________________

TCanvas *c3comparedoubleRatio3 = new TCanvas("c3comparedoubleRatio3","c3comparedoubleRatio3");
hdr->Draw();
gRAAratiominforwElena->Draw("L");
gRAAratiomaxforwElena->Draw("L");
gRatio_5_276->Draw("P");
gRatio_5_276s->Draw("E2");
box_glob20152011->Draw("F");
l1->Draw();

TLegend *legDoubleRatiocompare3 = new TLegend(0.6,0.75,0.9,0.9,"2.5<y<4");
legDoubleRatiocompare3->AddEntry(gRAAratiominforwElena,"Elena","l");
legDoubleRatiocompare3->AddEntry(gRatio_5_276,"ALICE, p_{T}>0.3 GeV/c","P");
legDoubleRatiocompare3->Draw();


}


