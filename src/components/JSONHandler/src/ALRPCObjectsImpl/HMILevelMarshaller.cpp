#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/HMILevel.h"
#include "HMILevelMarshaller.h"
#include "HMILevelMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



const HMILevel::HMILevelInternal HMILevelMarshaller::getIndex(const char* s)
{
  if(!s)
    return HMILevel::INVALID_ENUM;
  const struct PerfectHashTable* p=HMILevel_intHash::getPointer(s,strlen(s));
  return p ? static_cast<HMILevel::HMILevelInternal>(p->idx) : HMILevel::INVALID_ENUM;
}


bool HMILevelMarshaller::fromJSON(const Json::Value& s,HMILevel& e)
{
  e.mInternal=HMILevel::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=HMILevel::INVALID_ENUM);
}


Json::Value HMILevelMarshaller::toJSON(const HMILevel& e)
{
  if(e.mInternal==HMILevel::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool HMILevelMarshaller::fromString(const std::string& s,HMILevel& e)
{
  e.mInternal=HMILevel::INVALID_ENUM;
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

const std::string HMILevelMarshaller::toString(const HMILevel& e)
{
  Json::FastWriter writer;
  return e.mInternal==HMILevel::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable HMILevelMarshaller::mHashTable[4]=
{
  {"FULL",0},
  {"LIMITED",1},
  {"BACKGROUND",2},
  {"NONE",3}
};
