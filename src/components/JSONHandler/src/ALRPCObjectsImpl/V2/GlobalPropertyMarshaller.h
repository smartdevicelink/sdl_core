#ifndef NSAPPLINKRPCV2_GLOBALPROPERTYMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_GLOBALPROPERTYMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/GlobalProperty.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for GlobalProperty

  class GlobalPropertyMarshaller
  {
  public:
  
    static std::string toName(const GlobalProperty& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(GlobalProperty& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=GlobalProperty::INVALID_ENUM;
    }
  
    static bool checkIntegrity(GlobalProperty& e)		{ return e.mInternal!=GlobalProperty::INVALID_ENUM; } 
    static bool checkIntegrityConst(const GlobalProperty& e)	{ return e.mInternal!=GlobalProperty::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,GlobalProperty& e);
    static const std::string toString(const GlobalProperty& e);
  
    static bool fromJSON(const Json::Value& s,GlobalProperty& e);
    static Json::Value toJSON(const GlobalProperty& e);
  
    static const char* getName(GlobalProperty::GlobalPropertyInternal e)
    {
       return (e>=0 && e<4) ? mHashTable[e].name : NULL;
    }
  
    static const GlobalProperty::GlobalPropertyInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[4];
  };
  
}

#endif
