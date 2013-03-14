#ifndef NSAPPLINKRPC_SPEECHCAPABILITIESMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SPEECHCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V1/SpeechCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

//! marshalling class for SpeechCapabilities

  class SpeechCapabilitiesMarshaller
  {
  public:
  
    static std::string toName(const SpeechCapabilities& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SpeechCapabilities& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SpeechCapabilities::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SpeechCapabilities& e)		{ return e.mInternal!=SpeechCapabilities::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SpeechCapabilities& e)	{ return e.mInternal!=SpeechCapabilities::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SpeechCapabilities& e);
    static const std::string toString(const SpeechCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,SpeechCapabilities& e);
    static Json::Value toJSON(const SpeechCapabilities& e);
  
    static const char* getName(SpeechCapabilities::SpeechCapabilitiesInternal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const SpeechCapabilities::SpeechCapabilitiesInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
