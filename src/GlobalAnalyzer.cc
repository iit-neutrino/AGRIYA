#include "GlobalAnalyzer.hh"
using namespace std;

/// Reads the data file and stores it in the corresponding vectors
/// The number of experiments is also determined by
/// reading the textfile and the number of counted rows 
/// will be stored in the #fNumberofExp
/// PTS:: Consider using ReadMatrix function instead
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
      v_FF_235.ResizeTo(fNumberofExp+1);
      v_FF_238.ResizeTo(fNumberofExp+1);
      v_FF_239.ResizeTo(fNumberofExp+1);
      v_FF_240.ResizeTo(fNumberofExp+1);
      v_FF_241.ResizeTo(fNumberofExp+1);
      v_IBD_Exp.ResizeTo(fNumberofExp+1);
      v_Baseline.ResizeTo(fNumberofExp+1);
      while(streamA >>numberRead){
        if(columnsA == 0) v_FF_235[fNumberofExp]=numberRead;
        else if(columnsA == 1) v_FF_238[fNumberofExp]=numberRead;
        else if(columnsA == 2) v_FF_239[fNumberofExp]=numberRead;
        else if(columnsA == 3) v_FF_240[fNumberofExp]=numberRead;
        else if(columnsA == 4) v_FF_241[fNumberofExp]=numberRead;
        else if(columnsA == 5) v_IBD_Exp[fNumberofExp]=numberRead;
        else if(columnsA == 6) v_Baseline[fNumberofExp]=numberRead;
        columnsA++;
      }
      if(columnsA < 7) 
      {
        printf("Fewer columns than exoected exist; check to see if 240 is added\n");
        return false;
      }
      fNumberofExp++;
    }
  }
    
  std::cout << "Number of experiments = " <<fNumberofExp <<std::endl;
  return true;
}

bool GlobalAnalyzer:: LoadFissionFractionMap()
{
  // // Theoretical IBD yields from (TODO: add paper here)
  // kSigma241=kSigma241;
  // kSigma238=kSigma238;
  // kSigma239=kSigma239;
  // kSigma235=kSigma235;
  // kSigma240=kSigma240;
  
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

bool GlobalAnalyzer::ReadMatrix(TString fileName, TMatrixD &matrix)
{  
  int rowCounter = 0;
  int columnCounter = 0;
  double numberRead;   ///The number read out of the file
  string lineRead;    ///The line of text file currently read

  
  if(fileName.Contains("stat", TString::kIgnoreCase))
  {
    matrix.ResizeTo(fNumberofExp, fNumberofExp);
  }
  else if(fileName.Contains("syst", TString::kIgnoreCase))
  {
    matrix.ResizeTo(fNumberofExp, fNumberofExp);
  }
  else if(fileName.Contains("theo", TString::kIgnoreCase))
  {
    matrix.ResizeTo(fNumberofIso,fNumberofIso);
  }
  else
  {
    printf("Error in %s covariance matrix file name",fileName.Data());
    printf("Please check the correct file is input, exiting...");
    return false;
  }
  if(!CheckFileExists(fileName)) return false;
  
  ///Loading StatCov.txt into fStatCovarianceMatrix
  ifstream fileIn;
  fileIn.open(fileName.Data());
  while(fileIn.good()){
    while(getline(fileIn, lineRead)){
      istringstream streamA(lineRead);
      columnCounter = 0;
      while(streamA >>numberRead){
        matrix(rowCounter,columnCounter) = numberRead;
        columnCounter++;
      }
      rowCounter++;
    }
  }
  fileIn.close();
  printf("The %s covariance matrix is:",fileName.Data());
  matrix.Print();
  return true;
}

bool GlobalAnalyzer::LoadCovarianceMatrices()
{
  if(!ReadMatrix(fCovStatFileName,fStatCovarianceMatrix)) return false;
  if(!ReadMatrix(fCovSystFileName,fRedSystCovarianceMatrix)) return false;
  if(!ReadMatrix(fTheoUncFileName,fUncertainityMatrix)) return false;
  for (int i=0; i<fStatCovarianceMatrix.GetNrows(); i++)  
  {
    g_IBD_Exp.SetPointError(i,0,TMath::Sqrt(fStatCovarianceMatrix(i,i)));
  }
  return true;
}

bool GlobalAnalyzer::LoadTheoCovMat()
{
  //TODO: Read theoretical covariance matrix from file here. The file contains a 5x5 matrix of uncertainties in %
  //TODO: Use the covariance read above and the IBD yields to calculate the covariance matrix that goes into fitting
  switch (fFitType) {
    // In the case of fits involving 235 only as a free fit parameter, we need to include uncertainties associated with 238, 239, 240, 241
    // So the matrix is a 3*3 matrix
    case 1: case 6:case 9: // U235 only,U235+Osc and U235+Eq fits
      fTheoCovarianceMatrix.ResizeTo(4,4);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * kSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * kSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * kSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(2,2) * kSigma239, 2); // 239

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * kSigma241 * kSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * kSigma240 * kSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * kSigma241 * kSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * kSigma238 * kSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,2) , 2) * kSigma241 * kSigma239; //241-239
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(2,4) , 2) * kSigma239 * kSigma241; //239-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * kSigma240 * kSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * kSigma238 * kSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,2) , 2) * kSigma240 * kSigma239; //240-239
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(2,3) , 2) * kSigma239 * kSigma240; //239-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,2) , 2) * kSigma238 * kSigma239; //238-239
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(2,1) , 2) * kSigma239 * kSigma238; //239-238
      break;

    case 2: case 7:case 10: // U239 only,U239+Osc and U239+Eq fit
      fTheoCovarianceMatrix.ResizeTo(4,4);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * kSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * kSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * kSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(0,0) * kSigma235, 2); // 235

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * kSigma241 * kSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * kSigma240 * kSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * kSigma241 * kSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * kSigma238 * kSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,0) , 2) * kSigma241 * kSigma235; //241-235
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(0,4) , 2) * kSigma235 * kSigma241; //235-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * kSigma240 * kSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * kSigma238 * kSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,0) , 2) * kSigma240 * kSigma235; //240-235
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(0,3) , 2) * kSigma235 * kSigma240; //235-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,0) , 2) * kSigma238 * kSigma235; //238-235
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(0,1) , 2) * kSigma235 * kSigma238; //235-238
      
      break;
      
    case 3: // U235+U239 only fit
      fTheoCovarianceMatrix.ResizeTo(3,3);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * kSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * kSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * kSigma238 , 2); // 238

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * kSigma241 * kSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * kSigma240 * kSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * kSigma241 * kSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * kSigma238 * kSigma241; //238-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * kSigma240 * kSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * kSigma238 * kSigma240; //238-240
      break;
      
    case 4: // U235+U239+U238 only fit
      fTheoCovarianceMatrix.ResizeTo(2,2);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * kSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * kSigma240, 2) ; //240

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * kSigma241 * kSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * kSigma240 * kSigma241; //240-241
      break;
      
    case 5: case 8: // Osc only and Eq deficit fits
      fTheoCovarianceMatrix.ResizeTo(5,5);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * kSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * kSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * kSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(2,2) * kSigma239, 2); // 239
      fTheoCovarianceMatrix(4,4)= TMath::Power(fUncertainityMatrix(0,0) * kSigma235, 2); // 235

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * kSigma241 * kSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * kSigma240 * kSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * kSigma241 * kSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * kSigma238 * kSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,2) , 2) * kSigma241 * kSigma239; //241-239
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(2,4) , 2) * kSigma239 * kSigma241; //239-241

      fTheoCovarianceMatrix(0,4)= TMath::Power(fUncertainityMatrix(4,0) , 2) * kSigma241 * kSigma235; //241-235
      fTheoCovarianceMatrix(4,0)= TMath::Power(fUncertainityMatrix(0,4) , 2) * kSigma235 * kSigma241; //235-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * kSigma240 * kSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * kSigma238 * kSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,2) , 2) * kSigma240 * kSigma239; //240-239
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(2,3) , 2) * kSigma239 * kSigma240; //239-240

      fTheoCovarianceMatrix(1,4)= TMath::Power(fUncertainityMatrix(3,0) , 2) * kSigma240 * kSigma235; //240-235
      fTheoCovarianceMatrix(4,1)= TMath::Power(fUncertainityMatrix(0,3) , 2) * kSigma235 * kSigma240; //235-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,2) , 2) * kSigma238 * kSigma239; //238-239
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(2,1) , 2) * kSigma239 * kSigma238; //239-238

      fTheoCovarianceMatrix(2,4)= TMath::Power(fUncertainityMatrix(1,0) , 2) * kSigma238 * kSigma235; //238-235
      fTheoCovarianceMatrix(4,2)= TMath::Power(fUncertainityMatrix(0,1) , 2) * kSigma235 * kSigma238; //235-238

      fTheoCovarianceMatrix(3,4)= TMath::Power(fUncertainityMatrix(2,0) , 2) * kSigma239 * kSigma235; //239-235
      fTheoCovarianceMatrix(4,3)= TMath::Power(fUncertainityMatrix(0,2) , 2) * kSigma235 * kSigma239; //235-239
      break;
      
    default: // In case of linear fit
      break;
  }
  cout<<"Theoretical covariance matrix is:"<<endl;
  fTheoCovarianceMatrix.Print();
  if(fTheoCovarianceMatrix.Invert()==0 || !(fTheoCovarianceMatrix.IsValid()))
  {
    printf("Theoretical Covariance matrix is either non-invertible or not valid\n");
    return false;
  }
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
  yTheo.ResizeTo(fNumberofExp);
  TVectorD yTemp(fNumberofExp);
  
  if(fFitType<=4)
  {
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241; 
  }
  else if(fFitType>4 && fFitType<8)
  {
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241;
    // Apply oscillations
    for (int i=0;i<yTemp.GetNoElements(); i++) {
      yTemp[i]=EstimateAntiNuFlux(xx,v_Baseline[i]);
    }
    yTheo=ElementMult(yTheo,yTemp);
  }
  else if(fFitType>=8 && fFitType<=10){
    yTheo=xx[0]*v_FF_235 + xx[1]*v_FF_238 + xx[2]*v_FF_239 + xx[3]*v_FF_240 + xx[4]*v_FF_241;

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
/// The theoretical IBD yield vector and the IBD yields
bool GlobalAnalyzer::EvaluateCovarianceMatrix(const TVectorD &yTheo, TMatrixD &CovarianceMatrix) const{
  CovarianceMatrix.Zero();
  TMatrixD Tot_CovarianceMatrix=fRedSystCovarianceMatrix;
  TMatrixD theoIBDYieldProductMatrix(CovarianceMatrix);
  theoIBDYieldProductMatrix.Zero();
  theoIBDYieldProductMatrix=OuterProduct(yTheo,yTheo);
  CovarianceMatrix=ElementMult(Tot_CovarianceMatrix,theoIBDYieldProductMatrix);
  CovarianceMatrix+= fStatCovarianceMatrix;
  return true;
}

bool GlobalAnalyzer::EvaluateTheoDeltaVector(const double* xx, TVectorD &rValues) const{
  
  switch (fFitType) {
    case 1:case 6:case 9:// U235 only, U235+Osc and U235+Eq fits
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-kSigma241;
      rValues[1]=xx[3]-kSigma240;
      rValues[2]=xx[1]-kSigma238;
      rValues[3]=xx[2]-kSigma239;
      break;
      
    case 2:case 7:case 10: // U239 only, U239+Osc and U239+Eq fits
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-kSigma241;
      rValues[1]=xx[3]-kSigma240;
      rValues[2]=xx[1]-kSigma238;
      rValues[3]=xx[0]-kSigma235;
      break;
      
    case 3: // U235+U239 only fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-kSigma241;
      rValues[1]=xx[3]-kSigma240;
      rValues[2]=xx[1]-kSigma238;
      break;
      
    case 4:// U235+U239+U238 only fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-kSigma241;
      rValues[1]=xx[3]-kSigma240;
      break;
      
    case 5:case 8:  // Osc only and Eq fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-kSigma241;
      rValues[1]=xx[3]-kSigma240;
      rValues[2]=xx[1]-kSigma238;
      rValues[3]=xx[2]-kSigma239;
      rValues[4]=xx[0]-kSigma235;
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
  rValuesTemp*=fTheoCovarianceMatrix;
  
  TVectorD yTheo(fNumberofExp);
  TMatrixD CovarianceMatrix(fNumberofExp,fNumberofExp);
  CovarianceMatrix.Zero();
  EvaluateTheoreticalIBDYield(xx, yTheo);
  EvaluateCovarianceMatrix(yTheo,CovarianceMatrix);
  if(CovarianceMatrix.Invert()==0 || !(CovarianceMatrix.IsValid())) exit(1);
  TVectorD vTemp = yTheo;
  vTemp -= v_IBD_Exp;
  double Chi2Value=Mult(vTemp,CovarianceMatrix,vTemp);
  Chi2Value+=rValuesTemp*rValues;
  
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
bool GlobalAnalyzer::InitializeAnalyzer(TString dataInput, TString covStatFileName, TString CovSystFileName, TString redCovTheoFileName){
  if(!CheckFileExtension(dataInput,".txt")) return false;
  if(!CheckFileExtension(covStatFileName,".txt")) return false;
  if(!CheckFileExtension(CovSystFileName,".txt")) return false;
  if(!CheckFileExtension(redCovTheoFileName,".txt")) return false;
  fDataInput=dataInput;
  fCovStatFileName=covStatFileName;
  fCovSystFileName=CovSystFileName;
  fTheoUncFileName=redCovTheoFileName;
  std::cout << "Using " << dataInput.Data() <<  " data file, " << fCovStatFileName.Data() <<  " stat ," << fCovSystFileName.Data() <<  " syst, and "<< fCovSystFileName.Data() <<"theoretical files."<<std::endl;
  ///The information from Data text file is read when the object is initialized
  if(!ReadDataFromFile()) return false;

  v_Diff.ResizeTo(fNumberofExp);
  g_IBD_Exp.SetName("g_IBD_Exp");
  g_IBD_Fit.SetName("g_IBD_Fit");
  
  for(int i=0; i<fNumberofExp; i++){
    g_IBD_Exp.SetPoint(i,v_FF_239[i],v_IBD_Exp[i]);
    ff_239+=v_FF_239[i];
  }
  ff_239=ff_239/fNumberofExp;

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
  // if(!LoadFissionFractionMap()) return false;
  if(!LoadCovarianceMatrices()) return false;
  if(!LoadTheoCovMat()) return false;
  return true;
}
