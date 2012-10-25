#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/SingleTirePressureStatus.h"
#include "SingleTirePressureStatusMarshaller.h"
#include "SingleTirePressureStatusMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



const SingleTirePressureStatus::SingleTirePressureStatusInternal SingleTirePressureStatusMarshaller::getIndex(const char* s)
{
  if(!s)
    return SingleTirePressureStatus::INVALID_ENUM;
  const struct PerfectHashTable* p=SingleTirePressureStatus_intHash::getPointer(s,strlen(s));
  return p ? static_cast<SingleTirePressureStatus::SingleTirePressureStatusInternal>(p->idx) : SingleTirePressureStatus::INVALID_ENUM;
}


bool SingleTirePressureStatusMarshaller::fromJSON(const Json::Value& s,SingleTirePressureStatus& e)
{
  e.mInternal=SingleTirePressureStatus::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=SingleTirePressureStatus::INVALID_ENUM);
}


Json::Value SingleTirePressureStatusMarshaller::toJSON(const SingleTirePressureStatus& e)
{
  if(e.mInternal==SingleTirePressureStatus::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool SingleTirePressureStatusMarshaller::fromString(const std::string& s,SingleTirePressureStatus& e)
{
  e.mInternal=SingleTirePressureStatus::INVALID_ENUM;
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

const std::string SingleTirePressureStatusMarshaller::toString(const SingleTirePressureStatus& e)
{
  Json::FastWriter writer;
  return e.mInternal==SingleTirePressureStatus::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable SingleTirePressureStatusMarshaller::mHashTable[5]=
{
  {"UNKNOWN",0},
  {"NORMAL",1},
  {"LOW",2},
  {"FAULT",3},
  {"NOT_SUPPORTED",4}
};
