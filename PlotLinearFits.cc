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
  mg->Add(gExpSysts1,"AEZ");
  mg->Add(gExpSysts2,"AEZ");
  
  mg->Draw("ALPE");
  //  mg->GetYaxis()->SetRangeUser(5.7,6.1);
  mg->GetXaxis()->SetTitle("#it{F}_{239}");
  mg->GetYaxis()->SetTitle("#sigma_{#it{f}}");
  mg->GetXaxis()->CenterTitle();
  mg->GetYaxis()->CenterTitle();
  
  
  
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
  
  gExp2->SetMarkerStyle(20);
  gExp2->SetLineWidth(2);
  gExp2->SetMarkerSize(1.5);
  gFit2->SetLineWidth(2);
  gFit2->SetLineStyle(1);
  gExpSysts2->SetLineColorAlpha(kBlack,1);
  gExpSysts2->SetLineWidth(1);
  
  TLegend *leg= new TLegend(0.7,0.6,0.88,0.88);
  leg->SetBorderSize(0);
  leg->AddEntry(gExp2,"Daya Bay Data","PE");
  leg->AddEntry(gFit2,"Daya Bay Fit");
  leg->AddEntry(gExp1,"RENO Data","PE");
  leg->AddEntry(gFit1,"RENO Fit");
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
  mg1->Draw("AC");
  gPad->SetTicks(1,1);
  mg1->GetXaxis()->SetRangeUser(5.5,6.3);
  mg1->GetYaxis()->SetRangeUser(0,10);
  mg1->GetXaxis()->SetTitle("#sigma_{#it{f}}");
  mg1->GetYaxis()->SetTitle("#Delta#chi^{2}");
  mg1->GetXaxis()->CenterTitle();
  mg1->GetYaxis()->CenterTitle();
  mg1->GetXaxis()->SetTitleOffset(1);
  mg1->GetYaxis()->SetTitleOffset(1.5);
  mg1->GetXaxis()->SetNdivisions(505);
  
  g1235->SetLineColor(kRed);
  g1238->SetLineColor(kRed);
  g1235->SetLineWidth(2);
  g1238->SetLineWidth(2);
  g2235->SetLineWidth(2);
  g2238->SetLineWidth(2);
  TLegend *leg1= new TLegend(0.4,0.70,0.65,0.88);
  leg1->SetBorderSize(0);
  leg1->AddEntry(g2235,"Daya Bay");
  leg1->AddEntry(g1235,"RENO");
  //  leg1->Draw();
  outFile.ReplaceAll("LinFit.pdf","Mean.pdf");
  c1->Print(outFile);
  
  c1->Clear();
  TMultiGraph *mg2=new TMultiGraph();
  mg2->Add(g1238,"AC");
  mg2->Add(g2238,"AC");
  mg2->Draw("AC");
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
  outFile.ReplaceAll("Mean.pdf","Intercept.pdf");
  c1->Print(outFile);
  
  
  return 0;
}
