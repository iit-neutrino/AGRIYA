#!/bin/bash

source ~/Default_Startup

echo running DYB_ImprovedDE h1
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H1.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 1 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H1.log
echo running DYB_ImprovedDE h2
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H2.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 2 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H2.log
echo running DYB_ImprovedDE h3
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H3.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 3 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H3.log
echo running DYB_ImprovedDE h4
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H4.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 4 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H4.log
echo running DYB_ImprovedDE h5
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H5.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 5 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H5.log
echo running DYB_ImprovedDE h6
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H6.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 6 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H6.log
echo running DYB_ImprovedDE h7
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H7.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 7 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H7.log
echo running DYB_ImprovedDE h8
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H8.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 8 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H8.log
echo running DYB_ImprovedDE h9
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H9.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 9 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H9.log
echo running DYB_ImprovedDE h10
$GLOBALFITS/analyzeGlobalData $GLOBALFITS/outputDir/DYB_ImprovedDE/H10.root $GLOBALFITS/inputs/DYB_ImprovedDE.txt $GLOBALFITS/inputs/DYB_covstat.txt $GLOBALFITS/inputs/DYB_ImprovedDE_covsyst.txt 10 > $GLOBALFITS/outputDir/DYB_ImprovedDE/H10.log
