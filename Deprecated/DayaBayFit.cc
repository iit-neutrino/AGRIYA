////////////////////////////////
// Fitting Daya Bay data
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

// Utitlities include
#include "PTS_Utilities.hh"

// Method to fit IBD yield as a function of P239 fission fraction
void FitIBDYieldPerFission(){
  double covStatList[MATRIXSIZE]; //define stat cov matrix array
  double covSystList[MATRIXSIZE]; //define stat cov matrix array
  FillCovMatrix("./PTS_Files/cov_stat.txt",covStatList); //Fill the cov matrix from the txt file
  FillCovMatrix("./PTS_Files/cov_syst.txt",covSystList);//Fill the cov matrix from the txt file
  
  TMatrixD *covStatMatrix = new TMatrixD(NMEASUREMENTS,NMEASUREMENTS,covStatList); // define stat covariance matrix
  TMatrixD *covSystMatrix = new TMatrixD(NMEASUREMENTS,NMEASUREMENTS,covSystList);// define syst covariance matrix
  TMatrixD covTotMatrix(NMEASUREMENTS,NMEASUREMENTS);// define total covariance matrix
  
  
  
  // read the input file
  std::ifstream inputFile("./PTS_Files/flux_data.txt");
  //Check if the file is open
  if (!inputFile.is_open()) {
    printf("Error opening the file\n");
    exit(1);
  }
  
  std::string line;
  
  // Vectors containing fission fractions of each isotope
  TVectorD f_235;
  TVectorD f_238;
  TVectorD f_239;
  TVectorD f_241;
  
  TVectorD ExperimentalSigma;//Experimental(measured) total IBD yield value as measured by Daya Bay
  
  TVectorD TheoreticalSigma;// Theoretical total IBD yield values to be fit
  
  // Define the size of the vectors
  f_235.ResizeTo(NMEASUREMENTS);
  f_238.ResizeTo(NMEASUREMENTS);
  f_239.ResizeTo(NMEASUREMENTS);
  f_241.ResizeTo(NMEASUREMENTS);
  ExperimentalSigma.ResizeTo(NMEASUREMENTS);
  TheoreticalSigma.ResizeTo(NMEASUREMENTS);
  
  
  int rowCount = 0;
  // Read through the lines of the input file
  while (!inputFile.eof()) {
    int columnCount = 0;
    std::getline(inputFile, line);
    TString inputLine(line); // put the lines in Tstring form, makes it easier to manipulate
    
    if (inputLine.IsWhitespace()) continue;//skip if the line is empty
    if (inputLine.BeginsWith("#")) continue;// skip if the line starts with #
    if (inputLine.BeginsWith("239")) continue;// skip if the line starts with #
    
    TObjArray *objArr = inputLine.Tokenize(" ");//Divide the line delimited by =
    
    TIterator *iter=objArr->MakeIterator(); // define an iterator to iteratre over each word in the line
    TObjString *valueRead;
    while((valueRead=(TObjString*)iter->Next())) // While there is another string in the line
    {
      columnCount++;
      if(columnCount<3) continue;// The first 2 lines are the 239_lo and 239_hi, so not useful
      TString valueString=valueRead->GetString();
      
      // Store the elements from the text files apprpriately in the vectors
      switch (columnCount) {
        case 3:
          f_239[rowCount]=valueString.Atof();
          break;
        case 4:
          f_235(rowCount)=valueString.Atof();
        case 5:
          f_238[rowCount]=valueString.Atof();
        case 6:
          f_241[rowCount]=valueString.Atof();
          
        default:
          ExperimentalSigma[rowCount]=valueString.Atof();
      }
    }
    rowCount++;
  }
  
  double fAvg_239=0.299;// This number comes from Daya Bay paper
  
  
  int nBins=200; // Same number of bins used for x and y axes
  // Through out the rest of the function x stands for P239 fission fractions, and y for IBD yields
  double xMin=-2.2;
  double xMax=-1.6;
  double yMin=5.6;
  double yMax=6.2;
  double xDiff=(xMax-xMin)/nBins;
  double yDiff=(yMax-yMin)/nBins;
  
  // Define the fucntion to store the chi2 distribution
  TH2D* chi2Dist= new TH2D("chi2Dist","chi2Dist;#frac{df}{df_{239}};F_{239}",nBins,xMin,xMax,nBins,yMin,yMax);
  
  double chi2=0;
  // abitrarily large chi2 and min m and c values
  // Here m stand for the slope of line and c for the intercept as in y = mx+c
  double minchi2=1000;
  double minm=1000;
  double minc=1000;
  for(int c=0;c<nBins;c++){
    for(int m=0;m<nBins;m++){
      double sig_bar=(yDiff*c+yMin);// theoretical average IBD yield based on the bin numebrs
      double dsig_df239=(xMax-xDiff*m); // theoretical slope based on the bin numebrs
      
      for(int i=0;i<NMEASUREMENTS;i++){// iterate through 8 cases
        double pred_a=sig_bar + dsig_df239*(f_239[i]-fAvg_239); // predicted ibd yield
        TheoreticalSigma[i]=pred_a;
        TheoreticalSigma[i]=TheoreticalSigma[i]-ExperimentalSigma[i];//Deviation of theoretical from experimental
        for(int j=0;j<NMEASUREMENTS;j++){// Same as previous loop, but only used to define cov matrix
          double pred_b=sig_bar + dsig_df239*(f_239[j]-fAvg_239);
          double tempSystCovTerm=(*covSystMatrix)[i][j];
          double tempStatCovTerm=(*covStatMatrix)[i][j];
          covTotMatrix[i][j]=tempSystCovTerm*pred_a*pred_b + tempStatCovTerm ;// estimate covariance matrix elements cov_{i,j}
        }
      }
      //      covTotMatrix.Print();
      // Invert cov matrix
      covTotMatrix.Invert();
      
      TVectorD tempSigma=TheoreticalSigma;
      tempSigma *=covTotMatrix;//This step is same as (experimental IBD yield-theoretical IBD yield)*covariance matrix
      chi2 = tempSigma*TheoreticalSigma;//This step is same as (experimental IBD yield-theoretical IBD yield)*covariance matrix*(experimental IBD yield-theoretical IBD yield)
      chi2Dist->SetBinContent(nBins-m,c+1,chi2);// Set the bin content, keep in mind x axis decrease with increasing bins
      if(minchi2>chi2) {
        minchi2=chi2;
        minc=sig_bar;
        minm=dsig_df239;
      }
    }//end of m bins
  }// end of c bins
  
  printf("The minimum chi2 is %f, for m = %f and c =%f\n",minchi2,minm,minc);
  
  // defining a vector that corresponds to the line obtained from Daya Bay paper
  TVectorD DayaBaySigma=TheoreticalSigma;
  TVectorD ModelSigma=TheoreticalSigma;
  for(int i=0;i<NMEASUREMENTS;i++){
    TheoreticalSigma[i]=(f_239[i]-fAvg_239)*minm+minc;
    DayaBaySigma[i]=(f_239[i]-fAvg_239)*(-1.86)+5.9;
    ModelSigma[i]=((f_239[i]-fAvg_239)*(-2.46)+6.22);
  }
  
  // Bunch of drawing options, self evident
  TGraph *fG_239=new TGraph(NMEASUREMENTS);
  TGraph *fGT_239=new TGraph(NMEASUREMENTS);
  TGraph *fGDB_239=new TGraph(NMEASUREMENTS);
  
  TGraph *fModel=new TGraph(NMEASUREMENTS);
  
  
  
  for(int i=0;i<NMEASUREMENTS;i++){
    fG_239->SetPoint(i,f_239[i],ExperimentalSigma[i]);
    fGT_239->SetPoint(i,f_239[i],TheoreticalSigma[i]);
    fGDB_239->SetPoint(i,f_239[i],DayaBaySigma[i]);
    fModel->SetPoint(i,f_239[i],ModelSigma[i]);
  }
  
  fGDB_239->SetPoint(NMEASUREMENTS,0,6.45614);
  fModel->SetPoint(NMEASUREMENTS,0,6.95554);
  
  TCanvas *canvas=new TCanvas("c1","c1",1200,600);
  fG_239->SetLineColor(kGreen+3);
  fG_239->SetMarkerStyle(21);
  fG_239->SetMarkerSize(2);
  fG_239->SetMarkerColor(kCyan-2);
  fG_239->SetLineWidth(2);
  fG_239->Draw("AP3");
  canvas->Print("./PTS_Files/db_measurements.pdf");
  fGT_239->SetLineColor(kRed+2);
  fGT_239->SetLineWidth(2);
  fGT_239->Draw("ALP");
  canvas->Print("./PTS_Files/db_fitting.pdf");
  fGDB_239->SetLineColor(kCyan-1);
  fGDB_239->SetLineWidth(2);
  fGDB_239->Draw("ALP");
  canvas->Print("./PTS_Files/db_Actual.pdf");
  
  
  fModel->SetLineColor(kViolet+2);
  fModel->SetLineWidth(2);
  fModel->Draw("ALP");
  canvas->Print("./PTS_Files/hm_model.pdf");
  canvas->Print("./PTS_Files/hm_model.C");
  
  
  
  TGraph *fG_OtherHEU=new TGraph(7);
  
  fG_OtherHEU->SetPoint(0,0,5.2985);
  fG_OtherHEU->SetPoint(1,0,6.1883);
  fG_OtherHEU->SetPoint(2,0,6.3019);
  fG_OtherHEU->SetPoint(3,0,6.2618);
  fG_OtherHEU->SetPoint(4,0,6.3287);
  fG_OtherHEU->SetPoint(5,0,6.2953);
  fG_OtherHEU->SetPoint(6,0,6.7301);
  
  
  fG_OtherHEU->SetMarkerStyle(29);
  fG_OtherHEU->SetMarkerSize(2);
  fG_OtherHEU->SetMarkerColor(kRed);
//  fG_OtherHEU->Draw("AP3");
  
  //  canvas->Setcolzz();
  chi2Dist->Draw("COLZ");
  canvas->Print("./PTS_Files/chi2Dist.pdf");
  /// Actual fitting
  TMultiGraph *mg=new TMultiGraph();
  mg->Add(fG_OtherHEU,"AP");
//  mg->Add(fGT_239);
  mg->Add(fGDB_239);
  mg->Add(fModel);
  mg->Add(fG_239,"AP");
  mg->Draw("ALP");
  mg->GetXaxis()->SetTitle("F_{239}");
  mg->GetYaxis()->SetTitle("#sigma_{f} 10^{-43} cm^{2}/fission");
  TLegend *leg = new TLegend(0.65,0.7,0.9,0.9);
  leg->AddEntry(fG_239,"Daya Bay Data","AP3");
  leg->AddEntry(fG_OtherHEU,"HEU Experiments","AP3");
//  leg->AddEntry(fGT_239,"This fit","AP3");
  leg->AddEntry(fGDB_239,"Daya Bay fit","lP");
  leg->AddEntry(fModel,"Huber Model","lP");
  leg->Draw("lp3");
  canvas->Print("./PTS_Files/mg.pdf");
  
  TFile *dbRootFile = new TFile("./PTS_Files/dbFile.root","RECREATE");
  
  dbRootFile->cd();
  chi2Dist->Write();
  dbRootFile->Close();
  
  TH1D* hXChi2 = Project2DChi2X(chi2Dist);
  printf("For slope 1 sigma is %f\n",GetNSigma(hXChi2,1));
  hXChi2->Draw();
  canvas->Print("./PTS_Files/slope_Chi2.pdf");
  
  TH1D* hYChi2 = Project2DChi2Y(chi2Dist);
  printf("For mean 1 sigma is %f\n",GetNSigma(hYChi2,1));
  hYChi2->Draw();
  canvas->Print("./PTS_Files/Mean_Chi2.pdf");
  
}

// Method to fit IBD yield for 235 and 239 sfission isotopes separately
//*************** For comments check the previous function, most code is same more or less
void Fit235And239()
{
  // Numbers from Daya Bay paper
  double Sigma238=10.1;
  double Sigma241=6.04;
  
  double covStatList[MATRIXSIZE]; //define stat cov matrix array
  double covSystList[MATRIXSIZE]; //define stat cov matrix array
  FillCovMatrix("./PTS_Files/cov_stat.txt",covStatList);
  FillCovMatrix("./PTS_Files/cov_syst.txt",covSystList);
  
  TMatrixD *covStatMatrix = new TMatrixD(NMEASUREMENTS,NMEASUREMENTS,covStatList);
  TMatrixD covTotMatrix(NMEASUREMENTS,NMEASUREMENTS);
  TMatrixD *covSystMatrix = new TMatrixD(NMEASUREMENTS,NMEASUREMENTS,covSystList);
  
  
  
  // read the input file
  std::ifstream inputFile("./PTS_Files/flux_data.txt");
  //Check if the file is open
  if (!inputFile.is_open()) {
    printf("Error opening the file\n");
    exit(1);
  }
  
  std::string line;
  
  TVectorD f_235;
  TVectorD f_238;
  TVectorD f_239;
  float f_239V[8];
  TVectorD f_241;
  TVectorD ExperimentalSigma;
  
  TVectorD TheoreticalSigma;
  
  TMatrixD f_all;
  f_all.ResizeTo(NISOTOPES,NMEASUREMENTS);
  
  f_235.ResizeTo(NMEASUREMENTS);
  f_238.ResizeTo(NMEASUREMENTS);
  f_239.ResizeTo(NMEASUREMENTS);
  f_241.ResizeTo(NMEASUREMENTS);
  ExperimentalSigma.ResizeTo(NMEASUREMENTS);
  
  TheoreticalSigma.ResizeTo(NMEASUREMENTS);
  
  
  int rowCount = 0;
  while (!inputFile.eof()) {
    int columnCount = 0;
    std::getline(inputFile, line);
    TString inputLine(line);
    
    if (inputLine.IsWhitespace()) continue;//skip if the line is empty
    if (inputLine.BeginsWith("#")) continue;// skip if the line starts with #
    if (inputLine.BeginsWith("239")) continue;// skip if the line starts with #
    
    TObjArray *objArr = inputLine.Tokenize(" ");//Divide the line delimited by =
    
    TIterator *iter=objArr->MakeIterator();
    TObjString *valueRead;
    while((valueRead=(TObjString*)iter->Next())) // run through the covariance matrix
    {
      columnCount++;
      if(columnCount<3) continue;
      TString valueString=valueRead->GetString();
      
      if(columnCount<7)f_all[columnCount-3][rowCount]=valueString.Atof();// measured IBD yield for all of them in a matrix
      
      switch (columnCount) {
        case 3:
          f_239[rowCount]=valueString.Atof();
          break;
        case 4:
          f_235(rowCount)=valueString.Atof();
        case 5:
          f_238[rowCount]=valueString.Atof();
        case 6:
          f_241[rowCount]=valueString.Atof();
          
        default:
          ExperimentalSigma[rowCount]=valueString.Atof();
      }
    }
    rowCount++;
  }
  
  
  
  
  
  
  
  
  
  
  
  int nBins=20;
  double xMin=5.2;
  double xMax=7.2;
  double yMin=3.5;
  double yMax=5.0;
  double xDiff=(xMax-xMin)/nBins;
  double yDiff=(yMax-yMin)/nBins;
  TH2D* U235U238Chi2Dist= new TH2D("U235U238Chi2Dist",";#sigma_{235}[10^{-43} cm^{2} / fission];#sigma_{239}[10^{-43} cm2 / fission]",nBins,xMin,xMax,nBins,yMin,yMax);
  
  
  
  
  
  
  double zMin=8;
  double zMax=12;
  double pMin=4;
  double pMax=7;
  double zDiff=(zMax-zMin)/nBins;
  double pDiff=(pMax-pMin)/nBins;
  
  
  
  for(int i235=0;i235<nBins;i235++){
    for(int i239=0;i239<nBins;i239++){
      U235U238Chi2Dist->SetBinContent(i235+1,i239+1,10000);
    }
  }
  
  
  double chi2=0;
  double minchi2;
  double min235=100;
  double min239=100;
  for(int i235=0;i235<nBins;i235++){
    double v235=xMin+xDiff*i235;
    for(int i239=0;i239<nBins;i239++){
      minchi2 =10000;
      double v239=yMin+yDiff*i239;
      
      for(int i238=0;i238<nBins;i238++){
        
        double v238=zMin+zDiff*i238;
        for(int i241=0;i241<nBins;i241++){
          double v241=pMin+pDiff*i241;
          
          for(int i=0;i<NMEASUREMENTS;i++){
            double pred_a=f_235[i]*v235+f_238[i]*v238+f_239[i]*v239+f_241[i]*v241;
            // This is same as doing f*σ (equation 5 in Daya bay paper) where f_{ij} is fission fraction for each isotope for all measurements and σ is the IBD yield of each isotope
            TheoreticalSigma[i]=pred_a;
            TheoreticalSigma[i]=TheoreticalSigma[i]-ExperimentalSigma[i];
            for(int j=0;j<NMEASUREMENTS;j++){
              double pred_b=f_235[j]*v235+f_238[j]*v238+f_239[j]*v239+f_241[j]*v241;
              double tempSystCovTerm=(*covSystMatrix)[i][j];
              double tempStatCovTerm=(*covStatMatrix)[i][j];
              covTotMatrix[i][j]=(tempSystCovTerm)*pred_a*pred_b + tempStatCovTerm;
            }
          }
          covTotMatrix.Invert();
          TVectorD tempSigma=TheoreticalSigma;
          tempSigma *=covTotMatrix;
          chi2 = tempSigma*TheoreticalSigma;
          chi2+=pow(v238-Sigma238,2)/(pow(Sigma238/10,2)) + pow(v241-Sigma241,2)/(pow(Sigma241/10,2));
          if(U235U238Chi2Dist->GetBinContent(i235+1,i239+1)>chi2)U235U238Chi2Dist->SetBinContent(i235+1,i239+1,chi2);
        }
      }
      
      if(minchi2>chi2) {
        minchi2=chi2;
        min235=v235;
        min239=v239;
      }
    }
  }
  
  TFile *dbRootFile = new TFile("./PTS_Files/dbFile235239.root","RECREATE");
  dbRootFile->cd();
  
  TGraph *f_239241Ratio=new TGraph(NMEASUREMENTS);
  // Make plot of F_239 vs F_239/F_241
  for(int i=0;i<NMEASUREMENTS;i++){
    double f_241239= f_241[i]/f_239[i];
    f_239241Ratio->SetPoint(i,f_239[i],f_241239);
    std::cout<< f_239[i] << "  " <<f_241[i] << "     " << f_241239<<std::endl;
  }
  
  printf("The minimum chi2 is %f, for v235 = %f and v239 =%f\n",minchi2,min235,min239);
  
  f_239241Ratio->Write();
  U235U238Chi2Dist->Write();
  
  TH1D* Chi2U235=Project2DChi2X(U235U238Chi2Dist);
  Chi2U235->GetYaxis()->SetTitle("#Delta #chi^{2}");
  TH1D* Chi2U239=Project2DChi2Y(U235U238Chi2Dist);
  Chi2U239->GetYaxis()->SetTitle("#Delta #chi^{2}");
  
  
  int x,y,z;
  U235U238Chi2Dist->GetMinimumBin(x,y,z);
  min235 = U235U238Chi2Dist->GetXaxis()->GetBinCenter(x);
  min239 = U235U238Chi2Dist->GetYaxis()->GetBinCenter(y);
  
  
  Chi2U235->Write();
  Chi2U239->Write();
  gStyle->SetOptStat("");
  
  printf("For U235 1 sigma is %f\n",GetNSigma(Chi2U235,1));
  printf("For U239 1 sigma is %f\n",GetNSigma(Chi2U239,1));
  TCanvas * canvas = new TCanvas("c235238","c235238",1200,1200);
  TPad *pad1 = new TPad("pad1","pad1",0.05,0.05,0.65,0.7);
  TPad *pad2 = new TPad("pad2","pad2",0.05,0.7,0.65,0.98);
  TPad *pad3 = new TPad("pad3","pad3",0.65,0.05,0.98,0.7);
  TPad *pad4 = new TPad("pad4","pad4",0.65,0.7,0.98,0.98);
  
  
  pad1->SetRightMargin(0);
  pad1->SetTopMargin(0);
  pad1->SetGridx(2);
  pad1->SetGridy(2);
  pad1->SetTickx();
  pad1->SetTicky();
  pad1->Draw();
  
  pad2->SetGridx();
  pad2->SetGridy();
  pad2->SetRightMargin(0);
  pad2->SetBottomMargin(0);
  pad2->SetTickx();
  pad2->SetTicky();
  pad2->Draw();
  
  pad3->SetGridx();
  pad3->SetGridy();
  pad3->SetTopMargin(0);
  pad3->SetLeftMargin(0);
  pad3->SetTickx();
  pad3->SetTicky();
  pad3->Draw();
  
  pad4->SetLeftMargin(0);
  pad4->SetBottomMargin(0);
  pad4->Draw();
  
  
  
  pad2->cd();
  Chi2U235->GetYaxis()->SetRangeUser(0,10);
  Chi2U235->GetXaxis()->SetLabelOffset(999);
  Chi2U235->GetXaxis()->SetLabelSize(0);
  Chi2U235->SetFillColorAlpha(30,0.4);
  Chi2U235->Draw("bar0");
  pad3->cd();
  
  Chi2U239->GetYaxis()->SetRangeUser(0,10);
  Chi2U239->GetXaxis()->SetLabelOffset(999);
  Chi2U239->GetXaxis()->SetLabelSize(0);
  Chi2U239->SetLineColor(2);
  Chi2U239->SetFillColorAlpha(30,0.4);
  Chi2U239->Draw("hbar0");
  
  
  pad1->cd();
  Int_t colors[] = {1,kGreen-5, kGreen-8,kGreen-10, kWhite}; // #colors >= #levels - 1
  gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
  
  double contourSig[4];
  double minChi2=U235U238Chi2Dist->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  U235U238Chi2Dist->SetContour(4, contourSig);
  
  TGraph *BFPoint = new TGraph(1);
  double xpoint[1] = {min235};
  double ypoint[1] = {min239};
  //  TGraph *BFPoint = new TGraph(1,xpoint,ypoint);
  BFPoint->SetPoint(0,min235,min239);
  //  BFPoint->SetPoint(1,6,5);
  xpoint[0] = 6.17;
  ypoint[0] = 4.27;
  TGraph *DBBFPoint = new TGraph(1,xpoint,ypoint);
  //  BFPoint->SetPoint(1,min235,min239);
  //  DBBFPoint->SetPoint(1,6.17,4.27);
  //
  TMultiGraph *mg = new TMultiGraph();
  mg->Add(BFPoint);
  mg->Add(DBBFPoint);
  
  DBBFPoint->SetMarkerStyle(22);
  DBBFPoint->SetMarkerSize(3);
  DBBFPoint->SetMarkerColorAlpha(kRed+1,1);
  
  BFPoint->SetMarkerStyle(23);
  BFPoint->SetMarkerSize(3);
  BFPoint->SetMarkerColorAlpha(kBlack,1);
  
  TLegend *leg1 = new TLegend(0.7,0.2,0.9,0.25);
  leg1->SetBorderSize(0);
  TLegend *leg2 = new TLegend(0.7,0.25,0.9,0.3);
  leg2->SetBorderSize(0);
  TLegend *leg3 = new TLegend(0.7,0.3,0.9,0.35);
  leg3->SetBorderSize(0);
  TLegend *leg4 = new TLegend(0.15,0.8,0.4,0.9);
  leg4->SetBorderSize(0);
  
  U235U238Chi2Dist->GetXaxis()->SetTitleOffset(1.5);
  U235U238Chi2Dist->GetYaxis()->SetTitleOffset(1.5);
  //  BFPoint->Draw("APSame");
  
    U235U238Chi2Dist->Draw("cont4Same");
  
  //  mg->Draw("APSAME");
  //  U235U238Chi2Dist->Draw("cont4Same");
  //  canvas->Update();
  //  DBBFPoint->Draw("APSame");
  
  // Dummy histos because ROOT is weird with drawing legends
  TH2D *U235U238Chi2Dist1=(TH2D*)U235U238Chi2Dist->Clone("U235U238Chi2Dist1");
  TH2D *U235U238Chi2Dist2=(TH2D*)U235U238Chi2Dist->Clone("U235U238Chi2Dist2");
  
  U235U238Chi2Dist->SetFillColor(kGreen-5);
  leg1->AddEntry(U235U238Chi2Dist,"1 #sigma","f");
  leg1->Draw("f");
  U235U238Chi2Dist1->SetFillColor(kGreen-8);
  leg2->AddEntry(U235U238Chi2Dist1,"2 #sigma","f");
  leg2->Draw("f");
  U235U238Chi2Dist2->SetFillColor(kGreen-10);
  leg3->AddEntry(U235U238Chi2Dist2,"3 #sigma","f");
  leg3->Draw("f");
  
  
  leg4->AddEntry(DBBFPoint,"Daya Bay","P");
  leg4->AddEntry(BFPoint,"This Analysis","P");
  leg4->Draw("f");
  
  pad4->cd();
  TPaveText *pt= new TPaveText(0,0,0.98,0.98);
  
  pt->SetMargin(2);
  pt->SetBorderSize(0);
  pt->SetFillColor(0);
  //  pt->AddText("V_{ij}^{tot} = mV_{ij}^{syst} #sigma_{i,theo}#sigma_{j,theo} +V_{ij}^{stat} +f_{i,238}f_{j,238} +0.36f_{i,241}f_{j,241}");
  pt->AddText("V_{ij}^{tot} = mV_{ij}^{syst} #sigma_{i,theo}#sigma_{j,theo} +V_{ij}^{stat}");
  TString bufferString;
  bufferString.Form("Best fit point is (%f,%f)",min235,min239);
  pt->AddText(bufferString);
  bufferString.Form("Daya Bay Best fit point is (%f,%f)",6.17,4.27);
  pt->AddText(bufferString);
  bufferString.Form("with #chi^{2} = %f",minChi2);
  pt->AddText(bufferString);
  bufferString.Form("#sigma_{235} (DB) = %f (%f)",GetNSigma(Chi2U235,1),0.17);
  pt->AddText(bufferString);
  bufferString.Form("#sigma_{239} (DB) = %f(%f)",GetNSigma(Chi2U239,1),0.26);
  pt->AddText(bufferString);
  
  pt->Draw();
  
  canvas->Print("./PTS_Files/U235239Fit.pdf");
  
  dbRootFile->Close();
  
}

int main() {
    FitIBDYieldPerFission();
//  Fit235And239();
  return 0;
}