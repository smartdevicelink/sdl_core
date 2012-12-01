#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/TextFieldName_v2.h"
#include "TextFieldName_v2Marshaller.h"
#include "TextFieldName_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const TextFieldName_v2::TextFieldName_v2Internal TextFieldName_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return TextFieldName_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=TextFieldName_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TextFieldName_v2::TextFieldName_v2Internal>(p->idx) : TextFieldName_v2::INVALID_ENUM;
}


bool TextFieldName_v2Marshaller::fromJSON(const Json::Value& s,TextFieldName_v2& e)
{
  e.mInternal=TextFieldName_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TextFieldName_v2::INVALID_ENUM);
}


Json::Value TextFieldName_v2Marshaller::toJSON(const TextFieldName_v2& e)
{
  if(e.mInternal==TextFieldName_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TextFieldName_v2Marshaller::fromString(const std::string& s,TextFieldName_v2& e)
{
  e.mInternal=TextFieldName_v2::INVALID_ENUM;
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

const std::string TextFieldName_v2Marshaller::toString(const TextFieldName_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==TextFieldName_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TextFieldName_v2Marshaller::mHashTable[20]=
{
  {"mainField1",0},
  {"mainField2",1},
  {"mainField3",2},
  {"mainField4",3},
  {"statusBar",4},
  {"mediaClock",5},
  {"mediaTrack",6},
  {"alertText1",7},
  {"alertText2",8},
  {"alertText3",9},
  {"scrollableMessageBody",10},
  {"initialInteractionText",11},
  {"navigationText1",12},
  {"navigationText2",13},
  {"ETA",14},
  {"totalDistance",15},
  {"audioPassThruDisplayText1",16},
  {"audioPassThruDisplayText2",17},
  {"sliderHeader",18},
  {"sliderFooter",19}
};
