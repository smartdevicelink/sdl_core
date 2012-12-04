#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/SystemContext.h"
#include "SystemContextMarshaller.h"
#include "SystemContextMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const SystemContext::SystemContextInternal SystemContextMarshaller::getIndex(const char* s)
{
  if(!s)
    return SystemContext::INVALID_ENUM;
  const struct PerfectHashTable* p=SystemContext_intHash::getPointer(s,strlen(s));
  return p ? static_cast<SystemContext::SystemContextInternal>(p->idx) : SystemContext::INVALID_ENUM;
}


bool SystemContextMarshaller::fromJSON(const Json::Value& s,SystemContext& e)
{
  e.mInternal=SystemContext::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=SystemContext::INVALID_ENUM);
}


Json::Value SystemContextMarshaller::toJSON(const SystemContext& e)
{
  if(e.mInternal==SystemContext::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool SystemContextMarshaller::fromString(const std::string& s,SystemContext& e)
{
  e.mInternal=SystemContext::INVALID_ENUM;
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

const std::string SystemContextMarshaller::toString(const SystemContext& e)
{
  Json::FastWriter writer;
  return e.mInternal==SystemContext::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable SystemContextMarshaller::mHashTable[5]=
{
  {"MAIN",0},
  {"VRSESSION",1},
  {"MENU",2},
  {"HMI_OBSCURED",3},
  {"ALERT",4}
};
