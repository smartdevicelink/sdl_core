#ifndef AUDIOCAPTUREQUALITYMARSHALLER_INCLUDE
#define AUDIOCAPTUREQUALITYMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/AudioCaptureQuality.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


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

#endif
