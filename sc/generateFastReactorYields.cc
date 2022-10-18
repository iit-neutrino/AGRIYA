////Macro to generate VTR and PFBR yields
#include <iostream>
#include <fstream>
#include <sstream>

#include "TVectorD.h"
#include "TGraph.h"
#include "TSpline.h"

using namespace std;

void usage()
{
  printf("Example: generateFastReactorYields \n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if(argc!=1)
  {
    usage();
  }
  double numberRead;
  string lineA;
  ifstream fileIn;
  
  double U235Theo=6.046;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P240Theo=4.96;
  double P241Theo=6.03;

  double FF_5_VTR = 0.132;
  double FF_8_VTR = 0.126;
  double FF_9_VTR = 0.618;
  double FF_0_VTR = 0.082;
  double FF_1_VTR = 0.037;

  double FF_VTR_total = FF_5_VTR + FF_8_VTR + FF_9_VTR + FF_0_VTR + FF_1_VTR;

  FF_5_VTR = FF_5_VTR/FF_VTR_total;
  FF_8_VTR = FF_8_VTR/FF_VTR_total;
  FF_9_VTR = FF_9_VTR/FF_VTR_total;
  FF_0_VTR = FF_0_VTR/FF_VTR_total;
  FF_1_VTR = FF_1_VTR/FF_VTR_total;

  double yield_VTR = FF_5_VTR*U235Theo + FF_8_VTR*U238Theo + FF_9_VTR*P239Theo + FF_0_VTR*P240Theo + FF_1_VTR*P241Theo;

  std::cout<<FF_5_VTR<<'\t'<<FF_8_VTR<<'\t'<<FF_9_VTR<<'\t'<<FF_0_VTR<<'\t'<<FF_1_VTR<<'\t'<<yield_VTR<<'\t'<<0<<'\n';

  double FF_5_PFBR = 0.0093;
  double FF_8_PFBR = 0.1;
  double FF_9_PFBR = 0.71;
  double FF_0_PFBR = 0.05;
  double FF_1_PFBR = 0.11;

  double FF_PFBR_total = FF_5_PFBR + FF_8_PFBR + FF_9_PFBR + FF_0_PFBR + FF_1_PFBR;

  FF_5_PFBR = FF_5_PFBR/FF_PFBR_total;
  FF_8_PFBR = FF_8_PFBR/FF_PFBR_total;
  FF_9_PFBR = FF_9_PFBR/FF_PFBR_total;
  FF_0_PFBR = FF_0_PFBR/FF_PFBR_total;
  FF_1_PFBR = FF_1_PFBR/FF_PFBR_total;

  double yield_PFBR = FF_5_PFBR*U235Theo + FF_8_PFBR*U238Theo + FF_9_PFBR*P239Theo + FF_0_PFBR*P240Theo + FF_1_PFBR*P241Theo;

  std::cout<<FF_5_PFBR<<'\t'<<FF_8_PFBR<<'\t'<<FF_9_PFBR<<'\t'<<FF_0_PFBR<<'\t'<<FF_1_PFBR<<'\t'<<yield_PFBR<<'\t'<<0<<'\n';

  return 0;
}