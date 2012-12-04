#include <cstring>
#include "../include/JSONHandler/ALRPCObjects/V1/AudioCaptureQuality.h"
#include "AudioCaptureQualityMarshaller.h"
#include "AudioCaptureQualityMarshaller.inc"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;


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
  {"8KHZ",0},
  {"16KHZ",1}
};
