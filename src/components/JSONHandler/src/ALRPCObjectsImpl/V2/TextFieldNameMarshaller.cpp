#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/TextFieldName.h"
#include "TextFieldNameMarshaller.h"
#include "TextFieldNameMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const TextFieldName::TextFieldNameInternal TextFieldNameMarshaller::getIndex(const char* s)
{
  if(!s)
    return TextFieldName::INVALID_ENUM;
  const struct PerfectHashTable* p=TextFieldName_intHash::getPointer(s,strlen(s));
  return p ? static_cast<TextFieldName::TextFieldNameInternal>(p->idx) : TextFieldName::INVALID_ENUM;
}


bool TextFieldNameMarshaller::fromJSON(const Json::Value& s,TextFieldName& e)
{
  e.mInternal=TextFieldName::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=TextFieldName::INVALID_ENUM);
}


Json::Value TextFieldNameMarshaller::toJSON(const TextFieldName& e)
{
  if(e.mInternal==TextFieldName::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool TextFieldNameMarshaller::fromString(const std::string& s,TextFieldName& e)
{
  e.mInternal=TextFieldName::INVALID_ENUM;
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

const std::string TextFieldNameMarshaller::toString(const TextFieldName& e)
{
  Json::FastWriter writer;
  return e.mInternal==TextFieldName::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable TextFieldNameMarshaller::mHashTable[20]=
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
