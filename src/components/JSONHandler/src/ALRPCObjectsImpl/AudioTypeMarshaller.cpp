#include <cstring>
#include "../../include/JSONHandler/ALRPCObjects/AudioType.h"
#include "AudioTypeMarshaller.h"
#include "AudioTypeMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/



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

const PerfectHashTable AudioTypeMarshaller::mHashTable[3]=
{
  {"WAV",0},
  {"PCM",1},
  {"MP3",2}
};
