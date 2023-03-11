cmake_minimum_required(VERSION 3.5.1)
project( global_fits VERSION 1.0
                     LANGUAGES CXX)

find_package(ROOT REQUIRED)

add_library(GlobalAnalysis SHARED 
                        src/GlobalAnalyzer.cc src/GlobalAnalyzer.hh
                        src/StyleFile.cc src/StyleFile.hh
                        src/TCFGExtractor.cc src/TCFGExtractor.hh
                        src/TCFGInterface.cc src/TCFGInterface.hh)
target_link_libraries(GlobalAnalysis ROOT::Core ROOT::Matrix ROOT::Graf)
set_target_properties(GlobalAnalysis PROPERTIES SUFFIX ".so")
