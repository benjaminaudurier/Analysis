#include "Riostream.h"
#include "TH1.h"
#include <vector>
#include "TMath.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TGraph.h"
#include "TMinuit.h"
#include "TCanvas.h"
#include "TFitResult.h"
#include "TStyle.h"
#include "TMatrixD.h"

//
// Macro to get the hadronic to inclusive Raa factor for a given pt cut
// The macro compute only the low limit with the following formula :
//
//  (Raa_h_ptcut - Raa_inc)/Raa_inc = (1/cutEff)*(Fpp*Raa_ptcut -Raa_inc)/Raa_inc
//
//  where :
//    - cutEff    = the efficiency of the cut based on STARLIGHT simulation
//    - Fpp       = crosssection_pp_ptcut/crosssection_pp_inc
//    - Raa_ptcut = value of the Raa for a given range after applying the cut
//    - Raa_inc   = value of the Raa for a given range without applying the cut
//

// Input file should have the format :
//
//  0-10 0.760 0.760
//  10-20 0.738 0.738
//  20-30 0.752 0.752
//  30-40 0.645 0.645
//  40-50 0.739 0.739
//  50-60 0.815 0.815
//  60-90 1.122 1.122
//
// where first column is the centrality range, second is the Raa without the pt cut, third is the Raa with the pt cut

namespace
{
  const double cutEff_2015            = 0.65;    // efficiency cut from 2015 STARLIGHT simulation

  //Fpp for a bin = cross-section for a bin with the pt cut  / cross-section of the bin without pt cut
  const double Fpp_5TeV_03_8          = 0.993;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_12         = 0.993;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_2          = 0.969;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_1          = 0.919;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_1_y_25_325 = 0.888;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_1_y_325_4  = 0.880;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_12_y_25_3  = 0.983;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_12_y_3_35  = 0.984;  // from PbPb@5TeV A.N :
  const double Fpp_5TeV_03_12_y_35_4  = 0.980;  // from PbPb@5TeV A.N :
}


//------------------------------------------------------------------
void SystPtCut(const char* input="RAA.txt")
{

  ifstream in(input);
  char line[1024];

  char bin[1024];
  float a,b,corr,corr_ptcut;
  float value,value_ptcut;

  // read extern file
  while (in.getline(line,1023,'\n'))
  {

    if (strstr(line, "//")) continue;

    sscanf(line,"%s %e-%e %e %e ",&bin,&a,&b,&value,&value_ptcut);

    printf("%s %.0f-%.0f  --  Raa_inc = %.3f -- Raa_inc_ptcut = %.3f \n",bin,a,b,value,value_ptcut);

    corr       =  ( 1./ cutEff_2015 ) * ((Fpp_5TeV_03_1_y_325_4*value_ptcut -value)/value);
    // corr       =  ( Fpp_5TeV_03_1_y_325_4 / (Fpp_5TeV_03_1_y_325_4 - 1 + cutEff_2015) ) * ((value_ptcut -value)/value);
    corr_ptcut = ( ( 1 - cutEff_2015 ) / ( Fpp_5TeV_03_1_y_325_4 * cutEff_2015 ) ) * ( ( Fpp_5TeV_03_1_y_325_4*value_ptcut - value ) / value_ptcut );

    printf("  -> Correction of Raa_inc       : %.1f %% (cut efficiency : %.02f %%)\n",corr*100,cutEff_2015);
    printf("  -> Correction of Raa_inc_ptcut : %.1f %% (cut efficiency : %.02f %%)\n\n",corr_ptcut*100,cutEff_2015);

  }
  std::cout << std::endl;
  return ;
}
