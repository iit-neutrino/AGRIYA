#include <TColor.h>
#include <cassert>
#include <TGaxis.h>
#include <TH1.h>
#include <TStyle.h>
#include <TROOT.h>

/// @brief Setup the default style for all plotting
/// @param defaultStyle 
void setupStyle(TStyle* defaultStyle) {
  
  if(!defaultStyle) defaultStyle = gStyle;
  assert(defaultStyle);
  TH1::SetDefaultSumw2();
  
  // use plain black on white colors
  Int_t icol = 0;
  defaultStyle->SetFrameBorderMode(icol);
  defaultStyle->SetCanvasBorderMode(icol);
  defaultStyle->SetPadBorderMode(icol);
  defaultStyle->SetPadColor(icol);
  defaultStyle->SetCanvasColor(icol);
  defaultStyle->SetStatColor(icol);
  //defaultStyle->SetFillColor(icol);
  
  // hack: assure colors get initialized!
  gROOT->GetColor(10);
  
  // Color palette - added by NSB 9/14, adapted from that used by NIFFTE collaboration
  // Based on http://ultrahigh.org/2007/08/20/making-pretty-root-color-palettes/
  const Int_t NRGBs = 5;
  const Int_t NCont = 99;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  defaultStyle->SetNumberContours(NCont);
  
  // defaultStyle->SetPalette(1,0); // set a good color palette
  //defaultStyle->SetFillColor(0); // explicitly don't fill
  //Int_t colors[]={4,4,7,7,6,6,6,2,2,2,1};
  //   float levels[]={1,10,14,22,45,60,200,300,400,500,600,700};
  
  // set the paper & margin sizes
  defaultStyle->SetPaperSize(TStyle::kUSLetter);
  
  // original margin settings:
  // defaultStyle->SetPadTopMargin(.10);
  // defaultStyle->SetPadLeftMargin(.151);
  // defaultStyle->SetPadRightMargin(.071);
  // defaultStyle->SetPadBottomMargin(.111);
  
  // Margins, assuming no top label or z scale
  defaultStyle->SetPadTopMargin(.03); // just enough to clear exponent on y axis
  defaultStyle->SetPadLeftMargin(.151);
  defaultStyle->SetPadRightMargin(.04);
  defaultStyle->SetPadBottomMargin(.12);
  
  // set master font
  //Int_t font=132;      //times new roman, regular (13), high precision (2)
  Int_t font=42;     //tnr, bold (4), high precision (2)
  
  //set master font size
  Double_t tsize=0.05; //should be set between 0.03-0.05, is in units of "% of pad"
  //Double_t tsize=0.065; //should be set between 0.03-0.05, is in units of "% of pad"
  
  // Text, Labels, Titles
  // All set the same here
  defaultStyle->SetTextFont(font);
  defaultStyle->SetTextSize(tsize);
  
  defaultStyle->SetLabelFont(font,"xyz");
  defaultStyle->SetLabelSize(tsize,"xyz");
  defaultStyle->SetLabelOffset(0.002,"xyz");
  
  defaultStyle->SetTitleFont(font,"xyz");
  defaultStyle->SetTitleSize(0.06,"xyz");
  defaultStyle->SetTitleOffset(1.25,"z");
  defaultStyle->SetTitleOffset(0.8,"y");
  defaultStyle->SetTitleOffset(0.8,"x");
  defaultStyle->SetTitleBorderSize(0);
  
  defaultStyle->SetLegendBorderSize(0);
  
  //use bold lines and markers
  //defaultStyle->SetMarkerStyle(20);
  //defaultStyle->SetMarkerSize(1.2);
  defaultStyle->SetHistLineWidth(2.);
  defaultStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  
  //get rid of X error bars and y error bar caps
  //defaultStyle->SetErrorX(0.001);
  
  //do not display any of the standard histogram decorations
  defaultStyle->SetOptTitle(0);      //no title box
  defaultStyle->SetOptStat("");      //no stat info
  //defaultStyle->SetOptFit(11);     //standard fit info
  defaultStyle->SetOptFit(0);        //no fit info
  
  //legend and stat pad
  defaultStyle->SetStatBorderSize(1);
  //    defaultStyle->SetStatFont(132);
  defaultStyle->SetStatX(0.95);
  defaultStyle->SetStatY(0.95);
  defaultStyle->SetLegendBorderSize(1);
  
  // put tick marks on top and RHS of plots
  //defaultStyle->SetPadTickX(1);
  //defaultStyle->SetPadTickY(1);
  // tick marks only on left and bottom
  //    defaultStyle->SetPadTickX(0);
  //    defaultStyle->SetPadTickY(0);
  // fewer axis divisions to avoid label collisions
  //    defaultStyle->SetNdivisions(506,"XYZ");
  
  // additional series styles (Thanks, Jianglai)
  //   1 = solid
  //   2 = long dash (30 10)
  //   3 = dotted (4 8)
  //   4 = dash-dot (15 12 4 12)
  //   5 = short dash ( 15 15 )
  //   6 = dash-dot-dot   gStyle->SetLineStyleString(1,"[]");
  defaultStyle->SetLineStyleString(2,"[30 10]");
  defaultStyle->SetLineStyleString(3,"[4 8]");
  defaultStyle->SetLineStyleString(4,"[15 12 4 12]");
  defaultStyle->SetLineStyleString(5,"[15 15]");
  defaultStyle->SetLineStyleString(6,"[15 12 4 12 4 12]");
  defaultStyle->SetOptDate(0);
  defaultStyle->SetDateY(.98);
  defaultStyle->SetStripDecimals(kFALSE);
  
  TGaxis::SetMaxDigits(3);
  
  //  defaultStyle->SetEndErrorSize(5.0); //make the end of error bar longer
  
}
