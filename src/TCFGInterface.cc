#include "TCFGInterface.hh"

//Create instance
TCFGInterface& TCFGInterface::Instance()
{
  static TCFGInterface fCFGInterface;
  return fCFGInterface;
}

void TCFGInterface::Initialize(TString configFileName)
{
  TCFGExtractor::Instance().ExtractKeyValuePairs(configFileName);
  isInitialized =true;
}

int TCFGInterface::RetrieveValue(TString key, int& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TCFGExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data())) // make sure there is value for corresponding key
  {
    TString valueString = KeyValuePairs[key.Data()];
    // save the value
    value = valueString.Atoi();
    return 1;
  }
  return 0;
}

int TCFGInterface::RetrieveValue(TString key, double& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TCFGExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    TString valueString = KeyValuePairs[key.Data()];
    value = valueString.Atof();
    return 1;
  }
  return 0;
}

int TCFGInterface::RetrieveValue(TString key, TString& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TCFGExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    value = KeyValuePairs[key.Data()];
    return 1;
  }
  return 0;
}

int TCFGInterface::RetrieveValue(TString key, TObjArray& value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TCFGExtractor::Instance().GetKeyValueMap();
  if(KeyValuePairs.count(key.Data()))
  {
    TString list = KeyValuePairs[key.Data()];
    value = *(list.Tokenize(","));
    return 1;
  }
  return 0;
}

int TCFGInterface::CheckListForValue(TString key,TString value)
{
  key.ToUpper();
  usage();
  key_value_map KeyValuePairs = TCFGExtractor::Instance().GetKeyValueMap();
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

