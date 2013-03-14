#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/DeactivateReason.h"
#include "DeactivateReasonMarshaller.h"
#include "DeactivateReasonMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsSmartDeviceLinkRPCV2;


const DeactivateReason::DeactivateReasonInternal DeactivateReasonMarshaller::getIndex(const char* s)
{
  if(!s)
    return DeactivateReason::INVALID_ENUM;
  const struct PerfectHashTable* p=DeactivateReason_intHash::getPointer(s,strlen(s));
  return p ? static_cast<DeactivateReason::DeactivateReasonInternal>(p->idx) : DeactivateReason::INVALID_ENUM;
}


bool DeactivateReasonMarshaller::fromJSON(const Json::Value& s,DeactivateReason& e)
{
  e.mInternal=DeactivateReason::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=DeactivateReason::INVALID_ENUM);
}


Json::Value DeactivateReasonMarshaller::toJSON(const DeactivateReason& e)
{
  if(e.mInternal==DeactivateReason::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool DeactivateReasonMarshaller::fromString(const std::string& s,DeactivateReason& e)
{
  e.mInternal=DeactivateReason::INVALID_ENUM;
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

const std::string DeactivateReasonMarshaller::toString(const DeactivateReason& e)
{
  Json::FastWriter writer;
  return e.mInternal==DeactivateReason::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable DeactivateReasonMarshaller::mHashTable[6]=
{
  {"AUDIO",0},
  {"PHONECALL",1},
  {"NAVIGATIONMAP",2},
  {"PHONEMENU",3},
  {"SYNCSETTINGS",4},
  {"GENERAL",5}
};
