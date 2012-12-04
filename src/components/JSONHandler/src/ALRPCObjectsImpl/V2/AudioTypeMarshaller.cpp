#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V2/AudioType.h"
#include "AudioTypeMarshaller.h"
#include "AudioTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


const AudioType::AudioTypeInternal AudioTypeMarshaller::getIndex(const char* s)
{
  if(!s)
    return AudioType::INVALID_ENUM;
  const struct PerfectHashTable* p=AudioType_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AudioType::AudioTypeInternal>(p->idx) : AudioType::INVALID_ENUM;
}


bool AudioTypeMarshaller::fromJSON(const Json::Value& s,AudioType& e)
{
  e.mInternal=AudioType::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AudioType::INVALID_ENUM);
}


Json::Value AudioTypeMarshaller::toJSON(const AudioType& e)
{
  if(e.mInternal==AudioType::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AudioTypeMarshaller::fromString(const std::string& s,AudioType& e)
{
  e.mInternal=AudioType::INVALID_ENUM;
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

const std::string AudioTypeMarshaller::toString(const AudioType& e)
{
  Json::FastWriter writer;
  return e.mInternal==AudioType::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AudioTypeMarshaller::mHashTable[1]=
{
  {"PCM",0}
};
