#include "StyleFile.hh"

/// @brief Setup the default style for all plotting
/// @param defaultStyle 
void setupStyle(TStyle* defaultStyle) {
  
  if(!defaultStyle) defaultStyle = gStyle;
  assert(defaultStyle);
  
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
  // const Int_t NRGBs = 5;
  // const Int_t NCont = 99;
  // Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  // Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  // Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  // Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  // defaultStyle->SetNumberContours(NCont);
  
  // defaultStyle->SetPalette(1,0); // set a good color palette
  //defaultStyle->SetFillColor(0); // explicitly don't fill
  //Int_t colors[]={4,4,7,7,6,6,6,2,2,2,1};
  //   float levels[]={1,10,14,22,45,60,200,300,400,500,600,700};
  
  // set the paper & margin sizes
  defaultStyle->SetPaperSize(TStyle::kUSLetter);

  // Margins, assuming no top label or z scale
  defaultStyle->SetPadTopMargin(.05); // just enough to clear exponent on y axis
  defaultStyle->SetPadLeftMargin(.2);
  defaultStyle->SetPadRightMargin(.12);
  defaultStyle->SetPadBottomMargin(.15);
  
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
  defaultStyle->SetLabelOffset(0.01,"xyz");

  defaultStyle->SetTitleFont(font,"xyz");
  defaultStyle->SetTitleSize(0.06,"xyz");
  defaultStyle->SetTitleOffset(1.25,"z");
  defaultStyle->SetTitleOffset(1.1,"y");
  defaultStyle->SetTitleOffset(1.45,"y");
  defaultStyle->SetTitleBorderSize(0);
  
  //do not display any of the standard histogram decorations
  defaultStyle->SetOptTitle(0);      //no title box
  defaultStyle->SetOptStat("");      //no stat info
  defaultStyle->SetOptFit(0);        //no fit info

  //legend and stat pad
  defaultStyle->SetStatBorderSize(1);
  defaultStyle->SetStatFont(font);
  defaultStyle->SetStatX(0.95);
  defaultStyle->SetStatY(0.95);
  defaultStyle->SetLegendBorderSize(0);
  
  // put tick marks on top and RHS of plots
  defaultStyle->SetPadTickX(1);
  defaultStyle->SetPadTickY(1);

  TColor *color = new TColor(l1, 68.0/256, 119.0/256, 170.0/256);
  color = new TColor(l2, 238.0/256, 102.0/256, 119.0/256);
  color = new TColor(l3, 34.0/256, 34.0/256, 51.0/256);
  color = new TColor(l4, 102.0/256, 204.0/256, 238.0/256);
  color = new TColor(l5, 204.0/256, 187.0/256, 68.0/256);
  

  TColor *contColor = new TColor(c1, 102.0/256, 37.0/256, 6.0/256);
  color = new TColor(c2, 204.0/256, 76.0/256, 2.0/256);
  color = new TColor(c3, 251.0/256, 154.0/256, 41.0/256);
  color = new TColor(c4, 254.0/256, 226.0/256, 145.0/256);
  color = new TColor(c5, 255.0/256, 247.0/256, 188.0/256);
  
  defaultStyle->SetPalette(kInvertedDarkBodyRadiator);
  
  // defaultStyle->SetPalette((sizeof(contColors)/sizeof(Int_t)), contColors);
}
