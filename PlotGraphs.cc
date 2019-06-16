////////////////////////////////////////////////
// File to plot histograms
////////////////////////////////////////////////

#include "TKey.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLegend.h"
#include "TObjString.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMultiGraph.h"

using namespace std;

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

void usage(){
  std::cout << "Example: PlotGraphs file.root outputfolder\n";
  exit(1);
}

int main(int argc, char *argv[]){
  
  if(argc!=3) usage();
  
  gStyle->SetOptStat("");
  TString inputFileName=argv[1];
  TFile *inputFile=TFile::Open(inputFileName);
  
  // output ROOT file for saving plots
  TString outFolder = argv[2];
  outFolder.Append("/");
  inputFileName.ReplaceAll(".root","");
  TObjArray *objList=inputFileName.Tokenize("/");
  inputFileName=((TObjString*)objList->Last())->GetString();
  outFolder.Append(inputFileName);
  
  TCanvas *c = new TCanvas("c","c",1200,1200);
  c->SetGrid(1);
  c->SetTicks(1);
  
  TString outFile(outFolder);
  
  
  Int_t colors[] = {kBlue+1,kRed,kGreen+1,kMagenta+1}; // #colors >= #levels - 1
  Int_t contColors[] = {1,kGreen-5, kGreen-8,kGreen-10, kWhite}; // #colors >= #levels - 1
  gStyle->SetPalette((sizeof(contColors)/sizeof(Int_t)), contColors);
  TLegend *leg=new TLegend(0.6,0.65,0.9,0.9);  leg->SetBorderSize(0); leg->SetFillColorAlpha(kWhite,0.7);
  
  double U235Theo=6.69;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P241Theo=6.03;
 
  
  outFile.Append("1D");
  outFile.Append(".pdf");
  
//  hist1->GetXaxis()->SetTitle("#sigma_{f}");
//  hist1->GetYaxis()->SetTitle("#Delta #chi2");
  
  TVectorD *minValVector=(TVectorD*)inputFile->Get("minValues");
  double minimum=minValVector[0][10];
  
  TMultiGraph *mg=new TMultiGraph();
  TGraph *g235=(TGraph*)inputFile->Get("U235");
  TGraph *g238=(TGraph*)inputFile->Get("U238");
  TGraph *g239=(TGraph*)inputFile->Get("P239");
  TGraph *g241=(TGraph*)inputFile->Get("P241");
  
  MinimizeToZero(*g235,minimum);
  MinimizeToZero(*g238,minimum);
  MinimizeToZero(*g239,minimum);
  MinimizeToZero(*g241,minimum);
  mg->Add(g235,"AC");
  mg->Add(g238,"AC");
  mg->Add(g239,"AC");
  mg->Add(g241,"AC");
  mg->Draw("AC");
  g235->SetLineWidth(2);
  g238->SetLineWidth(2);
  g239->SetLineWidth(2);
  g241->SetLineWidth(2);
  g235->SetLineColor(colors[0]);
  g238->SetLineColor(colors[1]);
  g239->SetLineColor(colors[2]);
  g241->SetLineColor(colors[3]);
  TString legString;
  legString.Form("#sigma_{235} = %2.2f#pm%2.2f",minValVector[0][0],minValVector[0][4]);
  leg->AddEntry(g235,legString,"l");
  legString.Form("#sigma_{238} = %2.2f#pm%2.2f",minValVector[0][1],minValVector[0][5]);
  leg->AddEntry(g238,legString,"l");
  legString.Form("#sigma_{239} = %2.2f#pm%2.2f",minValVector[0][2],minValVector[0][6]);
  leg->AddEntry(g239,legString,"l");
  legString.Form("#sigma_{241} = %2.2f#pm%2.2f",minValVector[0][3],minValVector[0][7]);
  leg->AddEntry(g241,legString,"l");
  mg->GetXaxis()->SetRangeUser(2,12);
  mg->GetXaxis()->SetNdivisions(520);
  mg->GetYaxis()->SetRangeUser(0,10);
  mg->GetYaxis()->SetTitle("#Delta #chi2");
  mg->GetXaxis()->SetTitle("#sigma_{i} [10^{-43}cm^{2}/fission]");
  leg->Draw();
  c->Print(outFile);
  
  TGraph *gS235=new TGraph();
  TGraph *gS238=new TGraph();
  TGraph *gS239=new TGraph();
  TGraph *gS241=new TGraph();
  ConvertAbsoluteToRelativeGraph(*g235,*gS235,U235Theo);
  ConvertAbsoluteToRelativeGraph(*g238,*gS238,U238Theo);
  ConvertAbsoluteToRelativeGraph(*g239,*gS239,P239Theo);
  ConvertAbsoluteToRelativeGraph(*g241,*gS241,P241Theo);
  mg->RecursiveRemove(g235);
  mg->RecursiveRemove(g238);
  mg->RecursiveRemove(g239);
  mg->RecursiveRemove(g241);
  mg->Add(gS235,"AC");
  mg->Add(gS238,"AC");
  mg->Add(gS239,"AC");
  mg->Add(gS241,"AC");
  mg->Draw("AC");
  gS235->SetLineWidth(2);
  gS238->SetLineWidth(2);
  gS239->SetLineWidth(2);
  gS241->SetLineWidth(2);
  gS235->SetLineColor(colors[0]);
  gS238->SetLineColor(colors[1]);
  gS239->SetLineColor(colors[2]);
  gS241->SetLineColor(colors[3]);
  leg->Clear();
  legString.Form("#sigma_{235} = %2.2f#pm%2.2f",minValVector[0][0]/U235Theo,minValVector[0][4]/U235Theo);
  leg->AddEntry(g235,legString,"l");
  legString.Form("#sigma_{238} = %2.2f#pm%2.2f",minValVector[0][1]/U238Theo,minValVector[0][5]/U238Theo);
  leg->AddEntry(g238,legString,"l");
  legString.Form("#sigma_{239} = %2.2f#pm%2.2f",minValVector[0][2]/P239Theo,minValVector[0][6]/P239Theo);
  leg->AddEntry(g239,legString,"l");
  legString.Form("#sigma_{241} = %2.2f#pm%2.2f",minValVector[0][3]/P241Theo,minValVector[0][7]/P241Theo);
  leg->AddEntry(g241,legString,"l");
  mg->GetXaxis()->SetRangeUser(0,2);
  mg->GetXaxis()->SetNdivisions(520);
  mg->GetYaxis()->SetRangeUser(0,10);
  mg->GetYaxis()->SetTitle("#Delta #chi2");
  mg->GetXaxis()->SetTitle("R_{i}");
  outFile.ReplaceAll("1D","1DScaled");
  leg->Draw();
  c->Print(outFile);
  
  c->Clear();
  TGraph *g1=(TGraph*)inputFile->Get("U235_U238_1sigma");
  TGraph *g2=(TGraph*)inputFile->Get("U235_U238_2sigma");
  TGraph *g3=(TGraph*)inputFile->Get("U235_U238_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("1DScaled","2D58");
  c->Print(outFile);
  
  g1=(TGraph*)inputFile->Get("U235_P239_1sigma");
  g2=(TGraph*)inputFile->Get("U235_P239_2sigma");
  g3=(TGraph*)inputFile->Get("U235_P239_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("2D58","2D59");
  c->Print(outFile);
  
  g1=(TGraph*)inputFile->Get("U235_P241_1sigma");
  g2=(TGraph*)inputFile->Get("U235_P241_2sigma");
  g3=(TGraph*)inputFile->Get("U235_P241_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{235} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("2D59","2D51");
  c->Print(outFile);
  
  g1=(TGraph*)inputFile->Get("U238_P239_1sigma");
  g2=(TGraph*)inputFile->Get("U238_P239_2sigma");
  g3=(TGraph*)inputFile->Get("U238_P239_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("2D51","2D89");
  c->Print(outFile);
  
  g1=(TGraph*)inputFile->Get("U238_P241_1sigma");
  g2=(TGraph*)inputFile->Get("U238_P241_2sigma");
  g3=(TGraph*)inputFile->Get("U238_P241_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{238} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("2D89","2D81");
  c->Print(outFile);
  
  g1=(TGraph*)inputFile->Get("P239_P241_1sigma");
  g2=(TGraph*)inputFile->Get("P239_P241_2sigma");
  g3=(TGraph*)inputFile->Get("P239_P241_3sigma");
  g1->SetFillColor(contColors[1]);
  g2->SetFillColor(contColors[2]);
  g3->SetFillColor(contColors[3]);
  g3->GetXaxis()->SetTitle("#sigma_{239} [10^{-43}cm^{2}/fission]");
  g3->GetYaxis()->SetTitle("#sigma_{241} [10^{-43}cm^{2}/fission]");
  g3->Draw("AFC");
  g2->Draw("FC");
  g1->Draw("FC");
  outFile.ReplaceAll("2D81","2D91");
  c->Print(outFile);
    return 0;
}
