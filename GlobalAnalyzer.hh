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


// Theoretical IBD yield values for individual isotopes
static const double kSigma235=6.69;
static const double kSigma238=10.10;
static const double kSigma239=4.40;
static const double kSigma240=4.96; 
static const double kSigma241=6.03;

/// The main fitter class
class GlobalAnalyzer: public ROOT::Math::IBaseFunctionMultiDim{
public:
  
  GlobalAnalyzer():ROOT::Math::IBaseFunctionMultiDim(){}

  //Defualt Destructor
  virtual ~GlobalAnalyzer(){}
  
  //==============================Member variables==================================//
private:

  /// @brief Number of Experiments in Data file
  ///
  /// Expanded text
  ///
  int fNumberofExp = 0;

  /// @brief Number of isotopes used in the analysis
  ///
  /// It is set as static constant value since it is never modified
  ///
  static const int fNumberofIso = 5;
  
  //TODO: Check if this number is correct

  /// @brief Number of fit parameters used
  ///
  /// This is required quantity in the #NDim method It is set as a constant value since it is never modified once defined
  /// TODO: Check if this number is correct.
  ///
  int numberofFitPars;

   ///Experimental IBD measurment
  TVectorD v_IBD_Exp;
  
  /// Fission fractions for U235
  TVectorD v_FF_235;

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
 
  /// Average fission fraction for P239 in case of a fit to 239
  double ff_239=0; 
  
  ///Experimental IBD measurment
  TGraphErrors g_IBD_Exp;
  
  ///Fit IBD measurment
  TGraphErrors g_IBD_Fit;

  /// ROOT functions describing the neutrino flux from each isotope
  TF1 *f235Flux;
  TF1 *f238Flux;
  TF1 *f239Flux;
  TF1 *f240Flux; 
  TF1 *f241Flux;

  /// ROOT functions describing the IBD corss-section
  TF1 *fIBDxSec;
  
  ///Theroretical IBD yield - Experimental IBD yield
  TVectorD v_Diff;
  
  /// Covariance matrix contaning the systematic uncertainty terms
  /// For experiments in Giunti's paper this are the total uncertainty covariance
  /// This is reduced uncertainty values that need to be multiplied by 
  /// theoretical yields to obtain full systematic covariance matrix
  TMatrixD fRedSystCovarianceMatrix = TMatrixD();
  
  /// Covariance matrix contaning the statistical uncertainty terms
  /// This will be empty for experiments from the Guinti papers list
  TMatrixD fStatCovarianceMatrix = TMatrixD();
  
  /// Covariance matrix contaning the uncertaintiy associated with IBD yields for each isotope
  /// TODO: Add reference of where it comes from
  TMatrixD fTheoCovarianceMatrix = TMatrixD();

  /// Reduced theoreical uncertainties and correlations read from a file
  /// when you multipy this with the corresponding yields you get #fTheoCovarianceMatrix
  TMatrixD fUncertainityMatrix = TMatrixD();
  
  // Input file
  TString fDataInput;
  
  // Covariance statistical matrix file
  TString fCovStatFileName;
  
  // Covariance statistical matrix file
  TString fCovSystFileName;
  
  // Theoretical uncertainty covariance matrix
  TString fTheoUncFileName;
  
  // Fit type to be used for fitting
  int fFitType=-1;

  //==============================Private member functions =============================//
private:
  
  // Implementation of the pure virtual method NDim in IFunction
  inline unsigned int NDim() const{
    return numberofFitPars;
  }
  
  // Implementation of the pure virtual method Clone in IFunction
  inline ROOT::Math::IBaseFunctionMultiDim* Clone() const{
    return new GlobalAnalyzer();
  }
  /// Evaluate the theoretical spectrum given fission yields and anti-nu energy
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuSpectrum(const double *xx,double energy) const;
  
  /// Evaluate the theoretical oscillation flux given fission yields, baseline of the detector and oscillation parameters
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuFlux(const double *xx,double baseline) const;

  /// Evaluates the theoretical IBD yield for all the experiments for
  /// a given IBD yield of U235, U238, Pu239, Pu241, s22theta and dm2 respectively and saves in
  /// a vector of the theoretical IBD yield.
  bool EvaluateTheoreticalIBDYield(const double *xx, TVectorD& yTheo) const;
  
  /// Evaluate covariancs matrix term, input variables are:
  /// first and second int objects are index refering to experiments
  /// third and fourth int objects are refering to yield for experiment i and j
  bool EvaluateCovarianceMatrix(const TVectorD &yTheo, TMatrixD &CovarianceMatrix) const;

    /// @brief  Check whether the file actually exists
  /// @return bool
  inline bool CheckFileExists(TString fDataInput) {
    struct stat buffer;   
    return (stat (fDataInput, &buffer) == 0); 
  }

  /// @brief  Check whether the file extension is correct
  /// @return bool
  inline bool CheckFileExtension(TString dataInput,TString extension){
    if(dataInput.EndsWith(extension)) return true;
    return false;
  }
  
  // FIll up theoreeticla cov matrix
  bool LoadTheoCovMat();
  
  /// Load map of fission fractions
  bool LoadFissionFractionMap();

  /// Reads a matrix given a specified file
  /// TODO: This could go in Utils
  bool ReadMatrix(TString fileName, TMatrixD &covMatrix);
  
  /// Loads the covariance matrices into respective TMatrixDs
  bool LoadCovarianceMatrices();

  bool EvaluateTheoDeltaVector(const double* xx, TVectorD &rValues) const;

  //==============================Public member functions =============================//
public:
    
  ///Read data from text file and store it in corresponding vectors
  bool ReadDataFromFile();
  
  /// @brief Intialize analyzer by pointing to the data and covariance matrix files and load data
  /// @param dataInput string pointing to the input data file  
  /// @param covStat string pointing to the stat covariance matrix file
  /// @param covSyst string pointing to the syst covariance matrix file
  /// @return true if no error
  bool InitializeAnalyzer(TString dataInput, TString covStat, TString covSyst, TString redCovTheo);
  
  /// Plot data points in the supplied output file
  bool DrawDataPoints(TFile &outFile);

  /// Plot fit points assuming linear fit
  bool DrawFitPoints(TFile &outFile, double intercept, double slope);
  
  /// Function that is used for minimization
  double DoEval(const double* xx)const;

  /// runns the functions in private to load info from Data.txt and the covariance matrixes.  
  bool SetupExperiments(int fitType); 
};

#endif
