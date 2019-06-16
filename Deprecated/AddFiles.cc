////////////////////////////////////////////////
// File to properly add multiple files generated from vejur jobs
////////////////////////////////////////////////

#include "InverseBetaDecay.hh"
using namespace std;


void usage(){
  std::cout << "Example: AddFiles outputfile.root file.list\n";
  exit(1);
}

std::vector<TFile*> UnpackFiles(TString inputList) {
  std::vector<TFile*> outputVector;
  outputVector.clear();
  
  std::ifstream inputFile(inputList.Data());
  if (!inputFile.is_open()) {
    printf("File %s not found!",inputList.Data());
    exit(1);
  }
  
  TString fileName;
  while (!inputFile.eof()) {
    inputFile >> fileName;
    // Need in case of empty line, ensures file name exists.
    if (fileName.IsWhitespace()) continue;
    TFile* temp = new TFile(fileName.Data(), "READ");
    if (!temp->IsOpen() || temp->IsZombie()) exit(1);
    outputVector.push_back(temp);
  }
  
  inputFile.close();
  return outputVector;
}

int main(int argc, char *argv[]){
  
  if(argc!=3) usage();
  InverseBetaDecay reactorIBD;
  
  // output ROOT file for saving plots
  TFile *outputFile=new TFile(argv[1],"RECREATE");
  
  
  TString inputList= argv[2];
  // Check that the input file name ends with .list
  // It should be a list of .root files that you want to analyze together
  if (!inputList.EndsWith(".list")) {
    printf("Incorrect argument structure.\n");
    printf(" Argument: %s must end in '.list' \n",inputList.Data());
    exit(1);
  }
  
  //Get previously made energy-position graphs
  std::vector<TFile*> inputFiles = UnpackFiles(inputList);
  
  // Histograms
  TH1D* hChi2MapU235=(TH1D*)inputFiles[0]->Get("hChi2MapU235");
  TH1D* hChi2MapU238=(TH1D*)inputFiles[0]->Get("hChi2MapU238");
  TH1D* hChi2MapP239=(TH1D*)inputFiles[0]->Get("hChi2MapP239");
  TH1D* hChi2MapP241=(TH1D*)inputFiles[0]->Get("hChi2MapP241");
  
  TH1D* hChi2U235Total=(TH1D*)hChi2MapU235->Clone("hChi2U235");
  TH1D* hChi2U238Total=(TH1D*)hChi2MapU238->Clone("hChi2U238");
  TH1D* hChi2P239Total=(TH1D*)hChi2MapP239->Clone("hChi2P239");
  TH1D* hChi2P241Total=(TH1D*)hChi2MapP241->Clone("hChi2P241");
  
  hChi2U235Total->Scale(0);
  hChi2U238Total->Scale(0);
  hChi2P239Total->Scale(0);
  hChi2P241Total->Scale(0);
  
  TH2D* hChi2MapU235238=(TH2D*)inputFiles[0]->Get("hChi2MapU235238");
  TH2D* hChi2MapU235239=(TH2D*)inputFiles[0]->Get("hChi2MapU235239");
  TH2D* hChi2MapU235241=(TH2D*)inputFiles[0]->Get("hChi2MapU235241");
  TH2D* hChi2MapU238239=(TH2D*)inputFiles[0]->Get("hChi2MapU238239");
  TH2D* hChi2MapU238241=(TH2D*)inputFiles[0]->Get("hChi2MapU238241");
  TH2D* hChi2MapP239241=(TH2D*)inputFiles[0]->Get("hChi2MapP239241");
  
  TH2D* hChi2U235238Total=(TH2D*)hChi2MapU235238->Clone("hChi2U235238");
  TH2D* hChi2U235239Total=(TH2D*)hChi2MapU235239->Clone("hChi2U235239");
  TH2D* hChi2U235241Total=(TH2D*)hChi2MapU235241->Clone("hChi2U235241");
  TH2D* hChi2U238239Total=(TH2D*)hChi2MapU238239->Clone("hChi2U238239");
  TH2D* hChi2U238241Total=(TH2D*)hChi2MapU238241->Clone("hChi2U238241");
  TH2D* hChi2P239241Total=(TH2D*)hChi2MapP239241->Clone("hChi2P239241");
  
  hChi2U235238Total->Scale(0);
  hChi2U235239Total->Scale(0);
  hChi2U235241Total->Scale(0);
  hChi2U238239Total->Scale(0);
  hChi2U238241Total->Scale(0);
  hChi2P239241Total->Scale(0);
  
  // Set some randomly high values for 238, 239 and 241 histograms
  for(int i=1 ;i<=hChi2U238Total->GetXaxis()->GetNbins();i++){
    hChi2U238Total->SetBinContent(i,100000);
  }
  for(int i=1 ;i<=hChi2P239Total->GetXaxis()->GetNbins();i++){
    hChi2P239Total->SetBinContent(i,100000);
  }
  for(int i=1 ;i<=hChi2P241Total->GetXaxis()->GetNbins();i++){
    hChi2P241Total->SetBinContent(i,100000);
  }
  for(int i=1 ;i<=hChi2U238239Total->GetXaxis()->GetNbins();i++){
    for(int j=1 ;j<=hChi2U238239Total->GetYaxis()->GetNbins();j++){
      hChi2U238239Total->SetBinContent(i,j,100000);
    }
  }
  for(int i=1 ;i<=hChi2U238241Total->GetXaxis()->GetNbins();i++){
    for(int j=1 ;j<=hChi2U238241Total->GetYaxis()->GetNbins();j++){
      hChi2U238241Total->SetBinContent(i,j,100000);
    }
  }
  for(int i=1 ;i<=hChi2P239241Total->GetXaxis()->GetNbins();i++){
    for(int j=1 ;j<=hChi2P239241Total->GetYaxis()->GetNbins();j++){
      hChi2P239241Total->SetBinContent(i,j,100000);
    }
  }
  
  
  
  
  // Stitching U235 histograms together
  std::cout<<inputFiles.size()<<std::endl;
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH1D* temp;
    temp = (TH1D*)inputFiles[iL]->Get("hChi2MapU235");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      hChi2U235Total->AddBinContent(i,temp->GetBinContent(i));
    }
  }
  
  // Iterating over U238 and finding the minimum
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH1D* temp;
    temp = (TH1D*)inputFiles[iL]->Get("hChi2MapU238");
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      //      std::cout<< i << "   " << iL << "  "  << temp->GetBinContent(i) <<std::endl;
      if(hChi2U238Total->GetBinContent(i)>temp->GetBinContent(i)) hChi2U238Total->SetBinContent(i,temp->GetBinContent(i));
    }
  }
  // Iterating over P239 and finding the minimum
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH1D* temp;
    temp = (TH1D*)inputFiles[iL]->Get("hChi2MapP239");
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      if(hChi2P239Total->GetBinContent(i)>temp->GetBinContent(i)) hChi2P239Total->SetBinContent(i,temp->GetBinContent(i));
    }
  }
  // Iterating over U241 and finding the minimum
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH1D* temp;
    temp = (TH1D*)inputFiles[iL]->Get("hChi2MapP241");
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      if(hChi2P241Total->GetBinContent(i)>temp->GetBinContent(i)) hChi2P241Total->SetBinContent(i,temp->GetBinContent(i));
    }
  }
  
  //Stitching U235238 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapU235238");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if(hChi2U235238Total->GetBinContent(i,j)==0.0)hChi2U235238Total->SetBinContent(i,j,temp->GetBinContent(i,j));
      }
    }
  }
  //Stitching U235239 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapU235239");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if(hChi2U235239Total->GetBinContent(i,j)==0.0)hChi2U235239Total->SetBinContent(i,j,temp->GetBinContent(i,j));
      }
    }
  }
  //Stitching U235238 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapU235241");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if(hChi2U235241Total->GetBinContent(i,j)==0.0)hChi2U235241Total->SetBinContent(i,j,temp->GetBinContent(i,j));
      }
    }
  }
  //Stitching U238239 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapU238239");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if(hChi2U238239Total->GetBinContent(i,j)>temp->GetBinContent(i,j))hChi2U238239Total->SetBinContent(i,j,temp->GetBinContent(i,j));
      }
    }
  }  //Stitching U238241 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapU238241");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if(hChi2U238241Total->GetBinContent(i,j)>temp->GetBinContent(i,j))hChi2U238241Total->SetBinContent(i,j,temp->GetBinContent(i,j));
      }
    }
  }  //Stitching U239241 histogram
  for (unsigned int iL = 0; iL < inputFiles.size(); iL++) {
    TH2D* temp;
    temp = (TH2D*)inputFiles[iL]->Get("hChi2MapP239241");
    std::cout<<temp->Integral() <<std::endl;
    for(int i=1 ;i<=temp->GetXaxis()->GetNbins();i++){
      for(int j=1 ;j<=temp->GetYaxis()->GetNbins();j++){
        if((hChi2P239241Total->GetBinContent(i,j))>(temp->GetBinContent(i,j))){
          hChi2P239241Total->SetBinContent(i,j,temp->GetBinContent(i,j));
        };
      }
    }
  }
  
  
  
  outputFile->cd();
  
  // Restore the names, helpful for plotting files later on
  hChi2U235Total->SetName("hChi2MapU235");
  hChi2U238Total->SetName("hChi2MapU238");
  hChi2P239Total->SetName("hChi2MapP239");
  hChi2P241Total->SetName("hChi2MapP241");
  
  
  hChi2U235238Total->SetName("hChi2MapU235238");
  hChi2U235239Total->SetName("hChi2MapU235239");
  hChi2U235241Total->SetName("hChi2MapU235241");
  hChi2U238239Total->SetName("hChi2MapU238239");
  hChi2U238241Total->SetName("hChi2MapU238241");
  hChi2P239241Total->SetName("hChi2MapP239241");
  
  hChi2U235Total->Write();
  hChi2U238Total->Write();
  hChi2P239Total->Write();
  hChi2P241Total->Write();
  
  hChi2U235238Total->Write();
  hChi2U235239Total->Write();
  hChi2U235241Total->Write();
  hChi2U238239Total->Write();
  hChi2U238241Total->Write();
  hChi2P239241Total->Write();
  
  outputFile->Close();
  
  return 0;
}
