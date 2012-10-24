#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/DataType.h"
#include "DataTypeMarshaller.h"
#include "DataTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



const DataType::DataTypeInternal DataTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return DataType::INVALID_ENUM;
  const struct PerfectHashTable* p=DataType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<DataType::DataTypeInternal>(p->idx) : DataType::INVALID_ENUM;
}


bool DataTypeMarshaller::fromJSON(const Json::Value& s,DataType& e)
{
  e.mInternal=DataType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=DataType::INVALID_ENUM);
}


Json::Value DataTypeMarshaller::toJSON(const DataType& e)
{
  if(e.mInternal==DataType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool DataTypeMarshaller::fromString(const std::string& s,DataType& e)
{
  e.mInternal=DataType::INVALID_ENUM;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(fromJSON(json,e))  return true;
  }
  catch(...)
  {
    return false;
  }
  return false;
}

const std::string DataTypeMarshaller::toString(const DataType& e)
{
  Json::FastWriter writer;
  return e.mInternal==DataType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable DataTypeMarshaller::mHashTable[2]=
{
  {"HEX",0},
  {"BINARY",1}
};
