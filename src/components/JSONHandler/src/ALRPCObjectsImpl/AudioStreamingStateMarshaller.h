#ifndef AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE
#define AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/AudioStreamingState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


//! marshalling class for AudioStreamingState

class AudioStreamingStateMarshaller
{
public:

  static std::string toName(const AudioStreamingState& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(AudioStreamingState& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=AudioStreamingState::INVALID_ENUM;
  }

  static bool checkIntegrity(AudioStreamingState& e)		{ return e.mInternal!=AudioStreamingState::INVALID_ENUM; } 
  static bool checkIntegrityConst(const AudioStreamingState& e)	{ return e.mInternal!=AudioStreamingState::INVALID_ENUM; } 

  static bool fromString(const std::string& s,AudioStreamingState& e);
  static const std::string toString(const AudioStreamingState& e);

  static bool fromJSON(const Json::Value& s,AudioStreamingState& e);
  static Json::Value toJSON(const AudioStreamingState& e);

  static const char* getName(AudioStreamingState::AudioStreamingStateInternal e)
  {
     return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }

  static const AudioStreamingState::AudioStreamingStateInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[2];
};

#endif
