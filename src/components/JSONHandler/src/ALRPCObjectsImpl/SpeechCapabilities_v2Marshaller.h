#ifndef NSAPPLINKRPC_SPEECHCAPABILITIES_V2MARSHALLER_INCLUDE
#define NSAPPLINKRPC_SPEECHCAPABILITIES_V2MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/SpeechCapabilities_v2.h"


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

//! marshalling class for SpeechCapabilities_v2

  class SpeechCapabilities_v2Marshaller
  {
  public:
  
    static std::string toName(const SpeechCapabilities_v2& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(SpeechCapabilities_v2& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=SpeechCapabilities_v2::INVALID_ENUM;
    }
  
    static bool checkIntegrity(SpeechCapabilities_v2& e)		{ return e.mInternal!=SpeechCapabilities_v2::INVALID_ENUM; } 
    static bool checkIntegrityConst(const SpeechCapabilities_v2& e)	{ return e.mInternal!=SpeechCapabilities_v2::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,SpeechCapabilities_v2& e);
    static const std::string toString(const SpeechCapabilities_v2& e);
  
    static bool fromJSON(const Json::Value& s,SpeechCapabilities_v2& e);
    static Json::Value toJSON(const SpeechCapabilities_v2& e);
  
    static const char* getName(SpeechCapabilities_v2::SpeechCapabilities_v2Internal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const SpeechCapabilities_v2::SpeechCapabilities_v2Internal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
