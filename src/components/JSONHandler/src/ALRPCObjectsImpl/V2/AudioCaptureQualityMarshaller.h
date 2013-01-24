#ifndef NSAPPLINKRPCV2_AUDIOCAPTUREQUALITYMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_AUDIOCAPTUREQUALITYMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/AudioCaptureQuality.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for AudioCaptureQuality

  class AudioCaptureQualityMarshaller
  {
  public:
  
    static std::string toName(const AudioCaptureQuality& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(AudioCaptureQuality& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=AudioCaptureQuality::INVALID_ENUM;
    }
  
    static bool checkIntegrity(AudioCaptureQuality& e)		{ return e.mInternal!=AudioCaptureQuality::INVALID_ENUM; } 
    static bool checkIntegrityConst(const AudioCaptureQuality& e)	{ return e.mInternal!=AudioCaptureQuality::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,AudioCaptureQuality& e);
    static const std::string toString(const AudioCaptureQuality& e);
  
    static bool fromJSON(const Json::Value& s,AudioCaptureQuality& e);
    static Json::Value toJSON(const AudioCaptureQuality& e);
  
    static const char* getName(AudioCaptureQuality::AudioCaptureQualityInternal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const AudioCaptureQuality::AudioCaptureQualityInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
