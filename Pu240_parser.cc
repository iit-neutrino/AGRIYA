#include <fstream>
#include "TH1F.h"
#include <iostream>
#include <string>
#include <vector>
#include "TFile.h"
#include "TCanvas.h"

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}


int main(int argc, char *argv[]){
    std::ifstream XsecInFile("Pu240_Nu_data/TCS_CC_anue_p_VB.txt");
    std::ifstream YieldInFile("Pu240_Nu_data/240PuF.txt");

    double x1, x2, y1, y2;
    std::vector<double> XsecE, Xsec, YieldE, Yield;
    int frontCut = 0, endCut = 0, XfrontCut = 0, XendCut = 0;

    while (XsecInFile >> x1 >> x2){
        XsecE.push_back(x1);
        Xsec.push_back(x2);
    }

    std::string str;
    std::ofstream YieldOutFile("240PuF_modified.txt");

    while(std::getline(YieldInFile, str)){
        if (YieldOutFile){
            YieldOutFile << ReplaceAll(str, std::string(";"), std::string(" ")) << "\n";
        }
    }

    std::ifstream YieldInFileMod("240PuF_modified.txt");

    while (YieldInFileMod >> y1 >> y2){
        YieldE.push_back(y1);
        Yield.push_back(y2);
    }


    for (int i = 0; i < YieldE.size()-1; i++){
        if (YieldE.at(i) <= 1.8){
            frontCut = i;
        }
        if ((YieldE.at(i) > 10) && (endCut == 0)){
            endCut = i;
        }
    }

    for (int i = 0; i < XsecE.size()-1; i++){
        if (XsecE.at(i) <= 1.8){
            XfrontCut = i;
        }
        if ((XsecE.at(i) > 10) && (XendCut == 0)){
            XendCut = i;
        }
    }

    if (frontCut != 0){
        YieldE.erase(YieldE.begin(), YieldE.begin()+frontCut);
        Yield.erase(Yield.begin(), Yield.begin()+frontCut);
    }

    if (endCut != 0){
        YieldE.erase(YieldE.begin()+endCut-frontCut, YieldE.begin()+YieldE.size());
        Yield.erase(Yield.begin()+endCut-frontCut, Yield.begin()+Yield.size());
    }

    if (XfrontCut != 0){
        XsecE.erase(XsecE.begin(), XsecE.begin()+XfrontCut);
        Xsec.erase(Xsec.begin(), Xsec.begin()+XfrontCut);
    }

    if (XendCut != 0){
        XsecE.erase(XsecE.begin()+XendCut, XsecE.begin()+XsecE.size());
        Xsec.erase(Xsec.begin()+XendCut, Xsec.begin()+Xsec.size());
    }

    std::vector<double> Nu;
    double integral = 0;
    for (int i = 0; i < XsecE.size(); i++){
        Nu.push_back(Xsec[i] * Yield[i] * 1e3); // assuming Xsec units originally in 1e-46, multiplying by 1e3 adjusts to 1e-43
        integral += Nu[i];
    }
    

    // Histogram plotter

    // Converting the x,y data points to counts
    //      Just treat y val as the number of times to Fill() in the bin in a loop

    // for ( count in vector for particular energy)
    //      Fill ( that particular energy )

    // binning should be from 1.80 to 10.00 with widths of 0.01
    // that means 10-1.8 = 8.2, 8.2/0.01 = 820  (+1 counting start) = 821 bins of width 0.01 each

    auto countHist = new TH1F("counts",
        "Expected Spectrum of Pu240;Energy (MeV);Spectrum (1/fission)",
        821, // number of bins
        1.80, // lower bound
        10.00); // upper bound

    for(int i = 0; i < YieldE.size(); i++) {
        // Fill(bin, weight) where weight is the fractional "count"
        countHist->Fill(YieldE[i],Yield[i]);
    }

    auto c = new TCanvas();
    countHist->Draw("HIST");
    c->SaveAs("P240_spectrum.pdf");

    c->SetLogy();
    countHist->SetTitle("Log of Expected Spectrum of Pu240");
    countHist->GetYaxis()->SetTitle("");
    c->Draw("HIST");
    c->SaveAs("P240_spectrum_log.pdf");

    auto IBDHist = new TH1F("yield",
        "IBD Yield of Pu240;Energy (MeV);Yield (10^{-43}cm^2/fission)",
        821, // number of bins
        1.80, // lower bound
        10.00); // upper bound

    for(int i = 0; i < XsecE.size(); i++) {
        // Fill(bin, weight) where weight is the fractional "count"
        IBDHist->Fill(XsecE[i],Nu[i]);
    }

    auto c1 = new TCanvas();
    IBDHist->Draw("HIST");

    c1->SaveAs("P240_IBD_yield.pdf");

    c1->SetLogy();
    IBDHist->SetTitle("Log of IBD Yield of Pu240");
    IBDHist->GetYaxis()->SetTitle("");
    c1->Draw("HIST");
    c1->SaveAs("P240_IBD_yield_log.pdf");

    int bin_a = IBDHist->GetXaxis()->FindBin(1.80);
    int bin_b = IBDHist->GetXaxis()->FindBin(10.00);

    std::cout << "Bin start: " << bin_a << " | Bin end: " << bin_b << std::endl;

    std::cout << "Root internal integral func: " << IBDHist->Integral(bin_a,bin_b) << std::endl;

    std::cout << std::endl;

    std::cout << "'for loop' sum: " << integral << std::endl;

    return(0);
}