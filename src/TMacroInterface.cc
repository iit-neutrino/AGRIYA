#include "TMacroInterface.hh"

//Create instance
TMacroInterface& TMacroInterface::Instance()
{
  static TMacroInterface fMacroInterface;
  return fMacroInterface;
}

void TMacroInterface::Initialize(TString configFileName)
{
  TMacroExtractor::Instance().ExtractKeyValuePairs(configFileName);
  isInitialized =true;
}

int TMacroInterface::RetrieveValue(TString key, int& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TMacroExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data())) // make sure there is value for corresponding key
  {
    TString valueString = KeyValuePairs[key.Data()];
    // save the value
    value = valueString.Atoi();
    return 1;
  }
  return 0;
}

int TMacroInterface::RetrieveValue(TString key, double& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TMacroExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    TString valueString = KeyValuePairs[key.Data()];
    value = valueString.Atof();
    return 1;
  }
  return 0;
}

int TMacroInterface::RetrieveValue(TString key, TString& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TMacroExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    value = KeyValuePairs[key.Data()];
    return 1;
  }
  return 0;
}

int TMacroInterface::RetrieveValue(TString key, TObjArray& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TMacroExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    TString list = KeyValuePairs[key.Data()];
    value = *(list.Tokenize(","));
    return 1;
  }
  return 0;
}

int TMacroInterface::CheckListForValue(TString key,TString value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TMacroExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    TString list = KeyValuePairs[key.Data()];
    TObjArray *listItems = list.Tokenize(",");
    for(int i=0;i<listItems->GetEntries();i++){
      TString  item = ((TObjString*)listItems->At(i))->GetString();
      if(item.CompareTo(value.Data(),TString::kIgnoreCase)==0) return 1;
    }
  }
  return 0;
}
