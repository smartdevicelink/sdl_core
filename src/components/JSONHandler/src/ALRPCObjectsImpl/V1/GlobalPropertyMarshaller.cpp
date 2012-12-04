#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V1/GlobalProperty.h"
#include "GlobalPropertyMarshaller.h"
#include "GlobalPropertyMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;


const GlobalProperty::GlobalPropertyInternal GlobalPropertyMarshaller::getIndex(const char* s)
{
  if(!s)
    return GlobalProperty::INVALID_ENUM;
  const struct PerfectHashTable* p=GlobalProperty_intHash::getPointer(s,strlen(s));
  return p ? static_cast<GlobalProperty::GlobalPropertyInternal>(p->idx) : GlobalProperty::INVALID_ENUM;
}


bool GlobalPropertyMarshaller::fromJSON(const Json::Value& s,GlobalProperty& e)
{
  e.mInternal=GlobalProperty::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=GlobalProperty::INVALID_ENUM);
}


Json::Value GlobalPropertyMarshaller::toJSON(const GlobalProperty& e)
{
  if(e.mInternal==GlobalProperty::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool GlobalPropertyMarshaller::fromString(const std::string& s,GlobalProperty& e)
{
  e.mInternal=GlobalProperty::INVALID_ENUM;
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

const std::string GlobalPropertyMarshaller::toString(const GlobalProperty& e)
{
  Json::FastWriter writer;
  return e.mInternal==GlobalProperty::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable GlobalPropertyMarshaller::mHashTable[2]=
{
  {"HELPPROMPT",0},
  {"TIMEOUTPROMPT",1}
};
