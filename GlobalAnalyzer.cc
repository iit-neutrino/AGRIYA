#include "GlobalAnalyzer.hh"
using namespace std;

//Reads the data file and stores it in DataArray
//The number of experiments is also determined by
//reading the textfile and counting rows and stores this number in numberofExp
void GlobalAnalyzer::DataInput(){
  double x;
  string lineA;
  ifstream fileIn;
  fileIn.open(fDataInput.Data());
  
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      columnsA = 0;
      while(streamA >>x){
        DataArray[numberofExp][columnsA] = x;
        columnsA++;
      }
      numberofExp++;
    }
    
  }
  std::cout << "Number of experiments = " <<numberofExp <<std::endl;
}

// Initialize the global analyzer
void GlobalAnalyzer::InitializeAnalyzer(TString dataInput, TString covStat, TString covSyst){
  fDataInput=dataInput;
  fCovStat=covStat;
  fCovSyst=covSyst;
  std::cout << "Using " << dataInput.Data() <<  " data file" <<std::endl;
  std::cout << "Using " << covStat.Data() <<  " stat file" <<std::endl;
  std::cout << "Using " << covSyst.Data() <<  " syst file" <<std::endl;
  ///The information from Data text file is read when the object is initialized
  DataInput();
}

//// Stores information read from the Data text and stores it in the corresponding vectors
void GlobalAnalyzer::LoadingDataToVector(){
  v_FF_235.ResizeTo(numberofExp);
  v_FF_238.ResizeTo(numberofExp);
  v_FF_239.ResizeTo(numberofExp);
  v_FF_241.ResizeTo(numberofExp);
  v_FF_239241.ResizeTo(numberofExp);
  v_IBD_Exp.ResizeTo(numberofExp);
  v_IBD_Exp_temp.ResizeTo(numberofExp);
  
  v_Diff.ResizeTo(numberofExp);
  g_IBD_Exp=new TGraph();
  g_IBD_Exp->SetName("g_IBD_Exp");
  
  for(int i=0; i<numberofExp; i++){
    
    v_FF_235[i]=DataArray[i][0];
    v_FF_238[i]=DataArray[i][1];
    v_FF_239[i]=DataArray[i][2];
    v_FF_241[i]=DataArray[i][3];
    v_FF_239241[i]=v_FF_239[i]*(1.0+F239F241Ratio);
    
    //    v_FF_239241[i]=v_FF_239[i]+v_FF_241[i];
    v_IBD_Exp[i]=DataArray[i][4];
    g_IBD_Exp->SetPoint(i,v_FF_235[i],v_IBD_Exp[i]);
  }
  std::cout << "data file " <<std::endl;
  v_IBD_Exp.Print();
  v_IBD_Exp_temp=v_IBD_Exp;
}



/// loads the vectors contaning the fission fractions
/// to v_FissionFraction map.
void GlobalAnalyzer:: LoadFissionFractionMap(){
  for(int i=0;i<numberofExp;i++)v_FissionFraction[i].ResizeTo(numberofExp);
  v_FissionFraction[0]=v_FF_235;
  v_FissionFraction[1]=v_FF_238;
  v_FissionFraction[2]=v_FF_239;
  v_FissionFraction[3]=v_FF_241;
  
  xSectionSH[0]=6.05;
  xSectionSH[1]=10.10;
  xSectionSH[2]=4.40;
  xSectionSH[3]=6.69;
}


void GlobalAnalyzer::LoadTheoCovMat(){
  switch (fFitType) {
    case 1: // U235 only fit
      Theo_CovarianceMatrix.ResizeTo(3,3);
      Theo_CovarianceMatrix(0,0)=0.0246;
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.6776;
      
      Theo_CovarianceMatrix(0,2)=0.0194;
      Theo_CovarianceMatrix(2,0)=0.0194;
      
      Theo_CovarianceMatrix(1,2)=0;
      Theo_CovarianceMatrix(2,1)=0;
      
      Theo_CovarianceMatrix(2,2)=0.0161;
      
      break;
    case 2: // U239 only fit
      Theo_CovarianceMatrix.ResizeTo(3,3);
      Theo_CovarianceMatrix(0,0)=0.0246;
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.6776;
      
      Theo_CovarianceMatrix(0,2)=0.0255;
      Theo_CovarianceMatrix(2,0)=0.0255;
      
      Theo_CovarianceMatrix(2,1)=0;
      Theo_CovarianceMatrix(1,2)=0;
      
      Theo_CovarianceMatrix(2,2)=0.0267;
      break;
      
    case 3: // U235+U239 only fit
      Theo_CovarianceMatrix.ResizeTo(2,2);
      Theo_CovarianceMatrix(0,0)=0.0246;
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.6776;
      break;
      
    case 4: // U235+U239+U238 only fit
      Theo_CovarianceMatrix.ResizeTo(1,1);
      Theo_CovarianceMatrix(0,0)=0.0246;
      break;
    default: // Nothing for now, just exit if this case arises
      printf("Please select a proper fit type\n");
      exit(-1);
      break;
  }
  
  
  /*
   Theo_CovarianceMatrix(0,0)=0.0246;
   Theo_CovarianceMatrix(0,1)=0;
   
   Theo_CovarianceMatrix(1,0)=0;
   Theo_CovarianceMatrix(1,1)=0.6776;
   
   Theo_CovarianceMatrix(0,2)=0.0194;
   Theo_CovarianceMatrix(0,3)=0.0255;
   Theo_CovarianceMatrix(2,0)=0.0194;
   Theo_CovarianceMatrix(3,0)=0.0255;
   
   Theo_CovarianceMatrix(3,1)=0;
   Theo_CovarianceMatrix(2,1)=0;
   Theo_CovarianceMatrix(1,3)=0;
   Theo_CovarianceMatrix(1,2)=0;
   
   Theo_CovarianceMatrix(3,3)=0.0267;
   Theo_CovarianceMatrix(3,2)=0.0203;
   Theo_CovarianceMatrix(2,3)=0.0203;
   Theo_CovarianceMatrix(2,2)=0.0161;
   */
  Theo_CovarianceMatrix.Print();
  if(Theo_CovarianceMatrix.InvertFast()==0) exit(1);
  Theo_CovarianceMatrix.Print();
}

/// Stores the information in systematic covariancs elemenst and stores it in Syst_CovarianceMatrix and
/// Stores the information in statistical covariancs elemenst and stores it in Stat_CovarianceMatrix.
void GlobalAnalyzer::LoadCovarianceMatrix(){
  Syst_CovarianceMatrix.ResizeTo(numberofExp, numberofExp);
  Stat_CovarianceMatrix.ResizeTo(numberofExp, numberofExp);
  //  Tot_CovarianceMatrix.ResizeTo(numberofExp,numberofExp);
  //  theoIBDYieldProductMatrix.ResizeTo(numberofExp,numberofExp);
  
  int rowCounter = 0;
  int columnCounter = 0;
  
  double numberRead;   ///The number currently read from the file
  string lineRead;    ///The line of text file currently read
  
  ifstream systFileIn;
  
  ///Loading SystCov.txt into Syst_CovarianceMatrix
  systFileIn.open(fCovSyst.Data());
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
  std::cout << "Systematic cov matrix" <<std::endl;
  Syst_CovarianceMatrix.Print();
  
  rowCounter=0;
  columnCounter=0;
  ///Loading StatCov.txt into Stat_CovarianceMatrix
  ifstream statFileIn;
  statFileIn.open(fCovStat.Data());
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
  std::cout << "Statistical cov matrix" <<std::endl;
  Stat_CovarianceMatrix.Print();
  //  Tot_CovarianceMatrix = Syst_CovarianceMatrix;
}

void GlobalAnalyzer::AddingFluctuation(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(4.55e-04, 0.5);
  
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.268e-04, 0.5);
  ///syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=0; i<(numberofExp-1);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  stat_term[numberofExp-1] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  v_IBD_Exp_temp[numberofExp-1] = (systHEU_uncorrterm + stat_term[numberofExp-1] + 1) * v_IBD_Exp_temp[numberofExp-1];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}


void GlobalAnalyzer::AddingFluctuationOne(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(4.55e-04, 0.5);
  
  //  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.268e-04, 0.5);
  ///syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=0; i<(numberofExp);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  //  stat_term[numberofExp-1] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  //  v_IBD_Exp_temp[numberofExp-1] = (systHEU_uncorrterm + stat_term[numberofExp-1] + 1) * v_IBD_Exp_temp[numberofExp-1];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}

void GlobalAnalyzer::AddingFluctuationTwo(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(4.55e-04, 0.5);
  
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.268e-04, 0.5);
  ///syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=0; i<(numberofExp-1);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  stat_term[numberofExp-1] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  v_IBD_Exp_temp[numberofExp-1] = (systHEU_uncorrterm + stat_term[numberofExp-1] + 1) * v_IBD_Exp_temp[numberofExp-1];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}

void GlobalAnalyzer::AddingFluctuationThree(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.66e-04, 0.5);
  
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.268e-04, 0.5);
  ///syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=0; i<(numberofExp-1);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  stat_term[numberofExp-1] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  v_IBD_Exp_temp[numberofExp-1] = (systHEU_uncorrterm + stat_term[numberofExp-1] + 1) * v_IBD_Exp_temp[numberofExp-1];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}


void GlobalAnalyzer::AddingFluctuationFour(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  ///double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.66e-04, 0.5);
  
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(2.268e-04, 0.5);
  ///syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=1; i<(numberofExp);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm + stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  stat_term[0] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  v_IBD_Exp_temp[0] = (systHEU_uncorrterm + stat_term[0] + 1) * v_IBD_Exp_temp[0];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}


void GlobalAnalyzer::AddingFluctuationFive(const double & seed){
  ///Allows Statistical and Systematic fluctuation of the input
  ///data's IBDyield
  v_IBD_Exp_temp=v_IBD_Exp;
  TRandom1 myRandom(seed);
  printf("New seed %f generated for toy %i:\n",seed,count);
  
  double systHEU_uncorrterm = 0.0;
  double systLEU_uncorrterm = 0.0;
  double syst_corrterm = 0.0;
  double stat_term[numberofExp];
  
  systLEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(0.65e-04, 0.5);
  
  systHEU_uncorrterm  = myRandom.Gaus(0.0,1) * pow(0.258e-04, 0.5);
  syst_corrterm = myRandom.Gaus(0.0,1)*1.4e-02;
  
  for(int i=0; i<(numberofExp);i++){
    stat_term[i] = myRandom.Gaus(0.0,1) * (1/pow(266670, 0.5)); //324393.75
    
    v_IBD_Exp_temp[i] = (systLEU_uncorrterm +syst_corrterm+ stat_term[i] + 1) * v_IBD_Exp_temp[i];
  }
  
  stat_term[0] = myRandom.Gaus(0.0,1) * (1/pow(480000,0.5));
  v_IBD_Exp_temp[0] = (systHEU_uncorrterm +syst_corrterm+ stat_term[0] + 1) * v_IBD_Exp_temp[0];
  //  v_IBD_Exp_temp.Print();
  count += 1;
}

void GlobalAnalyzer::CalculateTheoreticalIBDYield(TMatrixD& mTheo,const TVectorD &v_5,const TVectorD &v_8,const TVectorD &v_9,const TVectorD &v_1) const
{
  mTheo.Zero();
  mTheo=OuterProduct(v_FF_235,v_5);
  mTheo+=OuterProduct(v_FF_238,v_8);
  mTheo+=OuterProduct(v_FF_239,v_9);
  mTheo+=OuterProduct(v_FF_241,v_1);
}
/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, Pu239 and Pu241 respectively and returns
/// a vector of the theoretical IBD yield.
void GlobalAnalyzer::CalculateTheoreticalIBDYield(TVectorD& yTheo,const double &y_U235,const double &y_U238,const double &y_P239,const double &y_P241) const{
  yTheo.ResizeTo(numberofExp);
  yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239*v_FF_239 + y_P241*v_FF_241;
}

/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
/// a vector of the theoretical IBD yields.
void GlobalAnalyzer::CalculateTheoreticalIBDYield(TVectorD& yTheo,const double &y_U235,const double &y_U238,const double &y_P239241) const{
  yTheo.ResizeTo(numberofExp);
  // Theoretical IBD yields assuming we combine 239 and 241
  yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239241*v_FF_239241;
}

/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, and combined Pu239-Pu241 respectively and returns
/// a vector of the theoretical IBD yields.
/*void GlobalAnalyzer::CalculateTheoreticalIBDYield(double F239F241Ratio,TVectorD &yTheo,const double &y_U235,const double &y_U238,const double &y_P239241 ) const{
 yTheo.ResizeTo(numberofExp);
 // Theoretical IBD yields assuming we combine 239 and 241
 // With an assumption that 239 and 241 are linear, v_FF_239*(1+F239F241Ratio)==v_FF_239241
 for(int i=0; i<numberofExp; i++){
 v_FF_239241[i]=v_FF_239[i]*(1+F239F241Ratio);
 }
 yTheo = y_U235*v_FF_235 + y_U238*v_FF_238 + y_P239241*v_FF_239241;
 }*/

//matrix inputs are:
/// The theoretical IBD yield vector and the IBD yield of U238 and Pu241 for Daya Bay
/// The additional Stat_CovarianceMatrix[i][j] term is zero for non DayaBay experiments
/// The if statment included is for Daya Bay experiments that have the U238 and Pu241
/// uncertantiy term in their covariance matrix.
void GlobalAnalyzer::CalculateCovarianceMatrix(const TVectorD &yTheo, TMatrixD &CovarianceMatrix) const{
  CovarianceMatrix.Zero();
  TMatrixD Tot_CovarianceMatrix(CovarianceMatrix);
  Tot_CovarianceMatrix.Zero();
  Tot_CovarianceMatrix=Syst_CovarianceMatrix;
  TMatrixD theoIBDYieldProductMatrix(CovarianceMatrix);
  theoIBDYieldProductMatrix.Zero();
  theoIBDYieldProductMatrix=OuterProduct(yTheo,yTheo);
  CovarianceMatrix=ElementMult(Tot_CovarianceMatrix,theoIBDYieldProductMatrix);
  CovarianceMatrix+= Stat_CovarianceMatrix;
}


//matrix inputs are:
/// The theoretical IBD yield vector and the IBD yield of U238 and Pu241 for Daya Bay
/// The additional Stat_CovarianceMatrix[i][j] term is zero for non DayaBay experiments
/// The if statment included is for Daya Bay experiments that have the U238 and Pu241
/// uncertantiy term in their covariance matrix.
void GlobalAnalyzer::CalculateCorrelatedErrors(const TVectorD &yTheo, TMatrixD &CorrelatedUncertaintyMatrix) const{
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

void GlobalAnalyzer::CalculateTheoDeltaVector(const double* xx, TVectorD &rValues) const{
  
  switch (fFitType) {
    case 1: // U235 only fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[3]-xSectionSH[0];
      rValues[1]=xx[1]-xSectionSH[1];
      rValues[2]=xx[2]-xSectionSH[2];
      break;
      
    case 2: // U239 only fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[3]-xSectionSH[0];
      rValues[1]=xx[1]-xSectionSH[1];
      rValues[2]=xx[0]-xSectionSH[3];
      break;
      
    case 3: // U235+U239 only fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[3]-xSectionSH[0];
      rValues[1]=xx[1]-xSectionSH[1];
      break;
      
    case 4: // U235+U239+U238 only fit
      rValues[0]=xx[3]-xSectionSH[0];
      break;
    default:
      assert(-1);
      break;
  }
}

double GlobalAnalyzer::DoEval(const double* xx)const{
  
  TVectorD rValues;
  CalculateTheoDeltaVector(xx,rValues);
  TVectorD rValuesTemp=rValues;
  rValuesTemp*=Theo_CovarianceMatrix;
  
  TVectorD yTheo(numberofExp);
  TMatrixD CovarianceMatrix(numberofExp,numberofExp);
  CovarianceMatrix.Zero();
  CalculateTheoreticalIBDYield(yTheo,xx[0],xx[1],xx[2],xx[3]);
  CalculateCovarianceMatrix(yTheo,CovarianceMatrix);
  if(CovarianceMatrix.Invert()==0) exit(1);
  TVectorD vTemp = yTheo;
  vTemp -= v_IBD_Exp_temp;
  double Chi2Value=Mult(vTemp,CovarianceMatrix,vTemp);
  //  std::cout << Chi2Value <<std::endl;
  Chi2Value+=rValuesTemp*rValues;
  //  std::cout << Chi2Value <<std::endl;
  //  std::cout << ""<<std::endl;
  return Chi2Value;
}

/// Calculates Chi2 value, input variables are:
/// yTheo is a vectro that has the theoretical IBD yield of experiments
/// InvCovarianceMatrix is the covariancs matrix for a particular combination of IBD yiedls
double GlobalAnalyzer::CalculateChi2(const TVectorD &yTheo,const TMatrixD &InvCovarianceMatrix){
  v_Diff = yTheo;
  v_Diff -= v_IBD_Exp_temp;
  
  return Mult(v_Diff,InvCovarianceMatrix,v_Diff);
}

/// returns the r or IBD of the one sigma point
double GlobalAnalyzer::OneSigmaCorrValue(TH1D * hist, const double &minIBD, const double &maxIBD, const int &nBins){
  
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

void GlobalAnalyzer::DrawDataPoints(TFile &outFile){
  outFile.cd();
  g_IBD_Exp->Write();
}

/// SetupExperiments will be called during runtime so that
/// the functions inside it can all be called at run time.
void GlobalAnalyzer::SetupExperiments(int fitType){
  fFitType=fitType;
  LoadingDataToVector();
  LoadFissionFractionMap();
  LoadCovarianceMatrix();
  LoadTheoCovMat();
}

