#!/bin/bash

source ~/Default_Startup

echo running DYB_ h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H1.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 1 > $GLOBALFITS/outputDir/DYB_Old/H1.log
echo running DYB_ h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H2.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 2 > $GLOBALFITS/outputDir/DYB_Old/H2.log
echo running DYB_ h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H3.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 3 > $GLOBALFITS/outputDir/DYB_Old/H3.log
echo running DYB_ h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H4.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 4 > $GLOBALFITS/outputDir/DYB_Old/H4.log
echo running DYB_ h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H5.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 5 > $GLOBALFITS/outputDir/DYB_Old/H5.log
echo running DYB_ h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H6.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 6 > $GLOBALFITS/outputDir/DYB_Old/H6.log
echo running DYB_ h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H7.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 7 > $GLOBALFITS/outputDir/DYB_Old/H7.log
echo running DYB_ h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H8.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 8 > $GLOBALFITS/outputDir/DYB_Old/H8.log
echo running DYB_ h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H9.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 9 > $GLOBALFITS/outputDir/DYB_Old/H9.log
echo running DYB_ h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_Old/H10.root $GLOBALFITS/inputs/DYB.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_covsyst.txt 10 > $GLOBALFITS/outputDir/DYB_Old/H10.log
