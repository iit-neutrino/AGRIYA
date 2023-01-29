#include <fstream>
#include <iostream>

#include <TH2.h>
#include <TGraph.h>
#include <TMath.h>

using namespace std;

/// @brief Translate the TGraph to 0
/// @param gIn input graph 
/// @param minChi2 minimum chi2
void MinimizeToZero(TGraph &gIn, double minChi2);

/// @brief Convert absolute to relative graph
/// @param gIn 
/// @param gOut 
/// @param Scaling 
void ConvertAbsoluteToRelativeGraph(const TGraph &gIn, TGraph &gOut, double Scaling);

/// @brief Convert covariance matrix to uncertainty; sign(element)*sqrt(abs(element))/yield
/// @param h input histogram
/// @param yields 
/// @return true if converted, false otherwise
bool ConvertCovarianceToUncertainty(TH2D &h, std::vector<double> yields);

/// @brief Write TH2D to a text file
/// @param h input histogram
/// @param filename to be written to
/// @return true if written, false otherwise
bool PrintMatrixToTextFile(const TH2D &h, const TString fName);