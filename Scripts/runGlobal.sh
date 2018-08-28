#!/bin/bash

source ~/Default_Startup

echo running global h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H1.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 1 > $GLOBALFITS/outputDir/global/H1.log
echo running global h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H2.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 2 > $GLOBALFITS/outputDir/global/H2.log
echo running global h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H3.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 3 > $GLOBALFITS/outputDir/global/H3.log
echo running global h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H4.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 4 > $GLOBALFITS/outputDir/global/H4.log
echo running global h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H5.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 5 > $GLOBALFITS/outputDir/global/H5.log
echo running global h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H6.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 6 > $GLOBALFITS/outputDir/global/H6.log
echo running global h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H7.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 7 > $GLOBALFITS/outputDir/global/H7.log
echo running global h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H8.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 8 > $GLOBALFITS/outputDir/global/H8.log
echo running global h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H9.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 9 > $GLOBALFITS/outputDir/global/H9.log
echo running global h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global/H10.root $GLOBALFITS/inputs/global.txt $GLOBALFITS/inputs/global_covstat.txt $GLOBALFITS/inputs/global_covsyst.txt 10 > $GLOBALFITS/outputDir/global/H10.log
