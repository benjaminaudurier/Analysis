/*
 *  MuonGenerator.C
 *  aliroot_dev
 *
 *  Created by philippe pillot on 05/03/13.
 *  Copyright 2013 SUBATECH. All rights reserved.
 *
 */


#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include "TRandom.h"
#include "AliGenerator.h"
#include "AliGenParam.h"
#endif


static Int_t IpMuon( TRandom *ran);
static Double_t PtMuon( const Double_t *px, const Double_t */*dummy*/ );
static Double_t YMuon( const Double_t *py, const Double_t */*dummy*/ );
static Double_t V2Muon( const Double_t *pv, const Double_t */*dummy*/ );


//-------------------------------------------------------------------------
AliGenerator* GenParamCustomSingle()
{
  AliGenParam *singleMu = new AliGenParam(1,-1,PtMuon,YMuon,V2Muon,IpMuon);
  singleMu->SetMomentumRange(0,1e6);
  singleMu->SetPtRange(0.8,999.);
  singleMu->SetYRange(-4.2, -2.3);
  singleMu->SetPhiRange(0., 360.);
  singleMu->SetForceDecay(kNoDecay);
  singleMu->SetTrackingFlag(1);
  return singleMu;
}

//-------------------------------------------------------------------------
Int_t IpMuon(TRandom *ran)
{
  // muon composition

  if (ran->Rndm() < 0.5 )
  {
    return 13;
  }
  else
  {
    return -13;
  }
}

//-------------------------------------------------------------------------
Double_t PtMuon( const Double_t *px, const Double_t */*dummy*/ )
{
  // muon pT

  Double_t x=*px;
  Float_t p0,p1,p2,p3,p4,p5;

  //Default param. = tuned MSL LHC15n
  p0= 349.454;//135.137
  p1= 0.965971;//0.555323
  p2= 0.83717;//0.578374
  p3= 7.82193;//10.1345
  p4= -0.00325109;//0.000232233
  p5= -1.79551;//-0.924726


  return p0 * (1. / TMath::Power(p1 + TMath::Power(x,p2), p3) + p4 * TMath::Exp(p5*x));
}

//-------------------------------------------------------------------------
Double_t YMuon( const Double_t *py, const Double_t */*dummy*/ )
{
  // muon y

  Double_t y = *py;
  //pol4 only valid in y= -4;-2.5
  Float_t p0,p1,p2;

  //Default param. = tuned MSL LHC15n
  p0= 1.77115;//1.95551
  p1= -0.0966005;//-0.104761
  p2= 0.00260707;//0.00311324
  return p0 * (1. + p1*y*y + p2*y*y*y*y);
}

//-------------------------------------------------------------------------
Double_t V2Muon( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  //muon v2
  return 0.;
}
