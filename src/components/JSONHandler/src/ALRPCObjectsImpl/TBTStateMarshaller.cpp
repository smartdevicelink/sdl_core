#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/TBTState.h"
#include "TBTStateMarshaller.h"
#include "TBTStateMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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

const PerfectHashTable TBTStateMarshaller::mHashTable[10]=
{
  {"ROUTE_UPDATE_REQUEST",0},
  {"ROUTE_ACCEPTED",1},
  {"ROUTE_REFUSED",2},
  {"ROUTE_CANCELLED",3},
  {"ETA_REQUEST",4},
  {"NEXT_TURN_REQUEST",5},
  {"ROUTE_STATUS_REQUEST",6},
  {"ROUTE_SUMMARY_REQUEST",7},
  {"TRIP_STATUS_REQUEST",8},
  {"ROUTE_UPDATE_REQUEST_TIMEOUT",9}
};
