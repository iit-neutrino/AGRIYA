# Building and running ARIYS

## Prerequisites
1. [GNU Make](https://www.gnu.org/software/make/) (Typically available by default on Linux and Mac OSX)
2. [ROOT](https://root.cern.ch) (5.34 or higher, tested with 6.18 and  6.26/06)

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

To get help, do:
```$ ./analyzeGlobalData -help     ```

To run an example, just do:
```$ analyzeGlobalData input/example.mac     ```     
The argument must end in `.mac`   

Macro file should contain values for atleast the following keys:     
OUTPUTFILE, DATAFILE, COVARIANCEFILESTAT, COVARIANCEFILESYST, COVARIANCEFILETHEO, FITTYPE      
For the contents of each of these files, open and check the example files in the repo         
Example macro file:      
***
OUTPUTFILE = outputFile.root      
DATAFILE = ./inputs/global.txt    
COVARIANCEFILESTAT = inputs/global_covstat.txt     
COVARIANCEFILESYST = inputs/global_covsyst.txt   
COVARIANCEFILETHEO = inputs/theo_arXiv_1703.00860.txt     
FITTYPE = 1    
THEORETICALIBDYIELDSFILE = inputs/IBD_yields_1703.00860.txt       
***

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

## Making relevant plots
Now, to generate histogram from the output file generated above       
First make the histogram plotting executable     
```$ make plotGraphs```    

Then Plot all the relevant graphs/ histograms etc and save in pdfs in untracked directory       
```$ ./plotGraphs ./output.root ./untracked/```    

## Documentation    
You can generate documentation using [Doxygen](https://doxygen.nl/). If you already have Doxygen installed, do:    
```$ doxygen Doxyfile```     
which will generate documentation in `html` and `latex` formats in docs directory      
You can open the pdf or open `./docs/html/index.html` in your favorite browser    
Or you can do `make` from `./docs/latex` to produce `refman.pdf` file      

## Other information
*_DATAFILE_ used for the fit has 7 columns in the following format:          
| U 235 fission fraction |U 238 fission fraction |Pu 239 fission fraction    | Pu 240 fission fraction  | Pu 241 fission fraction  | Measured yields  | Baseline (m) |        
--- | --- | --- | --- | --- | --- | --- |      
* And each row in the file represents one measurement/experiment       
* In case of Daya Bay example (_./inputs/DYB.txt_), the number of rows are 8 since they have 8 measured yields
* _COVARIANCEFILESTAT_ and _COVARIANCEFILESYST_ covariance matrix files used for the fit has as many rows and columns as the number of measurements              
* This is same as the number of columns in the _- input data file_
* In case of Daya Bay (_./inputs/DYB_covstat.txt_ and _./inputs/DYB_covsyst.txt_), it is 8 rows and 8 columns         
* The diagonal elements (i.e., __row number == column number__) of the matrix correspond to fully-correlated terms in covariance matrix     
* The non-digonal term are the correlations between different measurements            
* Typically all statistical covariance matrices have only non-zero diagonal terms
* For global experiments case, all the statitical and systematic uncertanities  are combined and saved in systematic uncertainties file  (_inputs/global_covsyst.txt_) and the statistical uncertainties are all taken to be zero     
* _COVARIANCEFILETHEO_ file used for the fit has as many rows and columns as the number of isotopes which is currently 5       
* _THEORETICALIBDYIELDSFILE_ can be used to input the theoretical IBD yields for a given isotope      
* The IBD yield values for a given isotope is only used if it is not used as a floating parameter     
