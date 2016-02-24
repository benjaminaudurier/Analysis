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
  AliGenParam *singleMu = new AliGenParam(5,-1,PtMuon,YMuon,V2Muon,IpMuon);
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
  p0 = 349.454; //4.05962;
  p1 = 0.965971; //1;
  p2 = 0.83717; //2.46187;
  p3 = 7.82193; //2.08644;
  p4 = -0.00325109;
  p5 = -1.79551;


  return p0 * (1. / TMath::Power(p1 + TMath::Power(x,p2), p3) + p4 * TMath::Exp(p5*x));
}

//-------------------------------------------------------------------------
Double_t YMuon( const Double_t *py, const Double_t */*dummy*/ )
{
  // muon y
  
  Double_t y = *py;
  //pol4 only valid in y= -4;-2.5
  Float_t p0,p1,p2,p3;
  p0 = 1.77115; //0.729545;
  p1 = -0.0966005; //0.53837;
  p2 = 0.00260707; //0.141776;
  p3 = 0.0130173; //0.0130173;
  return p0 * (1. + p1*y*y + p2*y*y*y*y );
}

//-------------------------------------------------------------------------
Double_t V2Muon( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  //muon v2
  return 0.;
}

