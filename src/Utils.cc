#include "Utils.hh"

/// @brief Translate the TGraph to 0
/// @param gIn input graph 
/// @param minChi2 minimum chi2
void MinimizeToZero(TGraph &gIn, double minChi2){
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

bool IsValid(const TH2 &h)
{
  if(h.GetNbinsX()<1 || h.GetNbinsY()<1) return false;
  return true;
}

bool ConvertCovarianceToUncertainty(TH2D &h, std::vector<double> yields)
{
  if(!IsValid(h)) return false;
  TH2D hCopy(h);
  h.Clear();
  for(int i=1; i<h.GetNbinsX()+1; i++)
  {
    for(int j=1; j<h.GetNbinsY()+1; j++)
    { 
      double binContent = TMath::Sqrt(TMath::Abs(hCopy.GetBinContent(i,j)));
      binContent*=hCopy.GetBinContent(i,j)/TMath::Abs(hCopy.GetBinContent(i,j));
      h.SetBinContent(i,j,binContent*100/(TMath::Sqrt(yields.at(i-1)*yields.at(j-1))));
    }
  }
  return true;
}

bool PrintMatrixToTextFile(const TH2D &h, const TString fName)
{
  if(!IsValid(h)) return false;

  fstream out_file;
  out_file.open(fName.Data(), ios::out);
	if (!out_file) 
  {
		printf("File not created!");
    return false;
	}

  for(int i=1; i<h.GetNbinsX()+1; i++)
  {
    for(int j=1; j<h.GetNbinsY()+1; j++)
    {
      out_file << h.GetBinContent(i,j)<<'\t';
    }
      out_file <<'\n';
  }
  out_file.close();
  printf("File %s created successfully!\n",fName.Data());
  return true;
}