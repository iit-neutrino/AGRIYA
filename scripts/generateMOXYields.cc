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
  printf("Example: generateMOXYields \n");
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

  fileIn.open("untracked/future/50MOX_fullcycle.txt");
  
  double U235Theo=6.046;
  double U238Theo=10.10;
  double P239Theo=4.40;
  double P241Theo=6.03;

  int numberofExp=0;
  /// Time
  TVectorD v_Time;
  /// Fission fractions for U235
  TVectorD v_F_235;
  /// Fission fractions for U238
  TVectorD v_F_238;
  /// Fission fractions for P239
  TVectorD v_F_239;
  /// Fission fractions for P241
  TVectorD v_F_241;
 

  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;
      v_Time.ResizeTo(numberofExp+1);
      v_F_235.ResizeTo(numberofExp+1);
      v_F_238.ResizeTo(numberofExp+1);
      v_F_239.ResizeTo(numberofExp+1);
      v_F_241.ResizeTo(numberofExp+1);
      while(streamA >>numberRead)
      {
        if(columnsA == 0) v_Time[numberofExp]=numberRead;
        else if(columnsA == 1) v_F_235[numberofExp]=numberRead;
        else if(columnsA == 2) v_F_238[numberofExp]=numberRead;
        else if(columnsA == 3) v_F_239[numberofExp]=numberRead;
        else if(columnsA == 4) v_F_241[numberofExp]=numberRead;
        columnsA++;
      }
      numberofExp++;
    }
  }

  TVectorD v_Total_Fissions = v_F_235+v_F_238+v_F_239+v_F_241;
  if(numberofExp<=1) 
  {
    cout << "Data doesn't exist in the file\n";
    cout << "Exiting\n";
    exit(-1);
  }

  // ROOT::Math::Interpolator interpolator(v_Time, v_F_235);

  TGraph *g_F_235 = new TGraph(v_Time, v_F_235);
  TGraph *g_F_238 = new TGraph(v_Time, v_F_238);
  TGraph *g_F_239 = new TGraph(v_Time, v_F_239);
  TGraph *g_F_241 = new TGraph(v_Time, v_F_241);
  TGraph *g_F_all = new TGraph(v_Time, v_Total_Fissions);

  TSpline3 *s_235 = new TSpline3("s_5",g_F_235);
  TSpline3 *s_238 = new TSpline3("s_8",g_F_238);
  TSpline3 *s_239 = new TSpline3("s_9",g_F_239);
  TSpline3 *s_241 = new TSpline3("s_1",g_F_241);
  TSpline3 *s_all = new TSpline3("s_all",g_F_all);

  int desiredNexpts = 10;
  fstream out_file_WG;
	out_file_WG.open("untracked/future/50WGMOX.txt", ios::out);
	if (!out_file_WG) {
		cout << "File not created!";
	}
	else 
  {
    for(int i = 0; i < desiredNexpts; i++)
    {
      double time = i*((1600.0/3.0)/(desiredNexpts-1)) ;
      double yield = (s_235->Eval(time)*U235Theo + s_238->Eval(time)*U238Theo +
      s_239->Eval(time)*P239Theo +s_241->Eval(time)*P241Theo)/s_all->Eval(time);
      out_file_WG << s_235->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_WG << s_238->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_WG << s_239->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_WG << 0.0 <<"\t";
      out_file_WG << s_241->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_WG << yield <<"\t";
      out_file_WG << 0.0 <<"\n";
    }
		out_file_WG.close();
		cout << "File created successfully!\n";
	}

  fstream out_file_RG;
	out_file_RG.open("untracked/future/50RGMOX.txt", ios::out);
	if (!out_file_RG) {
		cout << "File not created!";
	}
	else 
  {
    for(int i = 0; i < desiredNexpts; i++)
    {
      double time = 800.0+i*((1350.0-800.0)/(desiredNexpts-1)) ;
      double yield = (s_235->Eval(time)*U235Theo + s_238->Eval(time)*U238Theo +
      s_239->Eval(time)*P239Theo +s_241->Eval(time)*P241Theo)/s_all->Eval(time);
      out_file_RG << s_235->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_RG << s_238->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_RG << s_239->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_RG << 0.0 <<"\t";
      out_file_RG << s_241->Eval(time)/s_all->Eval(time)<<"\t";
      out_file_RG << yield <<"\t";
      out_file_RG << 0.0 <<"\n";
    }
		out_file_RG.close();
		cout << "File created successfully!\n";
	}
  return 0;
}