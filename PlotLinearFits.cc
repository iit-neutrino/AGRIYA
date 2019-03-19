////////////////////////////////////////////////
// File to plot histograms
////////////////////////////////////////////////

#include "Global_Utilities.hh"
#include "TKey.h"
#include <TStyle.h>
#include "TLegend.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TObjString.h"
#include "StyleFile.hh"
using namespace std;

void GenTheoIBDYield(double &avg,double &sigma,double ff5,double ff8,double ff9,double ff1){
//  std::cout << ff5+ff8+ff9+ff1<<std::endl;
//  assert((ff5+ff8+ff9+ff1)==1);
  double xSectionSH5=6.69;
  double xSectionSH8=10.10;
  double xSectionSH9=4.40;
  double xSectionSH1=6.03;
  double xSSectionSH5=0.0267;
  double xSSectionSH8=0.6776;
  double xSSectionSH9=0.0161;
  double xSSectionSH1=0.0246;
  avg= xSectionSH5*ff5+ xSectionSH8*ff8+ xSectionSH9*ff9+ xSectionSH1*ff1;
  sigma= TMath::Sqrt(xSSectionSH5*ff5*ff5+ xSSectionSH8*ff8*ff8+ xSSectionSH9*ff9*ff9+ xSSectionSH1*ff1*ff1);
  
//  slope=1.86;
}
//1.86
//0.18

void GenerateErrors(TGraphErrors &g1,TGraphErrors &g2,const TGraphErrors &cg1,const TGraphErrors &cg2){
  double x,y;
  
  TMatrixD Syst_CovarianceMatrix(8,8);
  int rowCounter = 0;
  int columnCounter = 0;
  ifstream systFileIn;
  double numberRead;   ///The number currently read from the file
  string lineRead;    ///The line of text file currently read
  systFileIn.open("inputs/DYB_ImprovedDE_covsyst.txt");
  while(systFileIn.good()){
    while(getline(systFileIn, lineRead)){
      istringstream streamA(lineRead);
      columnCounter = 0;
      while(streamA >>numberRead){
        Syst_CovarianceMatrix(rowCounter,columnCounter) = numberRead;
        columnCounter++;
      }
      rowCounter++;
    }
  }
  
  for(int i=0;i<cg1.GetN();i++){
    cg1.GetPoint(i,x,y);
    g1.SetPoint(i,x,y);
    g1.SetPointError(i,0,y*0.02);
    cg2.GetPoint(i,x,y);
    g2.SetPoint(i,x,y);
    g2.SetPointError(i,0,y*TMath::Sqrt(Syst_CovarianceMatrix(i,i)));
  }
}

void RebinGtoZero(TGraph &g,double minVal){
  double x,y;
  for(int i=0;i<g.GetN();i++){
    g.GetPoint(i,x,y);
    g.SetPoint(i,x,y-minVal);
  }
}

void usage(){
  std::cout << "Example: PlotLinearFits file1.root file2.root outputfolder\n";
  exit(1);
}

int main(int argc, char *argv[]){
  
  if(argc!=4) usage();
  
  gStyle->SetOptStat("");
  gStyle->SetHatchesSpacing(4);
  gStyle->SetHatchesLineWidth(5);
  TString inputFileName=argv[1];
  TFile *inputFile1=TFile::Open(inputFileName);
  TFile *inputFile2=TFile::Open(argv[2]);
  
  // output ROOT file for saving plots
  TString outFolder = argv[3];
  outFolder.Append("/");
  inputFileName.ReplaceAll(".root","");
  TObjArray *objList=inputFileName.Tokenize("/");
  inputFileName=((TObjString*)objList->Last())->GetString();
  outFolder.Append(inputFileName);
  TCanvas *c = new TCanvas("c","c",1200,600);
  c->SetLeftMargin(0.12);
  c->SetBottomMargin(0.15);
  
  gStyle->SetEndErrorSize(4);
  setupStyle();
  TMultiGraph *mg=new TMultiGraph();
  TGraphErrors *gExp1=(TGraphErrors*)inputFile1->Get("g_IBD_Exp");
  TGraphErrors *gFit1=(TGraphErrors*)inputFile1->Get("g_IBD_Fit");
  TGraphErrors *gExp2=(TGraphErrors*)inputFile2->Get("g_IBD_Exp");
  TGraphErrors *gFit2=(TGraphErrors*)inputFile2->Get("g_IBD_Fit");
  TGraphErrors *gExpSysts1=new TGraphErrors();
  TGraphErrors *gExpSysts2=new TGraphErrors();
  GenerateErrors(*gExpSysts1,*gExpSysts2,*gExp1,*gExp2);
  
  mg->Add(gExp1,"APE");
  mg->Add(gFit1,"AL");
  mg->Add(gExp2,"APE");
  mg->Add(gFit2,"AL");
  mg->Add(gExpSysts1,"A3");
  mg->Add(gExpSysts2,"A3");
  
  mg->Draw("ALPE");
  //  mg->GetYaxis()->SetRangeUser(5.7,6.1);
  mg->GetXaxis()->SetTitle("#it{F}_{239}");
//  mg->GetXaxis()->SetRangeUser(0.252,gExpSysts1[8]);
  mg->GetYaxis()->SetTitle("#sigma_{#it{f}}");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  mg->GetYaxis()->SetLabelOffset(0.01);
  
  
  TLine *mlineD = new TLine(6.22881,0,6.22881,10);
  TLine *mline = new TLine(6.223365,0,6.223365,10);
  TGraphErrors *mlineF = new TGraphErrors();
  mlineF->SetPoint(0,6.099049,5);
  mlineF->SetPoint(1,6.347681,5);
  mlineF->SetPointError(0,0,5);
  mlineF->SetPointError(1,0,5);
  TLine *mlineR = new TLine(6.21792,0,6.21792,10);
  mline->SetLineWidth(8);
  //  lineF->SetLineWidth(20);
  mlineF->SetFillColorAlpha(1,0.2);
  mlineF->SetFillStyle(1001);
  //  lineF->Draw("a3SAME");
  //  lineD->Draw();
  //  lineR->Draw();
  
  TLine *sline = new TLine(-2.46,0,-2.46,10);
  TGraphErrors *slineF = new TGraphErrors();
  slineF->SetPoint(0,-2.4,5);
  slineF->SetPoint(1,-2.52,5);
  slineF->SetPointError(0,0,5);
  slineF->SetPointError(1,0,5);
  sline->SetLineWidth(2);
  slineF->SetFillColorAlpha(1,0.2);
  slineF->SetFillStyle(1001);
  
  gExp1->SetMarkerStyle(9);
  gExp1->SetMarkerColor(kRed);
  gExp1->SetMarkerSize(1.5);
  gExp1->SetLineWidth(2);
  gExp1->SetLineColor(kRed);
  gFit1->SetLineColor(kRed);
  gFit1->SetLineWidth(2);
  gFit1->SetLineStyle(1);
  gFit1->SetMarkerColor(kRed);
  gExpSysts1->SetLineColorAlpha(kRed,1);
  gExpSysts1->SetLineWidth(1);
  gExpSysts1->SetFillColorAlpha(kRed,0.7);
  gExpSysts1->SetFillStyle(3004);
  
  gExp2->SetMarkerStyle(20);
  gExp2->SetMarkerColor(kBlue);
  gExp2->SetLineWidth(2);
  gExp2->SetMarkerSize(1.5);
  gExp2->SetLineColor(kBlue);
  gFit2->SetLineColor(kBlue);
  gFit2->SetLineWidth(2);
  gFit2->SetLineStyle(1);
  gExpSysts2->SetLineColorAlpha(kBlue,1);
  gExpSysts2->SetLineWidth(1);
  gExpSysts2->SetFillColorAlpha(kBlue,0.7);
  gExpSysts2->SetFillStyle(3005);
  
  TLegend *leg= new TLegend(0.7,0.65,0.88,0.88);
  leg->SetBorderSize(0);
  leg->AddEntry(gExp2,"Daya Bay Data","PE");
  leg->AddEntry(gFit2,"Daya Bay Fit");
  leg->AddEntry(gExp1,"RENO Data","PE");
  leg->AddEntry(gFit1,"RENO Fit");
//  leg->AddEntry(slineF,"Predicted","LF");
  leg->Draw();
  //  gExp1->SetMarkerSize(20);
  
  TString outFile(outFolder);
  outFile.Append("LinFit.pdf");
  c->Print(outFile);
  
  TCanvas *c1 = new TCanvas("c","c",600,600);
  c1->SetLeftMargin(0.2);
  c1->SetBottomMargin(0.2);
  TVectorD *minValVector=(TVectorD*)inputFile1->Get("minValues");
  double minimum1=minValVector[0][10];
  minValVector=(TVectorD*)inputFile2->Get("minValues");
  double minimum2=minValVector[0][10];
  
  
  setupStyle();
  TMultiGraph *mg1=new TMultiGraph();
  TGraph *g1235=(TGraph*)inputFile1->Get("U235");
  TGraph *g1238=(TGraph*)inputFile1->Get("U238");
  TGraph *g2235=(TGraph*)inputFile2->Get("U235");
  TGraph *g2238=(TGraph*)inputFile2->Get("U238");
  RebinGtoZero(*g1235,minimum1);
  RebinGtoZero(*g1238,minimum1);
  RebinGtoZero(*g2235,minimum2);
  RebinGtoZero(*g2238,minimum2);
  mg1->Add(g1235,"AC");
  mg1->Add(g2235,"AC");
  mg1->Add(mlineF,"A3");
  mg1->Draw("A3C");
  gPad->SetTicks(1,1);
  mg1->GetXaxis()->SetRangeUser(5.5,6.4);
  mg1->GetYaxis()->SetRangeUser(0,10);
  mg1->GetXaxis()->SetTitle("#bar#sigma_{#it{f}}");
  mg1->GetYaxis()->SetTitle("#Delta#chi^{2}");
  mg1->GetXaxis()->CenterTitle();
  mg1->GetYaxis()->CenterTitle();
  mg1->GetXaxis()->SetTitleOffset(1);
  mg1->GetYaxis()->SetTitleOffset(1.5);
  mg1->GetXaxis()->SetNdivisions(505);
  mg1->GetYaxis()->SetLabelOffset(0.01);
  
  g1235->SetLineColor(kRed);
  g1238->SetLineColor(kRed);
  g2235->SetLineColor(kBlue);
  g2238->SetLineColor(kBlue);
  g1238->SetMarkerColor(kRed);
  g2238->SetMarkerColor(kBlue);
  g1235->SetLineWidth(2);
  g1238->SetLineWidth(2);
  g2235->SetLineWidth(2);
  g2238->SetLineWidth(2);
  outFile.ReplaceAll("LinFit.pdf","Mean.pdf");
  mline->Draw();
  c1->Print(outFile);
  
  c1->Clear();
  TMultiGraph *mg2=new TMultiGraph();
  mg2->Add(g1238,"AC");
  mg2->Add(g2238,"AC");
  mg2->Draw("AC");
  mg2->Add(slineF,"A3");
  gPad->SetTicks(1,1);
  //  mg2->GetYaxis()->SetTicks(0,1);
  mg2->GetXaxis()->SetRangeUser(-2.9,-1.0);
  mg2->GetXaxis()->SetNdivisions(505);
  mg2->GetYaxis()->SetRangeUser(0,10);
  mg2->GetXaxis()->SetTitle("#it{d#sigma_{f}/dF}_{239}");
  mg2->GetYaxis()->SetTitle("#sigma_{#it{f}}");
  mg2->GetXaxis()->CenterTitle();
  mg2->GetYaxis()->CenterTitle();
  mg2->GetXaxis()->SetTitleOffset(1);
  mg2->GetYaxis()->CenterTitle(1);
  mg2->GetYaxis()->SetLabelOffset(0.01);
  TLegend *leg1= new TLegend(0.45,0.8,0.8,0.95);
  leg1->SetBorderSize(0);
  outFile.ReplaceAll("Mean.pdf","Intercept.pdf");
  sline->Draw();
  leg1->AddEntry(g2238,"Daya Bay");
  leg1->AddEntry(g1238,"RENO");
  slineF->SetLineWidth(2);
  leg1->AddEntry(slineF,"Prediction","LF");
  leg1->Draw("same");
  c1->Print(outFile);
  
  double avg, sigma;
  GenTheoIBDYield(avg,sigma,0.571,0.076,0.299,0.054);
  std::cout << avg <<" , " <<sigma <<std::endl;
  GenTheoIBDYield(avg,sigma,0.573,0.073,0.299,0.055);
  std::cout << avg <<" , " <<sigma <<std::endl;
  return 0;
}
