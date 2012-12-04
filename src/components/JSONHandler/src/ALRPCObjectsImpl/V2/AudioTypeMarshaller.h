#ifndef NSAPPLINKRPCV2_AUDIOTYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_AUDIOTYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/AudioType.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for AudioType

  class AudioTypeMarshaller
  {
  public:
  
    static std::string toName(const AudioType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(AudioType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=AudioType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(AudioType& e)		{ return e.mInternal!=AudioType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const AudioType& e)	{ return e.mInternal!=AudioType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,AudioType& e);
    static const std::string toString(const AudioType& e);
  
    static bool fromJSON(const Json::Value& s,AudioType& e);
    static Json::Value toJSON(const AudioType& e);
  
    static const char* getName(AudioType::AudioTypeInternal e)
    {
       return (e>=0 && e<1) ? mHashTable[e].name : NULL;
    }
  
    static const AudioType::AudioTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[1];
  };
  
}

#endif
