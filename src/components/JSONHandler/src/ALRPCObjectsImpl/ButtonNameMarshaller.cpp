#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/ButtonName.h"
#include "ButtonNameMarshaller.h"
#include "ButtonNameMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 04:31:05 2012
  source stamp	Wed Oct 24 14:57:16 2012
  author	robok0der
*/



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

const PerfectHashTable ButtonNameMarshaller::mHashTable[15]=
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
  {"PRESET_9",14}
};
