#ifndef NSAPPLINKRPCV2_MESSAGETYPEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_MESSAGETYPEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/messageType.h"


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

//! marshalling class for messageType

  class messageTypeMarshaller
  {
  public:
  
    static std::string toName(const messageType& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(messageType& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=messageType::INVALID_ENUM;
    }
  
    static bool checkIntegrity(messageType& e)		{ return e.mInternal!=messageType::INVALID_ENUM; } 
    static bool checkIntegrityConst(const messageType& e)	{ return e.mInternal!=messageType::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,messageType& e);
    static const std::string toString(const messageType& e);
  
    static bool fromJSON(const Json::Value& s,messageType& e);
    static Json::Value toJSON(const messageType& e);
  
    static const char* getName(messageType::messageTypeInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const messageType::messageTypeInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
