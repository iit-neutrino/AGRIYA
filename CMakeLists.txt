cmake_minimum_required(VERSION 3.5.1)
project( global_fits VERSION 1.0
                     LANGUAGES CXX)

find_package(ROOT REQUIRED)

#Custom functions for convenience
function(SCRIPT_GEN executable)
    add_executable(${executable} "scripts/${executable}.cc")
    target_link_libraries(${executable} ROOT::Core ROOT::Graf)
    target_include_directories(${executable} PRIVATE src)
endfunction()

#Getting the compile time information
execute_process(COMMAND git branch --show-current 
                COMMAND tr -d '\n'
                OUTPUT_VARIABLE git_branch)

execute_process(COMMAND git rev-parse HEAD
                COMMAND tr -d '\n'
                OUTPUT_VARIABLE git_hash)

execute_process(COMMAND date -u +'%Y-%m-%d\ %H:%M:%S\ UTC'
                COMMAND tr -d '\n'
                OUTPUT_VARIABLE compile_time)

#Custom Library for our use
add_library(GlobalAnalysis SHARED 
                        src/GlobalAnalyzer.cc src/GlobalAnalyzer.hh
                        src/StyleFile.cc src/StyleFile.hh
                        src/TCFGExtractor.cc src/TCFGExtractor.hh
                        src/TCFGInterface.cc src/TCFGInterface.hh)
target_link_libraries(GlobalAnalysis ROOT::Core ROOT::Matrix ROOT::Graf)
set_target_properties(GlobalAnalysis PROPERTIES SUFFIX ".so")

add_executable(analyzeGlobalData analyzeGlobalData.cc)
target_include_directories(analyzeGlobalData PRIVATE src)
target_link_libraries(analyzeGlobalData GlobalAnalysis)
target_compile_definitions(analyzeGlobalData PRIVATE
                                            COMPILE_TIME="${compile_time}"
                                            GIT_HASH="${git_hash}"
                                            GIT_BRANCH="${git_branch}")

add_executable(jsonParser src/JSONSerializer.cc src/JSONSerializer.hh)
target_link_libraries(jsonParser ROOT::Core ROOT::Matrix)

#Adding the scripts
file(GLOB scripts "scripts/*.cc")
foreach(filename ${scripts})
    get_filename_component(filename_wo_ext ${filename} NAME_WE)
    SCRIPT_GEN(${filename_wo_ext})
endforeach()

target_sources(plotGraphs PRIVATE src/StyleFile.hh)
target_link_libraries(plotGraphs GlobalAnalysis ROOT::Gpad)

target_sources(plotHypotheticalIBDYields PRIVATE src/GlobalAnalyzer.hh src/TCFGInterface.hh)
target_link_libraries(plotHypotheticalIBDYields GlobalAnalysis)

target_link_libraries(Pu240_parser ROOT::Gpad)