#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/PermissionStatus.h"
#include "PermissionStatusMarshaller.h"
#include "PermissionStatusMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



const PermissionStatus::PermissionStatusInternal PermissionStatusMarshaller::getIndex(const char* s)
{
  if(!s)
    return PermissionStatus::INVALID_ENUM;
  const struct PerfectHashTable* p=PermissionStatus_intHash::getPointer(s,strlen(s));
  return p ? static_cast<PermissionStatus::PermissionStatusInternal>(p->idx) : PermissionStatus::INVALID_ENUM;
}


bool PermissionStatusMarshaller::fromJSON(const Json::Value& s,PermissionStatus& e)
{
  e.mInternal=PermissionStatus::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=PermissionStatus::INVALID_ENUM);
}


Json::Value PermissionStatusMarshaller::toJSON(const PermissionStatus& e)
{
  if(e.mInternal==PermissionStatus::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool PermissionStatusMarshaller::fromString(const std::string& s,PermissionStatus& e)
{
  e.mInternal=PermissionStatus::INVALID_ENUM;
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

const std::string PermissionStatusMarshaller::toString(const PermissionStatus& e)
{
  Json::FastWriter writer;
  return e.mInternal==PermissionStatus::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable PermissionStatusMarshaller::mHashTable[4]=
{
  {"ALLOWED",0},
  {"DISALLOWED",1},
  {"USER_DISALLOWED",2},
  {"USER_CONSENT_PENDING",3}
};
