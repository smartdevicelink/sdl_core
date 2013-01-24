#ifndef NSAPPLINKRPC_MEDIACLOCKFORMATMARSHALLER_INCLUDE
#define NSAPPLINKRPC_MEDIACLOCKFORMATMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V1/MediaClockFormat.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsAppLinkRPC
{

//! marshalling class for MediaClockFormat

  class MediaClockFormatMarshaller
  {
  public:
  
    static std::string toName(const MediaClockFormat& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(MediaClockFormat& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=MediaClockFormat::INVALID_ENUM;
    }
  
    static bool checkIntegrity(MediaClockFormat& e)		{ return e.mInternal!=MediaClockFormat::INVALID_ENUM; } 
    static bool checkIntegrityConst(const MediaClockFormat& e)	{ return e.mInternal!=MediaClockFormat::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,MediaClockFormat& e);
    static const std::string toString(const MediaClockFormat& e);
  
    static bool fromJSON(const Json::Value& s,MediaClockFormat& e);
    static Json::Value toJSON(const MediaClockFormat& e);
  
    static const char* getName(MediaClockFormat::MediaClockFormatInternal e)
    {
       return (e>=0 && e<5) ? mHashTable[e].name : NULL;
    }
  
    static const MediaClockFormat::MediaClockFormatInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[5];
  };
  
}

#endif
