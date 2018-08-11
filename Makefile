##################################################################################
## Author: P T Surukuchi
## Date: Dec, 2016
## Makefile for global analysis
## 
##################################################################################

## Avoids trouble on systems where the SHELL variable is inherited from the environment
SHELL = /bin/sh

## Clear suffix lists
.SUFFIXES:
## Apply implicit rules on the following suffixes
.SUFFIXES: .cc .o

## C++ flags to be used, 
## O3: Extreme optimization (performance and code size) at expense of compile time
##	and debuggability. Can also use O2 or O1 based on what you want
## --std=c++11 : Use c++11 
## -fPIC: Generate position-independent code (PIC) suitable for use in a shared library
## `root-config --cflags` : Flags for compiling a source file against the ROOT header files.
## -Wpedantic : Issue all the warnings demanded by strict ISO C and ISO C++.
## -Wno-vla-extension : Suppress warnings related to variable length arrays (VLAs are supported for C99 and over)
## -Wall, -Wextra : Enforces strict compile-time warnings
## -g : Request gcc to emit extra information for debugging
##	 In case of debugging, replace -O3 with -Og for optimizing debugging
CXXFLAGS = -Ofast -std=c++0x -fPIC `root-config --cflags` -Wpedantic -Wno-vla-extension -Wall -Wextra

## root-config --libs : Use root-related libraries
LDFLAGS = `root-config --libs ` -L./

## Include locations to search for source files
INC = -I./ -I./PTS_Files

## Search in the source directory for .cc and .hh files
VPATH = ./ 

## Directory containing .o files
OBJDIR = ./

## Directory containing .o files
LIBDIR = ./

## Objects (saved in ./) of files used by the executables
OBJS = $(addprefix $(OBJDIR)/, GlobalAnalyzer.o Global_Utilities.o DayaBayFit.o)

## Oscillation Sensitivity library
LIBGLOB = $(LIBDIR)/libGlobalAnalysis.a

## Generate library using the object files
$(LIBGLOB): $(OBJS)
	ar rs $@ $(OBJS)

## Create .o files in the OBJDIR 
$(OBJDIR)/%.o : $(VPATH)/%.cc $(VPATH)/%.hh
	$(CXX) $(CXXFLAGS) $< -c -o $@

## All the executable files generated separately ($ make executable) 
% : %.cc $(LIBGLOB)
	$(CXX) $(CXXFLAGS) $(INC) $<  $(LIBGLOB) $(LDFLAGS) -o $@

## All the executable files generated separately ($ make executable) 
% : ./PTS_Files/%.cc $(LIBGLOB)
	$(CXX) $(CXXFLAGS) $(INC) $< $(LDFLAGS) $(LIBGLOB) -o $@

## Remove .o, .a files
.PHONY: clean
clean:
	-rm -rf $(OBJS) $(LIBGLOB)
