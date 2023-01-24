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
  printf("Example: generateDYBUncertainties \n");
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

  int n_exp=0;
  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);

      while(streamA >>numberRead)
      {
        if ((n_exp>31 && n_exp<40) || n_exp>40)
        {
          std::cout<<numberRead/2.0<<"\t";
        }
        else
        {
          std::cout<<numberRead<<"\t";
        }
      }
      n_exp++;
      std::cout<<"\n";
    }
  }
  return 0;
}