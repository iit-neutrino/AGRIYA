#include "GlobalAnalyzer.hh"
using namespace std;

/// Reads the data file and stores it in the corresponding vectors
/// The number of experiments is also determined by
/// reading the textfile and the number of counted rows 
/// will be stored in the #fNumberofExp
bool GlobalAnalyzer::ReadDataFromFile(){
  double numberRead;
  string lineA;
  ifstream fileIn;
  if(!CheckFileExists(fDataInput))
  {
    printf("Couldn;'t read data from file \n");
    return false;
  }

  fileIn.open(fDataInput.Data());
  
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;
      fVFF235.ResizeTo(fNumberofExp+1);
      fVFF238.ResizeTo(fNumberofExp+1);
      fVFF239.ResizeTo(fNumberofExp+1);
      fVFF240.ResizeTo(fNumberofExp+1);
      fVFF241.ResizeTo(fNumberofExp+1);
      fVIBDExp.ResizeTo(fNumberofExp+1);
      fVBaseline.ResizeTo(fNumberofExp+1);
      while(streamA >>numberRead){
        if(columnsA == 0) fVFF235[fNumberofExp]=numberRead;
        else if(columnsA == 1) fVFF238[fNumberofExp]=numberRead;
        else if(columnsA == 2) fVFF239[fNumberofExp]=numberRead;
        else if(columnsA == 3) fVFF240[fNumberofExp]=numberRead;
        else if(columnsA == 4) fVFF241[fNumberofExp]=numberRead;
        else if(columnsA == 5) fVIBDExp[fNumberofExp]=numberRead;
        else if(columnsA == 6) fVBaseline[fNumberofExp]=numberRead;
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
  printf("Number of experiments = %i\n",fNumberofExp); 
  printf("-----------------------------------------------------------------------\n"); 
  printf("U 235 fission fractions are \n");
  fVFF235.Print();
  printf("U 238 fission fractions are \n");
  fVFF238.Print();
  printf("Pu 239 fission fractions are \n");
  fVFF239.Print();
  printf("Pu 240 fission fractions are \n");
  fVFF240.Print();
  printf("Pu 241 fission fractions are \n");
  fVFF241.Print();
  printf("Experimental IBD yields are \n");
  fVIBDExp.Print();
  if(!EvaluateTotalTheoIBDYield()) return false;
  return true;
}

bool GlobalAnalyzer::ReadTheoreticalIBDYields(TString fileName)
{
  if(!CheckFileExists(fileName))
  {
    printf("File %s used for theoretical IBD yields does not exist\n", fileName.Data());
    return false;
  }
  double numberRead;
  int lineNo=0;
  string lineA;
  ifstream fileIn;

  fileIn.open(fileName.Data());
  
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;
      while(streamA >>numberRead){
        if(columnsA == 0) 
        {
          if(lineNo==0) fSigma235 = numberRead;
          else if(lineNo==1) fSigma238 = numberRead;
          else if(lineNo==2) fSigma239 = numberRead;
          else if(lineNo==3) fSigma240 = numberRead;
          else if(lineNo==4) fSigma241 = numberRead;
          else
          {
            printf("Should only contain five rows and should be ordered as U235, U238, Pu239, Pu240, Pu241\n");
            return false;
          }
        }
        columnsA++;
      }
      if(columnsA > 1) 
      {
        printf("Should only contain one column and should be ordered as U235, U238, Pu239, Pu240, Pu241\n");
        return false;
      }
    lineNo++;
    }
  }
  printf("Using IBD Yields from the text file %s\n", fileName.Data());
  return true;
}

bool GlobalAnalyzer::LoadFluxes()
{ 
  /// This is needed if you are doing any fits that include oscillations
  /// xxxxxxxxxxxxxxxxxxxxxxxxxxxWARNINGxxxxxxxxxxxxxxxxxxxxxxxxxxx//
  /// xxxxxxxxxxxxxxxxxxxxxTHIS IS NOT FUNCTIONING PROPERLYxxxxxxxxxxxxxxxxxxxx//
  f235Flux=new TF1("235Flux","TMath::Exp(0.87-0.160*x-0.091*TMath::Power(x,2))",1.8,10);
  f238Flux=new TF1("238Flux","TMath::Exp(0.976-0.162*x-0.0790*TMath::Power(x,2))",1.8,10);
  f239Flux=new TF1("239Flux","TMath::Exp(0.896-0.239*x-0.0981*TMath::Power(x,2))",1.8,10);
  f240Flux=new TF1("240Flux","TMath::Exp(1.044-0.232*x-0.0982*TMath::Power(x,2))",1.8,10); // TODO: Assumed same as for 241, get the correct value
  f241Flux=new TF1("241Flux","TMath::Exp(1.044-0.232*x-0.0982*TMath::Power(x,2))",1.8,10);
  fIBDxSec=new TF1("IBDxSec","9.52*(x-1.293)*TMath::Sqrt(TMath::Power(x-1.293,2)-TMath::Power(0.511,2))",1.8,10);
  return true;
  /// xxxxxxxxxxxxxxxxxxxxxxxxxxxWARNINGxxxxxxxxxxxxxxxxxxxxxxxxxxx//
  /// xxxxxxxxxxxxxxxxxxxxxTHIS IS NOT FUNCTIONING PROPERLYxxxxxxxxxxxxxxxxxxxx//
}

bool GlobalAnalyzer::ReadMatrix(TString fileName, TMatrixD &matrix)
{  
  int rowCounter = 0;
  int columnCounter = 0;
  double numberRead;   //The number read out of the file
  string lineRead;    //The line of text file currently read

  
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
    printf("Error in %s covariance matrix file name\n",fileName.Data());
    printf("Please check the correct file is input, exiting...\n");
    return false;
  }
  if(!CheckFileExists(fileName))
  {
    printf("Issue with opening file %s\n",fileName.Data());
    printf("Please check the correct file is input, exiting...\n");
    return false;
  }
  
  //Loading StatCov.txt into fStatCovarianceMatrix
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
  if(fileName.Contains("theo", TString::kIgnoreCase) && rowCounter!=fNumberofIso)
  {
    printf("Error in %s covariance matrix from the file\n",fileName.Data());
    printf("Wrong number of rows than expected...\n");
    return false;
  }
  else if ((fileName.Contains("syst", TString::kIgnoreCase) || fileName.Contains("stat", TString::kIgnoreCase) )  && rowCounter!=fNumberofExp)
  {
    printf("Error in %s covariance matrix from the file\n",fileName.Data());
    printf("Wrong number of rows than expected...\n");
    return false;
  }
  fileIn.close();
  printf("The %s covariance matrix is:",fileName.Data());
  matrix.Print();
  return true;
}

bool GlobalAnalyzer::LoadCovarianceMatrices()
{
  if(!ReadMatrix(fCovStatFileName,fStatCovarianceMatrix)) return false;
  if(fIsStatCovMatrixReduced) printf("Warning: The %s covariance matrix is reduced\n",fCovStatFileName.Data());
  if(!ReadMatrix(fCovSystFileName,fRedSystCovarianceMatrix)) return false;
  fSystCovarianceMatrix.ResizeTo(fNumberofExp, fNumberofExp);
  fTotalCovarianceMatrix.ResizeTo(fNumberofExp, fNumberofExp);
  EvaluateTotalCovarianceMatrix();

  if(!ReadMatrix(fTheoUncFileName,fUncertainityMatrix)) return false;
  for (int i=0; i<fStatCovarianceMatrix.GetNrows(); i++)  
  {
    fGIBDExp.SetPointError(i,0,TMath::Sqrt(fStatCovarianceMatrix(i,i)));
  }
  return true;
}

/// Default values used from Table 3 of arxiv.org/abs/1703.00860.
/// But the latest version uses the uncertainties from COVARIANCEFILETHEO file that is input through macro file.
/// The covariance matrix is calculated by doing #{uncertainity_ij * IBDyields_i * IBDyields_j} where uncertainties
/// are read from COVARIANCEFILETHEO and IBDyields_i are either the default values or are also read from 
/// a file THEORETICALIBDYIELDSFILE through the macro file.
bool GlobalAnalyzer::LoadTheoCovMat()
{
  switch (fFitType) {
    // In the case of fits involving 235 only as a free fit parameter, we need to include uncertainties associated with 238, 239, 240, 241
    // So the matrix is a 4*4 matrix
    case 1: case 6:case 9: // U235 only,U235+Osc and U235+Eq fits
      fTheoCovarianceMatrix.ResizeTo(4,4);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * fSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * fSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * fSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(2,2) * fSigma239, 2); // 239

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * fSigma241 * fSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * fSigma240 * fSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * fSigma241 * fSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * fSigma238 * fSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,2) , 2) * fSigma241 * fSigma239; //241-239
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(2,4) , 2) * fSigma239 * fSigma241; //239-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * fSigma240 * fSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * fSigma238 * fSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,2) , 2) * fSigma240 * fSigma239; //240-239
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(2,3) , 2) * fSigma239 * fSigma240; //239-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,2) , 2) * fSigma238 * fSigma239; //238-239
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(2,1) , 2) * fSigma239 * fSigma238; //239-238
      break;

    case 2: case 7:case 10: // U239 only,U239+Osc and U239+Eq fit
      fTheoCovarianceMatrix.ResizeTo(4,4);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * fSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * fSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * fSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(0,0) * fSigma235, 2); // 235

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * fSigma241 * fSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * fSigma240 * fSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * fSigma241 * fSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * fSigma238 * fSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,0) , 2) * fSigma241 * fSigma235; //241-235
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(0,4) , 2) * fSigma235 * fSigma241; //235-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * fSigma240 * fSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * fSigma238 * fSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,0) , 2) * fSigma240 * fSigma235; //240-235
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(0,3) , 2) * fSigma235 * fSigma240; //235-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,0) , 2) * fSigma238 * fSigma235; //238-235
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(0,1) , 2) * fSigma235 * fSigma238; //235-238
      
      break;
      
    case 3: // U235+U239 only fit
      fTheoCovarianceMatrix.ResizeTo(3,3);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * fSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * fSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * fSigma238 , 2); // 238

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * fSigma241 * fSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * fSigma240 * fSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * fSigma241 * fSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * fSigma238 * fSigma241; //238-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * fSigma240 * fSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * fSigma238 * fSigma240; //238-240
      break;
      
    case 4: // U235+U239+U238 only fit
      fTheoCovarianceMatrix.ResizeTo(2,2);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * fSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * fSigma240, 2) ; //240

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * fSigma241 * fSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * fSigma240 * fSigma241; //240-241
      break;
      
    case 5: case 8: // Osc only and Eq deficit fits
      fTheoCovarianceMatrix.ResizeTo(5,5);
      fTheoCovarianceMatrix(0,0)= TMath::Power(fUncertainityMatrix(4,4) * fSigma241, 2); //241
      fTheoCovarianceMatrix(1,1)= TMath::Power(fUncertainityMatrix(3,3) * fSigma240, 2) ; //240
      fTheoCovarianceMatrix(2,2)= TMath::Power(fUncertainityMatrix(1,1)  * fSigma238 , 2); // 238
      fTheoCovarianceMatrix(3,3)= TMath::Power(fUncertainityMatrix(2,2) * fSigma239, 2); // 239
      fTheoCovarianceMatrix(4,4)= TMath::Power(fUncertainityMatrix(0,0) * fSigma235, 2); // 235

      fTheoCovarianceMatrix(0,1)= TMath::Power(fUncertainityMatrix(4,3) , 2) * fSigma241 * fSigma240; //241-240
      fTheoCovarianceMatrix(1,0)= TMath::Power(fUncertainityMatrix(3,4) , 2) * fSigma240 * fSigma241; //240-241

      fTheoCovarianceMatrix(0,2)= TMath::Power(fUncertainityMatrix(4,1) , 2) * fSigma241 * fSigma238; //241-238
      fTheoCovarianceMatrix(2,0)= TMath::Power(fUncertainityMatrix(1,4) , 2) * fSigma238 * fSigma241; //238-241

      fTheoCovarianceMatrix(0,3)= TMath::Power(fUncertainityMatrix(4,2) , 2) * fSigma241 * fSigma239; //241-239
      fTheoCovarianceMatrix(3,0)= TMath::Power(fUncertainityMatrix(2,4) , 2) * fSigma239 * fSigma241; //239-241

      fTheoCovarianceMatrix(0,4)= TMath::Power(fUncertainityMatrix(4,0) , 2) * fSigma241 * fSigma235; //241-235
      fTheoCovarianceMatrix(4,0)= TMath::Power(fUncertainityMatrix(0,4) , 2) * fSigma235 * fSigma241; //235-241

      fTheoCovarianceMatrix(1,2)= TMath::Power(fUncertainityMatrix(3,1) , 2) * fSigma240 * fSigma238; //240-238
      fTheoCovarianceMatrix(2,1)= TMath::Power(fUncertainityMatrix(1,3) , 2) * fSigma238 * fSigma240; //238-240

      fTheoCovarianceMatrix(1,3)= TMath::Power(fUncertainityMatrix(3,2) , 2) * fSigma240 * fSigma239; //240-239
      fTheoCovarianceMatrix(3,1)= TMath::Power(fUncertainityMatrix(2,3) , 2) * fSigma239 * fSigma240; //239-240

      fTheoCovarianceMatrix(1,4)= TMath::Power(fUncertainityMatrix(3,0) , 2) * fSigma240 * fSigma235; //240-235
      fTheoCovarianceMatrix(4,1)= TMath::Power(fUncertainityMatrix(0,3) , 2) * fSigma235 * fSigma240; //235-240

      fTheoCovarianceMatrix(2,3)= TMath::Power(fUncertainityMatrix(1,2) , 2) * fSigma238 * fSigma239; //238-239
      fTheoCovarianceMatrix(3,2)= TMath::Power(fUncertainityMatrix(2,1) , 2) * fSigma239 * fSigma238; //239-238

      fTheoCovarianceMatrix(2,4)= TMath::Power(fUncertainityMatrix(1,0) , 2) * fSigma238 * fSigma235; //238-235
      fTheoCovarianceMatrix(4,2)= TMath::Power(fUncertainityMatrix(0,1) , 2) * fSigma235 * fSigma238; //235-238

      fTheoCovarianceMatrix(3,4)= TMath::Power(fUncertainityMatrix(2,0) , 2) * fSigma239 * fSigma235; //239-235
      fTheoCovarianceMatrix(4,3)= TMath::Power(fUncertainityMatrix(0,2) , 2) * fSigma235 * fSigma239; //235-239
      break;
      
    default: // In case of linear fit
      break;
  }
  printf("Theoretical covariance matrix is:");
  fTheoCovarianceMatrix.Print();
  if(fTheoCovarianceMatrix.Invert()==0 || !(fTheoCovarianceMatrix.IsValid()))
  {
    printf("Theoretical Covariance matrix is either non-invertible or not valid\n");
    return false;
  }
    fTheoCovarianceMatrix.Print();
  return true;
}

/// This is needed if you are doing any fits that include oscillations
/// xxxxxxxxxxxxxxxxxxxxxxxxxxxWARNINGxxxxxxxxxxxxxxxxxxxxxxxxxxx//
/// xxxxxxxxxxxxxxxxxxxxxTHIS IS NOT FUNCTIONING PROPERLYxxxxxxxxxxxxxxxxxxxx//
double GlobalAnalyzer::EstimateAntiNuSpectrum(const double *xx,double energy) const{
  double spectrum=f235Flux->Eval(energy)*xx[0];
  spectrum+=f238Flux->Eval(energy)*xx[1];
  spectrum+=f239Flux->Eval(energy)*xx[2];
  spectrum+=f240Flux->Eval(energy)*xx[3];
  spectrum+=f241Flux->Eval(energy)*xx[4];
  return spectrum;
}

/// This is needed if you are doing any fits that include oscillations
/// xxxxxxxxxxxxxxxxxxxxxxxxxxxWARNINGxxxxxxxxxxxxxxxxxxxxxxxxxxx//
/// xxxxxxxxxxxxxxxxxxxxxTHIS IS NOT FUNCTIONING PROPERLYxxxxxxxxxxxxxxxxxxxx//
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
/// the theoretical IBD yields of U235, U238, Pu239, Pu241
//// Assumes no contribution from oscillations
bool GlobalAnalyzer::EvaluateTotalTheoIBDYield() {
  fYTheo.ResizeTo(fNumberofExp);
  fYTheo= fSigma235*fVFF235 + fSigma238*fVFF238 + fSigma239*fVFF239 + fSigma240*fVFF240 + fSigma241*fVFF241; 
  return true;
}

/// Evaluates the total IBD yield for all the experiments for
/// a given IBD yield of U235, U238, Pu239, Pu241, sin22theta and dm2 respectively and returns
/// a vector of the theoretical IBD yield.
/// This is the function where the IBD yields are evaluated for a given fit type
bool GlobalAnalyzer::EvaluateTotalIBDYield(const double *xx, TVectorD& yIBD) const{
  yIBD.ResizeTo(fNumberofExp);
  TVectorD yTemp(fNumberofExp);
  
  if(fFitType<=4)
  {
    yIBD=xx[0]*fVFF235 + xx[1]*fVFF238 + xx[2]*fVFF239 + xx[3]*fVFF240 + xx[4]*fVFF241; 
  }
  else if(fFitType>4 && fFitType<8)
  {
    yIBD=xx[0]*fVFF235 + xx[1]*fVFF238 + xx[2]*fVFF239 + xx[3]*fVFF240 + xx[4]*fVFF241;
    // Apply oscillations
    for (int i=0;i<yTemp.GetNoElements(); i++) {
      yTemp[i]=EstimateAntiNuFlux(xx,fVBaseline[i]);
    }
    yIBD=ElementMult(yIBD,yTemp);
  }
  else if(fFitType>=8 && fFitType<=10){
    yIBD=xx[0]*fVFF235 + xx[1]*fVFF238 + xx[2]*fVFF239 + xx[3]*fVFF240 + xx[4]*fVFF241;

    yIBD*=xx[5];
  }
  else{
    for (int i=0;i<yTemp.GetNoElements(); i++) {
      yIBD[i]=xx[0] + xx[1]*(fVFF239[i]-fFF239) ;
    }
  }
  return true;
}

/// Covariance matrix including statistical and systematic correlation is calculated
/// This is only done once and is very useful if the statistical covariance matrix is not reduced 
bool GlobalAnalyzer::EvaluateTotalCovarianceMatrix()
{
  fSystCovarianceMatrix.Zero();
  fTotalCovarianceMatrix.Zero();
  TMatrixD tempRedSystCovarianceMatrix=fRedSystCovarianceMatrix;
  fSystCovarianceMatrix=ElementMult(tempRedSystCovarianceMatrix,OuterProduct(fYTheo,fYTheo));
  fTotalCovarianceMatrix=fSystCovarianceMatrix+fStatCovarianceMatrix;
  fTotalCovarianceMatrix.Print();
  return true;
}

/// Covariance matrix including statistical and systematic correlation is calculated
/// This function is only called if the stat covariance matrix is reduced
bool GlobalAnalyzer::EvaluateCovarianceMatrix(const TVectorD &yIBD, TMatrixD &CovarianceMatrix) const
{
  CovarianceMatrix.Zero();
  CovarianceMatrix = fSystCovarianceMatrix;
  TMatrixD tempRedStatCovarianceMatrix=fStatCovarianceMatrix;
  CovarianceMatrix+= ElementMult(tempRedStatCovarianceMatrix,OuterProduct(yIBD,yIBD));
  return true;
}

bool GlobalAnalyzer::EvaluateTheoDeltaVector(const double* xx, TVectorD &rValues) const
{
  
  switch (fFitType) {
    case 1:case 6:case 9:// U235 only, U235+Osc and U235+Eq fits
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-fSigma241;
      rValues[1]=xx[3]-fSigma240;
      rValues[2]=xx[1]-fSigma238;
      rValues[3]=xx[2]-fSigma239;
      break;
      
    case 2:case 7:case 10: // U239 only, U239+Osc and U239+Eq fits
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-fSigma241;
      rValues[1]=xx[3]-fSigma240;
      rValues[2]=xx[1]-fSigma238;
      rValues[3]=xx[0]-fSigma235;
      break;
      
    case 3: // U235+U239 only fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-fSigma241;
      rValues[1]=xx[3]-fSigma240;
      rValues[2]=xx[1]-fSigma238;
      break;
      
    case 4:// U235+U239+U238 only fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-fSigma241;
      rValues[1]=xx[3]-fSigma240;
      break;
      
    case 5:case 8:  // Osc only and Eq fit
      rValues.ResizeTo(fTheoCovarianceMatrix.GetNrows());
      rValues[0]=xx[4]-fSigma241;
      rValues[1]=xx[3]-fSigma240;
      rValues[2]=xx[1]-fSigma238;
      rValues[3]=xx[2]-fSigma239;
      rValues[4]=xx[0]-fSigma235;
      break;
      
    case 11:
      break;
      
    default:
    // It should never cxome to this if error handling was done properly before
      return false;
      break;
  }
  return true;
}

double GlobalAnalyzer::DoEval(const double* xx)const{
  
  
  TVectorD yIBD(fNumberofExp);
  TMatrixD CovarianceMatrix(fNumberofExp,fNumberofExp);
  CovarianceMatrix.Zero();
  EvaluateTotalIBDYield(xx, yIBD);
  if (fIsStatCovMatrixReduced)
  {
    EvaluateCovarianceMatrix(yIBD,CovarianceMatrix);
    // CovarianceMatrix.Print();
  }
  else CovarianceMatrix=fTotalCovarianceMatrix;
  if(CovarianceMatrix.Invert()==0 || !(CovarianceMatrix.IsValid())) exit(1);
  TVectorD vTemp = yIBD;
  vTemp -= fVIBDExp;
  double Chi2Value=Mult(vTemp,CovarianceMatrix,vTemp);

  TVectorD rValues;
  // This is to calc chi2 contribution from the theoretical covariance matrix
  if(!EvaluateTheoDeltaVector(xx,rValues)) return -1; // Need to handle error better
  
  // Add chi2 contribution from theoretical covariance matrix
  Chi2Value+=Mult(rValues,fTheoCovarianceMatrix,rValues);;
  
  return Chi2Value;
}

bool GlobalAnalyzer::DrawDataPoints(TFile &outFile){
  if(!outFile.IsOpen() || outFile.IsZombie()) return false;
  outFile.cd();
  fGIBDExp.Write();
  return true;
}

bool GlobalAnalyzer::DrawFitPoints(double intercept, double slope, TFile &outFile){
  if(!outFile.IsOpen() || outFile.IsZombie()) return false;
  outFile.cd();
  for(int i=0;i<8;i++){
    fGIBDFit.SetPoint(i,fVFF239[i],intercept+slope*(fVFF239[i]-fFF239));
  }
  fGIBDFit.Write();
  return true;
}


/// Assumes no contribution from oscillation terms
bool GlobalAnalyzer::PlotIBDYields(const double *xx, TFile &outFile) const
{
  outFile.cd();
  TVectorD yIBD5 = xx[0] * fVFF235;
  TVectorD yIBD8 = xx[1] * fVFF238;
  TVectorD yIBD9 = xx[2] * fVFF239;
  TVectorD yIBD0 = xx[3] * fVFF240;
  TVectorD yIBD1 = xx[4] * fVFF241;

  TGraph *gyIBD5= new TGraph(fVFF239, yIBD5); gyIBD5->SetName("yIBD5");
  TGraph *gyIBD8= new TGraph(fVFF239, yIBD8); gyIBD8->SetName("yIBD8");
  TGraph *gyIBD9= new TGraph(fVFF239, yIBD9); gyIBD9->SetName("yIBD9");
  TGraph *gyIBD0= new TGraph(fVFF239, yIBD0); gyIBD0->SetName("yIBD0");
  TGraph *gyIBD1= new TGraph(fVFF239, yIBD1); gyIBD1->SetName("yIBD1");

  gyIBD5->Write();
  gyIBD8->Write();
  gyIBD9->Write();
  gyIBD0->Write();
  gyIBD1->Write();
  return true;
}

/// Same as the above function but for a vector
/// Assumes no contribution from oscillation terms
bool GlobalAnalyzer::PlotIBDYields(const TVectorD &xx, TFile &outFile) const
{
  outFile.cd();
  if(fVFF241.GetNoElements()<5)
  {
    printf("There are not enough elements in the input vector.\n");
    return false;
  }
  TVectorD yIBD5 = xx[0] * fVFF235;
  TVectorD yIBD8 = xx[1] * fVFF238;
  TVectorD yIBD9 = xx[2] * fVFF239;
  TVectorD yIBD0 = xx[3] * fVFF240;
  TVectorD yIBD1 = xx[4] * fVFF241;
  TVectorD ff1Over9 = fVFF241;
  TVectorD yIBD1Over9 = yIBD1;
  for(int i = 0; i <fVFF241.GetNoElements(); i++)
  {
    ff1Over9[i]/=fVFF239[i];
    yIBD1Over9[i]/=yIBD9[i];
  }
  TVectorD yIBDAll = yIBD5+yIBD8+yIBD9+yIBD0+yIBD1;
  TVectorD yIBDAllNo5 = yIBD8+yIBD9+yIBD0+yIBD1;

  TGraph *gyIBD5= new TGraph(fVFF239, yIBD5); gyIBD5->SetName("yIBD5");
  TGraph *gyIBD8= new TGraph(fVFF239, yIBD8); gyIBD8->SetName("yIBD8");
  TGraph *gyIBD9= new TGraph(fVFF239, yIBD9); gyIBD9->SetName("yIBD9");
  TGraph *gyIBD0= new TGraph(fVFF239, yIBD0); gyIBD0->SetName("yIBD0");
  TGraph *gyIBD1= new TGraph(fVFF239, yIBD1); gyIBD1->SetName("yIBD1");
  TGraph *gFF1Over9 = new TGraph(fVFF239, ff1Over9); gFF1Over9->SetName("ff1Over9");
  TGraph *gyIBD1Over9 = new TGraph(fVFF239, yIBD1Over9); gyIBD1Over9->SetName("yIBD1Over9");
  TGraph *gyIBDAll= new TGraph(fVFF239, yIBDAll); gyIBDAll->SetName("yIBDAll");
  TGraph *gyIBDAllN05 = new TGraph(fVFF239, yIBDAllNo5); gyIBDAllN05->SetName("yIBDAllNo5");

  gyIBD5->Write();
  gyIBD8->Write();
  gyIBD9->Write();
  gyIBD0->Write();
  gyIBD1->Write();
  gFF1Over9->Write();
  gyIBD1Over9->Write();
  gyIBDAllN05->Write();
  gyIBDAll->Write();
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
  printf("Using %s data,\n%s stat,\n%s syst,\nand %s theoretical covariance matrix files\n",
  dataInput.Data(), fCovStatFileName.Data(), fCovSystFileName.Data(), fTheoUncFileName.Data());
  //The information from Data text file is read when the object is initialized
  if(!ReadDataFromFile()) return false;

  fVDiff.ResizeTo(fNumberofExp);
  fGIBDExp.SetName("fGIBDExp");
  fGIBDFit.SetName("fGIBDFit");
  
  for(int i=0; i<fNumberofExp; i++){
    fGIBDExp.SetPoint(i,fVFF239[i],fVIBDExp[i]);
    fFF239+=fVFF239[i];
  }
  fFF239=fFF239/fNumberofExp;

  return true;
}

/// SetupExperiments will be called during runtime so that
/// the functions inside it can all be called at run time.
bool GlobalAnalyzer::SetupExperiments(int fitType){
  fFitType=fitType;
  // The fitter acts crazy when trying to fit to sin22theta and dm2 when they are not really needed
  // so those are fit parameters only for fits including oscillations
  if(fitType>4 && fitType<8) fNumberofFitPars = 7;
  else fNumberofFitPars = 7;
  if(!LoadFluxes()) return false;
  if(!LoadCovarianceMatrices()) return false;
  if(!LoadTheoCovMat()) return false;
  return true;
}
