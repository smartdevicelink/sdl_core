#ifndef NSAPPLINKRPCV2_AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/AudioStreamingState.h"


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
  
}

#endif
