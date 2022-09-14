#include "GlobalAnalyzer.hh"

void usage(){
  printf("Wrong args\n");
  exit(1);
}

int main(int argc,char** argv){
  if(argc==1) usage();
  TString st(argv[1]);
  std::ofstream file;
  
  TString inFileName("../Global_Analysis/Outputs/global+DYB.root");
  TString outFileName(st);
  outFileName.Append(".txt");
  file.open(outFileName);
  file<< "#"<<st.Data()<< "\n \n";
//  std::cout<< st.Data()<< std::endl;
  TFile *f=TFile::Open(inFileName);
  TGraph *g=(TGraph*)f->Get(st.Data());
  
  double x,y;
  int nPs=g->GetMaxSize();
  for(int i=0;i<nPs;i++){
    g->GetPoint(i,x,y);
    file<< x<<","<<y<<"\n";
//    std::cout<< x<<","<<y<<"\n";
  }
}
