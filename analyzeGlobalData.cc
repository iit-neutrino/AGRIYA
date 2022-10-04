#include <math.h>

#include "TH2D.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"

#include "GlobalAnalyzer.hh"
#include "TMacroInterface.hh"

using namespace std;

static const vector<string> fitName={"U235 only","P239 only","U235+239","U235+239+238","Oscillation only","235 + Oscillation","239 + Oscillation","Eq","5 + Eq","9 + Eq","239 data linear"};

void macroHelp()
{
  printf("---------------------------------------------------------------------------\n");
  printf("Macro file should contain values for atleast the following keys:\n");
  printf("OUTPUTFILE, DATAFILE, COVARIANCEFILESTAT, COVARIANCEFILESYST, COVARIANCEFILETHEO, FITTYPE\n");
  printf("Example 'macrofile.mac' file:\n");
  printf("OUTPUTFILE = outputFile.root\n");
  printf("DATAFILE = ./inputs/global.txt\n");
  printf("COVARIANCEFILESTAT = inputs/global_covstat.txt\n");
  printf("COVARIANCEFILESYST = inputs/global_covsyst.txt\n");
  printf("COVARIANCEFILETHEO = inputs/theo_arXiv_1703.00860.txt\n");
  printf("FITTYPE = 1\n");
  printf("---------------------------------------------------------------------------\n");
  return;
}

void inputHelp()
{

  printf("---------------------------------------------------------------------------\n");
  printf("Example: analyzeGlobalData macrofile.mac\n");
  printf("Fit type should be a number between 1 and 11:\n");
  for(unsigned long i=0; i<fitName.size(); ++i)
  {
    printf("%lu = %s fit \n",i+1,fitName[i].c_str()); 
  }
  printf("---------------------------------------------------------------------------\n");
  return;
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

int main(int argc, char *argv[]){
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

  if(fitType>11) usage();
  
  printf("-----------------------------------------------------------------------\n"); 
  printf("Running %s fit at %s using  %s branch and git hash = %s\n\n",fitName.at(fitType-1).c_str(),COMPILE_TIME, GIT_BRANCH, GIT_HASH);

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

  if(!globalAnalyzer->SetupExperiments(fitType))
  {
    printf("Couldn't Setup experiments \n Exiting \n");
    exit(-1);
  }
  
  //Create output ROOT file for saving plots
  TFile *outputFile=new TFile(outputFileName,"RECREATE");

  //Initialize the minimizer used for the actual fits
  ROOT::Math::Minimizer* minimizer =
  ROOT::Math::Factory::CreateMinimizer("Minuit2","MIGRAD");
  minimizer->SetMaxFunctionCalls(100000);
  minimizer->SetTolerance(1E-6);
  minimizer->SetPrintLevel(0); //Could increase this value if debugging
  printf("Minimizer initialized\n");
  
  string varName[7] = {"U235","U238","P239","P240","P241","s22t","dm2"};
  double variable[7] = {globalAnalyzer->GetSigma235(),globalAnalyzer->GetSigma238(),globalAnalyzer->GetSigma239(),globalAnalyzer->GetSigma240(),globalAnalyzer->GetSigma241(),0,0};// Set variable staring point for the fit
  double step[7] = {0.001,0.001,0.001,0.001,0.001,0.001,0.001}; // Set step size for variables; setting all to 0.0001
  // Set minimum and maximum of the variable ranges for fit
  double minRange[7]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  double maxRange[7]={20,20,20,20,20,1,100};

  // Set the function that needs to be minimized over
  // In this case, it will minimize using return value of DoEval
  // GlobalAnalyzer is inherited from IBaseFunctionMultiDim
  minimizer->SetFunction(*globalAnalyzer);
  printf("Minimizer function set\n");
  
  // Set the free variables to be minimized over!
  for (int i=0;i<7;i++) { // There is a bug where data does not map correctly if index started from 0 in Minuit and Minuit2
    minimizer->SetVariable(i,varName[i],variable[i], step[i]);
    minimizer->SetVariableLimits(i,minRange[i],maxRange[i]);
  }

  // If the fits include oscillations, perform the fit by fixing oscillation parameters
  // and again fit by releasing those parameters
  if(fitType>4 && fitType<8)
  {
    minimizer->FixVariable(6);
    minimizer->FixVariable(7);
    printf("Fit includes oscillation; fixing oscillation parameters for the first fit\n");
  }
  
  int fitStatus;
  // do the minimization
  if(!minimizer->Minimize())
  {
    fitStatus = minimizer->Status();
    printf("Failed fit with status = %i \n",fitStatus);
    printf("Exiting \n");
    exit(-1);
  }
  printf("Minimization process completed successfully\n");

  // perform fit by releasing those parameters
  if(fitType>4 && fitType<8)
  {
    printf("Fit includes oscillation; releasing oscillation parameters and refitting\n");
    minimizer->ReleaseVariable(6);
    minimizer->ReleaseVariable(7);
    if(!minimizer->Minimize())
    {
      fitStatus = minimizer->Status();
      printf("Failed fit with status = %i \n",fitStatus);
      printf("Exiting \n");
      exit(-1);
    }
  }

  // Extract fit results and their errors
  const double *xs = minimizer->X();
  // const double *eXs = minimizer->Errors();
  double errorLow;
  double errorUp;

  // Save results and errors in a vector
  TVectorD v(15);
  // First save the fit results
  v[0]=xs[0];
  v[1]=xs[1];
  v[2]=xs[2];
  v[3]=xs[3];
  v[4]=xs[4]; 
  v[5]=xs[5];
  v[6]=xs[6]; 

  // Save results, errors, and the minimum in a vector
  minimizer->GetMinosError(0,errorLow,errorUp);
  v[7]=errorUp; 
  minimizer->GetMinosError(1,errorLow,errorUp);
  v[8]=errorUp;
  minimizer->GetMinosError(2,errorLow,errorUp);
  v[9]=errorUp;
  minimizer->GetMinosError(3,errorLow,errorUp);
  v[10]=errorUp; 
  minimizer->GetMinosError(4,errorLow,errorUp);
  v[11]=errorUp;
  minimizer->GetMinosError(4,errorLow,errorUp);
  v[12]=errorUp; 
  minimizer->GetMinosError(4,errorLow,errorUp);
  v[13]=errorUp; 
  v[14]=minimizer->MinValue();

  printf("--------------------------------\n");
  printf("U235 = %3.3f +/- %3.3f\n",v[0],v[7]); 
  printf("U238 = %3.3f +/- %3.3f\n",v[1],v[8]); 
  printf("P239 = %3.3f +/- %3.3f\n",v[2],v[9]); 
  printf("P240 = %3.3f +/- %3.3f\n",v[3],v[10]); 
  printf("P241 = %3.3f +/- %3.3f\n",v[4],v[11]); 
  printf("--------------------------------\n");
  printf("U235 = %3.3f +/- %3.3f\n",v[0]/globalAnalyzer->GetSigma235(),v[7]/globalAnalyzer->GetSigma235());
  printf("U238 = %3.3f +/- %3.3f\n",v[1]/globalAnalyzer->GetSigma238(),v[8]/globalAnalyzer->GetSigma238());
  printf("P239 = %3.3f +/- %3.3f\n",v[2]/globalAnalyzer->GetSigma239(),v[9]/globalAnalyzer->GetSigma239());
  printf("P240 = %3.3f +/- %3.3f\n",v[3]/globalAnalyzer->GetSigma240(),v[10]/globalAnalyzer->GetSigma240());
  printf("P241 = %3.3f +/- %3.3f\n",v[4]/globalAnalyzer->GetSigma241(),v[11]/globalAnalyzer->GetSigma241());
  printf("--------------------------------\n");
  printf("s22 = %3.3f +/- %2.3f\n",v[5],v[12]);
  printf("dm2 = %3.3f +/- %2.3f\n",v[6],v[13]);
  printf("minimum = %3.1f\n",minimizer->MinValue());

  //****************// Plotting Code //************************//
  unsigned int nSteps=500;
  double xValues5[nSteps];
  double xValues8[nSteps];
  double xValues9[nSteps];
  double xValues0[nSteps];
  double xValues1[nSteps];
  double xValuess22[nSteps];
  double xValuesdm2[nSteps];
  double xValues58[nSteps];
  double xValues59[nSteps];
  double xValues50[nSteps];
  double xValues51[nSteps];
  double xValues89[nSteps];
  double xValues80[nSteps];
  double xValues81[nSteps];
  double xValues90[nSteps];
  double xValues91[nSteps];
  double xValues01[nSteps];
  double xValuess22dm2[nSteps];
  
  double yValues5[nSteps];
  double yValues8[nSteps];
  double yValues9[nSteps];
  double yValues0[nSteps]; 
  double yValues1[nSteps];
  double yValuess22[nSteps];
  double yValuesdm2[nSteps];
  double yValues58[nSteps];
  double yValues59[nSteps];
  double yValues50[nSteps];
  double yValues51[nSteps];
  double yValues89[nSteps];
  double yValues80[nSteps];
  double yValues81[nSteps];
  double yValues90[nSteps];
  double yValues91[nSteps];
  double yValues01[nSteps];
  double yValuess22dm2[nSteps];
  
  minimizer->Scan(0,nSteps,xValues5,yValues5,minRange[0],maxRange[0]);
  minimizer->Scan(1,nSteps,xValues8,yValues8,minRange[1],maxRange[1]);
  minimizer->Scan(2,nSteps,xValues9,yValues9,minRange[2],maxRange[2]);
  minimizer->Scan(3,nSteps,xValues0,yValues0,minRange[3],maxRange[3]); 
  minimizer->Scan(4,nSteps,xValues1,yValues1,minRange[4],maxRange[4]); 
  minimizer->Scan(5,nSteps,xValuess22,yValuess22,minRange[5],maxRange[5]);
  minimizer->Scan(6,nSteps,xValuesdm2,yValuesdm2,minRange[6],maxRange[6]);
  TGraph *g5=new TGraph();
  g5->SetName("U235");
  TGraph *g8=new TGraph();
  g8->SetName("U238");
  TGraph *g9=new TGraph();
  g9->SetName("P239");
  TGraph *g0=new TGraph();
  g0->SetName("P240"); 
  TGraph *g1=new TGraph();
  g1->SetName("P241");
  TGraph *gs22t=new TGraph();
  gs22t->SetName("s22t");
  TGraph *gdm2=new TGraph();
  gdm2->SetName("dm2");
  
  // Plot the 1D profiles
  for(unsigned int i=0;i<nSteps;i++){
    g5->SetPoint(i,xValues5[i],yValues5[i]);
    g8->SetPoint(i,xValues8[i],yValues8[i]);
    g9->SetPoint(i,xValues9[i],yValues9[i]);
    g0->SetPoint(i,xValues0[i],yValues0[i]); 
    g1->SetPoint(i,xValues1[i],yValues1[i]);
    gs22t->SetPoint(i,xValuess22[i],yValuess22[i]);
    gdm2->SetPoint(i,xValuesdm2[i],yValuesdm2[i]);
  }
  
  // Values corresponding to 1σ, 2σ, 3σ for 2 DOF
  double errorDefs[3]={2.3,6.18,11.83};
  // Three graphs for 3 contours (1σ, 2σ, 3σ) each
  TGraph *g58[3];
  TGraph *g59[3];
  TGraph *g50[3]; 
  TGraph *g51[3];
  TGraph *g89[3];
  TGraph *g80[3]; 
  TGraph *g81[3];
  TGraph *g90[3]; 
  TGraph *g91[3];
  TGraph *g01[3]; 
  TGraph *gs22dm2[3];

  for (int i=0;i<3; i++) 
  {
    TString gName;
    gName.Form("U235_U238_%isigma",i+1);
    g58[i]=new TGraph();
    g58[i]->SetName(gName);
    
    gName.Form("U235_P239_%isigma",i+1);
    g59[i]=new TGraph();
    g59[i]->SetName(gName);

    gName.Form("U235_P240_%isigma",i+1);
    g50[i]=new TGraph();
    g50[i]->SetName(gName);
    
    gName.Form("U235_P241_%isigma",i+1);
    g51[i]=new TGraph();
    g51[i]->SetName(gName);
    
    gName.Form("U238_P239_%isigma",i+1);
    g89[i]=new TGraph();
    g89[i]->SetName(gName);

    gName.Form("U238_P240_%isigma",i+1);
    g80[i]=new TGraph(); 
    g80[i]->SetName(gName);
    
    gName.Form("U238_P241_%isigma",i+1);
    g81[i]=new TGraph();
    g81[i]->SetName(gName);

    gName.Form("P239_P240_%isigma",i+1); 
    g90[i]=new TGraph(); 
    g90[i]->SetName(gName);
    
    gName.Form("P239_P241_%isigma",i+1);
    g91[i]=new TGraph();
    g91[i]->SetName(gName);

    gName.Form("P240_P241_%isigma",i+1); 
    g01[i]=new TGraph(); 
    g01[i]->SetName(gName);
    
    gName.Form("s22_dm2_%isigma",i+1);
    gs22dm2[i]=new TGraph();
    gs22dm2[i]->SetName(gName);
    
    minimizer->SetErrorDef(errorDefs[i]);
    minimizer->Contour(0,1,nSteps,xValues58,yValues58);
    minimizer->Contour(0,2,nSteps,xValues59,yValues59);
    minimizer->Contour(0,3,nSteps,xValues50,yValues50);
    minimizer->Contour(0,4,nSteps,xValues51,yValues51);
    minimizer->Contour(1,2,nSteps,xValues89,yValues89);
    minimizer->Contour(1,3,nSteps,xValues80,yValues80);
    minimizer->Contour(1,4,nSteps,xValues81,yValues81);
    minimizer->Contour(2,3,nSteps,xValues90,yValues90);
    minimizer->Contour(2,4,nSteps,xValues91,yValues91);
    minimizer->Contour(3,4,nSteps,xValues01,yValues01);
    minimizer->Contour(5,6,nSteps,xValuess22dm2,yValuess22dm2);

    for(unsigned int j=0;j<nSteps;j++)
    {
      g58[i]->SetPoint(j,xValues58[j],yValues58[j]);
      g59[i]->SetPoint(j,xValues59[j],yValues59[j]);
      g50[i]->SetPoint(j,xValues50[j],yValues50[j]);
      g51[i]->SetPoint(j,xValues51[j],yValues51[j]);
      g89[i]->SetPoint(j,xValues89[j],yValues89[j]);
      g80[i]->SetPoint(j,xValues80[j],yValues80[j]);
      g81[i]->SetPoint(j,xValues81[j],yValues81[j]);
      g90[i]->SetPoint(j,xValues90[j],yValues90[j]);
      g91[i]->SetPoint(j,xValues91[j],yValues91[j]);
      g01[i]->SetPoint(j,xValues01[j],yValues01[j]);
      gs22dm2[i]->SetPoint(j,xValuess22dm2[j],yValuess22dm2[j]);
    }
  }
  
  g5->Write();
  g8->Write();
  g9->Write();
  g0->Write();
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
    g50[i]->Write();
    g80[i]->Write();
    g90[i]->Write();
    g01[i]->Write();
  }
  v.Write("minValues");

  TH2D *hResultantIsotopeCovarianceMatrix=new TH2D("ResultantIsotopeCovarianceMatrix","ResultantIsotopeCovarianceMatrix;Fit parameter;Fit parameter",5,0.5,5.5,5,0.5,5.5);
  // Testing covariance matrix generation
  for (int i=0;i<5;i++)
  {
    for (int j=0;j<5;j++)
    {
      hResultantIsotopeCovarianceMatrix->SetBinContent(i+1,j+1,minimizer->CovMatrix(i,j)/v[7+i]/v[7+j]);
    }
  }
  hResultantIsotopeCovarianceMatrix->Write();
  delete hResultantIsotopeCovarianceMatrix;

  if(!globalAnalyzer->DrawDataPoints(*outputFile)) 
  {
    printf("Error: Unable to draw data points\n");
    return -1;
  }
  
  if(fitType==11)
  {
    if(!globalAnalyzer->DrawFitPoints(v[0],v[1],*outputFile))
      printf("Error: Unable to draw fit points\n");
      return -1;
  }
  
  outputFile->Close();
  
  return 0;
}
