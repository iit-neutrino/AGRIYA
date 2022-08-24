#include "GlobalAnalyzer.hh"
#include <math.h>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
using namespace std;

static const double sigma235=6.69;
static const double sigma238=10.10;
static const double sigma239=4.40;
static const double sigma240=4.96; // ADD
static const double sigma241=6.03;

void usage(){
  printf("Example: analyzeGlobalData outputFileName inputFileName statistical_covariance_matrix statistical_covariance_matrix fitype\n");
  printf("Fit type should be a number between 1 and 11:\n 1 = U235 only \n 2 = P239 only \n 3 = U235+239 fit \n 4 = U235+239+238 fit \n 5 = OSC only \n 6 = 235+OSC only \n 7 = 239+OSC only  \n 8 = Eq \n 9 = 5+Eq \n 10 = 9+Eq \n 11 = linear fit to 239 data \n");
  
  exit(1);
}

int main(int argc, char *argv[]){
  if(argc!=6) usage();
  
  int fitType=stoi(argv[5]);
  if(fitType>11) usage();
  printf("Using git commit 73e465240018f164bd51222c5d3798aaa497992c \n");
  printf("Output file name : %s \n",argv[1]);
  printf("Fit type : %i \n",fitType);
  GlobalAnalyzer *globalAnalyzer= new GlobalAnalyzer();
  globalAnalyzer->InitializeAnalyzer(argv[2],argv[3],argv[4]);
  globalAnalyzer->SetupExperiments(fitType);
  
  // output ROOT file for saving plots
  TFile *outputFile=new TFile(argv[1],"RECREATE");
  
  ROOT::Math::Minimizer* minimum =
  ROOT::Math::Factory::CreateMinimizer("Minuit2","");
  minimum->SetMaxFunctionCalls(100000); // for Minuit/Minuit2
  minimum->SetTolerance(0.0001);
  //  minimum->SetPrecision(1E14);
  minimum->SetPrintLevel(0);
  
  double step[7] = {0.0001,0.0001,0.0001,0.0001,0.0001,0.0001,0.0001}; // MOD (6->7; ..., 0.0001, ...)
  double variable[7] = {sigma235,sigma238,sigma239,sigma240,sigma241,0,0.48}; // MOD (6->7; ..., sigma240, ...)
  double minRange[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // MOD (6->7; ...,0.0})
  double maxRange[7]={40,40,40,40,40,1,4}; // MOD (6->7; ...,40,...)
  string varName[7] = {"U235","U238","P239","P240","P241","s22t","dm2"};

  // These are for linear fits
//  double variable[6] = {6,-1,0,0,0,0};
//  double minRange[6]={0.0,-3,0.0,0.0,0.0,0.0};
//  double maxRange[6]={20,3,20,20,1,4};
  // These are for

  
  minimum->SetFunction(*globalAnalyzer);
  
  // Set the free variables to be minimized !
  for (int i=0;i<7;i++) { // There is a bug where data does not map correctly if index started from 0 in Minuit and Minuit2
    minimum->SetVariable(i,varName[i],variable[i], step[i]);
    minimum->SetVariableLimits(i,minRange[i],maxRange[i]);
  }

  if(fitType>4&& fitType<8)minimum->FixVariable(6);
  
  // do the minimization
  minimum->Minimize();
  if(fitType>4&& fitType<8){
    minimum->ReleaseVariable(6);
    minimum->Minimize();
  }
  const double *xs = minimum->X();
  const double *eXs = minimum->Errors();
  double errorLow;
  double errorUp;

  minimum->PrintResults();
  
  TVectorD v(13); // MOD (11->13)
  v[0]=xs[0];
  v[1]=xs[1];
  v[2]=xs[2];
  v[3]=xs[3];
  v[4]=xs[4]; // ADD
  minimum->GetMinosError(0,errorLow,errorUp);
  v[5]=errorUp; // MOD (4->5)
  minimum->GetMinosError(1,errorLow,errorUp);
  v[6]=errorUp; // MOD (5->6)
  minimum->GetMinosError(2,errorLow,errorUp);
  v[7]=errorUp; // MOD (6->7)
  minimum->GetMinosError(3,errorLow,errorUp);
  v[8]=errorUp; // MOD (7->8)
  minimum->GetMinosError(4,errorLow,errorUp); // ADD
  v[9]=errorUp; // ADD
  v[10]=xs[5]; // MOD (8->10;4->5)
  v[11]=xs[6]; // MOD (9->11;5->6)
  v[12]=minimum->MinValue(); // MOD (10->12)
  
  printf("--------------------------------\n");
  printf("U235 = %1.3f +/- %1.3f\n",v[0],v[5]); // MOD (0=0;4->5)
  printf("U238 = %1.3f +/- %1.3f\n",v[1],v[6]); // MOD (1=1;5->6)
  printf("P239 = %1.3f +/- %1.3f\n",v[2],v[7]); // MOD (2=2;6->7)
  printf("P240 = %1.3f +/- %1.3f\n",v[3],v[8]); // ADD
  printf("P241 = %1.3f +/- %1.3f\n",v[4],v[9]); // MOD (3->4;7->9)
  printf("--------------------------------\n");
  printf("U235 = %1.3f +/- %1.3f\n",v[0]/sigma235,v[5]/sigma235); // MOD (0=0;4->5)
  printf("U238 = %1.3f +/- %1.3f\n",v[1]/sigma238,v[6]/sigma238); // MOD (1=1;5->6)
  printf("P239 = %1.3f +/- %1.3f\n",v[2]/sigma239,v[7]/sigma239); // MOD (2=2;6->7)
  printf("P240 = %1.3f +/- %1.3f\n",v[3]/sigma240,v[8]/sigma240); // ADD
  printf("P241 = %1.3f +/- %1.3f\n",v[4]/sigma241,v[9]/sigma241); // MOD (3->4;7->9)
  printf("--------------------------------\n");
  printf("s22 = %1.3f +/- %2.3f\n",v[10],eXs[5]); // MOD (8->10;4->5)
  printf("dm2 = %1.3f +/- %2.3f\n",v[11],eXs[6]); // MOD (9->11;5->6)
  printf("minimum    =%3.1f\n",minimum->MinValue());
  
  
  /*
   std::cout << globalAnalyzer->DoEval(xs) << std::endl;
   variable[0]=6.29;
   variable[1]=xs[1];
   variable[2]=3.8412;
   variable[3]=xs[3];
   variable[4]=4000;
   variable[5]=5000;
   
   std::cout << globalAnalyzer->DoEval(variable) << std::endl;*/
  
  
  //****************// Plotting Code //************************//
  unsigned int nSteps=500;
  double xValues5[nSteps];
  double xValues8[nSteps];
  double xValues9[nSteps];
  double xValues0[nSteps]; // ADD (P240)
  double xValues1[nSteps];
  double xValuess22[nSteps];
  double xValuesdm2[nSteps];
  double xValues58[nSteps];
  double xValues59[nSteps];
  double xValues50[nSteps]; // ADD (U235vP240)
  double xValues51[nSteps];
  double xValues89[nSteps];
  double xValues80[nSteps]; // ADD (U238vP240)
  double xValues81[nSteps];
  double xValues90[nSteps]; // ADD (P239vP240)
  double xValues91[nSteps];
  double xValues01[nSteps]; // ADD (P240vP241)
  double xValuess22dm2[nSteps];
  
  double yValues5[nSteps];
  double yValues8[nSteps];
  double yValues9[nSteps];
  double yValues0[nSteps]; // ADD (P240)
  double yValues1[nSteps];
  double yValuess22[nSteps];
  double yValuesdm2[nSteps];
  double yValues58[nSteps];
  double yValues59[nSteps];
  double yValues50[nSteps]; // ADD (U235vP240)
  double yValues51[nSteps];
  double yValues89[nSteps];
  double yValues80[nSteps]; // ADD (U238vP240)
  double yValues81[nSteps];
  double yValues90[nSteps]; // ADD (P239vP240)
  double yValues91[nSteps];
  double yValues01[nSteps]; // ADD (P240vP241)
  double yValuess22dm2[nSteps];
  
  minimum->Scan(0,nSteps,xValues5,yValues5,minRange[0],maxRange[0]);
  minimum->Scan(1,nSteps,xValues8,yValues8,minRange[1],maxRange[1]);
  minimum->Scan(2,nSteps,xValues9,yValues9,minRange[2],maxRange[2]);
  minimum->Scan(3,nSteps,xValues0,yValues0,minRange[3],maxRange[3]); // ADD
  minimum->Scan(4,nSteps,xValues1,yValues1,minRange[4],maxRange[4]); // MOD (3->4;3->4;3->4)
  minimum->Scan(5,nSteps,xValuess22,yValuess22,minRange[5],maxRange[5]); // MOD (4->5;4->5;4->5)
  minimum->Scan(6,nSteps,xValuesdm2,yValuesdm2,minRange[6],maxRange[6]); // MOD (5->6;5->6;5->6)
  TGraph *g5=new TGraph();
  g5->SetName("U235");
  TGraph *g8=new TGraph();
  g8->SetName("U238");
  TGraph *g9=new TGraph();
  g9->SetName("P239");
  TGraph *g0=new TGraph(); // ADD
  g0->SetName("P240"); // ADD
  TGraph *g1=new TGraph();
  g1->SetName("P241");
  TGraph *gs22t=new TGraph();
  gs22t->SetName("s22t");
  TGraph *gdm2=new TGraph();
  gdm2->SetName("dm2");
  
  
  for(unsigned int i=0;i<nSteps;i++){
    g5->SetPoint(i,xValues5[i],yValues5[i]);
    g8->SetPoint(i,xValues8[i],yValues8[i]);
    g9->SetPoint(i,xValues9[i],yValues9[i]);
    g0->SetPoint(i,xValues0[i],yValues0[i]); // ADD
    g1->SetPoint(i,xValues1[i],yValues1[i]);
    gs22t->SetPoint(i,xValuess22[i],yValuess22[i]);
    gdm2->SetPoint(i,xValuesdm2[i],yValuesdm2[i]);
  }
  
  double errorDefs[3]={2.3,6.18,11.83};
  // Three graphs for 3 contours each
  TGraph *g58[3];
  TGraph *g59[3];
  TGraph *g50[3]; // ADD
  TGraph *g51[3];
  TGraph *g89[3];
  TGraph *g80[3]; // ADD
  TGraph *g81[3];
  TGraph *g90[3]; // ADD
  TGraph *g91[3];
  TGraph *g01[3]; // ADD
  TGraph *gs22dm2[3];
  for (int i=0;i<3; i++) {
    TString gName;
    gName.Form("U235_U238_%isigma",i+1);
    g58[i]=new TGraph();
    g58[i]->SetName(gName);
    
    gName.Form("U235_P239_%isigma",i+1);
    g59[i]=new TGraph();
    g59[i]->SetName(gName);

    gName.Form("U235_P240_%isigma",i+1); // ADD
    g50[i]=new TGraph(); // ADD
    g50[i]->SetName(gName); // ADD
    
    gName.Form("U235_P241_%isigma",i+1);
    g51[i]=new TGraph();
    g51[i]->SetName(gName);
    
    gName.Form("U238_P239_%isigma",i+1);
    g89[i]=new TGraph();
    g89[i]->SetName(gName);

    gName.Form("U238_P240_%isigma",i+1); // ADD
    g80[i]=new TGraph(); // ADD
    g80[i]->SetName(gName); // ADD
    
    gName.Form("U238_P241_%isigma",i+1);
    g81[i]=new TGraph();
    g81[i]->SetName(gName);

    gName.Form("P239_P240_%isigma",i+1); // ADD
    g90[i]=new TGraph(); // ADD
    g90[i]->SetName(gName); // ADD
    
    gName.Form("P239_P241_%isigma",i+1);
    g91[i]=new TGraph();
    g91[i]->SetName(gName);

    gName.Form("P240_P241_%isigma",i+1); // ADD
    g01[i]=new TGraph(); // ADD
    g01[i]->SetName(gName); // ADD
    
    gName.Form("s22_dm2_%isigma",i+1);
    gs22dm2[i]=new TGraph();
    gs22dm2[i]->SetName(gName);
    
    minimum->SetErrorDef(errorDefs[i]);
    minimum->Contour(0,1,nSteps,xValues58,yValues58);
    minimum->Contour(0,2,nSteps,xValues59,yValues59);
    minimum->Contour(0,4,nSteps,xValues51,yValues51); // MOD (0=0;3->4)
    minimum->Contour(1,2,nSteps,xValues89,yValues89);
    minimum->Contour(1,4,nSteps,xValues81,yValues81); // MOD (1=1;3->4)
    minimum->Contour(2,4,nSteps,xValues91,yValues91); // MOD (2=2;3->4)
    minimum->Contour(5,6,nSteps,xValuess22dm2,yValuess22dm2); // MOD (4->5;5->6)
    minimum->Contour(0,3,nSteps,xValues50,yValues50); // ADD
    minimum->Contour(1,3,nSteps,xValues80,yValues80); // ADD
    minimum->Contour(2,3,nSteps,xValues90,yValues90); // ADD
    minimum->Contour(3,4,nSteps,xValues01,yValues01); // ADD
    for(unsigned int j=0;j<nSteps;j++){
      g58[i]->SetPoint(j,xValues58[j],yValues58[j]);
      g59[i]->SetPoint(j,xValues59[j],yValues59[j]);
      g50[i]->SetPoint(j,xValues50[j],yValues50[j]); // ADD
      g51[i]->SetPoint(j,xValues51[j],yValues51[j]);
      g89[i]->SetPoint(j,xValues89[j],yValues89[j]);
      g80[i]->SetPoint(j,xValues80[j],yValues80[j]); // ADD
      g81[i]->SetPoint(j,xValues81[j],yValues81[j]);
      g90[i]->SetPoint(j,xValues90[j],yValues90[j]); // ADD
      g91[i]->SetPoint(j,xValues91[j],yValues91[j]);
      g01[i]->SetPoint(j,xValues01[j],yValues01[j]); // ADD
      gs22dm2[i]->SetPoint(j,xValuess22dm2[j],yValuess22dm2[j]);
    }
  }
  
  g5->Write();
  g8->Write();
  g9->Write();
  g0->Write(); // ADD
  g1->Write();
  gs22t->Write();
  gdm2->Write();
  for (int i=0;i<3; i++) {
    g58[i]->Write();
    g59[i]->Write();
    g51[i]->Write();
    g89[i]->Write();
    g81[i]->Write();
    g91[i]->Write();
    g50[i]->Write(); // ADD
    g80[i]->Write(); // ADD
    g90[i]->Write(); // ADD
    g01[i]->Write(); // ADD
  }
  v.Write("minValues");
  globalAnalyzer->DrawDataPoints(*outputFile);
  if(fitType==11)globalAnalyzer->DrawFitPoints(*outputFile,v[0],v[1]);
  
  outputFile->Close();
  
  return 0;
}
