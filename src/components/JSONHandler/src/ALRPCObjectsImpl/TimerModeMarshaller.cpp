#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/TimerMode.h"
#include "TimerModeMarshaller.h"
#include "TimerModeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/



const TimerMode::TimerModeInternal TimerModeMarshaller::getIndex(const char* s)
{
  if(!s)
    return TimerMode::INVALID_ENUM;
  const struct PerfectHashTable* p=TimerMode_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TimerMode::TimerModeInternal>(p->idx) : TimerMode::INVALID_ENUM;
}


bool TimerModeMarshaller::fromJSON(const Json::Value& s,TimerMode& e)
{
  e.mInternal=TimerMode::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TimerMode::INVALID_ENUM);
}


Json::Value TimerModeMarshaller::toJSON(const TimerMode& e)
{
  if(e.mInternal==TimerMode::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TimerModeMarshaller::fromString(const std::string& s,TimerMode& e)
{
  e.mInternal=TimerMode::INVALID_ENUM;
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

const std::string TimerModeMarshaller::toString(const TimerMode& e)
{
  Json::FastWriter writer;
  return e.mInternal==TimerMode::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TimerModeMarshaller::mHashTable[3]=
{
  {"UP",0},
  {"DOWN",1},
  {"NONE",2}
};
