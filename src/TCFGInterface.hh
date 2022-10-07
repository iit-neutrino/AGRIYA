///////////////////////////////////////////////////////////////////////
// Author: P T Surukuchi
// Date: Sep 03, 2016
// TODO:
// In the file, search for TODOs and EDITs:
//   TODO means this feature/detail has to be implemented
//   EDIT means this feature/detail/bug has to be edited
////////////////////////////////////////////////////////////////////////

#ifndef TCFGINTERFACE_HH
#define TCFGINTERFACE_HH

//Include standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <map>

//Include ROOT headers
#include "TString.h"

//OscSens Specific headers
#include "TCFGExtractor.hh"

///Interface calls between CFGExtractor and rest of the classes that use information from CFGs
/**
 *
 */
class TCFGInterface
{
public:
  
  /// Default destructor
  ~TCFGInterface(){};
  
  ///////////////////////////////////////////////////////////////////////
  //public attributes
  ///////////////////////////////////////////////////////////////////////
  
  /// Typedef to ease the use of the keyvalue map
  typedef std::map <TString,TString> key_value_map;
  
  ///////////////////////////////////////////////////////////////////////
  //public member functions
  ///////////////////////////////////////////////////////////////////////
  
  /// Function to initialize the singleton TCFGInterface instance and/or return the already created instance
  /*
   This is the only function that can actually initialize a TCFGInterface instance since the default constructor is made private to enforce singleton implementation
   */
  static TCFGInterface& Instance();
  
  /// Function to call the ExtractKeyValuePairs from the TCFGExtractor, needs to
  void Initialize(TString configFileName="");
  
  /// Retrive the values (2nd argument) for particular keys (1st value)
  /*
   For now, there are no checks to make sure that a key that corresponds to a praticular datatype (e.g integers) only have that particular type stored in the map
   */
  int RetrieveValue(TString, int&);
  int RetrieveValue(TString, double&);
  int RetrieveValue(TString, TString&);
  int RetrieveValue(TString, TObjArray&);
  
  /// Check to see if the string supplied as the second argument exists in a list with the key corresponding to the first argument
  /** Return 1 if the value exists and one if it doesn't
   */
  int CheckListForValue(TString, TString);
  
  /// Makes sure values can only be retreived after the map is actually saved
  void usage()
  {
    if (!isInitialized){
      printf("TCFGInterface::Initialize() needs to be called for the inputs from CFG to be used\n");
      exit(1);
    }
  }
  
  /// Make sure copy and assignment operators are deleted to enforce singleton implementation
  TCFGInterface(TCFGInterface const&) = delete;
  void operator=(TCFGInterface const&)  = delete;
  
private:
  
  /// Default constructoe
  TCFGInterface(){};
  
  bool isInitialized=false;
  
};

#endif
