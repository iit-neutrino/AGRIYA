//// Name : GlobalAnalyzer.hh
//// Desc : Class that defines function for importing data and calculating Chi2
//// Author : Initially Yonas Gebre, majorly modified by Pranava Teja Surukuchi and Yoshinobu Fujikake
//// Date : 2017/3

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
#include "TMultiGraph.h"

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

  /// Theoretical IBD yield values for U235 from arxiv.org/abs/1703.00860
  double fSigma235=6.69;
  /// Theoretical IBD yield values for U238 from arxiv.org/abs/1703.00860
  double fSigma238=10.10;
  /// Theoretical IBD yield values for Pu239 from arxiv.org/abs/1703.00860
  double fSigma239=4.40;
  /// Theoretical IBD yield values for Pu240 from arxiv.org/abs/1703.00860
  double fSigma240=4.96; 
  /// Theoretical IBD yield values for Pu241 from arxiv.org/abs/1703.00860
  double fSigma241=6.03;

  /// @brief Number of fit parameters used
  ///
  /// This is required quantity in the #NDim method It is set as a constant value since it is never modified once defined
  ///
  int fNumberofFitPars;

   /// Experimental IBD yield measurment
  TVectorD fVIBDExp;
  
  /// @brief Fission fractions of U235
  TVectorD fVFF235;

  /// @brief Fission fractions of U238
  TVectorD fVFF238;
  
  /// @brief Fission fractions of Pu239
  TVectorD fVFF239;

  /// @brief Fission fractions of Pu240
  TVectorD fVFF240;
  
  /// @brief Fission fractions of Pu241
  TVectorD fVFF241;
  
  /// @brief Vector with baselies of the experiments
  TVectorD fVBaseline;
 
  /// Average fission fraction for Pu239 in case of a fit to 239
  double fFF239=0; 
  
  /// @brief Graphs to store xperimental IBD measurement
  TGraphErrors fGIBDExp;
  
  /// @brief Graphs to store IBD fit
  TGraphErrors fGIBDFit;

  /// ROOT functions describing the neutrino flux from 235
  TF1 *f235Flux;
  /// ROOT functions describing the neutrino flux from 238
  TF1 *f238Flux;
  /// ROOT functions describing the neutrino flux from 239
  TF1 *f239Flux;
  /// ROOT functions describing the neutrino flux from 240
  TF1 *f240Flux; 
  /// ROOT functions describing the neutrino flux from 241
  TF1 *f241Flux;

  /// @brief ROOT function describing the IBD cross-section
  TF1 *fIBDxSec;
  
  /// @brief Theroretical IBD yield - Experimental IBD yield
  TVectorD fVDiff;
  
  /// @brief Covariance matrix contaning the systematic uncertainty terms
  /// This is reduced uncertainty values that need to be multiplied by 
  /// theoretical yields to obtain full systematic covariance matrix
  TMatrixD fRedSystCovarianceMatrix = TMatrixD();
  
  /// @brief Covariance matrix contaning the statistical uncertainty terms
  /// This will be empty for old experiments where all the uncertainties are treated as systematic uncertainties 
  TMatrixD fStatCovarianceMatrix = TMatrixD();
  
  /// @brief Covariance matrix contaning the uncertaintiy associated with IBD yields for each isotope
  TMatrixD fTheoCovarianceMatrix = TMatrixD();

  /// @brief Reduced theoreical uncertainties and correlations read from a file using #ReadMatrix
  /// when you multipy this with the corresponding yields you get #fTheoCovarianceMatrix
  TMatrixD fUncertainityMatrix = TMatrixD();
  
  /// @brief Input file name containing measured fissions fractions, IBD yields and distance of the experiment
  /// Each row corresponds either to a single experiment or to a data point for an experiment with evolution data like Daya Bay or RENO
  TString fDataInput;
  
  /// @brief Statistical covariance matrix file
  TString fCovStatFileName;
  
  /// @brief Systematic covariance matrix file name; really only contains reduced systematic uncertainties
  TString fCovSystFileName;
  
  /// @brief Theoretical uncertainty covariance matrix
  TString fTheoUncFileName;
  
  /// @brief  Fit type to be used for fitting
  int fFitType=-1;

  //==============================Private member functions =============================//
private:
  
  /// Implementation of the pure virtual method NDim in IFunction
  inline unsigned int NDim() const{
    return fNumberofFitPars;
  }
  
  /// Implementation of the pure virtual method Clone in IFunction
  inline ROOT::Math::IBaseFunctionMultiDim* Clone() const{
    return new GlobalAnalyzer();
  }
  /// Evaluate the theoretical spectrum given fission yields and anti-nu energy
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuSpectrum(const double *xx,double energy) const;
  
  /// Evaluate the theoretical oscillation flux given fission yields, baseline of the detector and oscillation parameters
  /// the first argument has fission fractions as well as the two additional fit parameters corresponding to the s22t and dm2
  double EstimateAntiNuFlux(const double *xx,double baseline) const;
  
  /// @brief Evaluate covariancs matrix term
  /// @param yTheo theoretical yields do be multipled with the systematic covariance matrix
  /// @param CovarianceMatrix TMatrixD where the covariance matrix is stored
  /// @return true if successful
  bool EvaluateCovarianceMatrix(const TVectorD &yTheo, TMatrixD &CovarianceMatrix) const;

    /// @brief  Check whether the file actually exists
  /// @return bool
  inline bool CheckFileExists(TString fDataInput) 
  {
    struct stat buffer;   
    return (stat (fDataInput, &buffer) == 0); 
  }

  /// @brief  Check whether the file extension is correct
  /// @return bool
  inline bool CheckFileExtension(TString dataInput,TString extension) const{
    if(dataInput.EndsWith(extension)) return true;
    return false;
  }
  
  /// @brief Fill the theoretical cov matrix based on the values read by #ReadMatrix for theoretical uncertainties from input file 
  /// @return true if successful
  bool LoadTheoCovMat();

  /// Load map of fluxes
  bool LoadFluxes();

  /// @brief Reads a matrix given a specified file
  /// @param fileName containing the matrix
  /// @param covMatrix reference to the TMatrixD object where the matrix is saved
  /// @return true if successful
  bool ReadMatrix(TString fileName, TMatrixD &covMatrix);
  
  /// @brief Loads the covariance matrices into respective TMatrixDs
  /// @return true if successful
  bool LoadCovarianceMatrices();

  /// @brief Evaluate the yield_iso - theoretica yield_iso for a given isotope
  /// @param xx fit parameters
  /// @param rValues reference to the vector where the delta values are actually stored
  /// @return true if succesfful
  bool EvaluateTheoDeltaVector(const double* xx, TVectorD &rValues) const;
    
  /// @brief Evaluates the theoretical IBD yield for all the experiments for
  /// given IBD yields of U235, U238, Pu239, Pu241 as well as oscillation parameters s22theta and dm2
  /// and saves in a vector
  /// @param xx 
  /// @param yTheo 
  /// @return true if successful
  bool EvaluateTheoreticalIBDYield(const double *xx, TVectorD& yTheo) const;

  //==============================Public member functions =============================//
public:
    
  /// @brief Plots the theoretical IBD yields based on the experimental fission fractions
  /// and saves in a TMultiGraphFile
  /// @param xx 
  /// @param TMultiGraph 
  /// @return true if successful
  bool PlotTheoreticalIBDYields(const TVectorD &xx, TFile &outFile) const;

  /// @brief Plots the theoretical IBD yields based on the experimental fission fractions
  /// and saves in a TMultiGraphFile
  /// @param xx 
  /// @param TMultiGraph 
  /// @return true if successful
  bool PlotTheoreticalIBDYields(const double *xx, TFile &outFile) const;

  /// @brief Read data from text file and store it in corresponding vectors
  /// @return true if successful
  bool ReadDataFromFile();
  
  /// @brief  Load map of theoretical yields from text file 
  /// @param fileName 
  /// @return true if successful
  bool ReadTheoreticalIBDYields(TString fileName);
  
  /// @brief Intialize analyzer by pointing to the data and covariance matrix files and load data
  /// @param dataInput string pointing to the input data file  
  /// @param covStat string pointing to the stat covariance matrix file
  /// @param covSyst string pointing to the syst covariance matrix file
  /// @return true if no error
  bool InitializeAnalyzer(TString dataInput, TString covStat, TString covSyst, TString redCovTheo);
  
  /// @brief Plot data points in the supplied output file
  /// @param outFile to write to
  /// @return true if successful
  bool DrawDataPoints(TFile &outFile);

  /// @brief  Plot fit points assuming linear fit
  /// @param outFile to write the graph to  
  /// @param intercept of the fit line
  /// @param slope slope of the fit line
  /// @return true if successful
  bool DrawFitPoints(double intercept, double slope, TFile &outFile);
  
  /// @brief  Function that is used for minimization
  /// @param fit parameters
  /// @return chi2 value from the fit
  double DoEval(const double* xx)const;

  /// @brief Read files, load the data to relevant data objects to be ready to perfor fits
  /// @param fitType 
  /// @return true if successful 
  bool SetupExperiments(int fitType); 

  /// @brief  getter for 235 IBD yield
  /// @return #fSigma235
  inline double GetSigma235() {return fSigma235; }

  /// @brief  getter for 238 IBD yield
  /// @return #fSigma238
  inline double GetSigma238() {return fSigma238; }

  /// @brief  getter for 239 IBD yield
  /// @return #fSigma239
  inline double GetSigma239() {return fSigma239; }

  /// @brief  getter for 240 IBD yield
  /// @return #fSigma240
  inline double GetSigma240() {return fSigma240; }

  /// @brief  getter for 241 IBD yield
  /// @return #fSigma241
  inline double GetSigma241() {return fSigma241; }

  /// @brief  getter for all the theoretical IBD yields
  /// @return #fSigma241
  inline bool GetAllSigma(TVectorD &sigma) { 
    sigma.ResizeTo(fNumberofIso);
    sigma[0]=fSigma235; 
    sigma[1]=fSigma238; 
    sigma[2]=fSigma239; 
    sigma[3]=fSigma240; 
    sigma[4]=fSigma241; 
    return true; }

  /// @brief  getter for the number of experiments
  /// @return #fNumberofExp
  inline double GetNExperiments() {return fNumberofExp; }

  /// @brief  getter for the number of isotopes
  /// @return #fNumberofIso
  inline double GetNIsotopes() {return fNumberofIso; }
};

#endif
