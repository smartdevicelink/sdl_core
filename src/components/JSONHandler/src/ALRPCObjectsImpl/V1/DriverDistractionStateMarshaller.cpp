#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V1/DriverDistractionState.h"
#include "DriverDistractionStateMarshaller.h"
#include "DriverDistractionStateMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const DriverDistractionState::DriverDistractionStateInternal DriverDistractionStateMarshaller::getIndex(const char* s)
{
  if(!s)
    return DriverDistractionState::INVALID_ENUM;
  const struct PerfectHashTable* p=DriverDistractionState_intHash::getPointer(s,strlen(s));
  return p ? static_cast<DriverDistractionState::DriverDistractionStateInternal>(p->idx) : DriverDistractionState::INVALID_ENUM;
}


bool DriverDistractionStateMarshaller::fromJSON(const Json::Value& s,DriverDistractionState& e)
{
  e.mInternal=DriverDistractionState::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=DriverDistractionState::INVALID_ENUM);
}


Json::Value DriverDistractionStateMarshaller::toJSON(const DriverDistractionState& e)
{
  if(e.mInternal==DriverDistractionState::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool DriverDistractionStateMarshaller::fromString(const std::string& s,DriverDistractionState& e)
{
  e.mInternal=DriverDistractionState::INVALID_ENUM;
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

const std::string DriverDistractionStateMarshaller::toString(const DriverDistractionState& e)
{
  Json::FastWriter writer;
  return e.mInternal==DriverDistractionState::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable DriverDistractionStateMarshaller::mHashTable[2]=
{
  {"DD_ON",0},
  {"DD_OFF",1}
};
