#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/Result.h"
#include "ResultMarshaller.h"
#include "ResultMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const Result::ResultInternal ResultMarshaller::getIndex(const char* s)
{
  if(!s)
    return Result::INVALID_ENUM;
  const struct PerfectHashTable* p=Result_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Result::ResultInternal>(p->idx) : Result::INVALID_ENUM;
}


bool ResultMarshaller::fromJSON(const Json::Value& s,Result& e)
{
  e.mInternal=Result::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=Result::INVALID_ENUM);
}


Json::Value ResultMarshaller::toJSON(const Result& e)
{
  if(e.mInternal==Result::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ResultMarshaller::fromString(const std::string& s,Result& e)
{
  e.mInternal=Result::INVALID_ENUM;
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

const std::string ResultMarshaller::toString(const Result& e)
{
  Json::FastWriter writer;
  return e.mInternal==Result::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ResultMarshaller::mHashTable[29]=
{
  {"SUCCESS",0},
  {"INVALID_DATA",1},
  {"UNSUPPORTED_REQUEST",2},
  {"OUT_OF_MEMORY",3},
  {"TOO_MANY_PENDING_REQUESTS",4},
  {"INVALID_ID",5},
  {"DUPLICATE_NAME",6},
  {"TOO_MANY_APPLICATIONS",7},
  {"APPLICATION_REGISTERED_ALREADY",8},
  {"UNSUPPORTED_VERSION",9},
  {"WRONG_LANGUAGE",10},
  {"APPLICATION_NOT_REGISTERED",11},
  {"IN_USE",12},
  {"SUBSCRIBED_ALREADY",13},
  {"VEHICLE_DATA_NOT_ALLOWED",14},
  {"VEHICLE_DATA_NOT_AVAILABLE",15},
  {"REJECTED",16},
  {"ABORTED",17},
  {"IGNORED",18},
  {"UNSUPPORTED_BUTTON",19},
  {"FILE_NOT_FOUND",20},
  {"GENERIC_ERROR",21},
  {"DISALLOWED",22},
  {"USER_DISALLOWED",23},
  {"TIMED_OUT",24},
  {"CANCEL_ROUTE",25},
  {"CLOSED",26},
  {"ENCRYPTED",27},
  {"WARNINGS",28}
};
