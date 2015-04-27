//
//  MyMacro.c
//  
//
//  Created by Benjamin Audurier on 03/12/14.
//
//

// Macro loadded by Fit.sh

{
    
//    gROOT->ProcessLine(".x runProof.C");// Make the analysis (can be commented after done once)
    
    gROOT->ProcessLine(".L FittingFunctions.C+");// Load fit functions
    
    gROOT->ProcessLine(".L FitMacro.C+");// Load fit macro
    FitMacro();// Show fit



    return ;
    
}