#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/AudioCaptureQuality_v2.h"
#include "AudioCaptureQuality_v2Marshaller.h"
#include "AudioCaptureQuality_v2Marshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


const AudioCaptureQuality_v2::AudioCaptureQuality_v2Internal AudioCaptureQuality_v2Marshaller::getIndex(const char* s)
{
  if(!s)
    return AudioCaptureQuality_v2::INVALID_ENUM;
  const struct PerfectHashTable* p=AudioCaptureQuality_v2_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AudioCaptureQuality_v2::AudioCaptureQuality_v2Internal>(p->idx) : AudioCaptureQuality_v2::INVALID_ENUM;
}


bool AudioCaptureQuality_v2Marshaller::fromJSON(const Json::Value& s,AudioCaptureQuality_v2& e)
{
  e.mInternal=AudioCaptureQuality_v2::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AudioCaptureQuality_v2::INVALID_ENUM);
}


Json::Value AudioCaptureQuality_v2Marshaller::toJSON(const AudioCaptureQuality_v2& e)
{
  if(e.mInternal==AudioCaptureQuality_v2::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AudioCaptureQuality_v2Marshaller::fromString(const std::string& s,AudioCaptureQuality_v2& e)
{
  e.mInternal=AudioCaptureQuality_v2::INVALID_ENUM;
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

const std::string AudioCaptureQuality_v2Marshaller::toString(const AudioCaptureQuality_v2& e)
{
  Json::FastWriter writer;
  return e.mInternal==AudioCaptureQuality_v2::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AudioCaptureQuality_v2Marshaller::mHashTable[2]=
{
  {"8_BIT",0},
  {"16_BIT",1}
};
