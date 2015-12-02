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
  AliGenParam *singleMu = new AliGenParam(30,-1,PtMuon,YMuon,V2Muon,IpMuon);
  singleMu->SetMomentumRange(0,1e6);
  singleMu->SetPtRange(0.8,999.);
  singleMu->SetYRange(-4.3, -2.3);
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
  // Float_t p0,p1,p2,p3;
  // p0 = 4.05962; //4.05962;
  // p1 = 1.0; //1;
  // p2 = 2.46187; //2.46187;
  // p3 = 2.08644; //2.08644;
  // p4 = 2.08644; //2.08644;
  // p5 = 2.08644; //2.08644;
  Double_t p[6] = {135.137, 0.555323, 0.578374, 10.1345, 0.000232233, -0.924726};

  return p[0] * (1. / TMath::Power(p[1] + TMath::Power(x,p[2]), p[3]) + p[4] * TMath::Exp(p[5]*x));
}

//-------------------------------------------------------------------------
Double_t YMuon( const Double_t *py, const Double_t */*dummy*/ )
{
  // muon y
  
  Double_t y = *py;
  //pol4 only valid in y= -4;-2.5
  // Float_t p0,p1,p2,p3;
  // p0 = 0.729545; //0.729545;
  // p1 = 0.53837; //0.53837;
  // p2 = 0.141776; //0.141776;
  // p3 = 0.0130173; //0.0130173;
  Double_t p[3] = {1.95551, -0.104761, 0.00311324};

  return p[0] * (1. + p[1]*y*y + p[2]*y*y*y*y);
}

//-------------------------------------------------------------------------
Double_t V2Muon( const Double_t */*dummy*/, const Double_t */*dummy*/ )
{
  //muon v2
  return 0.;
}

