//// Name : GlobalAnalyzer.hh
//// Desc : Class that defines function for importing data and calculating Chi2
//// Author : Yonas Gebre
//// Date : 2017/3
//// TODO: Add argrument names in the function declaration in the header file

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
#include <sys/stat.h>

/// ROOT imports
#include "TROOT.h"
#include "TApplication.h"
#include "TString.h"
#include "TF1.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "Math/IFunction.h"

/// The main fitter class
class GlobalAnalyzer: public ROOT::Math::IBaseFunctionMultiDim{
    
public:
  int numberofExp = 0; ///< Number of Experiments in Data file
  int columnsA = 0; ///< Number of Columns in the Data text file
  double DataArray[100][100];  ///< Array that stores the fission fractions,IBD yields, and distances extracted from the input text files
  
  ///defines function to take data from text file and store it in the array DataArray
  bool DataInput();
  
  GlobalAnalyzer():ROOT::Math::IBaseFunctionMultiDim(){}
  
  bool InitializeAnalyzer(TString, TString, TString);
  
  //Defualt Destructor
  virtual ~GlobalAnalyzer(){}
  
  static const int numberofIso = 5; //Number of isotopes considered in the analysis -- MOD (4->5)
  
  static const int numberofFitPars = 7; //Number of fit parameters considered in the analysis -- MOD (6->7)

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
  bool CalculateTheoreticalIBDYield(TVectorD&,const double *) const;
  
  /// Calculate covariancs matrix term, input variables are:
  /// first and second int objects are index refering to experiments
  /// third and fourth int objects are refering to yield for experiment i and j
  bool CalculateCovarianceMatrix(const TVectorD &,TMatrixD &) const;
  
  /// runns the functions in private to load info from Data.txt and the covariance matrixes.  
  bool SetupExperiments(int); 

  /// Plot data points in the supplied output file
  bool DrawDataPoints(TFile &);
  
  /// Plot fit points assuming linear fit
  bool DrawFitPoints(TFile &,double, double);
  
  /// Function that is used for minimization
  double DoEval(const double*)const;

  /// @brief  Check whether the file actually exists
  /// @return bool
  inline bool CheckFileExists(TString fDataInput) {
    struct stat buffer;   
    return (stat (fDataInput, &buffer) == 0); 
  }

  /// @brief  Check whether the file extension is correct
  /// @return bool
  bool CheckFileExtension(TString dataInput,TString extension);

private:
  
  // Implementation of the pure virtual method NDim in IFunction
  unsigned int NDim() const{
    return numberofFitPars;
  }
  
  // Implementation of the pure virtual method Clone in IFunction
  ROOT::Math::IBaseFunctionMultiDim* Clone() const{
    return new GlobalAnalyzer();
  }
  
  // Cross-section from Saclay-Huber for the four isotopes
  double xSectionSH[numberofIso];
  
  /// Fission fractions for U235
  TVectorD v_FF_235;

  /// Average fission fraction for P239 in case of a fit to 239
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
  bool LoadingDataToVector();
  
  // FIll up theoreeticla cov matrix
  bool LoadTheoCovMat();
  
  /// Load map of fission fractions
  bool LoadFissionFractionMap();
  
  bool CalculateTheoDeltaVector(const double *,TVectorD &) const;
  
  /// Reads the two covariance matrix files and stores them in
  /// Syst_CovarianceMatrix and Stat_CovarianceMatrix.
  bool LoadCovarianceMatrix();
  
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
