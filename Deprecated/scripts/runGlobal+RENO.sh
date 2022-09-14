#!/bin/bash

source ~/Default_Startup

echo running global+RENO h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H1.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 1 > $GLOBALFITS/outputDir/global+RENO/H1.log
echo running global+RENO h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H2.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 2 > $GLOBALFITS/outputDir/global+RENO/H2.log
echo running global+RENO h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H3.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 3 > $GLOBALFITS/outputDir/global+RENO/H3.log
echo running global+RENO h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H4.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 4 > $GLOBALFITS/outputDir/global+RENO/H4.log
echo running global+RENO h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H5.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 5 > $GLOBALFITS/outputDir/global+RENO/H5.log
echo running global+RENO h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H6.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 6 > $GLOBALFITS/outputDir/global+RENO/H6.log
echo running global+RENO h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H7.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 7 > $GLOBALFITS/outputDir/global+RENO/H7.log
echo running global+RENO h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H8.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 8 > $GLOBALFITS/outputDir/global+RENO/H8.log
echo running global+RENO h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H9.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 9 > $GLOBALFITS/outputDir/global+RENO/H9.log
echo running global+RENO h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+RENO/H10.root $GLOBALFITS/inputs/global+RENO.txt $GLOBALFITS/inputs/global+RENO_covstat.txt $GLOBALFITS/inputs/global+RENO_covsyst.txt 10 > $GLOBALFITS/outputDir/global+RENO/H10.log
