//// Name : GlobalAnalyzer.hh
//// Desc : Class that defines function for importing data and calculating Chi2
//// Author : Yonas Gebre
//// Date : 2017/3

#ifndef GLOBALANALYZER_HH
#define GLOBALANALYZER_HH

// Standard cpp library imports
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <map> 
#include <vector>
#include <math.h>

/// ROOT imports
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TError.h"
#include "TCanvas.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TMatrixD.h"
#include "TDecompLU.h"
#include "TArrayD.h"
#include "TDecompSVD.h"
#include "TVectorD.h"
// #include "TMatrixTLazy.h"
#include "TFile.h"
// #include "TGraph.h"
#include "TGraphErrors.h"
// #include "TGraph2D.h"
#include "TRandom1.h"
// #include "TVector.h"
#include "TMath.h"
#include "Math/IFunction.h"

////Class with the function used to calculate Chi2
class GlobalAnalyzer: public ROOT::Math::IBaseFunctionMultiDim{
    
public:
  int numberofExp = 0; // Number of Experiments in Data file
  int columnsA = 0; //Number of Columns in the Data text file
  double DataArray[100][100];  //Array used to colect information stored in data text file

  ///Seed that is used to generate random numbers
  /*double seed = 9;*/
  int count = 0;
  
  ///defines function to take data from text file and store it in the array DataArray
  void DataInput();
  
  GlobalAnalyzer():ROOT::Math::IBaseFunctionMultiDim(){}
  
  void InitializeAnalyzer(TString, TString, TString);
  
  //Defualt Destructor
  virtual ~GlobalAnalyzer(){}
  
  static const int numberofIso = 5; //Number of isotopes considered in the analysis -- MOD (4->5)
  
  static const int numberofFitPars = 7; //Number of fit parameters considered in the analysis -- MOD (6->7)

  // Covariance matrix histogram
  TH2D* hCovariance;
  
  // Inverted covariance matrix histogram
  TH2D* hInvCovariance;
  
   ///Experimental IBD measurment
  TVectorD v_IBD_Exp;
  
  ///Experimental IBD measurment
  TGraphErrors *g_IBD_Exp;
  
  ///Fit IBD measurment
  TGraphErrors *g_IBD_Fit;
  
  /// vector to store temp IBD vector
  TVectorD v_IBD_Exp_temp;

  /// Calculate the theoretical spectrum given fission yields and anti-nu energy
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuSpectrum(const double *,double) const;
  
  /// Calculate the theoretical oscillation flux given fission yields, baseline of the detector and oscillation parameters
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuFlux(const double *,double) const;

  /// Calculates the theoretical IBD yield for all the experiments for
  /// a given IBD yield of U235, U238, Pu239, Pu241, s22theta and dm2 respectively and saves in
  /// a vector of the theoretical IBD yield.
  void CalculateTheoreticalIBDYield(TVectorD&,const double *) const;
  
  /// Calculate covariancs matrix term, input variables are:
  /// first and second int objects are index refering to experiments
  /// third and fourth int objects are refering to yield for experiment i and j
  void CalculateCovarianceMatrix(const TVectorD &,TMatrixD &) const;

  /// returns the r or IBD of the one sigma point
  double OneSigmaCorrValue(TH1D *, const double &, const double &, const int &);
  
  /// runns the functions in private to load info from Data.txt and the covariance matrixes.  
  void SetupExperiments(int); 
  
  /// Plot data points in the supplied output file
  void DrawDataPoints(TFile &);
  
  /// Plot fit points assuming linear fit
  void DrawFitPoints(TFile &,double, double);
  
  double DoEval(const double*)const;

  // Boolean checking for presence of P240 in data file
  bool CheckUnity();
  
private:
  
  
  unsigned int NDim() const{
    return numberofFitPars;
  }
  
  ROOT::Math::IBaseFunctionMultiDim* Clone() const{
    return new GlobalAnalyzer();
  }
  
  // Cross-section from Saclay-Huber for the four isotopes
  double xSectionSH[numberofIso];
  
  // Theoretical ratio of fission fractions for 239 vs 241
  // Taken from the Fit to DayaBay Data
  double F239F241Ratio=0.18;
  
  /// Fission fractions for U235
  TVectorD v_FF_235;

  /// Average fission fraction for P239
  double ff_239=0;
  
  /// Fission fractions for U238
  TVectorD v_FF_238;
  
  /// Fission fractions for P239
  TVectorD v_FF_239;

  /// Fission fractions for P240 -- ADD
  TVectorD v_FF_240;
  
  /// Fission fractions for P241
  TVectorD v_FF_241;
  
  /// Baselines of the experiments
  TVectorD v_Baseline;
  
  /// Fission fractions for P239-241 combined
  TVectorD v_FF_239241;
 
  /// ROOT functions describing the yield from each isotope
  TF1 *f235Yield;
  TF1 *f238Yield;
  TF1 *f239Yield;
  TF1 *f240Yield; // ADD
  TF1 *f241Yield;
  /// ROOT functions describing the IBD corss-section
  TF1 *fIBDxSec;
  
  ///Theroretical IBD yield - Experimental IBD yield
  TVectorD v_Diff;
  
  /// Fission fractions map of vectors
  std::map<int,TVectorD> v_FissionFraction;
  
  
  /// Theorretical IBD yields
//  TVectorD yTheo;
  
  
  /// Covariance matrix contaning the systematic uncertainty terms
  /// For experiments in Guntiy's paper this are the total uncertainty covariance
  TMatrixD Syst_CovarianceMatrix = TMatrixD();
  
  /// Covariance matrix contaning the statistical uncertainty terms
  /// This will be empty for experiments from the Guiny papers list
  TMatrixD Stat_CovarianceMatrix = TMatrixD();
  
  
  /// Covariance matrix contaning the statistical uncertainty terms
  /// This will be empty for experiments from the Guiny papers list
  TMatrixD Theo_CovarianceMatrix = TMatrixD();
  
  /// Stores information about experiments from the array DataArray to vectors.
  void LoadingDataToVector();
  
  // FIll up theoreeticla cov matrix
  void LoadTheoCovMat();
  
  /// Load map of fission fractions
  void LoadFissionFractionMap();
  
  void CalculateTheoDeltaVector(const double *,TVectorD &) const;
  
  /// Reads the two covariance matrix files and stores them in
  /// Syst_CovarianceMatrix and Stat_CovarianceMatrix.
  void LoadCovarianceMatrix();
  
  // Input file
  TString fDataInput;
  
  // Covariance statistical matrix file
  TString fCovStat;
  
  // Covariance statistical matrix file
  TString fCovSyst;
  
  // Fit type to be used for fitting
  int fFitType=-1;

};

#endif
