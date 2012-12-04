#ifndef NSAPPLINKRPCV2_DISPLAYTYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DISPLAYTYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/DisplayType.h"


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

//! marshalling class for DisplayType

  class DisplayTypeMarshaller
  {
  public:
  
    static std::string toName(const DisplayType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(DisplayType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=DisplayType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(DisplayType& e)		{ return e.mInternal!=DisplayType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const DisplayType& e)	{ return e.mInternal!=DisplayType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,DisplayType& e);
    static const std::string toString(const DisplayType& e);
  
    static bool fromJSON(const Json::Value& s,DisplayType& e);
    static Json::Value toJSON(const DisplayType& e);
  
    static const char* getName(DisplayType::DisplayTypeInternal e)
    {
       return (e>=0 && e<9) ? mHashTable[e].name : NULL;
    }
  
    static const DisplayType::DisplayTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[9];
  };
  
}

#endif
