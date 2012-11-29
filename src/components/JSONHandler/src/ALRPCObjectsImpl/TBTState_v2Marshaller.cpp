#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/TBTState_v2.h"
#include "TBTState_v2Marshaller.h"
#include "TBTState_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const TBTState_v2::TBTState_v2Internal TBTState_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return TBTState_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=TBTState_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TBTState_v2::TBTState_v2Internal>(p->idx) : TBTState_v2::INVALID_ENUM;
}


bool TBTState_v2Marshaller::fromJSON(const Json::Value& s,TBTState_v2& e)
{
  e.mInternal=TBTState_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TBTState_v2::INVALID_ENUM);
}


Json::Value TBTState_v2Marshaller::toJSON(const TBTState_v2& e)
{
  if(e.mInternal==TBTState_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TBTState_v2Marshaller::fromString(const std::string& s,TBTState_v2& e)
{
  e.mInternal=TBTState_v2::INVALID_ENUM;
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

const std::string TBTState_v2Marshaller::toString(const TBTState_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==TBTState_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TBTState_v2Marshaller::mHashTable[10]=
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
