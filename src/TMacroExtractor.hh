///////////////////////////////////////////////////////////////////////
// Author: P T Surukuchi
// Date: Sep 02, 2016
// TODO:
// In the file, search for TODOs and EDITs:
//   TODO means this feature/detail has to be implemented
//   EDIT means this feature/detail/bug has to be edited
////////////////////////////////////////////////////////////////////////

#ifndef TMACROEXTRACTOR_HH
#define TMACROEXTRACTOR_HH

//Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <map>

//Include ROOT headers
#include "TString.h"
#include "TObjArray.h"
#include "TObjString.h"

///Class for extracting information from Macro file .mac
/**
 * Reads lines from macro file ending with .mac and stores the key-value pairs in map. Each line in the macro file needs to have a key-value pair delimited by '='.
 * A user can also specicy a list by specifying the list in braces and items in the list delimited by a ',', like so, <br>
 * list = {a,b,c}<br>
 * The list can also span over multiple lines like so,<br>
 * list = {a, b <br>
 * c,
 * d
 * }
 */
class TMacroExtractor
{
public:
  
  /// Default destructor
  ~TMacroExtractor(){};
  
  ///////////////////////////////////////////////////////////////////////
  //public attributes
  ///////////////////////////////////////////////////////////////////////
  
  typedef std::map <TString,TString> key_value_map;

  ///////////////////////////////////////////////////////////////////////
  //public member functions
  ///////////////////////////////////////////////////////////////////////
  
  /// Function to initialize the singleton TMacroExtractor instance and/or return the already created instance
  /*
   This is the only function that can actually initialize a TMacroExtractor object since the default constructor is made private to enforce singleton implementation
   */
  static TMacroExtractor& Instance();
  
  /// Extract the key value pairs from the input macro file, the name is provided by the argument
  void ExtractKeyValuePairs(TString);
  
  /// Since the key-value map has been defined private, this is a getter function for the map
  static const key_value_map& GetKeyValueMap()  {return fKeyValuePairs;}
  
  /// Make sure copy and assignment operators are deleted to enforce singleton implementation
  TMacroExtractor(TMacroExtractor const&)= delete;
  void operator=(TMacroExtractor const&) = delete;
  
private:
  
  /// Default constructor
  /*
   This constructor cannot be called directly, it is called from initialize() function
   */
  TMacroExtractor(){};
  
  /// The map that stores the inputs from macros as key-value pairs
  static key_value_map fKeyValuePairs;
  
  void usage()
  {
    printf("The macro file must end with .mac\n");
    printf("Each line in the macro file needs to have a key and value pair delimited by '='\n");
    exit(1);
  }
  
};

#endif
