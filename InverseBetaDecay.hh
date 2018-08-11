//// Name : InverseBetaDecay.hh
//// Desc : Class that defines function for importing data and calculating Chi2
//// Author : Yonas Gebre
//// Date : 2017/3

#ifndef INVERSEBETADECAY_HH
#define INVERSEBETADECAY_HH

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

/// ROOT imports
#include "TROOT.h"
#include "TApplication.h"
#include "TLegend.h"
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
#include "TRandom1.h"

////Class with the function used to calculate Chi2
class InverseBetaDecay{
  
public:
  int numberofExp = 0; // Number of Experiments in Data file
  int columnsA = 0; //Number of Columns in the Data text file
  double DataArray[100][100];  //Array used to colect information stored in data text file

  ///Seed that is used to generate random numbers
  
  int count = 0;
  
  ///defines function to take data from text file and store it in the array DataArray
  void DataInput();
  
  InverseBetaDecay(){	 //Default constructor
    
    ///The information from Data text file is read when the object is initialized
    DataInput();

     
  }
  
  //Defualt Destructor
  virtual ~InverseBetaDecay(){}
  
  static const int numberofIso = 4; //Number of isotopes considered in the analysis
  

  // Covariance matrix histogram
  TH2D* hCovariance;
  
  // Inverted covariance matrix histogram
  TH2D* hInvCovariance;
  
   ///Experimental IBD measurment
  TVectorD v_IBD_Exp;
  
  /// Calculates the theoretical IBD yield for all the experiments for
  /// a given IBD yield of U235, U238, Pu239 and Pu241 respectively and returns
  /// a vector of the theoretical IBD yield.
  //  TVectorD& TheorIBDCalculator(double &, double &, double &, double &);
  void TheorIBDCalculator(TVectorD&,const double &,const double &,const double &,const double &);
  
  /// Calculates the theoretical IBD yield for all the experiments for
  /// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
  /// a vector of the theoretical IBD yields.
  void TheorIBDCalculator(TVectorD&,const double &,const double &,const double &);
  
  /// Calculates the theoretical IBD yield for all the experiments for
  /// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
  /// a vector of the theoretical IBD yields.
  /// The last input is the fission fraction of 238 and 239 combined if this needs to be fit
  void TheorIBDCalculator(double,TVectorD&,const double &,const double &,const double &);
  
  /// Calculate covariancs matrix term, input variables are:
  /// first and second int objects are index refering to experiments
  /// third and fourth int objects are refering to yield for experiment i and j
  void CovarianceMatrixCalculator(TVectorD &,TMatrixD &);
  
  
  /// Calculate correlated errors
  void CalculateCorrelatedErrors(const TVectorD &, TMatrixD &);
  
  /// Chi2Calculator takes a vector contaning the theoretical IBDs of experiments
  /// and the a inverse covariance matrix and calculated and returns the Chi2 value
  double Chi2Calculator(TVectorD &, TMatrixD &);


  /// returns the r or IBD of the one sigma point
  double OneSigmaCorrValue(TH1D *, const double &, const double &, const int &);

  
  /// runns the functions in private to load info from Data.txt and the covariance matrixes.  
  void SetupExperiments(); 

    /// Adds stat and syst fluctuations to data
  void AddingFluctuation(const double &);
  
private:
  
  // Cross-section from Scaly-Huber for the four isotopes
  //double xSectionSH[numberofIso]={6.69,10.10,4.40,6.03};
  
  // Theoretical ratio of fission fractions for 239 vs 241
  // Taken from the Fit to DayaBay Data
  double F239F241Ratio=0.18;
  
  /// Fission fractions for U235
  TVectorD v_FF_235;
  
  /// Fission fractions for U238
  TVectorD v_FF_238;
  
  /// Fission fractions for P239
  TVectorD v_FF_239;
  
  /// Fission fractions for P241
  TVectorD v_FF_241;
  
  /// Fission fractions for P239-241 combined
  TVectorD v_FF_239241;
  
 
  
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
  
  /// Stores information about experiments from the array DataArray to vectors.
  void LoadingDataToVector();
  
  
  /// Load map of fission fractions
  void LoadFissionFractionMap();


  /// Reads the two covariance matrix files and stores them in
  /// Syst_CovarianceMatrix and Stat_CovarianceMatrix.
  void LoadCovarianceMatrix();
  
  
  
};

#endif
