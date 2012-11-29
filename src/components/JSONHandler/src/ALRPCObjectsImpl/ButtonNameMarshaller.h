#ifndef NSAPPLINKRPC_BUTTONNAMEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_BUTTONNAMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/ButtonName.h"


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

//! marshalling class for ButtonName

  class ButtonNameMarshaller
  {
  public:
  
    static std::string toName(const ButtonName& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(ButtonName& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=ButtonName::INVALID_ENUM;
    }
  
    static bool checkIntegrity(ButtonName& e)		{ return e.mInternal!=ButtonName::INVALID_ENUM; } 
    static bool checkIntegrityConst(const ButtonName& e)	{ return e.mInternal!=ButtonName::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,ButtonName& e);
    static const std::string toString(const ButtonName& e);
  
    static bool fromJSON(const Json::Value& s,ButtonName& e);
    static Json::Value toJSON(const ButtonName& e);
  
    static const char* getName(ButtonName::ButtonNameInternal e)
    {
       return (e>=0 && e<15) ? mHashTable[e].name : NULL;
    }
  
    static const ButtonName::ButtonNameInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[15];
  };
  
}

#endif
