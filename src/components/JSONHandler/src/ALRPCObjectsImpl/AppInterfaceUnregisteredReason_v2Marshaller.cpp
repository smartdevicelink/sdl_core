#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason_v2.h"
#include "AppInterfaceUnregisteredReason_v2Marshaller.h"
#include "AppInterfaceUnregisteredReason_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const AppInterfaceUnregisteredReason_v2::AppInterfaceUnregisteredReason_v2Internal AppInterfaceUnregisteredReason_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return AppInterfaceUnregisteredReason_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=AppInterfaceUnregisteredReason_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AppInterfaceUnregisteredReason_v2::AppInterfaceUnregisteredReason_v2Internal>(p->idx) : AppInterfaceUnregisteredReason_v2::INVALID_ENUM;
}


bool AppInterfaceUnregisteredReason_v2Marshaller::fromJSON(const Json::Value& s,AppInterfaceUnregisteredReason_v2& e)
{
  e.mInternal=AppInterfaceUnregisteredReason_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AppInterfaceUnregisteredReason_v2::INVALID_ENUM);
}


Json::Value AppInterfaceUnregisteredReason_v2Marshaller::toJSON(const AppInterfaceUnregisteredReason_v2& e)
{
  if(e.mInternal==AppInterfaceUnregisteredReason_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AppInterfaceUnregisteredReason_v2Marshaller::fromString(const std::string& s,AppInterfaceUnregisteredReason_v2& e)
{
  e.mInternal=AppInterfaceUnregisteredReason_v2::INVALID_ENUM;
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

const std::string AppInterfaceUnregisteredReason_v2Marshaller::toString(const AppInterfaceUnregisteredReason_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==AppInterfaceUnregisteredReason_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AppInterfaceUnregisteredReason_v2Marshaller::mHashTable[11]=
{
  {"USER_EXIT",0},
  {"IGNITION_OFF",1},
  {"BLUETOOTH_OFF",2},
  {"USB_DISCONNECTED",3},
  {"REQUEST_WHILE_IN_NONE_HMI_LEVEL",4},
  {"TOO_MANY_REQUESTS",5},
  {"DRIVER_DISTRACTION_VIOLATION",6},
  {"LANGUAGE_CHANGE",7},
  {"MASTER_RESET",8},
  {"FACTORY_DEFAULTS",9},
  {"APP_UNAUTHORIZED",10}
};
