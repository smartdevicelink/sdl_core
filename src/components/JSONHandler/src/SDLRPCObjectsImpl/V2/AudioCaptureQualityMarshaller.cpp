//
// Copyright (c) 2013 Ford Motor Company
//

#include <cstring>
#include "../include/JSONHandler/SDLRPCObjects/V2/AudioCaptureQuality.h"
#include "AudioCaptureQualityMarshaller.h"
#include "AudioCaptureQualityMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


const AudioCaptureQuality::AudioCaptureQualityInternal AudioCaptureQualityMarshaller::getIndex(const char* s)
{
  if(!s)
    return AudioCaptureQuality::INVALID_ENUM;
  const struct PerfectHashTable* p=AudioCaptureQuality_intHash::getPointer(s,strlen(s));
  return p ? static_cast<AudioCaptureQuality::AudioCaptureQualityInternal>(p->idx) : AudioCaptureQuality::INVALID_ENUM;
}


bool AudioCaptureQualityMarshaller::fromJSON(const Json::Value& s,AudioCaptureQuality& e)
{
  e.mInternal=AudioCaptureQuality::INVALID_ENUM;
  if(!s.isString())
    return false;

  e.mInternal=getIndex(s.asString().c_str());
  return (e.mInternal!=AudioCaptureQuality::INVALID_ENUM);
}


Json::Value AudioCaptureQualityMarshaller::toJSON(const AudioCaptureQuality& e)
{
  if(e.mInternal==AudioCaptureQuality::INVALID_ENUM) 
    return Json::Value(Json::nullValue);
  const char* s=getName(e.mInternal);
  return s ? Json::Value(s) : Json::Value(Json::nullValue);
}


bool AudioCaptureQualityMarshaller::fromString(const std::string& s,AudioCaptureQuality& e)
{
  e.mInternal=AudioCaptureQuality::INVALID_ENUM;
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

const std::string AudioCaptureQualityMarshaller::toString(const AudioCaptureQuality& e)
{
  Json::FastWriter writer;
  return e.mInternal==AudioCaptureQuality::INVALID_ENUM ? "" : writer.write(toJSON(e));

}

const PerfectHashTable AudioCaptureQualityMarshaller::mHashTable[2]=
{
  {"8_BIT",0},
  {"16_BIT",1}
};
