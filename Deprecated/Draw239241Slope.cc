////////////////////////////////
// Drawing F239 241 line
// Pranava Teja Surukuchi, April 2017
/////////////////////////////////

// C++ Includes
#include <iostream>
#include <algorithm>
#include <fstream>

// ROOT Includes
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TMultiGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TString.h>
#include <TMatrixD.h>
#include <TVectorD.h>
#include <TColor.h>
#include <TStyle.h>
#include <TPaveText.h>
#include <TGraphErrors.h>
#include <TColor.h>

// Utitlities include
#include "PTS_Utilities.hh"
#include "InverseBetaDecay.hh"



//bool sortFunction(double i, double j) { return  (i<j); }

// Method to draw fission fractions of 235 and 239 for all reactor neutrino experiments
void Draw235And239()
{
  
  // read the input file
  std::ifstream inputFile("./inputs/global+DYB.txt");
  //Check if the file is open
  if (!inputFile.is_open()) {
    printf("Error opening the file\n");
    exit(1);
  }
  
  //  // Define pretty colors
  //  TColor colors[10];
  //  for(int i=0;i<10;i++) colors[i] = new TColor();
  //  colors.clear();
  //  colors[0]=kBlack;
  //  colors[1]=kMagenta+2;
  //  colors[2]=kCyan+2;
  //  colors[3]=kPink-2;
  //  colors[4]=kGreen-2;
  //  colors[5]=kOrange+7;
  //  colors[6]=kGray+1;
  //  colors[7]=kOrange+2;
  //  colors[8]=kAzure-2;
  //  colors[9]=kViolet+2;
  
  
  std::string line;
  
  TVectorD f_235;
  TVectorD f_238;
  TVectorD f_239;
  TVectorD f_241;
  std::multimap<double,double> f_239241Ratio;
  std::multimap<double,std::pair<double,double>> f_239RRatio0;
  std::multimap<double,std::pair<double,double>> f_239RRatio1;
  std::multimap<double,std::pair<double,double>> f_239RRatio2;
  std::multimap<double,std::pair<double,double>> f_239RRatio3;
  std::multimap<double,std::pair<double,double>> f_239RRatio4;
  std::multimap<double,std::pair<double,double>> f_239RRatio5;
  std::multimap<double,std::pair<double,double>> f_239RRatio6;
  std::multimap<double,std::pair<double,double>> f_239RRatio7;
  std::multimap<double,std::pair<double,double>> f_239RRatio8;
  std::multimap<double,std::pair<double,double>> f_239RRatioDef;
  
  
  std::multimap<double,std::pair<double,double>> f_235Ratio;
  std::multimap<double,std::pair<double,double>> f_235DBRatio;
  
  
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr0;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr1;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr2;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr3;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr4;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr5;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr6;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr7;
  std::multimap<double,std::pair<double,double>> f_239RRatioCorr8;
  
  TVectorD ExperimentalSigma;
  
  TVectorD TheoreticalSigma;
  
  TMatrixD f_all;
  f_all.ResizeTo(NISOTOPES,NTOTMEASUREMENTS);
  
  f_235.ResizeTo(NTOTMEASUREMENTS);
  f_238.ResizeTo(NTOTMEASUREMENTS);
  f_239.ResizeTo(NTOTMEASUREMENTS);
  f_241.ResizeTo(NTOTMEASUREMENTS);
  ExperimentalSigma.ResizeTo(NTOTMEASUREMENTS);
  
  TheoreticalSigma.ResizeTo(NTOTMEASUREMENTS);
  
  
  int rowCount = 0;
  while (!inputFile.eof()) {
    int columnCount = 0;
    std::getline(inputFile, line);
    TString inputLine(line);
    TObjArray *objArr = inputLine.Tokenize("	");//Divide the line delimited by =
    
    TIterator *iter=objArr->MakeIterator();
    TObjString *valueRead;
    while((valueRead=(TObjString*)iter->Next())) // run through the covariance matrix
    {
      columnCount++;
      TString valueString=valueRead->GetString();
      
      switch (columnCount) {
        case 1:
          f_235[rowCount]=valueString.Atof();
          break;
        case 2:
          f_238(rowCount)=valueString.Atof();
        case 3:
          f_239[rowCount]=valueString.Atof();
        case 4:
          f_241[rowCount]=valueString.Atof();
          
        default:
          ExperimentalSigma[rowCount]=valueString.Atof();
      }
    }
    rowCount++;
  }
  
  TFile *dbRootFile = new TFile("./PTS_Files/dbFile235239.root","RECREATE");
  dbRootFile->cd();
  
  TGraph *g_239241Ratio=new TGraph();
  g_239241Ratio->SetName("g_239241Ratio");
  int count=0;
  // Make plot of F_239 vs F_239/F_241
  for(int i=0;i<NTOTMEASUREMENTS;i++){
    if(f_239[i]<0.1) continue;
    
    double f_241239= f_241[i]/f_239[i];
    f_239241Ratio.insert(std::pair<double, double>(f_239[i],f_241239));
    count++;
  }
  
  count=0;
  for (std::multimap<double,double>::iterator it=f_239241Ratio.begin(); it!=f_239241Ratio.end(); ++it){
    //    std::cout<< it->first << "   " << it->second <<std::endl;
    g_239241Ratio->SetPoint(count,it->first,it->second);
    count++;
  }
  
  g_239241Ratio->Write();
  
  
  InverseBetaDecay reactorIBD;
  reactorIBD.SetupExperiments();
  TVectorD yTheo(reactorIBD.numberofExp);
  TMatrixD CovarianceMatrix = TMatrixD(reactorIBD.numberofExp, reactorIBD.numberofExp);
  TMatrixD CorrelatedUncertaintyMatrix = TMatrixD(reactorIBD.numberofExp, reactorIBD.numberofExp);
  reactorIBD.TheorIBDCalculator(yTheo,6.69,10.1,4.4,6.03);//+0.3 seconds for 60 bins
  reactorIBD.CovarianceMatrixCalculator(yTheo,CovarianceMatrix);//+2.1 seconds
  reactorIBD.CalculateCorrelatedErrors(yTheo,CorrelatedUncertaintyMatrix);
  
  //  CovarianceMatrix.Print();
  CorrelatedUncertaintyMatrix.Print();
  
  count=0;
  int nGraphs=10;
  TGraphErrors *g_239R[nGraphs];
  TGraphErrors *g_239RCorr[nGraphs];
  TGraphErrors *g_235R=new TGraphErrors();
  TGraphErrors *g_235RDB=new TGraphErrors();
  
  //  int counts[6];
  for(int i=0;i<nGraphs;i++){
    TString buffer;
    buffer.Form("g_239R%i",i);
    g_239R[i]=new TGraphErrors();
    g_239R[i]->SetName(buffer);
    buffer.Form("g_239RCorr%i",i);
    g_239RCorr[i]=new TGraphErrors();
    g_239RCorr[i]->SetName(buffer);
  }
  g_235R->SetName("g_235R");
  g_235RDB->SetName("g_235RDB");
  
  TH2D* ErrorHist=new TH2D("ErrorHist","ErrorHist",40,0,0.4,30,4,7);
  // Make plot of F_239 vs R
  for(int i=0;i<NTOTMEASUREMENTS;i++){
    //        if(f_239[i]<0.1) continue;
    std::pair<double, double> ExperimentalSigmaCov = std::make_pair(ExperimentalSigma[i],sqrt(CovarianceMatrix(count,count)));
    if(i<2)f_239RRatio0.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else if(i>1 && i <7)f_239RRatio1.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else if(i>6 && i <10)f_239RRatio2.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else if(i>9 && i <14)f_239RRatio3.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else if(i>13 && i<16)f_239RRatio4.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else if(i>24)f_239RRatio5.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    else f_239RRatioDef.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    if(i==2 || i==3)f_239RRatio6.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    if(i>3 && i<7)f_239RRatio7.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    if(i>9 && i<13)f_239RRatio8.insert(std::pair<double, std::pair<double, double>>(f_239[i],ExperimentalSigmaCov));
    count++;
  }
  
  count =0;
  for(int i=0;i<NTOTMEASUREMENTS;i++){
    std::pair<double, double> ExperimentalSigmaCov = std::make_pair(ExperimentalSigma[i]/yTheo[i],sqrt(CovarianceMatrix(count,count))/yTheo[i]);
    if(i<25)f_235Ratio.insert(std::pair<double, std::pair<double, double>>(f_235[i],ExperimentalSigmaCov));
    else f_235DBRatio.insert(std::pair<double, std::pair<double, double>>(f_235[i],ExperimentalSigmaCov));
    count++;
  }
  
  
  count =0;
  for(int i=0;i<NTOTMEASUREMENTS;i++){
//    if(i<2)f_239RRatioCorr0.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(0,1))));
//    else if(i>1 && i <7)f_239RRatioCorr1.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(2,4)/yTheo[i])));
//    else if(i>6 && i <10)f_239RRatioCorr2.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(7,8)/yTheo[i])));
//    else if(i>9 && i <14)f_239RRatioCorr3.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(10,13)/yTheo[i])));
//    else if(i>13 && i<16)f_239RRatioCorr4.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(14,15)/yTheo[i])));
//    else if(i>24)f_239RRatioCorr5.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(25,26)/yTheo[i])));
//    if(i==2 || i==3)f_239RRatioCorr6.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(2,3)/yTheo[i])));
//    if(i>3 && i<7)f_239RRatioCorr7.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(4,5)/yTheo[i])));
//    if(i>9 && i<13)f_239RRatioCorr8.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i]/yTheo[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(10,11)/yTheo[i])));

    if(i<2)f_239RRatioCorr0.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(0,1))));
    else if(i>1 && i <7)f_239RRatioCorr1.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(2,4))));
    else if(i>6 && i <10)f_239RRatioCorr2.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(7,8))));
    else if(i>9 && i <14)f_239RRatioCorr3.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(10,13))));
    else if(i>13 && i<16)f_239RRatioCorr4.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(14,15))));
    else if(i>24)f_239RRatioCorr5.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(25,26))));
    if(i==2 || i==3)f_239RRatioCorr6.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(2,3))));
    if(i>3 && i<7)f_239RRatioCorr7.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(4,5))));
    if(i>9 && i<13)f_239RRatioCorr8.insert(std::pair<double, std::pair<double, double>>(f_239[i],std::make_pair(ExperimentalSigma[i],ExperimentalSigma[i]*CorrelatedUncertaintyMatrix(10,11))));
  
  }
  
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio0.begin(); it!=f_239RRatio0.end(); ++it){
    //        std::cout<< it->first << "   " << it->second << "   " <<count<<"   "<< sqrt(CovarianceMatrix(count,count))<<std::endl;
    g_239R[0]->SetPoint(count,it->first,it->second.first);
    g_239R[0]->SetPointError(count,0,it->second.second);
    count++;
  }
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio1.begin(); it!=f_239RRatio1.end(); ++it){
    g_239R[1]->SetPoint(count,it->first,it->second.first);
    g_239R[1]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio2.begin(); it!=f_239RRatio2.end(); ++it){
    g_239R[2]->SetPoint(count,it->first,it->second.first);
    g_239R[2]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio3.begin(); it!=f_239RRatio3.end(); ++it){
    g_239R[3]->SetPoint(count,it->first,it->second.first);
    g_239R[3]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio4.begin(); it!=f_239RRatio4.end(); ++it){
    g_239R[4]->SetPoint(count,it->first,it->second.first);
    g_239R[4]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio5.begin(); it!=f_239RRatio5.end(); ++it){
    g_239R[5]->SetPoint(count,it->first,it->second.first);
    g_239R[5]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio6.begin(); it!=f_239RRatio6.end(); ++it){
    g_239R[6]->SetPoint(count,it->first,it->second.first);
    g_239R[6]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio7.begin(); it!=f_239RRatio7.end(); ++it){
    g_239R[7]->SetPoint(count,it->first,it->second.first);
    g_239R[7]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatio8.begin(); it!=f_239RRatio8.end(); ++it){
    g_239R[8]->SetPoint(count,it->first,it->second.first);
    g_239R[8]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioDef.begin(); it!=f_239RRatioDef.end(); ++it){
    g_239R[9]->SetPoint(count,it->first,it->second.first);
    g_239R[9]->SetPointError(count,0,it->second.second);
    count++;
  }
  
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_235Ratio.begin(); it!=f_235Ratio.end(); ++it){
    g_235R->SetPoint(count,it->first,it->second.first);
    g_235R->SetPointError(count,0,it->second.second);
    count++;
  }
  
  
  count=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_235DBRatio.begin(); it!=f_235DBRatio.end(); ++it){
    g_235RDB->SetPoint(count,it->first,it->second.first);
    g_235RDB->SetPointError(count,0,it->second.second);
    count++;
  }
  
  
  
  count=0;
  int totCount=0;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr0.begin(); it!=f_239RRatioCorr0.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr1.begin(); it!=f_239RRatioCorr1.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr2.begin(); it!=f_239RRatioCorr2.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr3.begin(); it!=f_239RRatioCorr3.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr4.begin(); it!=f_239RRatioCorr4.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr5.begin(); it!=f_239RRatioCorr5.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr6.begin(); it!=f_239RRatioCorr6.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr7.begin(); it!=f_239RRatioCorr7.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  count=0;
  totCount++;
  for (std::multimap<double,std::pair<double,double>>::iterator it=f_239RRatioCorr8.begin(); it!=f_239RRatioCorr8.end(); ++it){
    g_239RCorr[totCount]->SetPoint(count,it->first,it->second.first);
    g_239RCorr[totCount]->SetPointError(count,0.001,it->second.second);
    count++;
  }
  
  //  g_239RCorr[9]->SetPoint(count,it->first,it->second.first);
  //  g_239RCorr[9]->SetPointError(count,0.003,it->second.second);
  
  //  ErrorHist->Fill(it->first,it->second.first,it->second.second);
  
  
  
  //  ErrorHist->Write();
  
  TMultiGraph *mg=new TMultiGraph();
  mg->SetName("mg");
  TMultiGraph *mgCorr=new TMultiGraph();
  mgCorr->SetName("mgCorr");
  
  for(int i=0;i<nGraphs;i++){
    g_239R[i]->SetMarkerStyle(21);
    g_239R[i]->SetMarkerSize(1);
    g_239R[i]->SetLineWidth(2);
    //    g_239R[i]->SetFillColorAlpha(0,0.0);
    //    g_239R[i]->SetMarkerColor(kAzure+5);
    g_239R[i]->SetLineColor(38);
//    if(i==5) g_239R[i]->SetLineColor(46);
    //    g_239R[i]->SetFillColor(0);
    
    if(i!=9){
      g_239RCorr[i]->SetMarkerSize(0);
      g_239RCorr[i]->SetLineWidth(10);
      g_239RCorr[i]->SetLineStyle(i);
      g_239RCorr[i]->SetFillColorAlpha(1,1);
      g_239RCorr[i]->SetFillStyle(3000+i);
      g_239RCorr[i]->Write();
    }
    g_239R[i]->Write();
    
    mg->Add(g_239R[i]);
    if(i!=9){
//      mg->Add(g_239RCorr[i],"AP E2");
      mgCorr->Add(g_239RCorr[i],"AP E1");
    }
  }
  g_235R->SetLineColor(38);
  g_235R->SetMarkerStyle(20);
  g_235R->SetMarkerSize(1);
  g_235R->SetLineWidth(2);
  
  g_235R->GetXaxis()->SetTitle("F_{235}");
  g_235R->GetYaxis()->SetTitle("Ratio");
  g_235R->GetYaxis()->SetTitleOffset(1.5);
  
  g_235R->Write();
  
  
  g_235RDB->SetLineColor(46);
  g_235RDB->SetMarkerStyle(20);
  g_235RDB->SetMarkerSize(1);
  g_235RDB->SetLineWidth(2);
  
  g_235RDB->GetXaxis()->SetTitle("F_{235}");
  g_235RDB->GetYaxis()->SetTitle("Ratio");
  g_235RDB->GetYaxis()->SetTitleOffset(1.5);
  
  g_235RDB->Write();
  
  
  //  g_239R[0]->SetMarkerColor(2);
  //  g_239R[1]->SetMarkerColor(kMagenta+2);
  //  g_239R[2]->SetMarkerColor(kCyan+2);
  //  g_239R[3]->SetMarkerColor(kPink-2);
  //  g_239R[4]->SetMarkerColor(kGreen-2);
  //  g_239R[5]->SetMarkerColor(kOrange+7);
  //  g_239R[6]->SetMarkerColor(kGray+1);
  //  g_239R[7]->SetMarkerColor(kOrange+2);
  //  g_239R[8]->SetMarkerColor(kAzure-2);
  //  g_239R[9]->SetMarkerColor(kBlack);
  
  
  g_239RCorr[0]->SetLineColorAlpha(2,0.2);
  g_239RCorr[1]->SetLineColorAlpha(kMagenta+2,0.2);
  g_239RCorr[2]->SetLineColorAlpha(kCyan+2,0.2);
  g_239RCorr[3]->SetLineColorAlpha(kPink-2,0.2);
  g_239RCorr[4]->SetLineColorAlpha(kGreen-2,0.2);
  g_239RCorr[5]->SetLineColorAlpha(kOrange+7,0.2);
  g_239RCorr[6]->SetLineColorAlpha(kGray+1,0.2);
  g_239RCorr[7]->SetLineColorAlpha(kOrange+2,0.2);
  g_239RCorr[8]->SetLineColorAlpha(kAzure-2,0.2);
  g_239RCorr[9]->SetLineColorAlpha(kBlack,0.2);
  
  
  g_239RCorr[0]->SetFillColorAlpha(2,0.99);
  g_239RCorr[1]->SetFillColorAlpha(kMagenta+2,0.99);
  g_239RCorr[2]->SetFillColorAlpha(kCyan+2,0.99);
  g_239RCorr[3]->SetFillColorAlpha(kPink-2,0.99);
  g_239RCorr[4]->SetFillColorAlpha(kGreen-2,0.99);
  g_239RCorr[5]->SetFillColorAlpha(kOrange+7,0.99);
  g_239RCorr[6]->SetFillColorAlpha(kGray+1,0.99);
  g_239RCorr[7]->SetFillColorAlpha(kOrange+2,0.99);
  g_239RCorr[8]->SetFillColorAlpha(kAzure-2,0.99);
  g_239RCorr[9]->SetFillColorAlpha(kBlack,0.99);
  
  TCanvas *c= new TCanvas("c","c",1200,1200);
  
  //  mg->GetXaxis()->SetRangeUser(0.24,0.37);
  mg->Draw("AP E1");
//    mg->Draw("AP2 E1 E2");
  
  mg->GetXaxis()->SetTitle("F_{239}");
  mg->GetYaxis()->SetTitle("Ratio");
  mg->GetYaxis()->SetTitleOffset(1.5);
  mg->GetXaxis()->SetRangeUser(0.24,0.37);
  
  TLegend *leg = new TLegend(0.2,0.75,0.4,0.85);
  leg->SetBorderSize(0);
//  leg->AddEntry(g_239R[1],"Global Data","L");
//  leg->AddEntry(g_239R[5],"Daya Bay","L");
//  leg->Draw("L");
  
  c->Print("SigmavsF239_Restricted.pdf");
  c->Print("SigmavsF239_Restricted.C");
  mg->Write();
  mgCorr->Draw("AP2 SAME");
  
  mgCorr->GetXaxis()->SetTitle("F_{239}");
  mgCorr->GetYaxis()->SetTitle("Ratio");
  mgCorr->GetYaxis()->SetTitleOffset(1.5);
  mgCorr->GetXaxis()->SetRangeUser(0.24,0.37);
  
//  c->Print("SigmavsF239_Corr.pdf");
//  c->Print("SigmavsF239_Corr.C");
  mg->Write();
  mgCorr->Write();
  
  TMultiGraph *mg235=new TMultiGraph();
  
  mg235->Add(g_235R);
  mg235->Add(g_235RDB);
  
  mg235->Draw("AP E1");
  leg->Draw("LP");
//  c->Print("RvsF235.pdf");
//  c->Print("RvsF235.C");
  
  
  dbRootFile->Close();
  
}

int main() {
  //  FitIBDYieldPerFission();
  Draw235And239();
  return 0;
}
