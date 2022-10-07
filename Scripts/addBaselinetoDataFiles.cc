////Macro to update a data file with no 240 ff ts to one with 240
#include <iostream>
#include <fstream>
#include <sstream>

#include "TVectorD.h"
using namespace std;

void usage()
{
  printf("Example: addBaselinetoDataFiles infilename.txt outfilename.txt\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  if(argc!=3) usage();
  double numberRead;
  string lineA;
  ifstream fileIn;

  fileIn.open(argv[1]);
  
  int numberofExp=0;
     ///Experimental IBD measurment
  TVectorD v_IBD_Exp;
  /// Fission fractions for U235
  TVectorD v_FF_235;
  /// Fission fractions for U238
  TVectorD v_FF_238;
  /// Fission fractions for P239
  TVectorD v_FF_239;
  /// Baselines of the experiments
  TVectorD v_FF_240;
  /// Fission fractions for P241
  TVectorD v_FF_241;
 

  while(fileIn.good()){
    while(getline(fileIn, lineA)){
      istringstream streamA(lineA);
      int columnsA = 0;
      v_FF_235.ResizeTo(numberofExp+1);
      v_FF_238.ResizeTo(numberofExp+1);
      v_FF_239.ResizeTo(numberofExp+1);
      v_FF_240.ResizeTo(numberofExp+1);
      v_FF_241.ResizeTo(numberofExp+1);
      v_IBD_Exp.ResizeTo(numberofExp+1);
      while(streamA >>numberRead)
      {
        if(columnsA == 0) v_FF_235[numberofExp]=numberRead;
        else if(columnsA == 1) v_FF_238[numberofExp]=numberRead;
        else if(columnsA == 2) v_FF_239[numberofExp]=numberRead;
        else if(columnsA == 3) v_FF_241[numberofExp]=numberRead;
        else if(columnsA == 4) v_FF_240[numberofExp]=numberRead;
        else if(columnsA == 5) v_IBD_Exp[numberofExp]=numberRead;
        columnsA++;
      }
      if(columnsA==7) 
		  {
        cout << "Looks like this file already contains baseline data\n";
        cout << "Exiting\n";
        exit(-1);
      }
      else if(columnsA!=6) 
		  {
        cout << "The doesn't have the right numpber of columns\n";
        cout << "Exiting\n";
        exit(-1);
      }
      numberofExp++;
    }
  }

  if(numberofExp<=1) 
  {
    cout << "Data doesn't exist in the file\n";
    cout << "Exiting\n";
    exit(-1);
  }

  fstream out_file;
	out_file.open(argv[2], ios::out);
	if (!out_file) {
		cout << "File not created!";
	}
	else 
  {
    for(int i = 0; i < numberofExp; i++)
    {
      out_file << v_FF_235[i]<<'\t';
      out_file << v_FF_238[i]<<'\t';
      out_file << v_FF_239[i]<<'\t';
      out_file << v_FF_240[i]<<'\t';
      out_file << v_FF_241[i]<<'\t';
      out_file << v_IBD_Exp[i]<<'\t';
      out_file << 0<<'\n';
    }
		out_file.close();
		cout << "File created successfully!\n";
	}
  return 0;
}