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
// Macro to get the prompt/non-prompt contribution from RAA distribution from an input file
//

//Input file should have the format :
//
//
//bin 0-1     117.697  117.697
//bin 1-2     242.073  242.073
//bin 2-3     207.527  207.527
//bin 3-4     138.534  138.534
//bin 4-5      81.585  81.585
//bin 5-6      44.827  44.827
//bin 6-8      17.842  17.842
//bin 8-15       2.419  2.419
//
// where  :
//  - bin = cent/pt/y
//  - x-y = range
//  - First column  : main values Raa
//  - Second column : Second values if one wants to compute ratio Raa_firstcol/Raa_second


// LHC_b cross-section from https://arxiv.org/pdf/1103.0423v2.pdf in nb
namespace
{
  const Double_t pt_lhcb[14] = {0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5,10.5,11.5,12.5,13.5};

  const Double_t cc_npromp_7TeV[3][14]      =
  {
    {75,147,140,98,57,35,22,12.1,8.2,5.2,3.2,2.2,1.6,0.9},    // y 2.5-3
    {60,123,113,75,44,26,14.9,9.4,5.3,3.4,2.0,1.5,0.9,0.6},   // y 3-3.5
    {41,82,71,48,28,15.6,8.6,5.5,3.2,1.8,1.2,0.6,0.3}         // y 3.5-4
  };

  const Double_t estatcc_npromp_7TeV[3][14] =
  {
    {4,4,3,2,1,1,1,0.5,0.4,0.3,0.2,0.2,0.2,0.1},  // y 2.5-3
    {2,3,2,2,1,1,0.6,0.4,0.3,0.2,0.2,0.2,0.1,0.1},// y 3-3.5
    {2,2,2,1,1,0.7,0.4,0.3,0.3,0.2,0.2,0.1,0.1}   // y 3.5-4
  };

  const Double_t esystcc_npromp_7TeV[3][14] =
  {
    {12,4,3,2,2,1,1,0.6,0.8,0.1,0.1,0.1,0.1,0.1}, // y 2.5-3
    {4,2,2,2,1,1,0.5,0.3,0.1,0.1,0.1,0.1,0.1,0.1},// y 3-3.5
    {2,2,2,1,1,0.7,0.3,0.2,0.1,0.1,0.1,0.1,0.1}   // y 3.5-4
  };

  // taking the non polarised value

  const Double_t cc_promp_7TeV[3][14]      =
  {
    {844,1490,1214,719,392,206,104,57,31,17.4,9.8,5.9,3.5,2.6}, // y 2.5-3
    {749,1376,1053,611,325,167,82,44,23,12.6,7.8,4.5,2.9,1.3},  // y 3-3.5
    {614,1101,839,471,244,119,59,29,15.9,8.2,4.9,2.6,1.2}       // y 3.5-4
  };

  const Double_t estatcc_promp_7TeV[3][14] =
  {
    {13,12,9,6,3,2,1,1,1,0.5,0.4,0.3,0.2,0.2},// y 2.5-3
    {7,8,7,5,3,2,1,1,1,0.4,0.3,0.3,0.2,0.2},  // y 3-3.5
    {6,7,6,4,3,2,1,1,0.5,0.4,0.3,0.2,0.1}     // y 3.5-4
  };

  const Double_t esystcc_promp_7TeV[3][14] =
  {
    {133,39,24,18,12,8,4,3,1,0.2,0.1,0.1,0.1,0.1},// y 2.5-3
    {46,26,19,14,9,5,3,1,1,0.1,0.1,0.1,0.1,0.1},  // y 3-3.5
    {23,23,19,13,7,5,2,1,0.1,0.1,0.1,0.1,0.1}     // y 3.5-4
  };

  // pt/y integrated fb
  const Double_t Fb276TeV = 7.7;
  const Double_t Fb5TeV   = 9.77;
  const Double_t Fb7TeV   = 10.67;

}

// range to integrate the function
const int xbinrange[2] = {0,11}; // 0 = 0-1 , 1 = 1-2 ... 13 = 13-14
const int ybinrange[2] = {0,2}; // 0 = 2.5-3 ; 1 = 3-3.5 ; 2 = 3.5-4

// Raa nprompt guess
// Watch out the ouputs !!!
const int nRaaprompt = 4 ;
Double_t Raanpromt[nRaaprompt] = {0,0.5,1,1.2} ;

// Scaling factor applyed to the nprompt/prompt ratio for LHC_b_7TeV
// For instance, if we look at 5TeV data : scaleFactor = Fb5TeV/Fb7TeV

double scaleFactor = Fb5TeV/Fb7TeV;
double scaleFactor2 = Fb276TeV/Fb7TeV;


Double_t Getfb(double scalerF);
Double_t Getfb_pty(TString* bin,double xmin, double xmax,double scalerF);


//------------------------------------------------------------------
void PrompNoPromptRAA(const char* input="RAA.txt")
{

  ifstream in(input);
  char line[1024];

  char bin[1024];
  float a,b;
  float value,value2;
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> y2;

  // read extern file
  std::cout << " --- file ---" <<std::endl;
  std::cout << std::endl;
  while (in.getline(line,1023,'\n'))
  {
    if (strstr(line, "//")) continue;

    sscanf(line,"%s %e-%e %e %e ",bin,&a,&b,&value,&value2);
    // printf("%s %.2e-%.2e %.2e %.2e \n",bin,a,b,value,value2);

    x.push_back(a);
    y.push_back(value);
    y2.push_back(value2);

  }
  std::cout << std::endl;

  gStyle->SetOptFit(1);
  x.push_back(b);

  TString sbin(bin);

 // init Raapro
  Double_t Raapro[nRaaprompt][x.size()];
  for (int i = 0; i < x.size(); ++i)
    for (int j = 0; j < nRaaprompt; ++j)
      Raapro[j][i]=0.;

  Double_t Raapro2[nRaaprompt][x.size()];
  for (int i = 0; i < x.size(); ++i)
    for (int j = 0; j < nRaaprompt; ++j)
      Raapro2[j][i]=0.;


  // init fd
  Double_t fb[x.size()];
  Double_t fb2[x.size()];
  for (int i = 0; i < x.size(); ++i)
  {
      fb[i]=0.;
      fb2[i]=0.;
      if(sbin.Contains("cent")&& y[i]!=0.)
        fb[i] = Getfb(scaleFactor);
      else if( (sbin.Contains("pt") || sbin.Contains("y")) && y[i]!=0.  )
        fb[i] = Getfb_pty(&sbin,x[i],x[i+1],scaleFactor);

      if(sbin.Contains("cent") && y2[i]!=0.)
        fb2[i] = Getfb(scaleFactor2);
      else if( (sbin.Contains("pt") || sbin.Contains("y")) && y2[i]!=0. )
        fb2[i] = Getfb_pty(&sbin,x[i],x[i+1],scaleFactor2);
  }

  // Compute Raapro
  for (int i = 0; i < x.size() ; ++i)
  {
    Double_t Raa  = y[i];
    Double_t Raa2 = y2[i];

    for (int j = 0; j < nRaaprompt; ++j)
    {
       if (fb[i]!=0.) Raapro[j][i] = Raa + Raa*fb[i] - Raanpromt[j]*fb[i];
       if (fb2[i]!=0.) Raapro2[j][i] = Raa2 + Raa2*fb2[i] - Raanpromt[j]*fb2[i];
    }
  }

  printf("==================\n");
  printf("====  Results  for %d <pt <%d and %d < y <%d ===\n",xbinrange[0],xbinrange[1]+1,ybinrange[0],ybinrange[1]+1);
  printf("==================\n");
  printf("\n");
  printf("Raa\n");
  printf("\n");
  printf("bin  --  Raa_inc  -- Fb --  Raa_npromt =0 -- Raa_npromt=0.5 -- Raa_npromt= 1 -- Raa_npromt = 1.2 \n");

  for (int i = 0; i < x.size()-1; ++i)
  {
    Double_t variation0 = (Raapro[0][i] - y[i] )*100/y[i];
    Double_t variation1 = (Raapro[1][i] - y[i] )*100/y[i];
    Double_t variation2 = (Raapro[2][i] - y[i] )*100/y[i];
    Double_t variation3 = (Raapro[3][i] - y[i] )*100/y[i];

    printf(" %.2f-%.2f  &  %.3f &  %0.4f &  %0.2f \\%% &  %0.2f \\%% & %0.2f \\%%&  %0.2f \\%% \\\\\n",x[i],x[i+1],y[i],fb[i]/(1+fb[i]),variation0,variation1,variation2,variation3);

  }

  printf("\n");
  printf("Ratio\n");
  printf("\n");
  printf("bin  --  r_inc  -- Fb (5TeV) -- Fb (2,76TeV) --  Raa_npromt =0 -- Raa_npromt=0.5 -- Raa_npromt= 1 -- Raa_npromt = 1.2 \n");


  for (int i = 0; i < x.size()-1; ++i)
  {
    Double_t r_inc = y[i]/y2[i];
    Double_t r_pro0 = Raapro[0][i]/Raapro2[0][i];
    Double_t r_pro1 = Raapro[1][i]/Raapro2[1][i];
    Double_t r_pro2 = Raapro[2][i]/Raapro2[2][i];
    Double_t r_pro3 = Raapro[3][i]/Raapro2[3][i];


    Double_t variation0 = (r_pro0 - r_inc )*100/(r_inc);
    Double_t variation1 = (r_pro1 - r_inc )*100/(r_inc);
    Double_t variation2 = (r_pro2 - r_inc )*100/(r_inc);
    Double_t variation3 = (r_pro3 - r_inc )*100/(r_inc);

    printf(" %.2f-%.2f  &  %.3f &  %0.4f & %0.4f &  %0.2f \\%% &  %0.2f \\%% & %0.2f \\%%&  %0.2f \\%% \\\\\n",x[i],x[i+1],r_inc,fb[i]/(1+fb[i]),fb2[i]/(1+fb2[i]),variation0,variation1,variation2,variation3);

  }

  return;

}

//------------------------------------------------------------------
Double_t Getfb(double scalerF)
{
  // evaluate fb as the mean in case of centrality

  double fb               =0.;
  double cc_prompt_tot    =0.;
  double cc_nonprompt_tot =0.;

  //protection
  if(xbinrange[0] < 0 ||  xbinrange[1] > 13){
    printf(" range in x should be between 0 and 13 \n");
    return -1;
  }

  // loop over pt
  for (int i = xbinrange[0]; i < xbinrange[1] + 1; ++i)
  {
    // protection
    if(ybinrange[0] < 0 ||  ybinrange[1] > 2 ){
      printf(" range in y should be between 0 and 2 \n");
      return -1;
    }

    // loop over pt
    for (int j = ybinrange[0]; j < ybinrange[1] + 1; ++j)
    {
      cc_prompt_tot    += cc_promp_7TeV[j][i];
      printf("cc_promp_7TeV[%d][%d] = %.1f\n",j,i,cc_promp_7TeV[j][i] );
      cc_nonprompt_tot += cc_npromp_7TeV[j][i];
      printf("cc_npromp_7TeV[%d][%d] = %.1f\n\n",j,i,cc_npromp_7TeV[j][i] );
    }
  }
  printf(" --- cc_prompt_tot = %.4f\n",cc_prompt_tot );
  printf(" --- cc_nonprompt_tot = %.4f\n\n\n",cc_nonprompt_tot );

  fb = scalerF*(cc_nonprompt_tot/cc_prompt_tot);
  return fb;

}

//------------------------------------------------------------------
Double_t Getfb_pty(TString* bin,double xmin, double xmax,double scalerF )
{
  // evaluate fb as the mean between xmin and xmax bins

  double fb               =0.;
  double cc_prompt_tot    =0.;
  double cc_nonprompt_tot =0.;

  if(bin->Contains("pt"))
  {
    int x1 = xmin;
    int x2 = xmax;
    int dx   = x2-x1;
    printf("x1 = %d\n",x1 );
    printf("x2 = %d\n",x2 );
    printf("dx = %d\n",dx );

    // protection
    if(ybinrange[0] < 0 ||  ybinrange[1] > 2 ){
      printf(" range in y should be between 0 and 2 \n");
      return -1;
    }
    // loop over y
    for (int j = ybinrange[0]; j < ybinrange[1] + 1; ++j)
    {
      // in case a bin is bigger than 1
      for (int k = 0; k < dx; ++k)
      {
        cc_prompt_tot    += cc_promp_7TeV[j][x1+k];
        printf("cc_promp_7TeV[%d][%d] = %.1f\n",j,x1+k,cc_promp_7TeV[j][x1+k] );
        cc_nonprompt_tot += cc_npromp_7TeV[j][x1+k];
        printf("cc_npromp_7TeV[%d][%d] = %.1f\n\n",j,x1+k,cc_npromp_7TeV[j][x1+k] );
      }
    }
    printf("--- cc_prompt_tot = %.4f\n",cc_prompt_tot);
    printf("---- cc_nonprompt_tot = %.4f\n\n",cc_nonprompt_tot);
    fb = scalerF*(cc_nonprompt_tot/cc_prompt_tot);
    printf("fb = %f \n", fb);
    return fb;
  }
  else if(bin->Contains("y"))
  {

    // Convert rapidty bin into int for the table
    int y1 = 0;
    if(xmin >= 3.0 ) y1 = 1;
    if(xmin >= 3.5 ) y1 = 2;

    int             y2 = 1;
    if(xmax > 3.0 ) y2 = 2;
    if(xmax > 3.5 ) y2 = 3;

    int dy   = y2-y1;
    //
    // printf("xmin = %f\n",xmin );
    // printf("xmax = %f\n",xmax );
    // printf("y1 = %d\n",y1 );
    // printf("y2 = %d\n",y2 );
    // printf("dy = %d\n",dy );

    // protection
    if(xbinrange[0] < 0 ||  xbinrange[1] > 13 ){
      printf(" range in y should be between 0 and 13 \n");
      return -1;
    }
    // loop over pt
    for (int j = xbinrange[0]; j < xbinrange[1] + 1; ++j)
    {
      // in case a bin is bigger than 1
      for (int k = 0; k < dy; ++k)
      {
        cc_prompt_tot    += cc_promp_7TeV[y1+k][j];
        printf("cc_promp_7TeV[%d][%d] = %.1f\n",y1+k,j,cc_promp_7TeV[y1+k][j] );
        cc_nonprompt_tot += cc_npromp_7TeV[y1+k][j];
        printf("cc_npromp_7TeV[%d][%d] = %.1f\n\n",y1+k,j,cc_npromp_7TeV[y1+k][j] );
      }
    }
    printf("--- cc_prompt_tot = %.4f\n",cc_prompt_tot);
    printf("---- cc_nonprompt_tot = %.4f\n\n",cc_nonprompt_tot);
    fb = scalerF*(cc_nonprompt_tot/cc_prompt_tot);
    printf("fb = %f \n", fb);
    return fb;
  }

}
