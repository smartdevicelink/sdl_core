#ifndef NSAPPLINKRPCV2_UPDATEMODEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_UPDATEMODEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/UpdateMode.h"


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

//! marshalling class for UpdateMode

  class UpdateModeMarshaller
  {
  public:
  
    static std::string toName(const UpdateMode& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(UpdateMode& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=UpdateMode::INVALID_ENUM;
    }
  
    static bool checkIntegrity(UpdateMode& e)		{ return e.mInternal!=UpdateMode::INVALID_ENUM; } 
    static bool checkIntegrityConst(const UpdateMode& e)	{ return e.mInternal!=UpdateMode::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,UpdateMode& e);
    static const std::string toString(const UpdateMode& e);
  
    static bool fromJSON(const Json::Value& s,UpdateMode& e);
    static Json::Value toJSON(const UpdateMode& e);
  
    static const char* getName(UpdateMode::UpdateModeInternal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const UpdateMode::UpdateModeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
