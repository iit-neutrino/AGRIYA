////Macro to read fission fractions from a file and theoretical IBD 
#include <iostream>
#include <fstream>
#include <sstream>

#include "TVectorD.h"

#include "GlobalAnalyzer.hh"
#include "TMacroInterface.hh"
using namespace std;

static const vector<string> fitName={"U235 only","P239 only","U235+239","U235+239+238","Oscillation only","235 + Oscillation","239 + Oscillation","Eq","5 + Eq","9 + Eq","239 data linear"};

void macroHelp()
{
  printf("---------------------------------------------------------------------------\n");
  printf("Macro file should contain values for atleast the following keys:\n");
  printf("OUTPUTFILE, DATAFILE, COVARIANCEFILESTAT, COVARIANCEFILESYST, COVARIANCEFILETHEO\n");
  printf("Example 'macrofile.mac' file:\n");
  printf("OUTPUTFILE = outputFile.root\n");
  printf("DATAFILE = ./inputs/global.txt\n");
  printf("COVARIANCEFILESTAT = inputs/global_covstat.txt\n");
  printf("COVARIANCEFILESYST = inputs/global_covsyst.txt\n");
  printf("COVARIANCEFILETHEO = inputs/theo_arXiv_1703.00860.txt\n");
  printf("---------------------------------------------------------------------------\n");
  return;
}

void inputHelp()
{
  printf("Example: plotHypotheticalIBDYields macrofilename.mac\n");
}

void help()
{
  macroHelp();
  inputHelp();
  exit(1);
}

void macroUsage()
{
  printf("Incorrect macro file provided\n");
  macroHelp();
  exit(1);
}

void usage()
{
  printf("Incorrect inputs provided \n");
  inputHelp();
  exit(1);
}

int main(int argc, char *argv[])
{
  if(argc!=2) usage();
  TString macroInput(argv[1]);
  
  TString dataFileName;
  TString systCovFileName;
  TString statCovFileName;
  TString theoCovFileName;
  TString outputFileName;
  int fitType;

  TMacroInterface& macroInterface = TMacroInterface::Instance();
  if(macroInput.EqualTo("-h", TString::kIgnoreCase) ||
   macroInput.EqualTo("--h", TString::kIgnoreCase) || 
   macroInput.EqualTo("-help", TString::kIgnoreCase) || 
   macroInput.EqualTo("--help", TString::kIgnoreCase)) help();
  else macroInterface.Initialize(macroInput);

  if(!macroInterface.RetrieveValue("FITTYPE",fitType)) macroUsage();
  if(!macroInterface.RetrieveValue("DATAFILE",dataFileName)) macroUsage();
  if(!macroInterface.RetrieveValue("COVARIANCEFILESTAT",statCovFileName)) macroUsage();
  if(!macroInterface.RetrieveValue("COVARIANCEFILESYST",systCovFileName)) macroUsage();
  if(!macroInterface.RetrieveValue("COVARIANCEFILETHEO",theoCovFileName)) macroUsage();
  if(!macroInterface.RetrieveValue("OUTPUTFILE",outputFileName)) macroUsage();

  //Instantiate GlobalAnalyzer where data is read and saved for applying fits
  GlobalAnalyzer *globalAnalyzer= new GlobalAnalyzer();
  TString ffName;
  if(macroInterface.RetrieveValue("THEORETICALIBDYIELDSFILE",ffName)) 
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
