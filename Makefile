##################################################################################
## Author: P T Surukuchi
## Date: Dec, 2016
## Makefile for global analysis
## 
##################################################################################

.PHONY: all clean

GIT_HASH=`git rev-parse HEAD`
COMPILE_TIME=`date -u +'%Y-%m-%d %H:%M:%S UTC'`
GIT_BRANCH=`git branch | grep "^\*" | sed 's/^..//'`
export VERSION_FLAGS=-DGIT_HASH="\"$(GIT_HASH)\"" -DCOMPILE_TIME="\"$(COMPILE_TIME)\"" -DGIT_BRANCH="\"$(GIT_BRANCH)\""

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
INC = -I./ -I./src

## Search in the source directory for .cc and .hh files
VPATH = ./src

## Directory containing .o files
OBJDIR = ./obj

## Directory containing .o files
LIBDIR = ./lib

## Objects (saved in ./) of files used by the executables
OBJS = $(addprefix $(OBJDIR)/, GlobalAnalyzer.o StyleFile.o TMacroInterface.o TMacroExtractor.o)

## Oscillation Sensitivity library
LIBGLOB = $(LIBDIR)/libGlobalAnalysis.so

## Generate library using the object files
$(LIBGLOB): $(OBJS)
	ar rs $@ $(OBJS)

## Create .o files in the OBJDIR 
$(OBJDIR)/%.o : $(VPATH)/%.cc $(VPATH)/%.hh
	$(CXX) $(CXXFLAGS) $< -c -o $@

## All the executable files generated separately ($ make executable) 
% : %.cc $(LIBGLOB)
	$(CXX) $(CXXFLAGS) $(INC) $(VERSION_FLAGS) $<  $(LIBGLOB) $(LDFLAGS) -o $@

% : ./scripts/%.cc $(LIBGLOB)
	$(CXX) $(CXXFLAGS) $(INC) $(VERSION_FLAGS) $<  $(LIBGLOB) $(LDFLAGS) -o $@


## Make sure to check if obj directory exists and create if it doesn’t
$(OBJS): | $(OBJDIR)
$(OBJDIR):
	mkdir $(OBJDIR)

## Make sure to check if lib directory exists and create if it doesn’t
$(LIBGLOB): | $(LIBDIR)
$(LIBDIR):
	mkdir $(LIBDIR)

## Remove .o, .a files
clean:
	-rm -rf $(OBJS) $(LIBGLOB)
# all:
#     g++ main.cpp $(VERSION_FLAGS)
