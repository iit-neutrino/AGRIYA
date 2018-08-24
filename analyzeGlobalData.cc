#include "GlobalAnalyzer.hh"
#include <math.h>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
using namespace std;

void usage(){
  printf("Example: analyzeGlobalData outputFileName inputFileName covStat covSyst fitype\n");
  printf("Fit type can be from 1-8:\n 1 = U235 only \n 2 = U239 only \n 3 = U235+239 fit \n 4 = U235+239+238 fit \n 5 = OSC only \n 6 = 235+OSC only \n 7 = 239+OSC only  \n 8 = Eq \n");
  
  exit(1);
}

int main(int argc, char *argv[]){
  
  // Use system time to check running time
  time_t currentTimer=time(NULL);
  time_t prevTimer=time(NULL);
  time(&prevTimer);
  
  if(argc!=6) usage();
  
  int fitType=stoi(argv[5]);
  if(fitType>4) usage();
  GlobalAnalyzer *globalAnalyzer= new GlobalAnalyzer();
  globalAnalyzer->InitializeAnalyzer(argv[2],argv[3],argv[4]);
  globalAnalyzer->SetupExperiments(fitType);
  
  std::cout << "Output file name : "<<argv[1] <<std::endl;
  // output ROOT file for saving plots
  TFile *outputFile=new TFile(argv[1],"RECREATE");
  
  ROOT::Math::Minimizer* minimum =
  ROOT::Math::Factory::CreateMinimizer("Minuit2","");
  minimum->SetMaxFunctionCalls(100000); // for Minuit/Minuit2
  minimum->SetTolerance(0.0001);
  minimum->SetPrintLevel(1);
  
  double step[4] = {0.0001,0.0001,0.0001,0.0001};
  // starting point
  
  double variable[4] = {0,0,0,0};
  
  minimum->SetFunction(*globalAnalyzer);
  
  // Set the free variables to be minimized !
  minimum->SetVariable(0,"x",variable[0], step[0]);
  minimum->SetVariable(1,"y",variable[1], step[1]);
  minimum->SetVariable(2,"z",variable[2], step[2]);
  minimum->SetVariable(3,"p",variable[3], step[3]);
  
  
  minimum->SetVariableLowerLimit(0,0);
  minimum->SetVariableLowerLimit(1,0);
  minimum->SetVariableLowerLimit(2,0);
  minimum->SetVariableLowerLimit(3,0);
  
  minimum->SetVariableUpperLimit(0,20);
  minimum->SetVariableUpperLimit(1,20);
  minimum->SetVariableUpperLimit(2,20);
  minimum->SetVariableUpperLimit(3,20);
  
  // do the minimization
  minimum->Minimize();
  
  const double *xs = minimum->X();
  const double *eXs = minimum->Errors();
  std::cout << "Minimum: f(" << xs[0] << "," << xs[1] <<","<<xs[2]<<"," <<xs[3] << "): "
  << minimum->MinValue()  << std::endl;
  TVectorD v(9);
  v[0]=xs[0];
  v[1]=xs[1];
  v[2]=xs[2];
  v[3]=xs[3];
  v[4]=eXs[0];
  v[5]=eXs[1];
  v[6]=eXs[2];
  v[7]=eXs[3];
  v[8]=minimum->MinValue();
  
  unsigned int nSteps=1000;
  double xValues5[nSteps];
  double xValues8[nSteps];
  double xValues9[nSteps];
  double xValues1[nSteps];
  
  double yValues5[nSteps];
  double yValues8[nSteps];
  double yValues9[nSteps];
  double yValues1[nSteps];
  
  minimum->Scan(0,nSteps,xValues5,yValues5,4,8);
  minimum->Scan(1,nSteps,xValues8,yValues8,6,12);
  minimum->Scan(2,nSteps,xValues9,yValues9,3,6);
  minimum->Scan(3,nSteps,xValues1,yValues1,4,8);
  
  TGraph *g5=new TGraph();
  g5->SetName("U235");
  TGraph *g8=new TGraph();
  g8->SetName("U238");
  TGraph *g9=new TGraph();
  g9->SetName("U239");
  TGraph *g1=new TGraph();
  g1->SetName("U241");
  
  unsigned int nBins=10;
  TH2D *hChi2MapU235238 = new TH2D("hChi2MapU235238","hChi2MapU235238",nBins,5,7.5,nBins,6,12);
  TH2D *hChi2MapU235239 = new TH2D("hChi2MapU235239","hChi2MapU235239",nBins,5,7.5,nBins,3,6);
  TH2D *hChi2MapU235241 = new TH2D("hChi2MapP235241","hChi2MapP235241",nBins,5,7.5,nBins,4,8);
  
  TH2D *hChi2MapU238239 = new TH2D("hChi2MapU238239","hChi2MapU238239",nBins,6,12,nBins,3,6);
  TH2D *hChi2MapU238241 = new TH2D("hChi2MapU238241","hChi2MapU238241",nBins,6,12,nBins,4,8);
  
  TH2D *hChi2MapP239241 = new TH2D("hChi2MapP239241","hChi2MapP239241",nBins,3,6,nBins,4,8);
  
  double maxChi2Value=10000.0;
  for(unsigned int i=0;i<nBins;i++){
    for(unsigned int j=0;j<nBins;j++){
      for(unsigned int k=0;k<nBins;k++){
        for(unsigned int l=0;l<nBins;l++){
          hChi2MapU235238->SetBinContent(i+1,j+1,maxChi2Value);
          hChi2MapU235239->SetBinContent(i+1,k+1,maxChi2Value);
          hChi2MapU235241->SetBinContent(i+1,l+1,maxChi2Value);
          hChi2MapU238239->SetBinContent(j+1,k+1,maxChi2Value);
          hChi2MapU238241->SetBinContent(j+1,l+1,maxChi2Value);
          hChi2MapP239241->SetBinContent(k+1,l+1,maxChi2Value);
        }
      }
    }
  }
  
  for(unsigned int i=0;i<nSteps;i++){
    g5->SetPoint(i,xValues5[i],yValues5[i]);
    g8->SetPoint(i,xValues8[i],yValues8[i]);
    g9->SetPoint(i,xValues9[i],yValues9[i]);
    g1->SetPoint(i,xValues1[i],yValues1[i]);
  }
  g5->Write();
  g8->Write();
  g9->Write();
  g1->Write();
  
  double values[4];
  for(unsigned int i=0;i<nBins;i++){
    for(unsigned int j=0;j<nBins;j++){
      for(unsigned int k=0;k<nBins;k++){
        for(unsigned int l=0;l<nBins;l++){
          double U235Value=hChi2MapU235238->GetXaxis()->GetBinCenter(i+1);
          double U238Value=hChi2MapU235238->GetYaxis()->GetBinCenter(j+1);
          double P239Value=hChi2MapP239241->GetXaxis()->GetBinCenter(k+1);
          double P241Value=hChi2MapP239241->GetYaxis()->GetBinCenter(l+1);
          values[0]=U235Value;
          values[1]=U238Value;
          values[2]=P239Value;
          values[3]=P241Value;
          double chi2Value=globalAnalyzer->DoEval(values);
          if(chi2Value<hChi2MapU235238->GetBinContent(i+1,j+1))hChi2MapU235238->SetBinContent(i+1,j+1,chi2Value);
          if(chi2Value<hChi2MapU235239->GetBinContent(i+1,k+1))hChi2MapU235239->SetBinContent(i+1,k+1,chi2Value);
          if(chi2Value<hChi2MapU235241->GetBinContent(i+1,l+1))hChi2MapU235241->SetBinContent(i+1,l+1,chi2Value);
          if(chi2Value<hChi2MapU238239->GetBinContent(j+1,k+1))hChi2MapU238239->SetBinContent(j+1,k+1,chi2Value);
          if(chi2Value<hChi2MapU238241->GetBinContent(j+1,l+1))hChi2MapU238241->SetBinContent(j+1,l+1,chi2Value);
          if(chi2Value<hChi2MapP239241->GetBinContent(k+1,l+1))hChi2MapP239241->SetBinContent(k+1,l+1,chi2Value);
        }
      }
    }
  }
  v.Write("minValues");
  hChi2MapU235238->Write();
  hChi2MapU235239->Write();
  hChi2MapU235241->Write();
  hChi2MapU238239->Write();
  hChi2MapU238241->Write();
  hChi2MapP239241->Write();
  
  outputFile->Close();
  
  return 0;
}
