////////////////////////////////////////////////
// File to plot h5ograms
////////////////////////////////////////////////

#include "Global_Utilities.hh"
#include "TKey.h"
#include <TStyle.h>
#include "TLegend.h"
#include "TLegend.h"
#include "TObjString.h"
using namespace std;


void usage(){
  std::cout << "Example: Ploth5ograms file.root\n";
  exit(1);
}

int main(int argc, char *argv[]){
  
  if(argc!=2) usage();

	std::cout << argv[1]	<< std::endl;
  TString inputFileName=argv[1];
  TFile *inputFile=TFile::Open(inputFileName);
  
  TH1D* h5=(TH1D*)inputFile->Get("hChi2MapU235");
  TH1D* h8=(TH1D*)inputFile->Get("hChi2MapU238");
  TH1D* h9=(TH1D*)inputFile->Get("hChi2MapP239");
  TH1D* h1=(TH1D*)inputFile->Get("hChi2MapP241");
  
  int binLow=h5->FindFirstBinAbove(0.01);
  int binHigh=h5->FindLastBinAbove(0.01);
  double binLowValue=h5->GetBinCenter(binLow);
  double binHighValue=h5->GetBinCenter(binHigh);
  
  h5->GetXaxis()->SetRangeUser(binLowValue,binHighValue);
  std::cout << "Minimum chi2% = " << h5->GetMinimum() <<std::endl;
  
  int x,y,z;
  h5->GetMinimumBin(x,y,z);
  std::cout << "235 Bestfit value% = " << h5->GetBinCenter(x)/6.69<<std::endl;
  
  binLow=h8->FindFirstBinAbove(0.01);
  binHigh=h8->FindLastBinAbove(0.01);
  binLowValue=h8->GetBinCenter(binLow);
  binHighValue=h8->GetBinCenter(binHigh);
  h8->GetXaxis()->SetRangeUser(binLowValue,binHighValue);
  h8->GetMinimumBin(x,y,z);
  std::cout << "238 Bestfit value% = " << h8->GetBinCenter(x)/10.1<<std::endl;
  
  binLow=h9->FindFirstBinAbove(0.01);
  binHigh=h9->FindLastBinAbove(0.01);
  binLowValue=h9->GetBinCenter(binLow);
  binHighValue=h9->GetBinCenter(binHigh);
  h9->GetXaxis()->SetRangeUser(binLowValue,binHighValue);
  h9->GetMinimumBin(x,y,z);
  std::cout << "239 Bestfit value% = " << h9->GetBinCenter(x)/4.4<<std::endl;
  
	binLow=h1->FindFirstBinAbove(0.01);
  binHigh=h1->FindLastBinAbove(0.01);
  binLowValue=h1->GetBinCenter(binLow);
  binHighValue=h1->GetBinCenter(binHigh);
  h1->GetXaxis()->SetRangeUser(binLowValue,binHighValue);
  h1->GetMinimumBin(x,y,z);
  std::cout << "241 Bestfit value% = " << h1->GetBinCenter(x)/6.04<<std::endl;
  
  double sigma=GetNSigma(h5,1);
  //std::cout << "U235 Sigma = " << sigma <<std::endl;
  std::cout << "U235 Sigma% = " << sigma*100/6.046 <<std::endl;
  sigma=GetNSigma(h9,1);
  //std::cout << "P239 Sigma = " << sigma <<std::endl;
  std::cout << "P239 Sigma% = " << sigma*100/4.4 <<std::endl;
  sigma=GetNSigma(h8,1);
  //std::cout << "U238 Sigma = " << sigma <<std::endl;
  std::cout << "U238 Sigma% = " << sigma*100/10.1 <<std::endl;
  sigma=GetNSigma(h1,1);
  //std::cout << "P241 Sigma = " << sigma <<std::endl;
  std::cout << "P241 Sigma% = " << sigma*100/6.03 <<std::endl;
  
  return 0;
}
