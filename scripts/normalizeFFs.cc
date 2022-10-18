////Macro to update a data file with no 240 ff ts to one with 240
#include <iostream>
#include <fstream>
#include <sstream>

#include "TVectorD.h"
using namespace std;

void usage()
{
  printf("Example: normalizeFFs infilename.txt outfilename.txt\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  int include240=-1;
  if(argc!=3)
  {
    if(argc!=4) usage();
     // If 0, include 240 in the output file otherwise no
    else include240= atoi(argv[3]);
  }
  double numberRead;
  string lineA;
  ifstream fileIn;

  fileIn.open(argv[1]);
  
  int numberofExp=0;
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
      v_F_235.ResizeTo(numberofExp+1);
      v_F_238.ResizeTo(numberofExp+1);
      v_F_239.ResizeTo(numberofExp+1);
      v_F_241.ResizeTo(numberofExp+1);
      while(streamA >>numberRead)
      {
        if(columnsA == 0) v_F_235[numberofExp]=numberRead;
        else if(columnsA == 1) v_F_238[numberofExp]=numberRead;
        else if(columnsA == 2) v_F_239[numberofExp]=numberRead;
        else if(columnsA == 3) v_F_241[numberofExp]=numberRead;
        columnsA++;
      }
      if(columnsA!=4) 
		  {
        cout << "The doesn't have the right numpber of columns\n";
        cout << "Exiting\n";
        exit(-1);
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

  fstream out_file;
	out_file.open(argv[2], ios::out);
	if (!out_file) {
		cout << "File not created!";
	}
	else 
  {
    for(int i = 0; i < numberofExp; i++)
    {
      out_file << v_F_235[i]/v_Total_Fissions[i]<<'\t';
      out_file << v_F_238[i]/v_Total_Fissions[i]<<'\t';
      out_file << v_F_239[i]/v_Total_Fissions[i]<<'\t';
      if(include240) out_file << 0<<'\t';
      out_file << v_F_241[i]/v_Total_Fissions[i]<<'\t';
      //THis is just to make sure there are no errors when reading the output file into GlobalAnalyzer
      out_file << 0 <<'\t';
      out_file << 0 <<'\n';
    }
		out_file.close();
		cout << "File created successfully!\n";
	}
  return 0;
}