#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/SystemAction.h"
#include "SystemActionMarshaller.h"
#include "SystemActionMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const SystemAction::SystemActionInternal SystemActionMarshaller::getIndex(const char* s)
{
  if(!s)
    return SystemAction::INVALID_ENUM;
  const struct PerfectHashTable* p=SystemAction_intHash::getPointer(s,strlen(s));
  return p ? static_cast<SystemAction::SystemActionInternal>(p->idx) : SystemAction::INVALID_ENUM;
}


bool SystemActionMarshaller::fromJSON(const Json::Value& s,SystemAction& e)
{
  e.mInternal=SystemAction::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=SystemAction::INVALID_ENUM);
}


Json::Value SystemActionMarshaller::toJSON(const SystemAction& e)
{
  if(e.mInternal==SystemAction::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool SystemActionMarshaller::fromString(const std::string& s,SystemAction& e)
{
  e.mInternal=SystemAction::INVALID_ENUM;
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

const std::string SystemActionMarshaller::toString(const SystemAction& e)
{
  Json::FastWriter writer;
  return e.mInternal==SystemAction::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable SystemActionMarshaller::mHashTable[3]=
{
  {"DEFAULT_ACTION",0},
  {"STEAL_FOCUS",1},
  {"KEEP_CONTEXT",2}
};
