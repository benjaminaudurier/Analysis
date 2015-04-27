//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
// DEFINING FITTING FUNCTION
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________

// To add it : .L FittinfFunctions.C

//------------------------------------
// define Crystal Ball 2 (7 parameters)
//------------------------------------
#include "TMath.h"


Double_t CrystalBall(Double_t *arg, Double_t *par)
{
    Double_t xcur = arg[0];
    Double_t alpha = par[0];
    Double_t alphaprime = par[1];
    Double_t n = par[2];
    Double_t nprime = par[3];
    Double_t mu = par[4];
    Double_t sigma = par[5];
    Double_t N = par[6];
    
    Double_t A = pow((n/(TMath::Abs(alpha))),n)*TMath::Exp(-(TMath::Abs(alpha)*TMath::Abs(alpha))/2);
    Double_t B = n/(TMath::Abs(alpha)) + TMath::Abs(alpha);
    Double_t C = pow((nprime/(TMath::Abs(alphaprime))),nprime)*TMath::Exp(-(TMath::Abs(alphaprime)*TMath::Abs(alphaprime))/2);
    Double_t D = nprime/(TMath::Abs(alphaprime)) - TMath::Abs(alphaprime);
    
    
    Double_t f=0.0;
    if (alphaprime > (xcur-mu)/sigma > (-1)*alpha)
        f = N*TMath::Exp((-1)*(xcur-mu)*(xcur-mu)/ (2*sigma*sigma));
    else if ((xcur-mu)/sigma <= (-1)*alpha)
        f = N*A*pow((B- (xcur-mu)/sigma),(-1*n));
    else if ((xcur-mu)/sigma >= (-1)*alphaprime)
        f = N*C*pow((D+ (xcur-mu)/sigma),(-1*nprime));
    return f;
}

//------------------------------------
// Define Background (4 parameters)
//------------------------------------

Double_t Background(Double_t *arg, Double_t *par)
{
    Double_t X = arg[0];
    Double_t N = par[0];
    Double_t alpha = par[1];
    Double_t beta = par[2];
    Double_t gamma = par[3];
    Double_t sigma = beta + gamma*((X-alpha)/alpha);
    Double_t f = N*TMath::Exp(-(pow(X-alpha,2))/(2*pow(sigma,2)));
    return f;
}


//------------------------------------
// Define Gaussian (3 paramters)
//------------------------------------


Double_t gaus(Double_t *arg, Double_t *par)
{
    Double_t X = arg[0];
    Double_t N = par[0];
    Double_t mu = par[1];
    Double_t sigma = par[2];
    return N*exp(-0.5*pow(((X-mu)/sigma),2));
    
}

//------------------------------------
// Define Sum
//------------------------------------

Double_t Sum1(Double_t *arg, Double_t *par)
{
    Double_t f;
    f=Background(arg,par)+CrystalBall(arg,&par[4]);
    return f ;
}

Double_t Sum2(Double_t *arg, Double_t *par)
{
    Double_t f;
    f=Background(arg,par)+gaus(arg,&par[4]);
    return f ;
}



