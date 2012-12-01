#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/UpdateMode_v2.h"
#include "UpdateMode_v2Marshaller.h"
#include "UpdateMode_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const UpdateMode_v2::UpdateMode_v2Internal UpdateMode_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return UpdateMode_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=UpdateMode_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<UpdateMode_v2::UpdateMode_v2Internal>(p->idx) : UpdateMode_v2::INVALID_ENUM;
}


bool UpdateMode_v2Marshaller::fromJSON(const Json::Value& s,UpdateMode_v2& e)
{
  e.mInternal=UpdateMode_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=UpdateMode_v2::INVALID_ENUM);
}


Json::Value UpdateMode_v2Marshaller::toJSON(const UpdateMode_v2& e)
{
  if(e.mInternal==UpdateMode_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool UpdateMode_v2Marshaller::fromString(const std::string& s,UpdateMode_v2& e)
{
  e.mInternal=UpdateMode_v2::INVALID_ENUM;
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

const std::string UpdateMode_v2Marshaller::toString(const UpdateMode_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==UpdateMode_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable UpdateMode_v2Marshaller::mHashTable[5]=
{
  {"COUNTUP",0},
  {"COUNTDOWN",1},
  {"PAUSE",2},
  {"RESUME",3},
  {"CLEAR",4}
};
