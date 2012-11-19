#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/TextFieldName.h"
#include "TextFieldNameMarshaller.h"
#include "TextFieldNameMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


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

const PerfectHashTable TextFieldNameMarshaller::mHashTable[7]=
{
  {"mainField1",0},
  {"mainField2",1},
  {"statusBar",2},
  {"mediaClock",3},
  {"mediaTrack",4},
  {"alertText1",5},
  {"alertText2",6}
};
