////////////////////////////////
// Fitting Daya Bay data
// Pranava Teja Surukuchi, March 2017
/////////////////////////////////
#ifndef GLOBALUTILITIES_HH
#define GLOBALUTILITIES_HH

// Standard cpp library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <math.h>
#include "time.h"

// ROOT imports
#include "TROOT.h"
#include "TError.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMatrixD.h"
#include "TDecompLU.h"
#include "TArrayD.h"
#include "TDecompSVD.h"
#include "TVectorD.h"
#include "TMatrixTLazy.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TObjArray.h"
#include "TColor.h"

// Function to return n sigma value for a given chi2 1D histogram
double GetNSigma(TH1D*,int);

TH1D *ShiftChi2PlotToZero(TH1D*);
#endif
