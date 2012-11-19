#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/HmiZoneCapabilities.h"
#include "HmiZoneCapabilitiesMarshaller.h"
#include "HmiZoneCapabilitiesMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 06:40:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const HmiZoneCapabilities::HmiZoneCapabilitiesInternal HmiZoneCapabilitiesMarshaller::getIndex(const char* s)
{
  if(!s)
    return HmiZoneCapabilities::INVALID_ENUM;
  const struct PerfectHashTable* p=HmiZoneCapabilities_intHash::getPointer(s,strlen(s));
  return p ? static_cast<HmiZoneCapabilities::HmiZoneCapabilitiesInternal>(p->idx) : HmiZoneCapabilities::INVALID_ENUM;
}


bool HmiZoneCapabilitiesMarshaller::fromJSON(const Json::Value& s,HmiZoneCapabilities& e)
{
  e.mInternal=HmiZoneCapabilities::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=HmiZoneCapabilities::INVALID_ENUM);
}


Json::Value HmiZoneCapabilitiesMarshaller::toJSON(const HmiZoneCapabilities& e)
{
  if(e.mInternal==HmiZoneCapabilities::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool HmiZoneCapabilitiesMarshaller::fromString(const std::string& s,HmiZoneCapabilities& e)
{
  e.mInternal=HmiZoneCapabilities::INVALID_ENUM;
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

const std::string HmiZoneCapabilitiesMarshaller::toString(const HmiZoneCapabilities& e)
{
  Json::FastWriter writer;
  return e.mInternal==HmiZoneCapabilities::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable HmiZoneCapabilitiesMarshaller::mHashTable[2]=
{
  {"FRONT",0},
  {"BACK",1}
};
