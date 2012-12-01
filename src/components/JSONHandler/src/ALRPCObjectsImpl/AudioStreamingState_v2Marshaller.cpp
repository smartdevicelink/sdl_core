#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/AudioStreamingState_v2.h"
#include "AudioStreamingState_v2Marshaller.h"
#include "AudioStreamingState_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const AudioStreamingState_v2::AudioStreamingState_v2Internal AudioStreamingState_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return AudioStreamingState_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=AudioStreamingState_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AudioStreamingState_v2::AudioStreamingState_v2Internal>(p->idx) : AudioStreamingState_v2::INVALID_ENUM;
}


bool AudioStreamingState_v2Marshaller::fromJSON(const Json::Value& s,AudioStreamingState_v2& e)
{
  e.mInternal=AudioStreamingState_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AudioStreamingState_v2::INVALID_ENUM);
}


Json::Value AudioStreamingState_v2Marshaller::toJSON(const AudioStreamingState_v2& e)
{
  if(e.mInternal==AudioStreamingState_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AudioStreamingState_v2Marshaller::fromString(const std::string& s,AudioStreamingState_v2& e)
{
  e.mInternal=AudioStreamingState_v2::INVALID_ENUM;
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

const std::string AudioStreamingState_v2Marshaller::toString(const AudioStreamingState_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==AudioStreamingState_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AudioStreamingState_v2Marshaller::mHashTable[3]=
{
  {"AUDIBLE",0},
  {"ATTENUATED",1},
  {"NOT_AUDIBLE",2}
};
