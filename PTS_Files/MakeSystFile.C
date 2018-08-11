
void MakeSystFile(){
  ofstream myfile;
  myfile.open ("../inputs/global+DYB_covsyst_DE1.txt");
  
  double numberRead;
  string lineRead;    ///The line of text file currently read
  ifstream infile;
  int columnCounter=0;
  int rowCounter=0;
  int nRows=0;
  
  infile.open("../inputs/global+DYB_covsyst.txt");
  
  while(infile.good()){
    while(getline(infile, lineRead)){
      int i=0;
      int j=0;
      istringstream streamA(lineRead);
      columnCounter = 0;
      while(streamA >>numberRead){
        if(rowCounter>24 && columnCounter >24){
        myfile << numberRead-0.0001;
        myfile << "\t";
        }
        else{
          myfile << numberRead;
          myfile << "\t";
        }
//          std::cout << numberRead-0.0001 <<std::endl;
        columnCounter++;
      }
      myfile << "\n";
      rowCounter++;
    }
  }
}
