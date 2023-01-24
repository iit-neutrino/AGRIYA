////Macro to generate WG and RG MOX files
#include <iostream>
#include <fstream>
#include <sstream>

#include "TVectorD.h"
#include "TGraph.h"
#include "TSpline.h"

using namespace std;

void usage()
{
  printf("Example: generateFakeDYBYields \n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if(argc!=2)
  {
    usage();
  }
  double numberRead;
  string lineA;
  ifstream fileIn;

  fileIn.open(argv[1]);
  
  double U235Theo=6.69;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P241Theo=6.03;

  int numberofExp=0;

  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;

      double f_235 = 0.0;
      double f_238 = 0.0;
      double f_239 = 0.0;
      double f_240 = 0.0;
      double f_241 = 0.0;
      double baseline = 0.0;

      while(streamA >>numberRead)
      {
        if(columnsA == 0) f_235=numberRead;
        else if(columnsA == 1) f_238=numberRead;
        else if(columnsA == 2) f_239=numberRead;
        else if(columnsA == 3) f_240=numberRead;
        else if(columnsA == 4) f_241=numberRead;
        else if(columnsA == 6) baseline=numberRead;
        columnsA++;
      }
      
      double yield = f_235*U235Theo + f_238*U238Theo+ f_239*P239Theo+ f_241*P241Theo;
      std::cout<< f_235<<"\t"<<f_238<<"\t"<<f_239<<"\t"<<f_240<<"\t"<<f_241<<"\t"<<yield<<"\t"<<baseline<<std::endl;
      numberofExp++;
    }
  }
  

  // TVectorD v_Total_Fissions = v_F_235+v_F_238+v_F_239+v_F_241;
  if(numberofExp<=1) 
  { 
    cout << "Data doesn't exist in the file\n";
    cout << "Exiting\n";
    exit(-1);
  }
  return 0;
}