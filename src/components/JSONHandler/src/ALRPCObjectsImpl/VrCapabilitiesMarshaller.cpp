#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/VrCapabilities.h"
#include "VrCapabilitiesMarshaller.h"
#include "VrCapabilitiesMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/



const VrCapabilities::VrCapabilitiesInternal VrCapabilitiesMarshaller::getIndex(const char* s)
{
  if(!s)
    return VrCapabilities::INVALID_ENUM;
  const struct PerfectHashTable* p=VrCapabilities_intHash::getPointer(s,strlen(s));
  return p ? static_cast<VrCapabilities::VrCapabilitiesInternal>(p->idx) : VrCapabilities::INVALID_ENUM;
}


bool VrCapabilitiesMarshaller::fromJSON(const Json::Value& s,VrCapabilities& e)
{
  e.mInternal=VrCapabilities::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=VrCapabilities::INVALID_ENUM);
}


Json::Value VrCapabilitiesMarshaller::toJSON(const VrCapabilities& e)
{
  if(e.mInternal==VrCapabilities::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool VrCapabilitiesMarshaller::fromString(const std::string& s,VrCapabilities& e)
{
  e.mInternal=VrCapabilities::INVALID_ENUM;
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

const std::string VrCapabilitiesMarshaller::toString(const VrCapabilities& e)
{
  Json::FastWriter writer;
  return e.mInternal==VrCapabilities::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable VrCapabilitiesMarshaller::mHashTable[1]=
{
  {"Text",0}
};
