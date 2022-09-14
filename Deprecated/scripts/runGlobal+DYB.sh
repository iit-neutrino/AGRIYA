#!/bin/bash

source ~/Default_Startup

echo running global+DYB h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H1.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 1 > $GLOBALFITS/outputDir/global+DYB/H1.log
echo running global+DYB h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H2.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 2 > $GLOBALFITS/outputDir/global+DYB/H2.log
echo running global+DYB h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H3.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 3 > $GLOBALFITS/outputDir/global+DYB/H3.log
echo running global+DYB h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H4.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 4 > $GLOBALFITS/outputDir/global+DYB/H4.log
echo running global+DYB h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H5.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 5 > $GLOBALFITS/outputDir/global+DYB/H5.log
echo running global+DYB h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H6.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 6 > $GLOBALFITS/outputDir/global+DYB/H6.log
echo running global+DYB h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H7.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 7 > $GLOBALFITS/outputDir/global+DYB/H7.log
echo running global+DYB h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H8.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 8 > $GLOBALFITS/outputDir/global+DYB/H8.log
echo running global+DYB h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H9.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 9 > $GLOBALFITS/outputDir/global+DYB/H9.log
echo running global+DYB h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/global+DYB/H10.root $GLOBALFITS/inputs/global+DYB.txt $GLOBALFITS/inputs/global+DYB_covstat.txt $GLOBALFITS/inputs/global+DYB_covsyst.txt 10 > $GLOBALFITS/outputDir/global+DYB/H10.log
