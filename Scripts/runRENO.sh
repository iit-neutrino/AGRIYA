#!/bin/bash

source ~/Default_Startup

echo running RENO h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H1.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 1 > $GLOBALFITS/outputDir/RENO/H1.log
echo running RENO h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H2.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 2 > $GLOBALFITS/outputDir/RENO/H2.log
echo running RENO h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H3.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 3 > $GLOBALFITS/outputDir/RENO/H3.log
echo running RENO h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H4.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 4 > $GLOBALFITS/outputDir/RENO/H4.log
echo running RENO h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H5.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 5 > $GLOBALFITS/outputDir/RENO/H5.log
echo running RENO h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H6.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 6 > $GLOBALFITS/outputDir/RENO/H6.log
echo running RENO h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H7.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 7 > $GLOBALFITS/outputDir/RENO/H7.log
echo running RENO h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H8.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 8 > $GLOBALFITS/outputDir/RENO/H8.log
echo running RENO h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H9.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 9 > $GLOBALFITS/outputDir/RENO/H9.log
echo running RENO h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO/H10.root $GLOBALFITS/inputs/RENO.txt $GLOBALFITS/inputs/RENO_covstat.txt $GLOBALFITS/inputs/RENO_covsyst.txt 10 > $GLOBALFITS/outputDir/RENO/H10.log
