## List of data, covariance matrix etc input files needed for performing fits     

Divided into two directories;      
- current: Corresponding to the global data.     
- future: Corresponding to the hypothetical experiments   

### Naming scheme:    
- FreeParameters_DataUsed_FitConstranints_Uncertainties
eg: `235+238+239_global_Pu2410.75_exp.txt` corresponds to the fits on global rates + evolution data using the free parameters as the isotopes U235, U238, and Pu239 using assigning 75% on the fit parameter Pu241 and the uncertainties as quoted by the experiments     
- suffix `.txt` is the data file used    
- suffix `_covstat.txt` is the statistical covariance matrix file used     
- suffix `_covsyst.txt` is the systematical covariance matrix data file used     


### Data and input covariance matrix files:    
- evol: All the evolution data i.e, RENO (add citation) + DYB (add citation)    
- global+evol: All rate data + evolution data.     

### Other files:
- theo_arXiv_1703.00860.txt: Theoretical covariance matrix from arXiv:1703.00860
- IBD_yields_1703.00860.txt: Theoretical IBD yields from arXiv:1703.00860
