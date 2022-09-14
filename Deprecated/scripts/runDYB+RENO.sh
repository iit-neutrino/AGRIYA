#!/bin/bash

source ~/Default_Startup

echo running DYB+RENO h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H1.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 1 > $GLOBALFITS/outputDir/DYB+RENO/H1.log
echo running DYB+RENO h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H2.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 2 > $GLOBALFITS/outputDir/DYB+RENO/H2.log
echo running DYB+RENO h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H3.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 3 > $GLOBALFITS/outputDir/DYB+RENO/H3.log
echo running DYB+RENO h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H4.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 4 > $GLOBALFITS/outputDir/DYB+RENO/H4.log
echo running DYB+RENO h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H5.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 5 > $GLOBALFITS/outputDir/DYB+RENO/H5.log
echo running DYB+RENO h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H6.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 6 > $GLOBALFITS/outputDir/DYB+RENO/H6.log
echo running DYB+RENO h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H7.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 7 > $GLOBALFITS/outputDir/DYB+RENO/H7.log
echo running DYB+RENO h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H8.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 8 > $GLOBALFITS/outputDir/DYB+RENO/H8.log
echo running DYB+RENO h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H9.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 9 > $GLOBALFITS/outputDir/DYB+RENO/H9.log
echo running DYB+RENO h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB+RENO/H10.root $GLOBALFITS/inputs/DYB+RENO.txt $GLOBALFITS/inputs/DYB+RENO_covstat.txt $GLOBALFITS/inputs/DYB+RENO_covsyst.txt 10 > $GLOBALFITS/outputDir/DYB+RENO/H10.log
