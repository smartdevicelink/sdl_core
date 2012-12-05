#ifndef NSAPPLINKRPC_AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_AUDIOSTREAMINGSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/AudioStreamingState.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
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
       return (e>=0 && e<2) ? mHashTable[e].name : NULL;
    }
  
    static const AudioStreamingState::AudioStreamingStateInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[2];
  };
  
}

#endif
