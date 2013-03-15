//
// Copyright (c) 2013 Ford Motor Company
//

#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/ButtonName.h"
#include "ButtonNameMarshaller.h"
#include "ButtonNameMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


const ButtonName::ButtonNameInternal ButtonNameMarshaller::getIndex(const char* s)
{
  if(!s)
    return ButtonName::INVALID_ENUM;
  const struct PerfectHashTable* p=ButtonName_intHash::getPointer(s,strlen(s));
  return p ? static_cast<ButtonName::ButtonNameInternal>(p->idx) : ButtonName::INVALID_ENUM;
}


bool ButtonNameMarshaller::fromJSON(const Json::Value& s,ButtonName& e)
{
  e.mInternal=ButtonName::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=ButtonName::INVALID_ENUM);
}


Json::Value ButtonNameMarshaller::toJSON(const ButtonName& e)
{
  if(e.mInternal==ButtonName::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool ButtonNameMarshaller::fromString(const std::string& s,ButtonName& e)
{
  e.mInternal=ButtonName::INVALID_ENUM;
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

const std::string ButtonNameMarshaller::toString(const ButtonName& e)
{
  Json::FastWriter writer;
  return e.mInternal==ButtonName::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable ButtonNameMarshaller::mHashTable[16]=
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
