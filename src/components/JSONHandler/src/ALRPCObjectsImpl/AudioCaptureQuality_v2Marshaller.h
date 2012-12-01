#ifndef NSAPPLINKRPC_AUDIOCAPTUREQUALITY_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_AUDIOCAPTUREQUALITY_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/AudioCaptureQuality_v2.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for AudioCaptureQuality_v2

  class AudioCaptureQuality_v2Marshaller
  {
  public:
  
    static std::string toName(const AudioCaptureQuality_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(AudioCaptureQuality_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=AudioCaptureQuality_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(AudioCaptureQuality_v2& e)		{ return e.mInternal!=AudioCaptureQuality_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const AudioCaptureQuality_v2& e)	{ return e.mInternal!=AudioCaptureQuality_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,AudioCaptureQuality_v2& e);
    static const std::string toString(const AudioCaptureQuality_v2& e);
  
    static bool fromJSON(const Json::Value& s,AudioCaptureQuality_v2& e);
    static Json::Value toJSON(const AudioCaptureQuality_v2& e);
  
    static const char* getName(AudioCaptureQuality_v2::AudioCaptureQuality_v2Internal e)
    {
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const AudioCaptureQuality_v2::AudioCaptureQuality_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
