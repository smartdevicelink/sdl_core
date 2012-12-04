#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/UpdateMode.h"
#include "UpdateModeMarshaller.h"
#include "UpdateModeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const UpdateMode::UpdateModeInternal UpdateModeMarshaller::getIndex(const char* s)
{
  if(!s)
    return UpdateMode::INVALID_ENUM;
  const struct PerfectHashTable* p=UpdateMode_intHash::getPointer(s,strlen(s));
  return p ? static_cast<UpdateMode::UpdateModeInternal>(p->idx) : UpdateMode::INVALID_ENUM;
}


bool UpdateModeMarshaller::fromJSON(const Json::Value& s,UpdateMode& e)
{
  e.mInternal=UpdateMode::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=UpdateMode::INVALID_ENUM);
}


Json::Value UpdateModeMarshaller::toJSON(const UpdateMode& e)
{
  if(e.mInternal==UpdateMode::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool UpdateModeMarshaller::fromString(const std::string& s,UpdateMode& e)
{
  e.mInternal=UpdateMode::INVALID_ENUM;
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

const std::string UpdateModeMarshaller::toString(const UpdateMode& e)
{
  Json::FastWriter writer;
  return e.mInternal==UpdateMode::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable UpdateModeMarshaller::mHashTable[5]=
{
  {"COUNTUP",0},
  {"COUNTDOWN",1},
  {"PAUSE",2},
  {"RESUME",3},
  {"CLEAR",4}
};
