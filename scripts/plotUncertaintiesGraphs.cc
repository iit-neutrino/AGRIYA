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

#include "StyleFile.hh"

using namespace std;


void usage(){
  std::cout << "Example: plotUncertaintiesGraphs\n";
  exit(1);
}

int main(int argc, char *argv[])
{  
  TStyle *style = gStyle;
  setupStyle(style);
  
  TCanvas *c = new TCanvas("c","c",1200,1000);

  TString outFile("./TheoreticalUncertainties.pdf");

  TH2D *hUncertainty = new TH2D("UncertaintyeMatrix",
  "UncertaintyeMatrix;;;Uncertainty [%]",5,0.5,5.5,5,0.5,5.5);

  hUncertainty->GetXaxis()->SetTitle("");
  hUncertainty->GetYaxis()->SetTitle("");
  hUncertainty->GetZaxis()->SetTitle("Uncertainty [%]");
  hUncertainty->GetZaxis()->SetRangeUser(-15,25);

  hUncertainty->SetBinContent(1, 1, 100*0.0244);
  hUncertainty->SetBinContent(2, 2, 100*0.08150);
  hUncertainty->SetBinContent(3, 3, 100*0.0288);
  hUncertainty->SetBinContent(4, 4, 100);
  hUncertainty->SetBinContent(5, 5, 100*0.026);

  hUncertainty->SetBinContent(1, 3, 100*0.02626);
  hUncertainty->SetBinContent(3, 1, 100*0.02626);
  hUncertainty->SetBinContent(1, 5, 100*0.02514);
  hUncertainty->SetBinContent(5, 1, 100*0.02514);
  hUncertainty->SetBinContent(3, 5, 100*0.02704);
  hUncertainty->SetBinContent(5, 3, 100*0.02704);

  // Just for teh sake of plotting
  hUncertainty->SetBinContent(1, 2,-100);
  hUncertainty->SetBinContent(2, 1,-100);
  hUncertainty->SetBinContent(1, 4,-100);
  hUncertainty->SetBinContent(4, 1,-100);
  hUncertainty->SetBinContent(2, 3,-100);
  hUncertainty->SetBinContent(3, 2,-100);
  hUncertainty->SetBinContent(2, 4,-100);
  hUncertainty->SetBinContent(4, 2,-100);
  hUncertainty->SetBinContent(2, 5,-100);
  hUncertainty->SetBinContent(5, 2,-100);
  hUncertainty->SetBinContent(3, 4,-100);
  hUncertainty->SetBinContent(4, 3,-100);
  hUncertainty->SetBinContent(4, 5,-100);
  hUncertainty->SetBinContent(5, 4,-100);
  
  

  TLegend *leg=new TLegend(0.5,0.65,0.85,0.92);  
  leg->SetFillColorAlpha(kWhite,0.8);
  
  gStyle->SetPaintTextFormat("2.1f");
  // c->SetRightMargin(0.15);
  hUncertainty->Draw("COLZTEXT");
  hUncertainty->GetZaxis()->SetLimits(-1.5,1.5);
  hUncertainty->SetMarkerSize(3);
  hUncertainty->SetMarkerColor(kWhite);
  hUncertainty->GetXaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetXaxis()->ChangeLabel(2,-1,-1,-1,-1,-1,"U^{235}");  
  hUncertainty->GetXaxis()->ChangeLabel(3,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetXaxis()->ChangeLabel(4,-1,-1,-1,-1,-1,"U^{238}");  
  hUncertainty->GetXaxis()->ChangeLabel(5,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetXaxis()->ChangeLabel(6,-1,-1,-1,-1,-1,"Pu^{239}");  
  hUncertainty->GetXaxis()->ChangeLabel(7,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetXaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{240}");  
  hUncertainty->GetXaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetXaxis()->ChangeLabel(10,-1,-1,-1,-1,-1,"Pu^{241}");
  hUncertainty->GetXaxis()->ChangeLabel(11,-1,-1,-1,-1,-1," ");  

  hUncertainty->GetYaxis()->ChangeLabel(1,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetYaxis()->ChangeLabel(2,-1,-1,-1,-1,-1,"U^{235}");  
  hUncertainty->GetYaxis()->ChangeLabel(3,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetYaxis()->ChangeLabel(4,-1,-1,-1,-1,-1,"U^{238}");  
  hUncertainty->GetYaxis()->ChangeLabel(5,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetYaxis()->ChangeLabel(6,-1,-1,-1,-1,-1,"Pu^{239}");  
  hUncertainty->GetYaxis()->ChangeLabel(7,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetYaxis()->ChangeLabel(8,-1,-1,-1,-1,-1,"Pu^{240}");  
  hUncertainty->GetYaxis()->ChangeLabel(9,-1,-1,-1,-1,-1," ");  
  hUncertainty->GetYaxis()->ChangeLabel(10,-1,-1,-1,-1,-1,"Pu^{241}");  
  hUncertainty->GetYaxis()->ChangeLabel(11,-1,-1,-1,-1,-1," "); 

  c->Update();
  TPaletteAxis *palette = (TPaletteAxis*)hUncertainty->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(0.89);
  palette->SetX2NDC(0.91);
  palette->SetY1NDC(0.15);
  palette->SetY2NDC(0.95);
  c->Modified();
  c->Update();

  c->Print(outFile);
  return 0;
}
