#include "GlobalAnalyzer.hh"
#include <math.h>
using namespace std;

void usage(){
  std::cout << "Example: analyzeGlobalData outputFileName NU235BINS NBINS U235MinBin U235MaxBin inputFileName covStat covSyst\n";
  exit(1);
}

int main(int argc, char *argv[]){
  
  // Use system time to check running time
  time_t currentTimer=time(NULL);
  time_t prevTimer=time(NULL);
  time(&prevTimer);
  
  if(argc!=9) usage();
  
  GlobalAnalyzer *globalAnalyzer= new GlobalAnalyzer(argv[6],argv[7],argv[8]);
  
  std::cout << "Output file name : "<<argv[1] <<std::endl;
  // output ROOT file for saving plots
  TFile *outputFile=new TFile(argv[1],"RECREATE");
  
  /// Number of bins used in the histograms
  int nU235Bins= atoi(argv[2]);
  int nBins= atoi(argv[3]);
  
  // Histograms
  TH1D* hChi2MapU235;
  TH1D* hChi2MapU238;
  TH1D* hChi2MapP239;
  TH1D* hChi2MapP241;
  
  TH2D* hChi2MapU235238;
  TH2D* hChi2MapU235239;
  TH2D* hChi2MapU235241;
  TH2D* hChi2MapU238239;
  TH2D* hChi2MapU238241;
  TH2D* hChi2MapP239241;
  
  // Numbers from Daya Bay paper
  double Sigma235=6.69;
  double Sigma238=10.1;
  double Sigma239=4.40;
  double Sigma241=6.03;
  
  /// The minimum and maximum IBD yield in the histograms
  double min_IBD = 2.0;
  double max_IBD = 14.0;
  
  /// The interval of IBD yield the fit runthrough for each isotope
  double min_IBDU235 =  atof(argv[4]);
  double max_IBDU235 =  atof(argv[5]);
  
  double min_IBDU238 = 7;
  double max_IBDU238 = 14;
  
  double min_IBDP239 = 3;
  double max_IBDP239 = 6;
  
  double min_IBDP241 = 5;
  double max_IBDP241 = 7;
  
  int nIsotope= atof(argv[6]);
  
  /// Rounding is important
  int nBinsU235= round((nU235Bins*1.0)*(max_IBD-min_IBD)/(max_IBDU235-min_IBDU235));
  int nBinsU238= round((nBins*1.0)*(max_IBD-min_IBD)/(max_IBDU238-min_IBDU238));
  int nBinsP239= round((nBins*1.0)*(max_IBD-min_IBD)/(max_IBDP239-min_IBDP239));
  int nBinsP241= round((nBins*1.0)*(max_IBD-min_IBD)/(max_IBDP241-min_IBDP241));
  
  // 1D histograms
  hChi2MapU235 = new TH1D("hChi2MapU235","hChi2MapU235",nBinsU235,min_IBD,max_IBD);
  hChi2MapU238 = new TH1D("hChi2MapU238","hChi2MapU238",nBinsU238,min_IBD,max_IBD);
  hChi2MapP239 = new TH1D("hChi2MapP239","hChi2MapP239",nBinsP239,min_IBD,max_IBD);
  hChi2MapP241 = new TH1D("hChi2MapP241","hChi2MapP241",nBinsP241,min_IBD,max_IBD);
  
  // 2D histograms
  hChi2MapU235238 = new TH2D("hChi2MapU235238","hChi2MapU235238",nBinsU235,min_IBD,max_IBD,nBinsU238,min_IBD,max_IBD);
  hChi2MapU235239 = new TH2D("hChi2MapU235239","hChi2MapU235239",nBinsU235,min_IBD,max_IBD,nBinsP239,min_IBD,max_IBD);
  hChi2MapU235241 = new TH2D("hChi2MapU235241","hChi2MapU235241",nBinsU235,min_IBD,max_IBD,nBinsP241,min_IBD,max_IBD);
  
  hChi2MapU238239 = new TH2D("hChi2MapU238239","hChi2MapU238239",nBinsU238,min_IBD,max_IBD,nBinsP239,min_IBD,max_IBD);
  hChi2MapU238241 = new TH2D("hChi2MapU238241","hChi2MapU238241",nBinsU238,min_IBD,max_IBD,nBinsP241,min_IBD,max_IBD);
  
  hChi2MapP239241 = new TH2D("hChi2MapP239241","hChi2MapP239241",nBinsP239,min_IBD,max_IBD,nBinsP241,min_IBD,max_IBD);
  
  
  outputFile->cd();
  globalAnalyzer->SetupExperiments();
  
  /// The precsion of the IBD yield scan for each isotope
  double fracChangeU235 = (max_IBDU235 - min_IBDU235)/nU235Bins;
  double fracChangeU238 = (max_IBDU238 - min_IBDU238)/nBins;
  double fracChangeP239 = (max_IBDP239 - min_IBDP239)/nBins;
  double fracChangeP241 = (max_IBDP241 - min_IBDP241)/nBins;
  
  TVectorD yTheo(globalAnalyzer->numberofExp);
  
  /// Covariance matrix
  TMatrixD CovarianceMatrix(globalAnalyzer->numberofExp, globalAnalyzer->numberofExp);
  CovarianceMatrix.Zero();
  
  int nThCovMatSize=2;
  // Theoretical covariance matrix between isotopes
  TMatrixD thCovarianceMatrix;
  thCovarianceMatrix.ResizeTo(nThCovMatSize,nThCovMatSize);
  
  /*
   thCovarianceMatrix(0,0)=0.0267;
   thCovarianceMatrix(0,1)=0.0203;
   thCovarianceMatrix(1,0)=0.0203;
   thCovarianceMatrix(1,1)=0.0161;
   
   thCovarianceMatrix(0,2)=0;
   thCovarianceMatrix(0,3)=0.0255;
   
   thCovarianceMatrix(1,2)=0;
   thCovarianceMatrix(1,3)=0.0194;
   
   thCovarianceMatrix(2,0)=0;
   thCovarianceMatrix(2,1)=0;
   thCovarianceMatrix(2,2)=0.6776;
   thCovarianceMatrix(2,3)=0;
   
   thCovarianceMatrix(3,0)=0.0255;
   thCovarianceMatrix(3,1)=0.0194;
   thCovarianceMatrix(3,2)=0;
   thCovarianceMatrix(3,3)=0.0246;
   
   thCovarianceMatrix(0,0)=0.0161;
   thCovarianceMatrix(0,1)=0;
   thCovarianceMatrix(0,2)=0.0194;
   
   thCovarianceMatrix(1,0)=0;
   thCovarianceMatrix(1,1)=0.6776;
   thCovarianceMatrix(1,2)=0;
   
   thCovarianceMatrix(2,0)=0.0194;
   thCovarianceMatrix(2,1)=0;
   thCovarianceMatrix(2,2)=0.0246;
   */
  
  
  // This is a subset of the theoretical covariance matrices, uncomment the above code to use more theoretical covariance matix elements
  thCovarianceMatrix(0,0)=0.6776;
  thCovarianceMatrix(1,1)=0.0246;
  thCovarianceMatrix(0,1)=0;
  thCovarianceMatrix(1,0)=0;
  
  thCovarianceMatrix.Print();
  
  if(thCovarianceMatrix.InvertFast()==0) exit(1);
  
  thCovarianceMatrix.Print();
  
  double minChi2=1000000;
  int firstBinU235 = round((nBinsU235*1.0)*(min_IBDU235-min_IBD)/(max_IBD-min_IBD));
  int firstBinU238 = round((nBinsU238*1.0)*(min_IBDU238-min_IBD)/(max_IBD-min_IBD));
  int firstBinP239 = round((nBinsP239*1.0)*(min_IBDP239-min_IBD)/(max_IBD-min_IBD));
  int firstBinP241 = round((nBinsP241*1.0)*(min_IBDP241-min_IBD)/(max_IBD-min_IBD));
  
  // Set the bins to arbitrarily high values
  for(int i=1;i<=nU235Bins;i++)hChi2MapU235->SetBinContent(firstBinU235+i,minChi2);
  for(int i=1;i<=nBins;i++)hChi2MapU238->SetBinContent(firstBinU238+i,minChi2);
  for(int i=1;i<=nBins;i++)hChi2MapP239->SetBinContent(firstBinP239+i,minChi2);
  for(int i=1;i<=nBins;i++)hChi2MapP241->SetBinContent(firstBinP241+i,minChi2);
 
  
  for(int i=1;i<=nU235Bins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapU235238->SetBinContent(firstBinU235+i,firstBinU238+j,minChi2);
  }
  for(int i=1;i<=nU235Bins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapU235239->SetBinContent(firstBinU235+i,firstBinP239+j,minChi2);
  }
  for(int i=1;i<=nU235Bins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapU235241->SetBinContent(firstBinU235+i,firstBinP241+j,minChi2);
  }
  for(int i=1;i<=nBins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapU238239->SetBinContent(firstBinU238+i,firstBinP239+j,minChi2);
  }
  for(int i=1;i<=nBins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapU238241->SetBinContent(firstBinU238+i,firstBinP241+j,minChi2);
  }
  for(int i=1;i<=nBins;i++){
    for(int j=1;j<=nBins;j++)hChi2MapP239241->SetBinContent(firstBinP239+i,firstBinP241+j,minChi2);
  }
  
  time(&currentTimer);
  double dt=difftime(currentTimer,prevTimer);
  printf("Time since the start of program dt=%f seconds\n",dt);
  
  minChi2=10000;
  for(int i=0;i<nU235Bins;i++){ // U-235 loop
    double chi2Value = 0.0;
    double y_U235=fracChangeU235*i + min_IBDU235;
    double r_5=y_U235/Sigma235;
    
    for(int j=0;j<nBins;j++){ // U-238 loop
      double y_U238=fracChangeU238*j + min_IBDU238;
      double r_8=y_U238/Sigma238;
      
      for(int k=0;k<nBins;k++){ // P-239 loop
        double y_P239=fracChangeP239*k + min_IBDP239;
        double r_9=y_P239/Sigma239;
        
        for(int l=0;l<nBins;l++){ // P-241 loop
          double y_P241=fracChangeP241*l + min_IBDP241;
          double r_1=y_P241/Sigma241;
          globalAnalyzer->CalculateTheoreticalIBDYield(yTheo,y_U235, y_U238, y_P239, y_P241);//+0.3 seconds for 60 bins
          
          globalAnalyzer->CalculateCovarianceMatrix(yTheo,CovarianceMatrix);//+2.1 seconds
          
          if(CovarianceMatrix.InvertFast()==0) exit(1);
          
          chi2Value = globalAnalyzer->CalculateChi2(yTheo, CovarianceMatrix);//+0.675
          
          
          TVectorD rValues;
          rValues.ResizeTo(nThCovMatSize);
//          rValues[0]=Sigma235*(r_5-1);
//          rValues[1]=Sigma239*(r_9-1);
          rValues[0]=Sigma238*(r_8-1);
          rValues[1]=Sigma241*(r_1-1);
          
          TVectorD rValuesTemp=rValues;
          rValuesTemp*=thCovarianceMatrix;
          
          chi2Value+=rValuesTemp*rValues;
          
          
          // Fill 1D histograms
          if(hChi2MapU235->GetBinContent(firstBinU235+i+1)>chi2Value)hChi2MapU235->SetBinContent(firstBinU235+i+1,chi2Value);
          if(hChi2MapU238->GetBinContent(firstBinU238+j+1)>chi2Value)hChi2MapU238->SetBinContent(firstBinU238+j+1,chi2Value);
          if(hChi2MapP239->GetBinContent(firstBinP239+k+1)>chi2Value)hChi2MapP239->SetBinContent(firstBinP239+k+1,chi2Value);
          if(hChi2MapP241->GetBinContent(firstBinP241+l+1)>chi2Value)hChi2MapP241->SetBinContent(firstBinP241+l+1,chi2Value);
          
          //Fill 2D hists
          if(hChi2MapU235238->GetBinContent(firstBinU235+i+1,firstBinU238+j+1)>chi2Value)hChi2MapU235238->SetBinContent(firstBinU235+i+1,firstBinU238+j+1,chi2Value);
          if(hChi2MapU235239->GetBinContent(firstBinU235+i+1,firstBinP239+k+1)>chi2Value)hChi2MapU235239->SetBinContent(firstBinU235+i+1,firstBinP239+k+1,chi2Value);
          if(hChi2MapU235241->GetBinContent(firstBinU235+i+1,firstBinP241+l+1)>chi2Value)hChi2MapU235241->SetBinContent(firstBinU235+i+1,firstBinP241+l+1,chi2Value);
          if(hChi2MapU238239->GetBinContent(firstBinU238+j+1,firstBinP239+k+1)>chi2Value)hChi2MapU238239->SetBinContent(firstBinU238+j+1,firstBinP239+k+1,chi2Value);
          if(hChi2MapU238241->GetBinContent(firstBinU238+j+1,firstBinP241+l+1)>chi2Value)hChi2MapU238241->SetBinContent(firstBinU238+j+1,firstBinP241+l+1,chi2Value);
          if(hChi2MapP239241->GetBinContent(firstBinP239+k+1,firstBinP241+l+1)>chi2Value)hChi2MapP239241->SetBinContent(firstBinP239+k+1,firstBinP241+l+1,chi2Value);
        }//end of P241
      }//end of P239
    }//end of U238
    time(&currentTimer);
    double dt=difftime(currentTimer,prevTimer);
    printf("Running %i U235 bin, dt=%f seconds\n",i,dt);
  }//end of U235
  
  hChi2MapU235->Write();
  hChi2MapU238->Write();
  hChi2MapP239->Write();
  hChi2MapP241->Write();
  
  hChi2MapU235238->Write();
  hChi2MapU235239->Write();
  hChi2MapU235241->Write();
  hChi2MapU238239->Write();
  hChi2MapU238241->Write();
  hChi2MapP239241->Write();
  
  outputFile->Close();
  
  return 0;
}
