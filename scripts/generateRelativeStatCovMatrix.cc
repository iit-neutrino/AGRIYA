////Macro to generate a statistical covariance matrix
#include <iostream>
#include <fstream>
#include <sstream>

#include "TString.h"

#include "TCFGInterface.hh"
#include "GlobalAnalyzer.hh"

using namespace std;

void usage()
{
  printf("Incorrect inputs provided \n");
  inputHelp();
  exit(1);
}

void CFGHelp()
{
  printf("---------------------------------------------------------------------------\n");
  printf("CFG file should contain values for atleast the following keys:\n");
  printf("OUTPUTFILE, DATAFILE, COVARIANCEFILESTAT, COVARIANCEFILESYST, COVARIANCEFILETHEO\n");
  printf("Example 'CFGfile.cfg' file:\n");
  printf("OUTPUTFILE = outputFile.root\n");
  printf("DATAFILE = ./inputs/global.txt\n");
  printf("COVARIANCEFILESTAT = inputs/global_covstat.txt\n");
  printf("COVARIANCEFILESYST = inputs/global_covsyst.txt\n");
  printf("COVARIANCEFILETHEO = inputs/theo_arXiv_1703.00860.txt\n");
  printf("---------------------------------------------------------------------------\n");
  return;
}

void CFGUsage()
{
  printf("Incorrect CFG file provided\n");
  CFGHelp();
  exit(1);
}

int main(int argc, char *argv[])
{
  if(argc!=2) usage();
  TString CFGInput(argv[1]);
  
  TString dataFileName;
  TString systCovFileName;
  TString statCovFileName;
  TString theoCovFileName;
  TString outputFileName;
  int fitType;

  TCFGInterface& CFGInterface = TCFGInterface::Instance();
  else CFGInterface.Initialize(CFGInput);

  if(!CFGInterface.RetrieveValue("NEventsPerDay",nEventsPerDay)) CFGUsage();
  if(!CFGInterface.RetrieveValue("NSignalDays",nSignalDays)) CFGUsage();
  if(!CFGInterface.RetrieveValue("NBinsPerDataSet",nBinsPerDataSet)) CFGUsage();
  if(!CFGInterface.RetrieveValue("DATAFILE",dataFileName)) CFGUsage();
  if(!CFGInterface.RetrieveValue("",)) CFGUsage();
  
  //Instantiate GlobalAnalyzer where data is read and saved for applying fits
  GlobalAnalyzer *globalAnalyzer= new GlobalAnalyzer();
  TString ffName;
  if(CFGInterface.RetrieveValue("THEORETICALIBDYIELDSFILE",ffName)) 
  {
    if(!globalAnalyzer->ReadTheoreticalIBDYields(ffName)) exit(-1);
  }

  //Initialize GlobalAnalyzer and read 
  if(!globalAnalyzer->InitializeAnalyzer(dataFileName, statCovFileName, systCovFileName, theoCovFileName)) 
  {
    printf("Couldn't initialize analyzer \n Exiting \n");
    exit(-1);
  }

  if(!globalAnalyzer->SetupExperiments(1))
  {
    printf("Couldn't Setup experiments \n Exiting \n");
    exit(-1);
  }
  
  //Create output ROOT file for saving plots
  TFile *outputFile=new TFile(outputFileName,"RECREATE");
  
  TVectorD sigma;
  globalAnalyzer->GetAllSigma(sigma);

  globalAnalyzer->PlotTheoreticalIBDYields(sigma, *outputFile);
  outputFile->Close();
  
  return 0;
}
