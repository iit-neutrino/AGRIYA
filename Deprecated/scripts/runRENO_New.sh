#!/bin/bash

source ~/Default_Startup

echo running RENO_New h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H1.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 1 > $GLOBALFITS/outputDir/RENO_New/H1.log
echo running RENO_New h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H2.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 2 > $GLOBALFITS/outputDir/RENO_New/H2.log
echo running RENO_New h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H3.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 3 > $GLOBALFITS/outputDir/RENO_New/H3.log
echo running RENO_New h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H4.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 4 > $GLOBALFITS/outputDir/RENO_New/H4.log
echo running RENO_New h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H5.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 5 > $GLOBALFITS/outputDir/RENO_New/H5.log
echo running RENO_New h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H6.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 6 > $GLOBALFITS/outputDir/RENO_New/H6.log
echo running RENO_New h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H7.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 7 > $GLOBALFITS/outputDir/RENO_New/H7.log
echo running RENO_New h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H8.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 8 > $GLOBALFITS/outputDir/RENO_New/H8.log
echo running RENO_New h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H9.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 9 > $GLOBALFITS/outputDir/RENO_New/H9.log
echo running RENO_New h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/RENO_New/H10.root $GLOBALFITS/inputs/RENO_New.txt $GLOBALFITS/inputs/RENO_New_covstat.txt $GLOBALFITS/inputs/RENO_New_covsyst.txt 10 > $GLOBALFITS/outputDir/RENO_New/H10.log
