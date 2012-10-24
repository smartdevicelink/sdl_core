#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/ButtonPressMode.h"
#include "ButtonPressModeMarshaller.h"
#include "ButtonPressModeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



const ButtonPressMode::ButtonPressModeInternal ButtonPressModeMarshaller::getIndex(const char* s)
{
  if(!s)
    return ButtonPressMode::INVALID_ENUM;
  const struct PerfectHashTable* p=ButtonPressMode_intHash::getPointer(s,strlen(s));
  return p ? static_cast<ButtonPressMode::ButtonPressModeInternal>(p->idx) : ButtonPressMode::INVALID_ENUM;
}


bool ButtonPressModeMarshaller::fromJSON(const Json::Value& s,ButtonPressMode& e)
{
  e.mInternal=ButtonPressMode::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=ButtonPressMode::INVALID_ENUM);
}


Json::Value ButtonPressModeMarshaller::toJSON(const ButtonPressMode& e)
{
  if(e.mInternal==ButtonPressMode::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ButtonPressModeMarshaller::fromString(const std::string& s,ButtonPressMode& e)
{
  e.mInternal=ButtonPressMode::INVALID_ENUM;
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

const std::string ButtonPressModeMarshaller::toString(const ButtonPressMode& e)
{
  Json::FastWriter writer;
  return e.mInternal==ButtonPressMode::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ButtonPressModeMarshaller::mHashTable[2]=
{
  {"LONG",0},
  {"SHORT",1}
};
