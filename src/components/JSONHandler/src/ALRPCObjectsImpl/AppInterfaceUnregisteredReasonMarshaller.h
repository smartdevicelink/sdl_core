#ifndef APPINTERFACEUNREGISTEREDREASONMARSHALLER_INCLUDE
#define APPINTERFACEUNREGISTEREDREASONMARSHALLER_INCLUDE

#include <string>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

#include "PerfectHashTable.h"

#include "../../include/JSONHandler/ALRPCObjects/AppInterfaceUnregisteredReason.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Oct 25 06:32:04 2012
  source stamp	Thu Oct 25 06:28:28 2012
  author	robok0der
*/


//! marshalling class for AppInterfaceUnregisteredReason

class AppInterfaceUnregisteredReasonMarshaller
{
public:

  static std::string toName(const AppInterfaceUnregisteredReason& e) 	{ return getName(e.mInternal) ?: ""; }

  static bool fromName(AppInterfaceUnregisteredReason& e,const std::string& s)
  { 
    return (e.mInternal=getIndex(s.c_str()))!=AppInterfaceUnregisteredReason::INVALID_ENUM;
  }

  static bool checkIntegrity(AppInterfaceUnregisteredReason& e)		{ return e.mInternal!=AppInterfaceUnregisteredReason::INVALID_ENUM; } 
  static bool checkIntegrityConst(const AppInterfaceUnregisteredReason& e)	{ return e.mInternal!=AppInterfaceUnregisteredReason::INVALID_ENUM; } 

  static bool fromString(const std::string& s,AppInterfaceUnregisteredReason& e);
  static const std::string toString(const AppInterfaceUnregisteredReason& e);

  static bool fromJSON(const Json::Value& s,AppInterfaceUnregisteredReason& e);
  static Json::Value toJSON(const AppInterfaceUnregisteredReason& e);

  static const char* getName(AppInterfaceUnregisteredReason::AppInterfaceUnregisteredReasonInternal e)
  {
     return (e>=0 && e<10) ? mHashTable[e].name : NULL;
  }

  static const AppInterfaceUnregisteredReason::AppInterfaceUnregisteredReasonInternal getIndex(const char* s);

  static const PerfectHashTable mHashTable[10];
};

#endif
