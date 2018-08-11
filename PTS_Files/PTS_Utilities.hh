////////////////////////////////
// Fitting Daya Bay data
// Pranava Teja Surukuchi, March 2017
/////////////////////////////////


static const int NMEASUREMENTS=8;
static const int NTOTMEASUREMENTS=33;
static const int NISOTOPES=4;
static const int MATRIXSIZE=NMEASUREMENTS*NMEASUREMENTS;

// Function to return n sigma value for a given chi2 1D histogram
double GetNSigma(TH1D* hist,int n)
{
  int minBin=hist->GetMinimumBin();
  double minValue = hist->GetMinimum();
  double lowValue=0;
  double highValue=0;
  for(int i=minBin;i>0;i--){
    if(hist->GetBinContent(i)>minValue+n){
      lowValue=hist->GetBinCenter(i);
      break;
    }
  }
  for(int i=minBin;i<hist->GetXaxis()->GetNbins();i++){
    if(hist->GetBinContent(i)>minValue+n){
      highValue=hist->GetBinCenter(i);
      break;
    }
  }
  return (highValue-lowValue)/2;
}

//Project a 2D histogram to 1D chi2 plot in x axis
TH1D* Project2DChi2X(const TH2D* h2DChi2)
{
  TH1D* hChi2X = new TH1D("hChi2X","",h2DChi2->GetXaxis()->GetNbins(),h2DChi2->GetXaxis()->GetBinCenter(1),h2DChi2->GetXaxis()->GetBinCenter(h2DChi2->GetXaxis()->GetNbins()));
  hChi2X->Reset();
  for(int i=1;i<=h2DChi2->GetXaxis()->GetNbins();i++){
    double minChi2 = 10E5;
    for(int j=1;j<=h2DChi2->GetYaxis()->GetNbins();j++){
      if(h2DChi2->GetBinContent(i,j)<minChi2){
        minChi2=h2DChi2->GetBinContent(i,j);
      }
    }
    hChi2X->SetBinContent(i, minChi2-(h2DChi2->GetMinimum()));
  }
  return hChi2X;
}


//Project a 2D histogram to 1D chi2 plot in y axis
TH1D* DirectProject2DChi2X(const TH2D* h2DChi2)
{
  TH1D* hChi2X = new TH1D("hDChi2X","",h2DChi2->GetXaxis()->GetNbins(),h2DChi2->GetXaxis()->GetBinCenter(1),h2DChi2->GetXaxis()->GetBinCenter(h2DChi2->GetXaxis()->GetNbins()));
  hChi2X->Reset();
  
  int x,y,z;
  h2DChi2->GetMinimumBin(x,y,z);
  
  for(int i=1;i<=h2DChi2->GetXaxis()->GetNbins();i++){
    double minChi2=h2DChi2->GetBinContent(i,y);
    hChi2X->SetBinContent(i, minChi2-(h2DChi2->GetMinimum()));
  }
  return hChi2X;
}

//project minimum value for each x bin in the 2D histogram onto the corresponding x axis
TH1D* DirectProject2DChi2Y(const TH2D* h2DChi2)
{
  TH1D* hChi2Y = new TH1D("hDChi2Y","",h2DChi2->GetYaxis()->GetNbins(),h2DChi2->GetYaxis()->GetBinCenter(1),h2DChi2->GetYaxis()->GetBinCenter(h2DChi2->GetYaxis()->GetNbins()));
  hChi2Y->Reset();
  
  int x,y,z;
  h2DChi2->GetMinimumBin(x,y,z);
  
  for(int i=1;i<=h2DChi2->GetYaxis()->GetNbins();i++){
    double minChi2=h2DChi2->GetBinContent(i,y);
    hChi2Y->SetBinContent(i, minChi2-(h2DChi2->GetMinimum()));
  }
  return hChi2Y;
}

//project minimum value for each y bin in the 2D histogram onto the corresponding y axis
TH1D* Project2DChi2Y(const TH2D* h2DChi2)
{
  TH1D* hChi2Y = new TH1D("hChi2Y","",h2DChi2->GetYaxis()->GetNbins(),h2DChi2->GetYaxis()->GetBinCenter(1),h2DChi2->GetYaxis()->GetBinCenter(h2DChi2->GetYaxis()->GetNbins()));
  hChi2Y->Reset();
  for(int i=1;i<=h2DChi2->GetYaxis()->GetNbins();i++){
    double minChi2 = 10E5;
    for(int j=1;j<=h2DChi2->GetXaxis()->GetNbins();j++){
      if(h2DChi2->GetBinContent(j,i)<minChi2){
        minChi2=h2DChi2->GetBinContent(j,i);
      }
    }
    hChi2Y->SetBinContent(i, minChi2-(h2DChi2->GetMinimum()));
  }
  return hChi2Y;
}


// function takes in the txt file and an array as arguments and fills in the array with the covariance matrix terms from the text file
void FillCovMatrix(TString fileName,double (&covList)[MATRIXSIZE])
{
  // read the input file
  std::ifstream covFile(fileName.Data());
  //Check if the file is open
  if (!covFile.is_open()) {
    printf("Error opening the file\n");
    exit(1);
  }
  
  std::string line;
  int count = 0;
  while (!covFile.eof()) {
    std::getline(covFile, line);
    TString inputLine(line);
    if (inputLine.IsWhitespace()) continue;//skip if the line is empty
    if (inputLine.BeginsWith("#")) continue;// skip if the line starts with #
    
    TObjArray *objArr = inputLine.Tokenize(" ");//Divide the line delimited by =
    
    TIterator *iter=objArr->MakeIterator();
    TObjString *valueRead;
    while((valueRead=(TObjString*)iter->Next())) // run through the covariance matrix
    {
      TString valueString=valueRead->GetString();
      if (valueString.BeginsWith("p")) continue;// skip if the line starts with #
      covList[count]=valueString.Atof(); // Store the value obtained in the array
      count++;
    }
  }
}
