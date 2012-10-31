#ifndef VRCAPABILITIESMARSHALLER_INCLUDE
#define VRCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/VrCapabilities.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Oct 30 08:29:32 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/


//! marshalling class for VrCapabilities

class VrCapabilitiesMarshaller
{
public:

  static std::string toName(const VrCapabilities& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(VrCapabilities& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=VrCapabilities::INVALID_ENUM;
  }

  static bool checkIntegrity(VrCapabilities& e)		{ return e.mInternal!=VrCapabilities::INVALID_ENUM; } 
  static bool checkIntegrityConst(const VrCapabilities& e)	{ return e.mInternal!=VrCapabilities::INVALID_ENUM; } 

  static bool fromString(const std::string& s,VrCapabilities& e);
  static const std::string toString(const VrCapabilities& e);

  static bool fromJSON(const Json::Value& s,VrCapabilities& e);
  static Json::Value toJSON(const VrCapabilities& e);

  static const char* getName(VrCapabilities::VrCapabilitiesInternal e)
  {
     return (e>=0 && e<1) ? mHashTable[e].name : NULL;
  }

  static const VrCapabilities::VrCapabilitiesInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[1];
};

#endif
