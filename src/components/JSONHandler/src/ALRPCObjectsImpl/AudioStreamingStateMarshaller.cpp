#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/AudioStreamingState.h"
#include "AudioStreamingStateMarshaller.h"
#include "AudioStreamingStateMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Wed Nov  7 13:10:41 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace AppLinkRPC;


const AudioStreamingState::AudioStreamingStateInternal AudioStreamingStateMarshaller::getIndex(const char* s)
{
  if(!s)
    return AudioStreamingState::INVALID_ENUM;
  const struct PerfectHashTable* p=AudioStreamingState_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AudioStreamingState::AudioStreamingStateInternal>(p->idx) : AudioStreamingState::INVALID_ENUM;
}


bool AudioStreamingStateMarshaller::fromJSON(const Json::Value& s,AudioStreamingState& e)
{
  e.mInternal=AudioStreamingState::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AudioStreamingState::INVALID_ENUM);
}


Json::Value AudioStreamingStateMarshaller::toJSON(const AudioStreamingState& e)
{
  if(e.mInternal==AudioStreamingState::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AudioStreamingStateMarshaller::fromString(const std::string& s,AudioStreamingState& e)
{
  e.mInternal=AudioStreamingState::INVALID_ENUM;
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

const std::string AudioStreamingStateMarshaller::toString(const AudioStreamingState& e)
{
  Json::FastWriter writer;
  return e.mInternal==AudioStreamingState::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AudioStreamingStateMarshaller::mHashTable[2]=
{
  {"AUDIBLE",0},
  {"NOT_AUDIBLE",1}
};
