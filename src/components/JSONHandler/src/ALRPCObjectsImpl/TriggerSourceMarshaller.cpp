#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/TriggerSource.h"
#include "TriggerSourceMarshaller.h"
#include "TriggerSourceMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 10:46:35 2012
  source stamp	Wed Oct 24 10:44:30 2012
  author	robok0der
*/



const TriggerSource::TriggerSourceInternal TriggerSourceMarshaller::getIndex(const char* s)
{
  if(!s)
    return TriggerSource::INVALID_ENUM;
  const struct PerfectHashTable* p=TriggerSource_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TriggerSource::TriggerSourceInternal>(p->idx) : TriggerSource::INVALID_ENUM;
}


bool TriggerSourceMarshaller::fromJSON(const Json::Value& s,TriggerSource& e)
{
  e.mInternal=TriggerSource::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TriggerSource::INVALID_ENUM);
}


Json::Value TriggerSourceMarshaller::toJSON(const TriggerSource& e)
{
  if(e.mInternal==TriggerSource::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TriggerSourceMarshaller::fromString(const std::string& s,TriggerSource& e)
{
  e.mInternal=TriggerSource::INVALID_ENUM;
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

const std::string TriggerSourceMarshaller::toString(const TriggerSource& e)
{
  Json::FastWriter writer;
  return e.mInternal==TriggerSource::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TriggerSourceMarshaller::mHashTable[2]=
{
  {"MENU",0},
  {"VR",1}
};
