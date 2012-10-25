#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/TBTState.h"
#include "TBTStateMarshaller.h"
#include "TBTStateMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



const TBTState::TBTStateInternal TBTStateMarshaller::getIndex(const char* s)
{
  if(!s)
    return TBTState::INVALID_ENUM;
  const struct PerfectHashTable* p=TBTState_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TBTState::TBTStateInternal>(p->idx) : TBTState::INVALID_ENUM;
}


bool TBTStateMarshaller::fromJSON(const Json::Value& s,TBTState& e)
{
  e.mInternal=TBTState::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TBTState::INVALID_ENUM);
}


Json::Value TBTStateMarshaller::toJSON(const TBTState& e)
{
  if(e.mInternal==TBTState::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TBTStateMarshaller::fromString(const std::string& s,TBTState& e)
{
  e.mInternal=TBTState::INVALID_ENUM;
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

const std::string TBTStateMarshaller::toString(const TBTState& e)
{
  Json::FastWriter writer;
  return e.mInternal==TBTState::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TBTStateMarshaller::mHashTable[4]=
{
  {"ROUTE_UPDATE_REQUEST",0},
  {"ROUTE_ACCEPTED",1},
  {"ROUTE_REFUSED",2},
  {"ROUTE_CANCELLED",3}
};
