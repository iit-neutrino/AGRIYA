# Global_Analysis
Global reactor neutrino rate + Daya Bay evolution data analysis.

To compile the code and link libraries and generate an executable, run the following  

  $ make    
  $ make analyzeGlobalData    

This will generate an executable which can then be used to fit data
The executable takes 8 inputs. In order, they are as follows:
- Name of the outout file
- Number of bins for isotopes 238, 239 and 241
- Minimum values of U235 to be used in the fit
- Maximum values of U235 to be used in the fit
- input data file
- statistical covariance matrix files
- systematic covariance matrix file

As an example, if you want to just fit only Daya Bay data, you can do:    
  $ ./analyzeGlobalData output.root 60 60 5 7 inputs/DYB.txt inputs/DYB_covstat.txt inputs/DYB_covsyst.txt

If other wise you want to fit global rate data, do:    
  $ ./analyzeGlobalData output.root 60 60 5 7 inputs/global.txt inputs/global_covstat.txt inputs/global_covsyst.txt

Now, to generate histogram from the output file generated above first make a directory to store histograms     
  $ mkdir outputDir

Then make the histogram plotting executable     
  $ make PlotHistograms

Plots histograms and saves in outputDir     
  $ ./PlotHistograms ./output.root ./outputDir/

If you just want to get the minimum chi2, isotopic bestfit values and their 1 sigmas, make GetSigma executable     
  $ make GetSigma

Run the executablr on the output root file                         
  $ ./GetSigma output.root
