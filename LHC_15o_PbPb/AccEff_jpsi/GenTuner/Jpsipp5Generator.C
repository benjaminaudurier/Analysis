/*
 *  JPsipp5Generator.C
 *
 *  Created by Cynthia Hadjidakis on 08/03/13.
 *
 */


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenParam.h"
#endif 


static Int_t IpJpsipp5( TRandom *ran);
static Double_t PtJpsipp5( const Double_t *px, const Double_t */*dummy*/ );
static Double_t YJpsipp5( const Double_t *py, const Double_t */*dummy*/ );
static Double_t V2Jpsipp5( const Double_t *pv, const Double_t */*dummy*/ );


//-------------------------------------------------------------------------
AliGenerator* Jpsipp5Generator()
{
  cout<<"Jpsipp5Generator()"<<endl;
  //  AliGenParam *singleMu = new AliGenParam(10, AliGenMUONlib::kMuon,"","");
  AliGenParam *jpsi = new AliGenParam(1,-1,PtJpsipp5,YJpsipp5,V2Jpsipp5,IpJpsipp5);
  jpsi->SetMomentumRange(0,1e6);
  jpsi->SetPtRange(0,999.);
  jpsi->SetYRange(-4.5, -2.);
  jpsi->SetPhiRange(0., 360.);
  jpsi->SetCutOnChild(1);
  jpsi->SetChildPhiRange(0.,360.);
  jpsi->SetChildThetaRange(0.,180.);
  jpsi->SetForceDecay(kDiMuon);
  jpsi->SetTrackingFlag(1);
  
  return jpsi;
}

//-------------------------------------------------------------------------
Int_t IpJpsipp5( TRandom *)
{
  //jpsi composition
  return 443;
}

//-------------------------------------------------------------------------
Double_t PtJpsipp5( const Double_t *px, const Double_t */*dummy*/ )
{
  // jpsi pT in pPb
  // power law tuned on pPb data
  Double_t x=*px;
  Float_t p0,p1,p2,p3;
  p0 = 1.13e9;
  p1 = 3.404;
  p2 = 3.822;
  p3 = 1.962;
  return p0 *x / TMath::Power( 1. + TMath::Power(x/p1,p2), p3 );
}

//-------------------------------------------------------------------------
Double_t YJpsipp5( const Double_t *py, const Double_t */*dummy*/ )
{
  // jpsi y in pPb
  // pol2 tuned on pPb data only valid in ylab = -4;-2.5
  Double_t y = *py;
  Float_t p0,p1;
  p0 = 6.266e5;
  p1 = -3.007e4;
  return p0 + p1*y*y;
}

//-------------------------------------------------------------------------
Double_t V2Jpsipp5( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  //jpsi v2
  return 0.;
}

