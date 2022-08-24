#include "GlobalAnalyzer.hh"
using namespace std;

/// Reads the data file and stores it in DataArray
/// The number of experiments is also determined by
/// reading the textfile and the number of counted rows 
/// will be stored in the #numberofExp
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
  v_FF_240.ResizeTo(numberofExp); // ADD
  v_FF_241.ResizeTo(numberofExp);
  v_Baseline.ResizeTo(numberofExp);
  v_FF_239241.ResizeTo(numberofExp);
  v_IBD_Exp.ResizeTo(numberofExp);
  v_IBD_Exp_temp.ResizeTo(numberofExp);
  
  v_Diff.ResizeTo(numberofExp);
  g_IBD_Exp=new TGraphErrors();
  g_IBD_Fit=new TGraphErrors();
  g_IBD_Exp->SetName("g_IBD_Exp");
  g_IBD_Fit->SetName("g_IBD_Fit");
  
  for(int i=0; i<numberofExp; i++){
    v_FF_235[i]=DataArray[i][0];
    v_FF_238[i]=DataArray[i][1];
    v_FF_239[i]=DataArray[i][2];
    v_FF_240[i]=DataArray[i][3]; // ADD -- insert P240 data column after P239
    v_FF_241[i]=DataArray[i][4];
    v_FF_239241[i]=v_FF_239[i]*(1.0+F239F241Ratio);
    //    v_FF_239241[i]=v_FF_239[i]+v_FF_241[i];
    v_IBD_Exp[i]=DataArray[i][5]; // MOD (4->5)
    v_Baseline[i]=DataArray[i][6]; // MOD (5->6)
    g_IBD_Exp->SetPoint(i,v_FF_239[i],v_IBD_Exp[i]);
    ff_239+=v_FF_239[i];
  }
  ff_239=ff_239/numberofExp;
  std::cout << "Number of Experiments: " << numberofExp << std::endl; // DEBUG LINE
  std::cout << "data file " <<std::endl;
  v_FF_235.Print(); // DEBUG LINE
  v_FF_238.Print(); // DEBUG LINE
  v_FF_239.Print(); // DEBUG LINE
  v_FF_241.Print(); // DEBUG LINE
  v_FF_240.Print(); // DEBUG LINE
  v_IBD_Exp.Print();
  v_IBD_Exp_temp=v_IBD_Exp;
}



/// loads the vectors contaning the fission fractions
/// to v_FissionFraction map.
void GlobalAnalyzer:: LoadFissionFractionMap(){
  for(int i=0;i<numberofIso;i++)v_FissionFraction[i].ResizeTo(numberofExp);
  v_FissionFraction[0]=v_FF_235;
  v_FissionFraction[1]=v_FF_238;
  v_FissionFraction[2]=v_FF_239;
  v_FissionFraction[3]=v_FF_240; // ADD
  v_FissionFraction[4]=v_FF_241;

  std::cout << "v_FissionFraction[4]" << std::endl; // DEBUG LINE
  v_FissionFraction[3].Print(); // DEBUG LINE
  
  xSectionSH[0]=6.03; // P241
  xSectionSH[1]=10.10;// U238
  xSectionSH[2]=4.40; // P239
  xSectionSH[3]=6.69; // U235
  xSectionSH[4]=4.96; // P240 ADD
  
  /* comment blocked 6-9-2021 to test whether or not this is important PTS: This is needed if you are doing any fits that include oscillations
  f235Yield=new TF1("235Yield","TMath::Exp(0.87-0.160*x-0.091*TMath::Power(x,2))",1.8,10);
  f238Yield=new TF1("238Yield","TMath::Exp(0.976-0.162*x-0.0790*TMath::Power(x,2))",1.8,10);
  f239Yield=new TF1("239Yield","TMath::Exp(0.896-0.239*x-0.0981*TMath::Power(x,2))",1.8,10);
  f240Yield=new TF1("240Yield","TMath::Exp(*TMath::Power(x,2))",1.8,10); // ADD -- What is this?
  f241Yield=new TF1("241Yield","TMath::Exp(1.044-0.232*x-0.0982*TMath::Power(x,2))",1.8,10);
  fIBDxSec=new TF1("IBDxSec","9.52*(x-1.293)*TMath::Sqrt(TMath::Power(x-1.293,2)-TMath::Power(0.511,2))",1.8,10);
  */
}

//PTS: Need to include theo covariances when using 240, but you already know it. 
void GlobalAnalyzer::LoadTheoCovMat(){
  switch (fFitType) {
    case 1: case 6:case 9: // U235 only,U235+Osc and U235+Eq fits
      Theo_CovarianceMatrix.ResizeTo(4,4); // MOD 3->4
      Theo_CovarianceMatrix(0,0)=0.0246;
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.6776;

      Theo_CovarianceMatrix(1,2)=0;
      Theo_CovarianceMatrix(2,1)=0;
      
      Theo_CovarianceMatrix(2,2)=0.6776; // ADD
      
      Theo_CovarianceMatrix(0,3)=0.0194;
      Theo_CovarianceMatrix(3,0)=0.0194;

      Theo_CovarianceMatrix(1,3)=0; // ADD
      Theo_CovarianceMatrix(3,1)=0; // ADD
      Theo_CovarianceMatrix(2,3)=0; // ADD
      Theo_CovarianceMatrix(3,2)=0; // ADD
      
      Theo_CovarianceMatrix(3,3)=0.0161;
      break;
    case 2: case 7:case 10: // U239 only,U239+Osc and U239+Eq fit
      Theo_CovarianceMatrix.ResizeTo(4,4); // MOD 3->4
      Theo_CovarianceMatrix(0,0)=0.0246;
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.6776;

      Theo_CovarianceMatrix(1,2)=0;
      Theo_CovarianceMatrix(2,1)=0;
      
      Theo_CovarianceMatrix(2,2)=0.6776; // ADD
      
      Theo_CovarianceMatrix(0,3)=0.0255;
      Theo_CovarianceMatrix(3,0)=0.0255;

      Theo_CovarianceMatrix(1,3)=0; // ADD
      Theo_CovarianceMatrix(3,1)=0; // ADD
      Theo_CovarianceMatrix(2,3)=0; // ADD
      Theo_CovarianceMatrix(3,2)=0; // ADD
      
      Theo_CovarianceMatrix(3,3)=0.0267;
      break;
      
    case 3: // U235+U239 only fit
      Theo_CovarianceMatrix.ResizeTo(3,3); // MOD (2,2)->(3,3)
      Theo_CovarianceMatrix(0,0)=0.0246; // original  value 0.0246; value = (uncer * theo)^2 (3.27 for 30%)
      Theo_CovarianceMatrix(0,1)=0;
      Theo_CovarianceMatrix(1,0)=0;
      Theo_CovarianceMatrix(1,1)=0.1777; // originally 0.1777 (8.15%)
      
      Theo_CovarianceMatrix(0,2)=0; // ADD
      Theo_CovarianceMatrix(2,0)=0; // ADD

      Theo_CovarianceMatrix(1,2)=0; // ADD
      Theo_CovarianceMatrix(2,1)=0; // ADD

      Theo_CovarianceMatrix(2,2)=0.6776; // ADD -- original value 0.6776 (8.15%) (9.1 for 30%)
      break;
      
    case 4: // U235+U239+U238 only fit
      Theo_CovarianceMatrix.ResizeTo(2,2);
      Theo_CovarianceMatrix(0,0)=0.0246;

      Theo_CovarianceMatrix(0,1)=0;   // ADD
      Theo_CovarianceMatrix(1,0)=0;   // ADD
      Theo_CovarianceMatrix(1,1)=0.6776;  // ADD
      break;
      
    case 5: case 8: // Osc only and Eq deficit fits
      Theo_CovarianceMatrix.ResizeTo(4,4);
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
      break;
      
    default: // In case of linear fit
      break;
  }
  Theo_CovarianceMatrix.Print();
  // Do not invert if there is only for element in the matrix corresponding to 241 uncertainity
  if(fFitType!=4 && fFitType!=11) {
    if(Theo_CovarianceMatrix.Invert()==0 || !(Theo_CovarianceMatrix.IsValid())) exit(1);
  }
  Theo_CovarianceMatrix.Print();
}

/// Stores the information in systematic covariancs elemenst and stores it in Syst_CovarianceMatrix
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
  
  for (int i=0; i<Stat_CovarianceMatrix.GetNrows(); i++) {
    g_IBD_Exp->SetPointError(i,0,TMath::Sqrt(Stat_CovarianceMatrix(i,i)));
  }
  
  std::cout << "Statistical cov matrix" <<std::endl;
  Stat_CovarianceMatrix.Print();
}

/*
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
}*/

double GlobalAnalyzer::EstimateAntiNuSpectrum(const double *xx,double energy) const{
  double spectrum=f235Yield->Eval(energy)*xx[0];
  spectrum+=f238Yield->Eval(energy)*xx[1];
  spectrum+=f239Yield->Eval(energy)*xx[2];
  spectrum+=f240Yield->Eval(energy)*xx[3];//PTS:Need to add this when using 240
  spectrum+=f241Yield->Eval(energy)*xx[4];
  return spectrum;
}

double GlobalAnalyzer::EstimateAntiNuFlux(const double *xx,double baseline) const{
  double totalFlux=0.0;
  double oscillatedFlux=0.0;
  for (int j=1;j<=82; j++) {
    double flux=0;
    double energy=1.8+j*0.1;
    double spectrum=EstimateAntiNuSpectrum(xx,energy);
    double xSec=fIBDxSec->Eval(energy);
    flux=spectrum*xSec;
    totalFlux+=flux;
    oscillatedFlux+=flux*TMath::Power(TMath::Sin(1.27*xx[6]*baseline/energy),2); // MOD (xx[5]->xx[6])
  }
  oscillatedFlux=1-xx[5]*oscillatedFlux/totalFlux;
  return oscillatedFlux;
}

/// Calculates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, Pu239, Pu241, sin22theta and dm2 respectively and returns
/// a vector of the theoretical IBD yield.
void GlobalAnalyzer::CalculateTheoreticalIBDYield(TVectorD& yTheo,const double *xx) const{
  yTheo.ResizeTo(numberofExp);
  TVectorD yTemp(numberofExp);
  
  if(fFitType<=4)
  {
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241; //PTS:Need to add + xx[4]*v_FF_240 if you are also using 240
  }
  else if(fFitType>4 && fFitType<8)
  {
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241;//PTS:Need to add + xx[4]*v_FF_240 if you are also using 240
    // Apply oscillations
    for (int i=0;i<yTemp.GetNoElements(); i++) {
      yTemp[i]=EstimateAntiNuFlux(xx,v_Baseline[i]);
    }
    yTheo=ElementMult(yTheo,yTemp);
  }
  else if(fFitType>=8 && fFitType<=10){
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241;//PTS:Need to add + xx[4]*v_FF_240 if you are also using 240

    yTheo*=xx[5];
  }
  else{
    for (int i=0;i<yTemp.GetNoElements(); i++) {
      yTheo[i]=xx[0] + xx[1]*(v_FF_239[i]-ff_239) ;
    }
  }
}

/*
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
void GlobalAnalyzer::CalculateTheoreticalIBDYield(double F239F241Ratio,TVectorD &yTheo,const double &y_U235,const double &y_U238,const double &y_P239241 ) const{
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
    case 1:case 6:case 9:// U235 only, U235+Osc and U235+Eq fits
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0]; // P241 - P241
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240  <--- ADJUST THEO COV
      rValues[2]=xx[1]-xSectionSH[1]; // U238 - U238
      rValues[3]=xx[2]-xSectionSH[2]; // P239 - P239
      break;
      
    case 2:case 7:case 10: // U239 only, U239+Osc and U239+Eq fits
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0]; // P241 - P241
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240  <--- ADJUST THEO COV
      rValues[2]=xx[1]-xSectionSH[1]; // U238 - U238
      rValues[3]=xx[0]-xSectionSH[3]; // U235 - U235
      break;
      
    case 3: // U235+U239 only fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0];
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240  <--- ADJUST THEO COV
      rValues[2]=xx[1]-xSectionSH[1];
      break;
      
    case 4:// U235+U239+U238 only fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0];
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240  <--- ADJUST THEO COV
      break;
      
    case 5:case 8:  // Osc only and Eq fit
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0];
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240  <--- ADJUST THEO COV
      rValues[2]=xx[1]-xSectionSH[1];
      rValues[3]=xx[2]-xSectionSH[2];
      rValues[4]=xx[0]-xSectionSH[3];
      break;
      
    case 11:
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
  if((fFitType!=4)&&(fFitType!=11))rValuesTemp*=Theo_CovarianceMatrix;
  
  TVectorD yTheo(numberofExp);
  TMatrixD CovarianceMatrix(numberofExp,numberofExp);
  CovarianceMatrix.Zero();
  CalculateTheoreticalIBDYield(yTheo,xx);
  CalculateCovarianceMatrix(yTheo,CovarianceMatrix);
  if(CovarianceMatrix.Invert()==0 || !(CovarianceMatrix.IsValid())) exit(1);
  TVectorD vTemp = yTheo;
  vTemp -= v_IBD_Exp_temp;
  double Chi2Value=Mult(vTemp,CovarianceMatrix,vTemp);
  if(fFitType==4)Chi2Value+=TMath::Power(rValuesTemp[0],2)/Theo_CovarianceMatrix[0][0];
  else if(fFitType==11) return Chi2Value;
  else Chi2Value+=rValuesTemp*rValues;
  
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

/// Deprecated, after 
/// returns the r or IBD of the one sigma point
// double GlobalAnalyzer::OneSigmaCorrValue(TH1D * hist, const double &minIBD, const double &maxIBD, const int &nBins){
  
//   /// First non zero bin
//   int FirstBin =hist->FindFirstBinAbove(0);
  
//   /// Minimum of the histogram
//   double MinChi2 =hist->GetMinimum(0);
  
//   /// This will store the value in the bins as the for loop below
//   /// goes through the histogram
//   double BinValue;
  
//   double OneSigmaValue = MinChi2 + 1;
//   double Chi2Diff = 10E8;
  
//   /// Stores the bin where one sigmavalue is found
//   int OneSigmaBin;
  
//   for(int i = FirstBin; i < (FirstBin + nBins); i++){
//     BinValue= hist->GetBinContent(i);
//     if(abs(BinValue - OneSigmaValue) < Chi2Diff){
//       Chi2Diff = abs(BinValue - OneSigmaValue);
//       OneSigmaBin = i;
//     }
//   }
  
//   double OneSigma = (OneSigmaBin - FirstBin)*((maxIBD - minIBD) /nBins) + minIBD;
//   return OneSigma;
  
}

void GlobalAnalyzer::DrawDataPoints(TFile &outFile){
  outFile.cd();
  g_IBD_Exp->Write();
}

void GlobalAnalyzer::DrawFitPoints(TFile &outFile,double a, double b){
  outFile.cd();
  for(int i=0;i<8;i++){
    g_IBD_Fit->SetPoint(i,v_FF_239[i],a+b*(v_FF_239[i]-ff_239));
  }
  g_IBD_Fit->Write();
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
