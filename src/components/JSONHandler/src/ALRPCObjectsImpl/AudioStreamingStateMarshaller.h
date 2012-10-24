#ifndef AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE
#define AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/AudioStreamingState.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
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
     return (e>=0 && e<3) ? mHashTable[e].name : NULL;
  }

  static const AudioStreamingState::AudioStreamingStateInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[3];
};

#endif
