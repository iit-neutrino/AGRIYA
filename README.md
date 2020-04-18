# Global Analysis
**Global reactor neutrino rate + Daya Bay evolution data analysis.**

## Prerequisites
1. [GNU Make](https://www.gnu.org/software/make/) (Typically available by default on Linux and Mac OSX)
2. [ROOT](https://root.cern.ch) (5.34 or higher, tested with 6.18)

## Obtaining a copy of code
To obtain the latest copy of the code do:    
```$ git clone git@github.com:iit-neutrino/Global_Fits.git     ```

## Building executable
cd to the fits directory     
```$ cd Global_Fits     ```

To compile the code and link libraries and generate an executable, run the following  

```$ make    ```
```$ make analyzeGlobalData      ```

## Running the executable
This will generate an executable which can then be used to fit data     
The executable takes 5 inputs. In order, they are as follows:     
* Name of the output file        
* Data file      
* Statistical covariance matrix files        
* Systematic covariance matrix file       
* Fit type    

Fit type should be a number from 1-11:    
_1 = U235 only_      
_2 = P239 only_    
_3 = U235+239_    
_4 = U235+239+238_    
_5 = OSC only_    
_6 = 235+OSC only_     
_7 = 239+OSC only_    
_8 = Equal contribution_      
_9 = 5+Equal contribution_    
_10 = 9+Equal contribution_    
_11 = linear fit to 239 data_    

As an example, if you want to fit Daya Bay data for U235 only hypothesis, you can do:    
```$ ./analyzeGlobalData output.root ./inputs/DYB.txt ./inputs/DYB_covstat.txt ./inputs/DYB_covsyst.txt 1```     

If other wise you want to fit global rate data for oscillation hypothesis, do:    
```$ ./analyzeGlobalData output.root inputs/global.txt inputs/global_covstat.txt inputs/global_covsyst.txt 5```     

## Making relevant plots
Now, to generate histogram from the output file generated above first make a directory to store histograms     
```$ mkdir outputDir```    

Then make the histogram plotting executable     
```$ make PlotGraphs```    

Plots histograms and saves in outputDir     
```$ ./PlotGraphs ./output.root ./outputDir/```    

## Other information
_- input data file_ used for the fit has 6 columns in the following format:          
| U 235 fission fraction |U 238 fission fraction |Pu 239 fission fraction   | ˘Pu 241 fission fraction  | Measured yields  | Baseline (m) |        
--- | --- | --- | --- | --- | --- |      
* And each row in the file represents one measurement/experiment       
* In case of Daya Bay example (_./inputs/DYB.txt_), the number of rows are 8 since they have 8 measured yields
* _-statistical and systematic covariance matrix files_ used for the fit has as many rows and columns as the number of measurements       
* This is same as the number of columns in the _- input data file_
* In case of Daya Bay (_./inputs/DYB_covstat.txt_ and _./inputs/DYB_covsyst.txt_), it is 8 rows and 8 columns         
* The diagonal elements (i.e., __row number == column number__) of the matrix correspond to fully-correlated terms in covariance matrix     
* The non-digonal term are the correlations between different measurements            
* Typically all statistical covariance matrices have only non-zero diagonal terms      
* For global experiments case, all the statitical and systematic uncertanities  are combined and saved in systematic uncertainties file  (_inputs/global_covsyst.txt_) and the statistical uncertainties are all taken to be zero    
