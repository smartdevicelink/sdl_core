//
// Copyright (c) 2013 Ford Motor Company
//

#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/AppType.h"
#include "AppTypeMarshaller.h"
#include "AppTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


const AppType::AppTypeInternal AppTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return AppType::INVALID_ENUM;
  const struct PerfectHashTable* p=AppType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AppType::AppTypeInternal>(p->idx) : AppType::INVALID_ENUM;
}


bool AppTypeMarshaller::fromJSON(const Json::Value& s,AppType& e)
{
  e.mInternal=AppType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AppType::INVALID_ENUM);
}


Json::Value AppTypeMarshaller::toJSON(const AppType& e)
{
  if(e.mInternal==AppType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AppTypeMarshaller::fromString(const std::string& s,AppType& e)
{
  e.mInternal=AppType::INVALID_ENUM;
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

const std::string AppTypeMarshaller::toString(const AppType& e)
{
  Json::FastWriter writer;
  return e.mInternal==AppType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AppTypeMarshaller::mHashTable[10]=
{
  {"DEFAULT",0},
  {"COMMUNICATION",1},
  {"MEDIA",2},
  {"MESSAGING",3},
  {"NAVIGATION",4},
  {"INFORMATION",5},
  {"SOCIAL",6},
  {"BACKGROUND_PROCESS",7},
  {"TESTING",8},
  {"SYSTEM",9}
};
