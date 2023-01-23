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
  
  double U235Theo=6.37333;
  double U238Theo=7.36813;
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
      double f_241 = 0.0;
      double baseline = 0.0;

      while(streamA >>numberRead)
      {
        if(columnsA == 0) f_235=numberRead;
        else if(columnsA == 1) f_238=numberRead;
        else if(columnsA == 2) f_239=numberRead;
        else if(columnsA == 3) f_241=numberRead;
        else if(columnsA == 5) baseline=numberRead;
        columnsA++;
      }
      double f_239over241= f_239/f_241;
      double f_239plus241 = f_239 + f_241;
      double fractionalChange=1.025;
      double ratio = f_239over241*fractionalChange;
      double fm_239 = f_239plus241/(1.0/ratio+1.0);
      double fm_241 = f_239plus241/(ratio+1.0);

      double yield;
      
      // std::cout<< f_239<<" : "<<f_241 <<"--"<<fm_239<<" : "<<fm_241<< "    " << f_239plus241<< " : "<<fm_239+fm_241<< "    " << (fm_239/fm_241)/f_239over241<<std::endl;
      if(fm_239>0)
      {
        yield = f_235*U235Theo + f_238*U238Theo+ fm_239*P239Theo+ fm_241*P241Theo;
        std::cout<< f_235<<"\t"<<f_238<<"\t"<<fm_239<<"\t"<<0.0<<"\t"<<fm_241<<"\t"<<yield<<"\t"<<baseline<<std::endl;
      }
      else 
      {
        yield = f_235*U235Theo + f_238*U238Theo+ f_239*P239Theo+ f_241*P241Theo;
        std::cout<< f_235<<"\t"<<f_238<<"\t"<<0<<"\t"<<0.0<<"\t"<<0<<"\t"<<yield<<"\t"<<baseline<<std::endl;  
      }

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