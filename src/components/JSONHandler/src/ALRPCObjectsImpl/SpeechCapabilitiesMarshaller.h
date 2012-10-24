#ifndef SPEECHCAPABILITIESMARSHALLER_INCLUDE
#define SPEECHCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/SpeechCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
     return (e>=0 && e<4) ? mHashTable[e].name : NULL;
  }

  static const SpeechCapabilities::SpeechCapabilitiesInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[4];
};

#endif
