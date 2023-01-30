////////////////////////////////////////////////
// File to plot histograms
////////////////////////////////////////////////

#include <iostream>

#include "TKey.h"
#include "TLegend.h"
#include "TObjString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TMultiGraph.h"
#include "TPaletteAxis.h"
#include "TPaveText.h"
#include "TMath.h"
#include "TMarker.h"


#include "StyleFile.hh"

using namespace std;

// double yields[5]={6.69,10.10,4.40,4.96,6.03};

void MinimizeToZero(TGraph &gIn,double minChi2){
  double x,y;
  for(int i=0;i<gIn.GetN();i++){
    gIn.GetPoint(i,x,y);
    gIn.SetPoint(i,x,y-minChi2);
  }
}

void ConvertAbsoluteToRelativeGraph(const TGraph &gIn,TGraph &gOut,double Scaling){
  double x,y;
  for(int i=0;i<gIn.GetN();i++){
    gIn.GetPoint(i,x,y);
    gOut.SetPoint(i,x/Scaling,y);
  }
}

bool ConvertCovarianceToUncertainty(TH2D &h, std::vector<double> yields)
{
  TH2D *hCopy= new TH2D(h);
  h.Clear();
  for(int i=1; i<h.GetNbinsX()+1; i++)
  {
    for(int j=1; j<h.GetNbinsY()+1; j++)
    {
      double binContent = TMath::Sqrt(TMath::Abs(hCopy->GetBinContent(i,j)));
      h.SetBinContent(i,j,binContent*100/(TMath::Sqrt(yields.at(i-1)*yields.at(j-1))));
    }
  }
  return true;
}


void usage(){
  std::cout << "Example: PlotGraphs file.root outputfolder isFuture draw240\n";
  exit(1);
}

int main(int argc, char *argv[])
{  
  int isFuture=0;
  int draw240=0;
  if(argc!=3)
  {
    if(argc!=4)
    {
      if(argc!=5) usage();
      else draw240= atoi(argv[4]); // If 0, the dont draw otherwise draw
    }
    isFuture= atoi(argv[3]); // If 0, this is for future experiemnts
  }
  printf("                         NOTE                         \n");
  printf("-----------------------------------------------------------\n");
  if(isFuture==0) printf("Making plots for future hypothetical experiments\n");
  else printf("Making plots for existing experimental data\n");
  if(draw240!=0) printf("Drawing curves for Pu 240 as well\n");
  printf("-----------------------------------------------------------\n");
  
  TStyle *style = gStyle;
  setupStyle(style);
  TString inputFileName=argv[1];
  TFile *inputFile=TFile::Open(inputFileName);
  
  // output ROOT file for saving plots
  TString outFolder = argv[2];
  outFolder.Append("/");
  inputFileName.ReplaceAll(".root","");
  TObjArray *objList=inputFileName.Tokenize("/");
  inputFileName=((TObjString*)objList->Last())->GetString();
  outFolder.Append(inputFileName);
  
  TCanvas *c = new TCanvas("c","c",1200,1000);

  TString outFile(outFolder);

  TLegend *leg=new TLegend(0.23,0.8,0.85,0.90);
  leg->SetNColumns(2);
  leg->SetColumnSeparation(0.05);
  // gStyle->SetLegendTextSize(0.04);
  leg->SetFillColorAlpha(kWhite,0.8);
  
  double U235Theo=6.69;
  if(isFuture!=0) U235Theo=6.69;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P240Theo=4.96;
  double P241Theo=6.03;
 
  outFile.Append("1D");
  outFile.Append(".pdf");
  
  TVectorD *minValVector=(TVectorD*)inputFile->Get("minValues");
  double minimum=minValVector[0][14];
  
  TMultiGraph *mg=new TMultiGraph();
  TGraph *g235=(TGraph*)inputFile->Get("U235");
  TGraph *g238=(TGraph*)inputFile->Get("U238");
  TGraph *g239=(TGraph*)inputFile->Get("P239");
  TGraph *g240;
  TGraph *g241=(TGraph*)inputFile->Get("P241");
  if(draw240!=0) g240=(TGraph*)inputFile->Get("P240");
  
  // output1D
  MinimizeToZero(*g235,minimum);
  MinimizeToZero(*g238,minimum);
  MinimizeToZero(*g239,minimum);
  if(draw240!=0) MinimizeToZero(*g240,minimum);
  MinimizeToZero(*g241,minimum);
  mg->Add(g235,"AC");
  mg->Add(g238,"AC");
  mg->Add(g239,"AC");
  if(draw240!=0) mg->Add(g240,"AC");
  mg->Add(g241,"AC");
  mg->Draw("AC");
  g235->SetLineWidth(3);
  g238->SetLineWidth(3);
  g239->SetLineWidth(3);
  if(draw240!=0) g240->SetLineWidth(3);
  g241->SetLineWidth(3);
  g235->SetLineColor(kViolet+2);
  g238->SetLineColor(kAzure+1);
  g239->SetLineColor(kGreen+1);
  if(draw240!=0) g240->SetLineColor(kBlack);
  g241->SetLineColor(kPink+7);
  TString legString;
  if(isFuture!=0) legString.Form("#sigma_{235} = %2.3f #pm %2.3f",minValVector[0][0],minValVector[0][7]);
  else legString.Form("#sigma_{#sigma_{235}} = #pm %2.2f",minValVector[0][7]);
  leg->AddEntry(g235,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{238} = %2.3f #pm %2.3f",minValVector[0][1],minValVector[0][8]);
  else legString.Form("#sigma_{#sigma_{238}} = #pm %2.2f",minValVector[0][8]);
  leg->AddEntry(g238,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{239} = %2.3f #pm %2.3f",minValVector[0][2],minValVector[0][9]);
  else legString.Form("#sigma_{#sigma_{239}} = #pm %2.2f",minValVector[0][9]);
  leg->AddEntry(g239,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{240} = %2.3f #pm %2.3f",minValVector[0][3],minValVector[0][10]);
  else legString.Form("#sigma_{#sigma_{240}} = #pm %2.2f",minValVector[0][10]);
  if(draw240!=0) leg->AddEntry(g240,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{241} = %2.3f #pm %2.3f",minValVector[0][4],minValVector[0][11]);
  else legString.Form("#sigma_{#sigma_{241}} = #pm %2.2f",minValVector[0][11]);
  leg->AddEntry(g241,legString,"l");
  mg->GetXaxis()->SetRangeUser(2,12);
  mg->GetXaxis()->SetNdivisions(520);
  mg->GetYaxis()->SetRangeUser(0,10);
  mg->GetYaxis()->SetTitle("#Delta #chi2");
  mg->GetXaxis()->SetTitle("#sigma_{i} [10^{-43}cm^{2}/fission]");
  leg->Draw();
  c->Print(outFile);
  
  // output1DScaled
  TGraph *gS235=new TGraph();
  TGraph *gS238=new TGraph();
  TGraph *gS239=new TGraph();
  TGraph *gS240;
  if(draw240!=0) gS240=new TGraph();
  TGraph *gS241=new TGraph();
  ConvertAbsoluteToRelativeGraph(*g235,*gS235,U235Theo);
  ConvertAbsoluteToRelativeGraph(*g238,*gS238,U238Theo);
  ConvertAbsoluteToRelativeGraph(*g239,*gS239,P239Theo);
  if(draw240!=0) ConvertAbsoluteToRelativeGraph(*g240,*gS240,P240Theo);
  ConvertAbsoluteToRelativeGraph(*g241,*gS241,P241Theo);
  mg->RecursiveRemove(g235);
  mg->RecursiveRemove(g238);
  mg->RecursiveRemove(g239);
  if(draw240!=0) mg->RecursiveRemove(g240);
  mg->RecursiveRemove(g241);
  mg->Add(gS235,"AC");
  mg->Add(gS238,"AC");
  mg->Add(gS239,"AC");
  if(draw240!=0) mg->Add(gS240,"AC");
  mg->Add(gS241,"AC");
  mg->Draw("AC");
  gS235->SetLineWidth(3);
  gS238->SetLineWidth(3);
  gS239->SetLineWidth(3);
  if(draw240!=0) gS240->SetLineWidth(3);
  gS241->SetLineWidth(3);
  gS235->SetLineColor(kViolet+2);
  gS238->SetLineColor(kAzure+1);
  gS239->SetLineColor(kGreen+1);
  if(draw240!=0) gS240->SetLineColor(kBlack);
  gS241->SetLineColor(kPink+7);
  leg->Clear();
  if(isFuture!=0) legString.Form("#sigma_{235} = %2.3f #pm %2.3f",minValVector[0][0]/U235Theo,minValVector[0][7]/minValVector[0][0]);
  else legString.Form("#sigma_{#sigma_{235}} = #pm %2.2f",minValVector[0][7]/minValVector[0][0]);
  leg->AddEntry(g235,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{238} = %2.3f #pm %2.3f",minValVector[0][1]/U238Theo,minValVector[0][8]/minValVector[0][1]);
  else legString.Form("#sigma_{#sigma_{238}} = #pm %2.2f",minValVector[0][8]/minValVector[0][1]);
  leg->AddEntry(g238,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{239} = %2.3f #pm %2.3f",minValVector[0][2]/P239Theo,minValVector[0][9]/minValVector[0][2]);
  else legString.Form("#sigma_{#sigma_{239}} = #pm %2.2f",minValVector[0][9]/minValVector[0][2]);
  leg->AddEntry(g239,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{240} = %2.3f #pm %2.3f",minValVector[0][3]/P240Theo,minValVector[0][10]/minValVector[0][3]);
  else legString.Form("#sigma_{#sigma_{240}} = #pm %2.2f",minValVector[0][10]/minValVector[0][3]);
  if(draw240!=0) leg->AddEntry(g240,legString,"l");
  if(isFuture!=0) legString.Form("#sigma_{241} = %2.3f #pm %2.3f",minValVector[0][4]/P241Theo,minValVector[0][11]/minValVector[0][4]);
  else legString.Form("#sigma_{#sigma_{241}} = #pm %2.2f",minValVector[0][11]/minValVector[0][4]);
  leg->AddEntry(g241,legString,"l");
  mg->GetXaxis()->SetRangeUser(0.5,1.6);
  //mg->GetXaxis()->SetNdivisions(420);
  mg->GetYaxis()->SetRangeUser(0,10);
  mg->GetYaxis()->SetTitle("#Delta #chi2");
  mg->GetXaxis()->SetTitle("R_{i}");
  outFile.ReplaceAll("1D","1DScaled");
  leg->Draw();
  c->Print(outFile);
  

  gROOT->ForceStyle();
  // resultant cov matrix
  TH2D *hResCovMat=(TH2D*)inputFile->Get("ResultantIsotopeCovarianceMatrix");
  
  TPaveText *pt = new TPaveText(.5,.85,.85,.9,"NDC");
  pt->SetTextFont(42);
  pt->SetTextColor(kBlack);
  pt->SetTextSize(0.05);
  pt->SetFillColorAlpha(kWhite,0.7);

  // output2D58
  c->Clear();
  TGraph *g1=(TGraph*)inputFile->Get("U235_U238_1sigma");
  TGraph *g2=(TGraph*)inputFile->Get("U235_U238_2sigma");
  TGraph *g3=(TGraph*)inputFile->Get("U235_U238_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  g3->GetXaxis()->SetLimits(5.6,6.5);
  g3->GetXaxis()->SetNdivisions(405);
  g3->GetYaxis()->SetRangeUser(6,14);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);
  c->Update();
  pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(1,2)/minValVector[0][7]/minValVector[0][8]));
  pt->Draw("SAME");
  outFile.ReplaceAll("1DScaled","2D58");
  c->Print(outFile);
  
  // output2D59
  g1=(TGraph*)inputFile->Get("U235_P239_1sigma");
  g2=(TGraph*)inputFile->Get("U235_P239_2sigma");
  g3=(TGraph*)inputFile->Get("U235_P239_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  pt->Clear();
  g3->GetXaxis()->SetLimits(5.6,6.5);
  g3->GetYaxis()->SetRangeUser(4,4.8);
  g3->GetXaxis()->SetNdivisions(405);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);
  pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(1,3)/minValVector[0][7]/minValVector[0][9]));
  pt->Draw("SAME");
  outFile.ReplaceAll("2D58","2D59");
  c->Print(outFile);

  if(draw240!=0) 
  {
    // output2D50
    g1=(TGraph*)inputFile->Get("U235_P240_1sigma");
    g2=(TGraph*)inputFile->Get("U235_P240_2sigma");
    g3=(TGraph*)inputFile->Get("U235_P240_3sigma");
    g1->SetFillColor(kMagenta+4);
    g2->SetFillColor(kMagenta-2);
    g3->SetFillColor(kMagenta-9);
    g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
    g3->GetYaxis()->SetTitle("#sigma_{240} [10^{-43}cm^{2}/fission]");
    g3->Draw("AFC");
    g2->Draw("FC");
    g1->Draw("FC");
    pt->Clear();
    g3->GetXaxis()->SetLimits(5.6,6.5);
    g3->GetYaxis()->SetRangeUser(0,12);
    g3->GetXaxis()->SetNdivisions(405);
    g3->GetXaxis()->SetNdivisions(405);
    pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(1,4)/minValVector[0][7]/minValVector[0][10]));
    pt->Draw("SAME");
    outFile.ReplaceAll("2D59","2D50");
    c->Print(outFile);
  
    // output2D80
    g1=(TGraph*)inputFile->Get("U238_P240_1sigma");
    g2=(TGraph*)inputFile->Get("U238_P240_2sigma");
    g3=(TGraph*)inputFile->Get("U238_P240_3sigma");
    g1->SetFillColor(kMagenta+4);
    g2->SetFillColor(kMagenta-2);
    g3->SetFillColor(kMagenta-9);
    g3->GetXaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
    g3->GetYaxis()->SetTitle("#sigma_{240} [10^{-43}cm^{2}/fission]");
    g3->Draw("AFC");
    g2->Draw("FC");
    g1->Draw("FC");
    pt->Clear();
    g3->GetXaxis()->SetLimits(7,13);
    g3->GetYaxis()->SetRangeUser(0,12);
    g3->GetXaxis()->SetNdivisions(405);
    pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(2,4)/minValVector[0][8]/minValVector[0][10]));
    pt->Draw("SAME");
    outFile.ReplaceAll("2D50","2D80");
    c->Print(outFile);  
  
    // output2D90
    g1=(TGraph*)inputFile->Get("P239_P240_1sigma");
    g2=(TGraph*)inputFile->Get("P239_P240_2sigma");
    g3=(TGraph*)inputFile->Get("P239_P240_3sigma");
    g1->SetFillColor(kMagenta+4);
    g2->SetFillColor(kMagenta-2);
    g3->SetFillColor(kMagenta-9);
    g3->GetXaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
    g3->GetYaxis()->SetTitle("#sigma_{240} [10^{-43}cm^{2}/fission]");
    g3->Draw("AFC");
    g2->Draw("FC");
    g1->Draw("FC");
    pt->Clear();
    g3->GetXaxis()->SetLimits(4,4.8);
    g3->GetYaxis()->SetRangeUser(0,12);
    g3->GetYaxis()->SetNdivisions(405);
    pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(3,4)/minValVector[0][9]/minValVector[0][10]));
    pt->Draw("SAME");
    outFile.ReplaceAll("2D80","2D90");
    c->Print(outFile);
  
    // output2D01
    g1=(TGraph*)inputFile->Get("P240_P241_1sigma");
    g2=(TGraph*)inputFile->Get("P240_P241_2sigma");
    g3=(TGraph*)inputFile->Get("P240_P241_3sigma");
    g1->SetFillColor(kMagenta+4);
    g2->SetFillColor(kMagenta-2);
    g3->SetFillColor(kMagenta-9);
    g3->GetXaxis()->SetTitle("#sigma_{240} [10^{-43}cm^{2}/fission]");
    g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
    g3->Draw("AFC");
    g2->Draw("FC");
    g1->Draw("FC");
    pt->Clear();
    g3->GetXaxis()->SetLimits(0,13);
    g3->GetYaxis()->SetRangeUser(5.2,7);
    g3->GetYaxis()->SetNdivisions(405);
    pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(4,5)/minValVector[0][10]/minValVector[0][11]));
    pt->Draw("SAME");
    outFile.ReplaceAll("2D90","2D10");
    c->Print(outFile);
  }
  
  // output2D51
  g1=(TGraph*)inputFile->Get("U235_P241_1sigma");
  g2=(TGraph*)inputFile->Get("U235_P241_2sigma");
  g3=(TGraph*)inputFile->Get("U235_P241_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  pt->Clear();
  g3->GetXaxis()->SetLimits(6,6.85);
  g3->GetYaxis()->SetRangeUser(0,22);
  g3->GetXaxis()->SetNdivisions(405);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);

  //Drawing the vertical lines 
  TLine *line1 = new TLine(6.69,0,6.69,6.03);
  line1 -> SetLineColor(kCyan+2);
  line1 -> SetLineWidth(3);
  line1 -> SetLineStyle(9);
  line1 -> Draw("sames");

  TLine *line2 = new TLine(6,6.03,6.68,6.03);
  line2 -> SetLineColor(kCyan+2);
  line2 -> SetLineWidth(3);
  line2 -> SetLineStyle(9);
  line2 -> Draw("sames");

  // Create marker
  auto marker_theo_2D51 = new TMarker(6.69, 6.03,kFullCircle);
  marker_theo_2D51->SetMarkerColor(kCyan+2);
  marker_theo_2D51->SetMarkerSize(3);
  marker_theo_2D51->Draw();

  if(draw240==0) pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(1,4)/minValVector[0][7]/minValVector[0][11]));
  else pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(1,5)/minValVector[0][7]/minValVector[0][11]));
  pt->Draw("SAME");
  if(draw240!=0) outFile.ReplaceAll("2D10","2D51");
  else outFile.ReplaceAll("2D59","2D51");
  c->Print(outFile);
  
  // output2D89
  g1=(TGraph*)inputFile->Get("U238_P239_1sigma");
  g2=(TGraph*)inputFile->Get("U238_P239_2sigma");
  g3=(TGraph*)inputFile->Get("U238_P239_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  pt->Clear();
  g3->GetXaxis()->SetLimits(6,14);
  g3->GetYaxis()->SetRangeUser(4,4.8);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);
  pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(2,3)/minValVector[0][8]/minValVector[0][9]));
  pt->Draw("SAME");
  outFile.ReplaceAll("2D51","2D89");
  c->Print(outFile);

  // output2D81
  g1=(TGraph*)inputFile->Get("U238_P241_1sigma");
  g2=(TGraph*)inputFile->Get("U238_P241_2sigma");
  g3=(TGraph*)inputFile->Get("U238_P241_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  pt->Clear();
  g3->GetXaxis()->SetLimits(0,15);
  g3->GetYaxis()->SetRangeUser(0,22);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);

  //Drawing the vertical lines 
  TLine *line3 = new TLine(10.10,0,10.10,6.03);
  line3 -> SetLineColor(kCyan+2);
  line3 -> SetLineWidth(3);
  line3 -> SetLineStyle(9);
  line3 -> Draw("sames");

  TLine *line4 = new TLine(0,6.03,10.10,6.03);
  line4 -> SetLineColor(kCyan+2);
  line4 -> SetLineWidth(3);
  line4 -> SetLineStyle(9);
  line4 -> Draw("sames");

  // Create marker
  auto marker_theo_2D81 = new TMarker(10.10, 6.03,kFullCircle);
  marker_theo_2D81->SetMarkerColor(kCyan+2);
  marker_theo_2D81->SetMarkerSize(3);
  marker_theo_2D81->Draw();

  if(draw240==0) pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(2,4)/minValVector[0][8]/minValVector[0][11]));
  else pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(2,5)/minValVector[0][8]/minValVector[0][11]));
  pt->Draw("SAME");
  outFile.ReplaceAll("2D89","2D81");
  c->Print(outFile);
  
  // output2D91
  g1=(TGraph*)inputFile->Get("P239_P241_1sigma");
  g2=(TGraph*)inputFile->Get("P239_P241_2sigma");
  g3=(TGraph*)inputFile->Get("P239_P241_3sigma");
  g1->SetFillColor(kMagenta+4);
  g2->SetFillColor(kMagenta-2);
  g3->SetFillColor(kMagenta-9);
  g3->GetXaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  pt->Clear();
  g3->GetXaxis()->SetLimits(0,8);
  g3->GetYaxis()->SetRangeUser(0,22);
  g3->GetXaxis()->SetLabelOffset(0.018);
  g3->GetYaxis()->SetLabelOffset(0.018);
  g3->GetXaxis()->SetTitleOffset(1.1);

  if(draw240==0) pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(3,4)/minValVector[0][9]/minValVector[0][11]));
  else pt->AddText(Form("Correlation: %1.3f",hResCovMat->GetBinContent(3,5)/minValVector[0][9]/minValVector[0][11]));
  pt->Draw("SAME");

  //Drawing the vertical lines 
  TLine *line5 = new TLine(4.40,0,4.40,6.03);
  line5 -> SetLineColor(kCyan+2);
  line5 -> SetLineWidth(3);
  line5 -> SetLineStyle(9);
  line5 -> Draw("sames");

  TLine *line6 = new TLine(0,6.03,4.40,6.03);
  line6 -> SetLineColor(kCyan+2);
  line6 -> SetLineWidth(3);
  line6 -> SetLineStyle(9);
  line6 -> Draw("sames");

  // Create markers
  auto marker_theo_2D91 = new TMarker(4.40, 6.03,kFullCircle);
  marker_theo_2D91->SetMarkerColor(kCyan+2);
  marker_theo_2D91->SetMarkerSize(3);
  marker_theo_2D91->Draw();

  auto star = new TMarker(4.0, 8.16, kFullStar);
  star->SetMarkerColor(kPink+7);
  star->SetMarkerSize(6);
  star->Draw();

  auto star_modifiedCase = new TMarker(5.3, 2.9, kFullStar);
  star_modifiedCase->SetMarkerColor(kCyan);
  star_modifiedCase->SetMarkerSize(6);
  star_modifiedCase->Draw();

  outFile.ReplaceAll("2D81","2D91");
  c->Print(outFile);
  c->Clear();
  c->SetTicks(0);

  std::vector<double> yields;

  if(draw240==0) 
  {
    yields.push_back(minValVector[0][0]);
    yields.push_back(minValVector[0][1]);
    yields.push_back(minValVector[0][2]);
    yields.push_back(minValVector[0][4]);
  }
  else
  {
    yields.push_back(minValVector[0][0]);
    yields.push_back(minValVector[0][1]);
    yields.push_back(minValVector[0][2]);
    yields.push_back(minValVector[0][3]);
    yields.push_back(minValVector[0][4]);
  }
  
  ConvertCovarianceToUncertainty(*hResCovMat, yields);
  hResCovMat->GetZaxis()->SetTitle("Fission Yields [10^{-43}cm^{2}/fission]");
  hResCovMat->GetZaxis()->SetRangeUser(0,25);
  // c->SetRightMargin(0.15);
  hResCovMat->Draw("COLZTEXT");
  hResCovMat->GetZaxis()->SetLimits(-1.5,1.5);
  hResCovMat->SetMarkerSize(3);
  gStyle->SetPaintTextFormat("2.1f");
  hResCovMat->GetXaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetXaxis()->ChangeLabel(2,-1,-1,-1,-1,-1,"U^{235}");  
  hResCovMat->GetXaxis()->ChangeLabel(3,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetXaxis()->ChangeLabel(4,-1,-1,-1,-1,-1,"U^{238}");  
  hResCovMat->GetXaxis()->ChangeLabel(5,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetXaxis()->ChangeLabel(6,-1,-1,-1,-1,-1,"Pu^{239}");  
  hResCovMat->GetXaxis()->ChangeLabel(7,-1,-1,-1,-1,-1," ");  
  if(draw240!=0) 
  {
    hResCovMat->GetXaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{240}");  
    hResCovMat->GetXaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
    hResCovMat->GetXaxis()->ChangeLabel(10,-1,-1,-1,-1,-1,"Pu^{241}");
    hResCovMat->GetXaxis()->ChangeLabel(11,-1,-1,-1,-1,-1," ");  
  }
  else 
  {
    hResCovMat->GetXaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{241}");  
    hResCovMat->GetXaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
  }

  hResCovMat->GetYaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetYaxis()->ChangeLabel(2,-1,-1,-1,-1,-1,"U^{235}");  
  hResCovMat->GetYaxis()->ChangeLabel(3,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetYaxis()->ChangeLabel(4,-1,-1,-1,-1,-1,"U^{238}");  
  hResCovMat->GetYaxis()->ChangeLabel(5,-1,-1,-1,-1,-1," ");  
  hResCovMat->GetYaxis()->ChangeLabel(6,-1,-1,-1,-1,-1,"Pu^{239}");  
  hResCovMat->GetYaxis()->ChangeLabel(7,-1,-1,-1,-1,-1," ");  
  if(draw240!=0) 
  {
    hResCovMat->GetYaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{240}");  
    hResCovMat->GetYaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
    hResCovMat->GetYaxis()->ChangeLabel(10,-1,-1,-1,-1,-1,"Pu^{241}");  
    hResCovMat->GetYaxis()->ChangeLabel(11,-1,-1,-1,-1,-1," "); 
  } 
  else{
    hResCovMat->GetYaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{241}");  
    hResCovMat->GetYaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
  }

  c->Update();
  TPaletteAxis *palette = (TPaletteAxis*)hResCovMat->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.89);
  palette->SetX2NDC(0.91);
  palette->SetY1NDC(0.15);
  palette->SetY2NDC(0.95);
  c->Modified();
  c->Update();

  outFile.ReplaceAll("2D91","ResCov");
  c->Print(outFile);
  return 0;
}
