#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/ButtonName_v2.h"
#include "ButtonName_v2Marshaller.h"
#include "ButtonName_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const ButtonName_v2::ButtonName_v2Internal ButtonName_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return ButtonName_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=ButtonName_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<ButtonName_v2::ButtonName_v2Internal>(p->idx) : ButtonName_v2::INVALID_ENUM;
}


bool ButtonName_v2Marshaller::fromJSON(const Json::Value& s,ButtonName_v2& e)
{
  e.mInternal=ButtonName_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=ButtonName_v2::INVALID_ENUM);
}


Json::Value ButtonName_v2Marshaller::toJSON(const ButtonName_v2& e)
{
  if(e.mInternal==ButtonName_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ButtonName_v2Marshaller::fromString(const std::string& s,ButtonName_v2& e)
{
  e.mInternal=ButtonName_v2::INVALID_ENUM;
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

const std::string ButtonName_v2Marshaller::toString(const ButtonName_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==ButtonName_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ButtonName_v2Marshaller::mHashTable[16]=
{
  {"OK",0},
  {"SEEKLEFT",1},
  {"SEEKRIGHT",2},
  {"TUNEUP",3},
  {"TUNEDOWN",4},
  {"PRESET_0",5},
  {"PRESET_1",6},
  {"PRESET_2",7},
  {"PRESET_3",8},
  {"PRESET_4",9},
  {"PRESET_5",10},
  {"PRESET_6",11},
  {"PRESET_7",12},
  {"PRESET_8",13},
  {"PRESET_9",14},
  {"CUSTOM_BUTTON",15}
};
