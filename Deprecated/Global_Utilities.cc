#include "Global_Utilities.hh"

// Function to return n sigma value for a given chi2 1D histogram
double GetNSigma(TH1D* hist,int n)
{
  int binLow=hist->FindFirstBinAbove(0.01);
  int binHigh=hist->FindLastBinAbove(0.01);
  double binLowValue=hist->GetBinCenter(binLow);
  double binHighValue=hist->GetBinCenter(binHigh);
  
  hist->GetXaxis()->SetRangeUser(binLowValue,binHighValue);
  int minBin=hist->GetMinimumBin();
  double minValue = hist->GetMinimum();
  double lowValue=0;
  double highValue=0;
  
  for(int i=minBin;i>0;i--){
    if(hist->GetBinContent(i)>minValue+n){
      lowValue=hist->GetBinCenter(i+1);
      break;
    }
  }
  for(int i=minBin;i<hist->GetXaxis()->GetNbins();i++){
    if(hist->GetBinContent(i)>minValue+n){
      highValue=hist->GetBinCenter(i-1);
      break;
    }
  }
  return (highValue-lowValue)/2;
}

TH1D *ShiftChi2PlotToZero(TH1D* hist)
{
  TH1D* hist1=(TH1D*)hist->Clone("temp");
  int x,y,z;
  hist->GetMinimumBin(x,y,z);
  double minChi2=hist->GetBinContent(x);
  for(int i=1; i<hist->GetNbinsX(); i++){
    hist1->SetBinContent(i, (hist->GetBinContent(i)-minChi2));
  }
  return hist1;
}
