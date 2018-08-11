#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <string>
#include <map>
#include <vector>
#include <cmath>
using namespace std;

int rowA = 0; //stores the number of experiments
int colA = 0;//stores the number of columbs
double arrayA[100][100];

///Reads the text with the information about the experiments and placess
///the fission fractions, rates and uncertainty in arrayA
int DataInput(){
	double x;
	string lineA;
	ifstream fileIn;

	fileIn.open("ExpInput.txt");// Name of file where the experiments info like ,fission fraction, rates and uncertainty is stored.
  //writes data from the above text file and stores it in arrayA
	while(fileIn.good()){
		while(getline(fileIn, lineA)){
			istringstream streamA(lineA);
			colA = 0;
			while(streamA >>x){
				arrayA[rowA][colA] = x;
				colA++;
			}
			rowA++;
		}

	}
	return(0);

};



int main()
{
	DataInput(); //calls DataInput so the experimental info is stored in arrayA


  
/// Enumerator for experimental indices
enum TExperimentIndex_t 
{
  BUGEY_4=0,
  ROVNO91,
  ROVNO88_1I,
  ROVNO88_2I,
  ROVNO88_1S,
  ROVNO88_2S,
  ROVNO88_3S,
  BUGEY_3_15,
  BUGEY_3_40,
  BUGEY_3_95,
  GOESGEN_38,
  GOESGEN_46,
  GOESGEN_65,
  ILL,
  KRASNOYARSK87_33,
  KRASNOYARSK87_92,
  KRASNOYARSK94_57,
  KRASNOYARSK94_34,
  SRP_18,
  SRP_24,
  NUCIFER,
  CHOOZ,
  PALOVERDE,
  DayaBay,
  RENO,
  DOUBLECHOOZ
};

//sets the correlateduncertainty matrix to zero
double m_CorrUncertainty[rowA][rowA] = {{0}};



// Bugey4 and Rovno 91 uncertainties
m_CorrUncertainty[BUGEY_4][ROVNO91]=1.4;
m_CorrUncertainty[ROVNO91][BUGEY_4]=1.4;

// Rovno88 uncertainties
m_CorrUncertainty[ROVNO88_1I][ROVNO88_2I]=3.8;
m_CorrUncertainty[ROVNO88_2I][ROVNO88_1I]=3.8;

m_CorrUncertainty[ROVNO88_1S][ROVNO88_2S]=3.8;
m_CorrUncertainty[ROVNO88_1S][ROVNO88_3S]=3.8;
m_CorrUncertainty[ROVNO88_2S][ROVNO88_1S]=3.8;
m_CorrUncertainty[ROVNO88_2S][ROVNO88_3S]=3.8;
m_CorrUncertainty[ROVNO88_3S][ROVNO88_1S]=3.8;
m_CorrUncertainty[ROVNO88_3S][ROVNO88_2S]=3.8;

m_CorrUncertainty[ROVNO88_1I][ROVNO88_1S]=2.2;
m_CorrUncertainty[ROVNO88_1I][ROVNO88_2S]=2.2;
m_CorrUncertainty[ROVNO88_1I][ROVNO88_3S]=2.2;
m_CorrUncertainty[ROVNO88_2I][ROVNO88_1S]=2.2;
m_CorrUncertainty[ROVNO88_2I][ROVNO88_2S]=2.2;
m_CorrUncertainty[ROVNO88_2I][ROVNO88_3S]=2.2;

m_CorrUncertainty[ROVNO88_1S][ROVNO88_1I]=2.2;
m_CorrUncertainty[ROVNO88_1S][ROVNO88_2I]=2.2;
m_CorrUncertainty[ROVNO88_2S][ROVNO88_1I]=2.2;
m_CorrUncertainty[ROVNO88_2S][ROVNO88_2I]=2.2;
m_CorrUncertainty[ROVNO88_3S][ROVNO88_1I]=2.2;
m_CorrUncertainty[ROVNO88_3S][ROVNO88_2I]=2.2;

// Bugey3 uncertainties
m_CorrUncertainty[BUGEY_3_15][BUGEY_3_40]=4.0;
m_CorrUncertainty[BUGEY_3_15][BUGEY_3_95]=4.0;
m_CorrUncertainty[BUGEY_3_40][BUGEY_3_15]=4.0;
m_CorrUncertainty[BUGEY_3_40][BUGEY_3_95]=4.0;
m_CorrUncertainty[BUGEY_3_95][BUGEY_3_15]=4.0;
m_CorrUncertainty[BUGEY_3_95][BUGEY_3_40]=4.0;

// Goesgen and ILL uncertainties
m_CorrUncertainty[GOESGEN_38][GOESGEN_46]=2.0;
m_CorrUncertainty[GOESGEN_38][GOESGEN_65]=2.0;
m_CorrUncertainty[GOESGEN_46][GOESGEN_38]=2.0;
m_CorrUncertainty[GOESGEN_46][GOESGEN_65]=2.0;
m_CorrUncertainty[GOESGEN_65][GOESGEN_38]=2.0;
m_CorrUncertainty[GOESGEN_65][GOESGEN_46]=2.0;

m_CorrUncertainty[GOESGEN_38][ILL]=3.8;
m_CorrUncertainty[GOESGEN_46][ILL]=3.8;
m_CorrUncertainty[GOESGEN_65][ILL]=3.8;
m_CorrUncertainty[ILL][GOESGEN_38]=3.8;
m_CorrUncertainty[ILL][GOESGEN_46]=3.8;
m_CorrUncertainty[ILL][GOESGEN_65]=3.8;

//Krasnoyarsk uncertainties
m_CorrUncertainty[KRASNOYARSK87_33][KRASNOYARSK87_92]=4.1;
m_CorrUncertainty[KRASNOYARSK87_92][KRASNOYARSK87_33]=4.1;




fstream OFile;
OFile.open("cov_stat26exp.txt"); //This file is where the covariance matrix willll be written

for(int i = 0; i < rowA; i++){
  for(int j = 0; j<rowA; j++){
    if(!(i == j)){
    OFile << (0* pow((m_CorrUncertainty[i][j]/100.0),2)); // writes the non-diagonal elements of the covariance matrix
    OFile << "\t";
    }
    
    else{
      OFile << (0* pow(arrayA[i][5],2));//writes the diagonal elements of the covariancs matrix
      OFile <<"\t";
    }
  }
  OFile << "\n";
};
}

