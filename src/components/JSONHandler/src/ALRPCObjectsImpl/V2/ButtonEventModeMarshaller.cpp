#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/ButtonEventMode.h"
#include "ButtonEventModeMarshaller.h"
#include "ButtonEventModeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const ButtonEventMode::ButtonEventModeInternal ButtonEventModeMarshaller::getIndex(const char* s)
{
  if(!s)
    return ButtonEventMode::INVALID_ENUM;
  const struct PerfectHashTable* p=ButtonEventMode_intHash::getPointer(s,strlen(s));
  return p ? static_cast<ButtonEventMode::ButtonEventModeInternal>(p->idx) : ButtonEventMode::INVALID_ENUM;
}


bool ButtonEventModeMarshaller::fromJSON(const Json::Value& s,ButtonEventMode& e)
{
  e.mInternal=ButtonEventMode::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=ButtonEventMode::INVALID_ENUM);
}


Json::Value ButtonEventModeMarshaller::toJSON(const ButtonEventMode& e)
{
  if(e.mInternal==ButtonEventMode::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ButtonEventModeMarshaller::fromString(const std::string& s,ButtonEventMode& e)
{
  e.mInternal=ButtonEventMode::INVALID_ENUM;
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

const std::string ButtonEventModeMarshaller::toString(const ButtonEventMode& e)
{
  Json::FastWriter writer;
  return e.mInternal==ButtonEventMode::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ButtonEventModeMarshaller::mHashTable[2]=
{
  {"BUTTONUP",0},
  {"BUTTONDOWN",1}
};
