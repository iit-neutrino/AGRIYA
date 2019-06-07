# Global_Analysis
Global reactor neutrino rate + Daya Bay evolution data analysis.

To obtain the latest copy of the code do:    
$ git clone git@github.com:iit-neutrino/Global_Fits.git     

To compile the code and link libraries and generate an executable, run the following  

  $ make    
  $ make analyzeGlobalData    

This will generate an executable which can then be used to fit data
The executable takes 8 inputs. In order, they are as follows:
- Name of the output file
- input data file
- statistical covariance matrix files
- systematic covariance matrix file
- Fit type


Fit type should be a number from 1-11:    
- 1 = U235 only     
- 2 = P239 only 
- 3 = U235+239  
- 4 = U235+239+238  
- 5 = OSC only 
- 6 = 235+OSC only 
- 7 = 239+OSC only  
- 8 = Equal contribution   
- 9 = 5+Equal contribution    
- 10 = 9+Equal contribution    
- 11 = linear fit to 239 data    


As an example, if you want to fit Daya Bay data for U235 only hypothesis, you can do:    
  $ ./analyzeGlobalData output.root ./inputs/DYB.txt ./inputs/DYB_covstat.txt ./inputs/DYB_covsyst.txt 1     

If other wise you want to fit global rate data for oscillation hypothesis, do:    
  $ ./analyzeGlobalData output.root inputs/global.txt inputs/global_covstat.txt inputs/global_covsyst.txt 5     

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
