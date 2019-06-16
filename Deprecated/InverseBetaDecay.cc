#include "InverseBetaDecay.hh"
using namespace std;


//Reads the data file and stores it in DataArray
//The number of experiments is also determined by
//reading the textfile and counting rows and stores this number in numberofExp
void InverseBetaDecay::DataInput(){
  double x;
  string lineA;
  ifstream fileIn;
  fileIn.open("inputs/PRHEULEUE8.txt");
  
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      columnsA = 0;
      while(streamA >>x){
        DataArray[numberofExp][columnsA] = x;
        columnsA++;
      }
      numberofExp++;
      std::cout <<numberofExp;
    }
    
  }
  
} 


//// Stores information read from the Data text and stores it in the corresponding vectors
void InverseBetaDecay::LoadingDataToVector(){
  v_FF_235.ResizeTo(numberofExp);
  v_FF_238.ResizeTo(numberofExp);
  v_FF_239.ResizeTo(numberofExp);
  v_FF_241.ResizeTo(numberofExp);
  v_FF_239241.ResizeTo(numberofExp);
  v_IBD_Exp.ResizeTo(numberofExp);

  v_Diff.ResizeTo(numberofExp);
  
  for(int i=0; i<numberofExp; i++){
    
    v_FF_235[i]=DataArray[i][0];
    v_FF_238[i]=DataArray[i][1];
    v_FF_239[i]=DataArray[i][2];
    v_FF_241[i]=DataArray[i][3];
    v_FF_239241[i]=v_FF_239[i]*(1.0+F239F241Ratio);
    
    //    v_FF_239241[i]=v_FF_239[i]+v_FF_241[i];
    v_IBD_Exp[i]=DataArray[i][4];
  }
}



/// loads the vectors contaning the fission fractions
/// to v_FissionFraction map.
void InverseBetaDecay:: LoadFissionFractionMap(){
  for(int i=0;i<numberofExp;i++)v_FissionFraction[i].ResizeTo(numberofExp);
  v_FissionFraction[0]=v_FF_235;
  v_FissionFraction[1]=v_FF_238;
  v_FissionFraction[2]=v_FF_239;
  v_FissionFraction[3]=v_FF_241;
}

/// Stores the information in systematic covariancs elemenst and stores it in Syst_CovarianceMatrix and
/// Stores the information in statistical covariancs elemenst and stores it in Stat_CovarianceMatrix.
void InverseBetaDecay::LoadCovarianceMatrix(){
  Syst_CovarianceMatrix.ResizeTo(numberofExp, numberofExp);
  Stat_CovarianceMatrix.ResizeTo(numberofExp, numberofExp);
  
  int rowCounter = 0;
  int columnCounter = 0;
  
  double numberRead;   ///The number currently read from the file
  string lineRead;    ///The line of text file currently read
  
  ifstream systFileIn;
  
  ///Loading SystCov.txt into Syst_CovarianceMatrix
  systFileIn.open("inputs/DYB+PRHEU_covsyst.txt");
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
  
  rowCounter=0;
  columnCounter=0;
  ///Loading StatCov.txt into Stat_CovarianceMatrix
  ifstream statFileIn;
  statFileIn.open("inputs/DYB+PRHEU_covstat2E.txt");
  while(statFileIn.good()){
    while(getline(statFileIn, lineRead)){
      istringstream streamA(lineRead);
      columnCounter = 0;
      while(streamA >>numberRead){
        Stat_CovarianceMatrix(rowCounter,columnCounter) = numberRead;
        columnCounter++;
      }
      rowCounter++;
    }
  }
}

void InverseBetaDecay::AddingFluctuation(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input 
  ///data's IBDyield 
  TRandom1 myRandom(seed);
  std::cout << "\n";
  std::cout << "\n";
  std::cout << "new seed generated:" << seed << " " <<count;
  std::cout << "\n";
 
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
     
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(4.5e-04, 0.5);
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * 1.506e-02;//pow(2.21e-04, 0.5);
  /*syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;*/

  for(int i=0; i<(numberofExp - 1);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(275000, 0.5)); //324393.75
    /*std::cout<< stat_term[i] << "\n";*/
    v_IBD_Exp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp[i];
   /* std::cout<< systLEU_uncorrterm + stat_term[i] << "\n";*/
  }

  stat_term[numberofExp - 1] = myRandom.Gaus(0.0,1) * 0.2041e-02;//(pow(2,0.5))*(1/pow(480000,0.5));
  v_IBD_Exp[numberofExp - 1] = (systHEU_uncorrterm + stat_term[numberofExp - 1] + 1) * v_IBD_Exp[numberofExp - 1];
  //std::cout<< "\n" << v_IBD_Exp[0] << "\n";
  //std::cout<< systHEU_uncorrterm + stat_term[numberofExp - 1] << "\n";
  count += 1;

} 


/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, Pu239 and Pu241 respectively and returns
/// a vector of the theoretical IBD yield.
void InverseBetaDecay::TheorIBDCalculator(TVectorD& yTheo,const double &y_U235,const double &y_U238,const double &y_P239,const double &y_P241){
  yTheo.ResizeTo(numberofExp);
  yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239*v_FF_239 + y_P241*v_FF_241;
}

/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
/// a vector of the theoretical IBD yields.
void InverseBetaDecay::TheorIBDCalculator(TVectorD& yTheo,const double &y_U235,const double &y_U238,const double &y_P239241){
  yTheo.ResizeTo(numberofExp);
  // Theoretical IBD yields assuming we combine 239 and 241
  yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239241*v_FF_239241;
}

/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
/// a vector of the theoretical IBD yields.
void InverseBetaDecay::TheorIBDCalculator(double F239F241Ratio,TVectorD &yTheo,const double &y_U235,const double &y_U238,const double &y_P239241 ){
  yTheo.ResizeTo(numberofExp);
  // Theoretical IBD yields assuming we combine 239 and 241
  // With an assumption that 239 and 241 are linear, v_FF_239*(1+F239F241Ratio)==v_FF_239241
  for(int i=0; i<numberofExp; i++){
    v_FF_239241[i]=v_FF_239[i]*(1+F239F241Ratio);
  }
  yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239241*v_FF_239241;
}

//matrix inputs are:
/// The theoretical IBD yield vector and the IBD yield of U238 and Pu241 for Daya Bay
/// The additional Stat_CovarianceMatrix[i][j] term is zero for non DayaBay experiments
/// The if statment included is for Daya Bay experiments that have the U238 and Pu241
/// uncertantiy term in their covariance matrix.
void InverseBetaDecay::CovarianceMatrixCalculator(TVectorD &yTheo, TMatrixD &CovarianceMatrix){
  CovarianceMatrix = Syst_CovarianceMatrix;//+0.25
  double yTheoij=0.0;
  for(int i=0;i<numberofExp;i++){
    for(int j=0;j<numberofExp; j++){

      yTheoij=yTheo[i] * yTheo[j];
      CovarianceMatrix(i,j)*= yTheoij;
      /*if (i != j){
       CovarianceMatrix(i,j)*=0; 
      }*/
    }
  }//+2.1
  CovarianceMatrix+= Stat_CovarianceMatrix;
}




//matrix inputs are:
/// The theoretical IBD yield vector and the IBD yield of U238 and Pu241 for Daya Bay
/// The additional Stat_CovarianceMatrix[i][j] term is zero for non DayaBay experiments
/// The if statment included is for Daya Bay experiments that have the U238 and Pu241
/// uncertantiy term in their covariance matrix.
void InverseBetaDecay::CalculateCorrelatedErrors(const TVectorD &yTheo, TMatrixD &CorrelatedUncertaintyMatrix){
  if(CorrelatedUncertaintyMatrix.GetNoElements()!=Syst_CovarianceMatrix.GetNoElements()){
    printf("Covariance matrix elements do not match %i != %i",CorrelatedUncertaintyMatrix.GetNoElements(),Syst_CovarianceMatrix.GetNoElements());
  }
 // double yTheoij=0.0;
  for(int i=0;i<numberofExp;i++){
    for(int j=0;j<numberofExp; j++){
      CorrelatedUncertaintyMatrix(i,j) = sqrt(Syst_CovarianceMatrix(i,j));//+0.25
      //      CorrelatedUncertaintyMatrix(i,j)*= yTheo[i];
      CorrelatedUncertaintyMatrix(i,j)+= sqrt(Stat_CovarianceMatrix(i,j));
    }
  }//+2.1
}


/// Calculates Chi2 value, input variables are:
/// yTheo is a vectro that has the theoretical IBD yield of experiments
/// InvCovarianceMatrix is the covariancs matrix for a particular combination of IBD yiedls
double InverseBetaDecay::Chi2Calculator(TVectorD &yTheo, TMatrixD &InvCovarianceMatrix){
  double Chi2Value;
  
  v_Diff = yTheo;
  v_Diff -= v_IBD_Exp;
  
  ///Multyplying the above difference with the inverse covariance matrix
  TVectorD v_Mult = v_Diff;
  v_Mult *= InvCovarianceMatrix;
  
  /// Calculating Chi2
  Chi2Value = v_Mult*v_Diff;
  
  return Chi2Value;
}

/// returns the r or IBD of the one sigma point
double InverseBetaDecay::OneSigmaCorrValue(TH1D * hist, const double &minIBD, const double &maxIBD, const int &nBins){

  /// First non zero bin
  int FirstBin =hist->FindFirstBinAbove(0);

  /// Minimum of the histogram
  double MinChi2 =hist->GetMinimum(0);

    /// This will store the value in the bins as the for loop below 
    /// goes through the histogram
  double BinValue;

  double OneSigmaValue = MinChi2 + 1;
  double Chi2Diff = 10E8;

  /// Stores the bin where one sigmavalue is found
  int OneSigmaBin;

  for(int i = FirstBin; i < (FirstBin + nBins); i++){
    BinValue= hist->GetBinContent(i);
    if(abs(BinValue - OneSigmaValue) < Chi2Diff){
      Chi2Diff = abs(BinValue - OneSigmaValue);
      OneSigmaBin = i;
    }
     }

  double OneSigma = (OneSigmaBin - FirstBin)*((maxIBD - minIBD) /nBins) + minIBD;
  return OneSigma;

}

 
/// SetupExperiments will be called during runtime so that
/// the functions inside it can all be called at run time.
void InverseBetaDecay::SetupExperiments(){
  LoadingDataToVector();
  LoadFissionFractionMap();
  LoadCovarianceMatrix();
  

}

