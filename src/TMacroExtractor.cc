#include "TMacroExtractor.hh"

// Since it is a static member, declare it outside the class
std::map <TString,TString> TMacroExtractor::fKeyValuePairs;

TMacroExtractor& TMacroExtractor::Instance()
{
  /// TMacroExtractor object, made sure that there is only one such object
  static TMacroExtractor fMacroExtractor;
  return fMacroExtractor;
}

void TMacroExtractor::ExtractKeyValuePairs(TString macroFileName)
{
  if(!macroFileName.EndsWith(".mac")) usage();
  // read the input file
  std::ifstream inputFile(macroFileName.Data());
  //Check if the file is open
  if (!inputFile.is_open()) {
    printf("Error opening the file\n");
    exit(1);
  }
  bool isList=false;
  std::string line;
  TString key;
  TString value;
  while(std::getline(inputFile, line)){
    TString inputLine(line);
    inputLine.ReplaceAll(" ","");//remove any spaces
    if (inputLine.IsWhitespace()) continue;//skip if the line is empty
    if (inputLine.BeginsWith("#")) continue;// skip if the line starts with #
    
    if(inputLine.Contains("=")){// Check if this line contains '='
      TObjArray *objArr = inputLine.Tokenize("=");//Divide the line delimited by =
      if(objArr->GetEntries() != 2) usage(); // Make sure there are only 2 entries i.e., key and value
      key = ((TObjString*)objArr->At(0))->GetString();// Get the key
      key.ToUpper();// Convert to upper level to maintain consistency
      if (fKeyValuePairs.count(key)){
        printf("Key %s already exists\n",key.Data());
        printf("Please make sure to only include a key once\n");
        exit(1);
      }
      value = ((TObjString*)objArr->At(1))->GetString();// Get the value
      
      if(inputLine.Contains("{")){//Check if it is a list of items
        isList=true;
        value.ReplaceAll("{","");
        if(value.Contains("}")){// Check the list is specified in one line
          value.ReplaceAll("}","");
          fKeyValuePairs.insert(std::make_pair(key,value));// Save in the map since the list is specified in single line
          isList=false;
        }
      }
      else{
        if(isList){// The mistake case, i.e., if the user forgot to close the braces for a list
          printf("Please close all the braces in macro file for the right functionality\n");
          exit(1);
        }
        fKeyValuePairs.insert(std::make_pair(key,value));// Save in the map
      }
    }
    
    else{// This means that this line is part of a list
      if(inputLine.Contains("}")){ // When the list ends
        inputLine.ReplaceAll("}","");
        value.Append(inputLine);
        fKeyValuePairs.insert(std::make_pair(key,value));// Save in the map
        isList=false;
      }
      else value.Append(inputLine); // In case the list still continues
    }
  }
}

