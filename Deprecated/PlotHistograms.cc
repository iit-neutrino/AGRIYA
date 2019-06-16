////////////////////////////////////////////////
// File to plot histograms
////////////////////////////////////////////////

#include "Global_Utilities.hh"
#include "TKey.h"
#include <TStyle.h>
#include "TLegend.h"
#include "TLegend.h"
#include "TObjString.h"
using namespace std;


void usage(){
  std::cout << "Example: PlotHistograms file.root outputfolder\n";
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
  //  outFolder.Append("OutputFile.root");
  
  TCanvas *c = new TCanvas("c","c",1200,1200);
  
  
  TString outFile(outFolder);
  double contourSig[4];
  double minChi2;
  
  
  Int_t colors[] = {1,kGreen-5, kGreen-8,kGreen-10, kWhite}; // #colors >= #levels - 1
  gStyle->SetPalette((sizeof(colors)/sizeof(Int_t)), colors);
  
  
  double U235Theo=6.69;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P241Theo=6.03;
  
  
  TH1D* hist1;
  TH1D *hist2;
  TH1D *hist3;
  TH1D *hist4;
  
  TH1D *hist1R;
  TH1D *hist2R;
  TH1D *hist3R;
  TH1D *hist4R;
  
  hist1=(TH1D*)inputFile->Get("hChi2MapU235");
  hist2=(TH1D*)inputFile->Get("hChi2MapU238");
  hist3=(TH1D*)inputFile->Get("hChi2MapP239");
  hist4=(TH1D*)inputFile->Get("hChi2MapP241");
  
  hist1->GetXaxis()->SetTitle("#sigma_{f}");
  hist1->GetYaxis()->SetTitle("#Delta #chi2");
  
  
  hist1R=new TH1D("hChi2MapU235R","",hist1->GetNbinsX(),hist1->GetBinLowEdge(1)/U235Theo,hist1->GetBinLowEdge(hist1->GetNbinsX()+1)/U235Theo);
  hist2R=new TH1D("hChi2MapU238R","",hist2->GetNbinsX(),hist2->GetBinLowEdge(1)/U238Theo,hist2->GetBinLowEdge(hist2->GetNbinsX()+1)/U238Theo);
  
  hist1R->GetXaxis()->SetTitle("#r_{f}");
  hist1R->GetYaxis()->SetTitle("#Delta #chi2");
  
  hist3R=new TH1D("hChi2MapP239R","",hist3->GetNbinsX(),hist3->GetBinLowEdge(1)/P239Theo,hist3->GetBinLowEdge(hist3->GetNbinsX()+1)/P239Theo);
  hist4R=new TH1D("hChi2MapP241R","",hist4->GetNbinsX(),hist4->GetBinLowEdge(1)/P241Theo,hist4->GetBinLowEdge(hist4->GetNbinsX()+1)/P241Theo);
  
  TLegend *leg=new TLegend(0.6,0.5,0.88,0.6);  leg->SetBorderSize(0);
  TLegend *leg0=new TLegend(0.6,0.64,0.88,0.69);  leg0->SetBorderSize(0);
  TLegend *leg1=new TLegend(0.6,0.7,0.88,0.75);  leg1->SetBorderSize(0);
  TLegend *leg2=new TLegend(0.6,0.76,0.88,0.81);  leg2->SetBorderSize(0);
  TLegend *leg3=new TLegend(0.6,0.82,0.88,0.87);  leg3->SetBorderSize(0);
  
  
  hist1->SetLineColor(kBlue);
  hist2->SetLineColor(kRed);
  hist3->SetLineColor(kGreen);
  hist4->SetLineColor(kViolet);
  
  hist1R->SetLineColor(kBlue);
  hist2R->SetLineColor(kRed);
  hist3R->SetLineColor(kGreen);
  hist4R->SetLineColor(kViolet);
  
  hist1->GetXaxis()->SetRangeUser(5.5,6.5);
  minChi2=hist1->GetMinimum();
  
  
  hist1->GetXaxis()->SetRangeUser(2,12);
  
  hist1->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist2->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist3->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist4->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  
  for(int i=1;i<=hist1R->GetNbinsX();i++){
    hist1R->SetBinContent(i,hist1->GetBinContent(i));
  }
  for(int i=1;i<=hist2R->GetNbinsX();i++){
    hist2R->SetBinContent(i,hist2->GetBinContent(i));
  }
  for(int i=1;i<=hist3R->GetNbinsX();i++){
    hist3R->SetBinContent(i,hist3->GetBinContent(i));
  }
    for(int i=1;i<=hist4R->GetNbinsX();i++){
      hist4R->SetBinContent(i,hist4->GetBinContent(i));
    }
  hist1R->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist2R->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist3R->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  hist4R->GetYaxis()->SetRangeUser(minChi2,minChi2+10);
  
  hist1->Draw("][ l");
  hist2->Draw("SAME ][ l");
  hist3->Draw("SAME ][ l");
  hist4->Draw("SAME ][ l");
  
  TString buffer;
  buffer.Form("#chi2 min = %.1f", minChi2);
  leg->AddEntry("hist2",buffer,"");
  
  int meanBin;
  int y,z;
  hist1->GetXaxis()->SetRangeUser(5.6,6.5);
  hist1->GetMinimumBin(meanBin,y,z);
  double mean=hist1->GetBinCenter(meanBin);
  double sigma=GetNSigma(hist1,1);
  hist1->GetXaxis()->SetRangeUser(2,12);
  buffer.Form("U 235, %.2f #pm %.2f",mean,sigma);
  leg3->AddEntry(hist1,buffer.Data(),"l");
  
  hist2->GetXaxis()->SetRangeUser(9.1,10.9);
  hist2->GetMinimumBin(meanBin,y,z);
  mean =hist2->GetBinCenter(meanBin);
  sigma=GetNSigma(hist2,1);
  hist2->GetXaxis()->SetRangeUser(2,12);
  buffer.Form("U 238, %.2f #pm %.2f",mean,sigma);
  leg2->AddEntry(hist2,buffer.Data(),"l");
  
  hist3->GetXaxis()->SetRangeUser(4,4.5);
  hist3->GetMinimumBin(meanBin,y,z);
  mean =hist3->GetBinCenter(meanBin);
  sigma=GetNSigma(hist3,1);
  hist3->GetXaxis()->SetRangeUser(2,12);
  buffer.Form("P 239, %.2f #pm %.2f",mean,sigma);
  leg1->AddEntry(hist3,buffer.Data(),"l");
  
  
  hist4->GetXaxis()->SetRangeUser(5.7,6.35);
  hist4->GetMinimumBin(meanBin,y,z);
  mean =hist4->GetBinCenter(meanBin);
  sigma=GetNSigma(hist4,1);
  hist4->GetXaxis()->SetRangeUser(2,12);
  buffer.Form("P 241, %.2f #pm %.2f",mean,sigma);
  leg0->AddEntry(hist4,buffer.Data(),"l");
  
  outFile.Append("h1D4Isotopes");
  outFile.Append(".png");
  
  leg0->Draw("l");
  leg1->Draw("l");
  leg2->Draw("l");
  leg3->Draw("l");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  outFile.ReplaceAll("h1D4Isotopes.C","");
  
  
  leg0->Clear();
  leg1->Clear();
  leg2->Clear();
  leg3->Clear();
  
  /////////////// plot rescaled histograms ///////////////
  hist1R->Draw("][ l");
  hist2R->Draw("SAME ][ l");
  hist3R->Draw("SAME ][ l");
  hist4R->Draw("SAME ][ l");
  
  hist1R->GetXaxis()->SetRangeUser(5.5/U235Theo,7.5/U235Theo);
  hist1R->GetMinimumBin(meanBin,y,z);
  mean=hist1R->GetBinCenter(meanBin);
  sigma=GetNSigma(hist1R,1);
  hist1R->GetXaxis()->SetRangeUser(0,2);
  buffer.Form("U 235, %.3f #pm %.2f",mean,sigma);
  leg3->AddEntry(hist1R,buffer.Data(),"l");
  
  hist2R->GetXaxis()->SetRangeUser(9/U238Theo,11/U238Theo);
  hist2R->GetMinimumBin(meanBin,y,z);
  mean =hist2R->GetBinCenter(meanBin);
  sigma=GetNSigma(hist2R,1);
  hist2R->GetXaxis()->SetRangeUser(0,2);
  buffer.Form("U 238, %.3f #pm %.2f",mean,sigma);
  leg2->AddEntry(hist2R,buffer.Data(),"l");
  
  
  hist3R->GetXaxis()->SetRangeUser(3/P239Theo,6/P239Theo);
  hist3R->GetMinimumBin(meanBin,y,z);
  mean =hist3R->GetBinCenter(meanBin);
  sigma=GetNSigma(hist3R,1);
  hist3R->GetXaxis()->SetRangeUser(0,2);
  buffer.Form("P 239, %.3f #pm %.2f",mean,sigma);
  leg1->AddEntry(hist3R,buffer.Data(),"l");
  
  
  hist4R->GetXaxis()->SetRangeUser(4/P241Theo,8/P241Theo);
  hist4R->GetMinimumBin(meanBin,y,z);
  mean =hist4R->GetBinCenter(meanBin);
  sigma=GetNSigma(hist4R,1);
  hist4R->GetXaxis()->SetRangeUser(0,2);
  buffer.Form("P 241, %.3f #pm %.2f",mean,sigma);
  leg0->AddEntry(hist4R,buffer.Data(),"l");
  
  outFile.Append("h1D4Isotopes_Rescaled");
  outFile.Append(".png");
  
  leg0->Draw("l");
  leg1->Draw("l");
  leg2->Draw("l");
  leg3->Draw("l");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  outFile.ReplaceAll("h1D4Isotopes_Rescaled.C","");
  
  /////////////// plot rescaled histograms ///////////////
  
  leg0->Clear();
  leg1->Clear();
  leg2->Clear();
  leg3->Clear();
  
  
  TH2D* temp;
  
  temp=(TH2D*)inputFile->Get("hChi2MapU235238");
  TH2D *temp1=(TH2D*)temp->Clone("temp1");
  TH2D *temp2=(TH2D*)temp->Clone("temp2");
  
  temp->GetXaxis()->SetRangeUser(5.6,6.8);
  temp->GetYaxis()->SetRangeUser(6,12);
  temp->GetXaxis()->SetTitle("#sigma_{235}");
  temp->GetYaxis()->SetTitle("#sigma_{238}");
  
  outFile.Append("hChi2MapU235238");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  temp->SetFillColor(kGreen-5);
  leg1->AddEntry(temp,"1 #sigma","f");
  leg1->Draw("f");
  temp1->SetFillColor(kGreen-8);
  leg2->AddEntry(temp1,"2 #sigma","f");
  leg2->Draw("f");
  temp2->SetFillColor(kGreen-10);
  leg3->AddEntry(temp2,"3 #sigma","f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png","");
  outFile.ReplaceAll("hChi2MapU235238.C","");
  
  temp=(TH2D*)inputFile->Get("hChi2MapU235239");
  temp->GetXaxis()->SetRangeUser(5.6,6.8);
  temp->GetYaxis()->SetRangeUser(3,6);
  temp->GetXaxis()->SetTitle("#sigma_{235}");
  temp->GetYaxis()->SetTitle("#sigma_{239}");
  
  outFile.Append("hChi2MapU235239");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  
  leg1->Draw("f");
  leg2->Draw("f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  
  outFile.ReplaceAll("hChi2MapU235239.C","");
  
  
  temp=(TH2D*)inputFile->Get("hChi2MapU235241");
  temp->GetXaxis()->SetRangeUser(5.6,6.8);
  temp->GetYaxis()->SetRangeUser(3.5,9.5);
  temp->GetXaxis()->SetTitle("#sigma_{235}");
  temp->GetYaxis()->SetTitle("#sigma_{241}");
  
  outFile.Append("hChi2MapU235241");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  
  leg1->Draw("f");
  leg2->Draw("f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  
  outFile.ReplaceAll("hChi2MapU235241.C","");
  
  temp=(TH2D*)inputFile->Get("hChi2MapU238239");
  temp->GetXaxis()->SetRangeUser(6,12);
  temp->GetYaxis()->SetRangeUser(3,6);
  temp->GetXaxis()->SetTitle("#sigma_{238}");
  temp->GetYaxis()->SetTitle("#sigma_{239}");
  
  outFile.Append("hChi2MapU238239");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  
  leg1->Draw("f");
  leg2->Draw("f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  
  outFile.ReplaceAll("hChi2MapU238239.C","");
  
  temp=(TH2D*)inputFile->Get("hChi2MapU238241");
  temp->GetXaxis()->SetRangeUser(6,12);
  temp->GetYaxis()->SetRangeUser(3.5,9.5);
  temp->GetXaxis()->SetTitle("#sigma_{238}");
  temp->GetYaxis()->SetTitle("#sigma_{241}");
  
  outFile.Append("hChi2MapU238241");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  
  leg1->Draw("f");
  leg2->Draw("f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  
  outFile.ReplaceAll("hChi2MapU238241.C","");
  
  temp=(TH2D*)inputFile->Get("hChi2MapP239241");
  temp->GetXaxis()->SetRangeUser(3,6);
  temp->GetYaxis()->SetRangeUser(3.5,9.5);
  temp->GetXaxis()->SetTitle("#sigma_{239}");
  temp->GetYaxis()->SetTitle("#sigma_{241}");
  
  outFile.Append("hChi2MapP239241");
  outFile.Append(".png");
  
  minChi2=temp->GetMinimum();
  contourSig[0]= minChi2;
  contourSig[1]= 2.3+minChi2;
  contourSig[2]= 5.99+minChi2;
  contourSig[3]= 11.83+minChi2;
  
  temp->SetContour(4, contourSig);
  temp->Draw("CONT4");
  
  
  leg1->Draw("f");
  leg2->Draw("f");
  leg3->Draw("f");
  leg->Draw("");
  
  c->Print(outFile);
  outFile.ReplaceAll(".png",".C");
  c->Print(outFile);
  
  outFile.ReplaceAll("hChi2MapP239241.C","");
  
  return 0;
}
