#include "GlobalAnalyzer.hh"
using namespace std;

/// Reads the data file and stores it in the corresponding vectors
/// The number of experiments is also determined by
/// reading the textfile and the number of counted rows 
/// will be stored in the #f_NumberofExp
bool GlobalAnalyzer::ReadDataFromFile(){
  double numberRead;
  string lineA;
  ifstream fileIn;
  if(!CheckFileExists(fDataInput)) return false;

  fileIn.open(fDataInput.Data());
  
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;
      v_FF_235.ResizeTo(f_NumberofExp+1);
      v_FF_238.ResizeTo(f_NumberofExp+1);
      v_FF_239.ResizeTo(f_NumberofExp+1);
      v_FF_240.ResizeTo(f_NumberofExp+1);
      v_FF_241.ResizeTo(f_NumberofExp+1);
      v_IBD_Exp.ResizeTo(f_NumberofExp+1);
      v_Baseline.ResizeTo(f_NumberofExp+1);
      while(streamA >>numberRead){
        if(columnsA == 0) v_FF_235[f_NumberofExp]=numberRead;
        else if(columnsA == 1) v_FF_238[f_NumberofExp]=numberRead;
        else if(columnsA == 2) v_FF_239[f_NumberofExp]=numberRead;
        else if(columnsA == 3) v_FF_240[f_NumberofExp]=numberRead;
        else if(columnsA == 4) v_FF_241[f_NumberofExp]=numberRead;
        else if(columnsA == 5) v_IBD_Exp[f_NumberofExp]=numberRead;
        else if(columnsA == 6) v_Baseline[f_NumberofExp]=numberRead;
        columnsA++;
      }
      if(columnsA < 7) 
      {
        printf("Fewer columns than exoected exist; check to see if 240 is added\n");
        return false;
      }
      f_NumberofExp++;
    }
  }
    
  std::cout << "Number of experiments = " <<f_NumberofExp <<std::endl;
  return true;
}

bool GlobalAnalyzer:: LoadFissionFractionMap()
{
  // Theoretical IBD yields from (TODO: add paper here)
  xSectionSH[0]=sigma241;
  xSectionSH[1]=sigma238;
  xSectionSH[2]=sigma239;
  xSectionSH[3]=sigma235;
  xSectionSH[4]=sigma240;
  
  //PTS: This is needed if you are doing any fits that include oscillations
  //TODO:Remove this from the version that goes out in public
  f235Flux=new TF1("235Flux","TMath::Exp(0.87-0.160*x-0.091*TMath::Power(x,2))",1.8,10);
  f238Flux=new TF1("238Flux","TMath::Exp(0.976-0.162*x-0.0790*TMath::Power(x,2))",1.8,10);
  f239Flux=new TF1("239Flux","TMath::Exp(0.896-0.239*x-0.0981*TMath::Power(x,2))",1.8,10);
  f240Flux=new TF1("240Flux","TMath::Exp(1.044-0.232*x-0.0982*TMath::Power(x,2))",1.8,10); // TODO: Assumed same as for 241, get the correct value
  f241Flux=new TF1("241Flux","TMath::Exp(1.044-0.232*x-0.0982*TMath::Power(x,2))",1.8,10);
  fIBDxSec=new TF1("IBDxSec","9.52*(x-1.293)*TMath::Sqrt(TMath::Power(x-1.293,2)-TMath::Power(0.511,2))",1.8,10);
  return true;
}

//PTS: Need to include theo covariances when using 240, but you already know it. 
bool GlobalAnalyzer::LoadTheoCovMat(){
  //TODO: Read theoretical covariance matrix from file here. The file contains a 5x5 matrix of uncertainties in %
  //TODO: Use the covariance read above and the IBD yields to calculate the covariance matrix that goes into fitting
  switch (fFitType) {
    // In the case of fits involving 235 only as a free fit parameter, we need to include uncertainties associated with 238, 239, 240, 241
    // So the matrix is a 3*3 matrix
    case 1: case 6:case 9: // U235 only,U235+Osc and U235+Eq fits
      Theo_CovarianceMatrix.ResizeTo(4,4);
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
  // Do not invert if there is only for element in the matrix corresponding to 241 uncertainity
  if(Theo_CovarianceMatrix.Invert()==0 || !(Theo_CovarianceMatrix.IsValid()))
  {
    printf("Theoretical Covariance matrix is either non-invertible or not valid\n");
    return false;
  }
  return true;
}

bool GlobalAnalyzer::LoadCovarianceMatrix(){
  Stat_CovarianceMatrix.ResizeTo(f_NumberofExp, f_NumberofExp);
  Syst_CovarianceMatrix.ResizeTo(f_NumberofExp, f_NumberofExp);
  
  int rowCounter = 0;
  int columnCounter = 0;
  
  double numberRead;   ///The number read out of the file
  string lineRead;    ///The line of text file currently read

  if(!CheckFileExists(fCovSyst)) return false;

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

  for (int i=0; i<Stat_CovarianceMatrix.GetNrows(); i++) {
    g_IBD_Exp.SetPointError(i,0,TMath::Sqrt(Stat_CovarianceMatrix(i,i)));
  }
  

  rowCounter=0;
  columnCounter=0;
  
  if(!CheckFileExists(fCovSyst)) return false;

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
  
  return true;
}

double GlobalAnalyzer::EstimateAntiNuSpectrum(const double *xx,double energy) const{
  double spectrum=f235Flux->Eval(energy)*xx[0];
  spectrum+=f238Flux->Eval(energy)*xx[1];
  spectrum+=f239Flux->Eval(energy)*xx[2];
  spectrum+=f240Flux->Eval(energy)*xx[3];
  spectrum+=f241Flux->Eval(energy)*xx[4];
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
    oscillatedFlux+=flux*TMath::Power(TMath::Sin(1.27*xx[6]*baseline/energy),2); 
  }
  oscillatedFlux=1-xx[5]*oscillatedFlux/totalFlux;
  return oscillatedFlux;
}

/// Evaluates the theoretical IBD yield for all the experiments for
/// a given IBD yield of U235, U238, Pu239, Pu241, sin22theta and dm2 respectively and returns
/// a vector of the theoretical IBD yield.
// This is the function where the IBD yields are evaluated for a given fit type
bool GlobalAnalyzer::EvaluateTheoreticalIBDYield(const double *xx, TVectorD& yTheo) const{
  yTheo.ResizeTo(f_NumberofExp);
  TVectorD yTemp(f_NumberofExp);
  
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
  return true;
}

//matrix inputs are:
/// The theoretical IBD yield vector and the IBD yield of U238 and Pu241 for Daya Bay
/// The additional Stat_CovarianceMatrix[i][j] term is zero for non DayaBay experiments
/// The if statment included is for Daya Bay experiments that have the U238 and Pu241
/// uncertantiy term in their covariance matrix.
bool GlobalAnalyzer::EvaluateCovarianceMatrix(const TVectorD &yTheo, TMatrixD &CovarianceMatrix) const{
  CovarianceMatrix.Zero();
  TMatrixD Tot_CovarianceMatrix;
  Tot_CovarianceMatrix.Zero();
  Tot_CovarianceMatrix=Syst_CovarianceMatrix;
  TMatrixD theoIBDYieldProductMatrix(CovarianceMatrix);
  theoIBDYieldProductMatrix.Zero();
  theoIBDYieldProductMatrix=OuterProduct(yTheo,yTheo);
  CovarianceMatrix=ElementMult(Tot_CovarianceMatrix,theoIBDYieldProductMatrix);
  CovarianceMatrix+= Stat_CovarianceMatrix;
  return true;
}

bool GlobalAnalyzer::EvaluateTheoDeltaVector(const double* xx, TVectorD &rValues) const{
  
  switch (fFitType) {
    case 1:case 6:case 9:// U235 only, U235+Osc and U235+Eq fits
      rValues.ResizeTo(Theo_CovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-xSectionSH[0]; // P241 - P241
      rValues[1]=xx[3]-xSectionSH[4]; // P240 - P240 <--- ADJUST THEO COV
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
      return false;
      break;
  }
  return true;
}

double GlobalAnalyzer::DoEval(const double* xx)const{
  
  TVectorD rValues;
  if(!EvaluateTheoDeltaVector(xx,rValues)) return -1; // Need to handle error better
  TVectorD rValuesTemp=rValues;
  if((fFitType!=4)&&(fFitType!=11))rValuesTemp*=Theo_CovarianceMatrix;
  
  TVectorD yTheo(f_NumberofExp);
  TMatrixD CovarianceMatrix(f_NumberofExp,f_NumberofExp);
  CovarianceMatrix.Zero();
  EvaluateTheoreticalIBDYield(xx, yTheo);
  EvaluateCovarianceMatrix(yTheo,CovarianceMatrix);
  if(CovarianceMatrix.Invert()==0 || !(CovarianceMatrix.IsValid())) exit(1);
  TVectorD vTemp = yTheo;
  vTemp -= v_IBD_Exp;
  double Chi2Value=Mult(vTemp,CovarianceMatrix,vTemp);
  if(fFitType==4)Chi2Value+=TMath::Power(rValuesTemp[0],2)/Theo_CovarianceMatrix[0][0];
  else if(fFitType==11) return Chi2Value;
  else Chi2Value+=rValuesTemp*rValues;
  
  return Chi2Value;
}

bool GlobalAnalyzer::DrawDataPoints(TFile &outFile){
  if(!outFile.IsOpen() || outFile.IsZombie()) return false;
  outFile.cd();
  g_IBD_Exp.Write();
  return true;
}

bool GlobalAnalyzer::DrawFitPoints(TFile &outFile, double intercept, double slope){
  if(!outFile.IsOpen() || outFile.IsZombie()) return false;
  outFile.cd();
  for(int i=0;i<8;i++){
    g_IBD_Fit.SetPoint(i,v_FF_239[i],intercept+slope*(v_FF_239[i]-ff_239));
  }
  g_IBD_Fit.Write();
  return true;
}

// Initialize the global analyzer
bool GlobalAnalyzer::InitializeAnalyzer(TString dataInput, TString covStat, TString covSyst){
  if(!CheckFileExtension(dataInput,".txt")) return false;
  if(!CheckFileExtension(covStat,".txt")) return false;
  if(!CheckFileExtension(covSyst,".txt")) return false;
  fDataInput=dataInput;
  fCovStat=covStat;
  fCovSyst=covSyst;
  std::cout << "Using " << dataInput.Data() <<  " data file, " << fCovStat.Data() <<  " stat file, and " << fCovSyst.Data() <<  " syst file" <<std::endl;
  ///The information from Data text file is read when the object is initialized
  if(!ReadDataFromFile()) return false;

  v_Diff.ResizeTo(f_NumberofExp);
  g_IBD_Exp.SetName("g_IBD_Exp");
  g_IBD_Fit.SetName("g_IBD_Fit");
  
  for(int i=0; i<f_NumberofExp; i++){
    g_IBD_Exp.SetPoint(i,v_FF_239[i],v_IBD_Exp[i]);
    ff_239+=v_FF_239[i];
  }
  ff_239=ff_239/f_NumberofExp;

  return true;
}

/// SetupExperiments will be called during runtime so that
/// the functions inside it can all be called at run time.
bool GlobalAnalyzer::SetupExperiments(int fitType){
  fFitType=fitType;
  // The fitter acts crazy when trying to fit to sin22theta and dm2 when they are not really needed
  // so those are fit parameters only for fits including oscillations
  if(fitType>4 && fitType<8) numberofFitPars = 7;
  else numberofFitPars = 7;
  if(!LoadFissionFractionMap()) return false;
  if(!LoadCovarianceMatrix()) return false;
  if(!LoadTheoCovMat()) return false;
  return true;
}
