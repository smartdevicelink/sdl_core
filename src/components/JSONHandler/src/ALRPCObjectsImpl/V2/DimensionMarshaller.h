#ifndef NSAPPLINKRPCV2_DIMENSIONMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DIMENSIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/ALRPCObjects/V2/Dimension.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

//! marshalling class for Dimension

  class DimensionMarshaller
  {
  public:
  
    static std::string toName(const Dimension& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(Dimension& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=Dimension::INVALID_ENUM;
    }
  
    static bool checkIntegrity(Dimension& e)		{ return e.mInternal!=Dimension::INVALID_ENUM; } 
    static bool checkIntegrityConst(const Dimension& e)	{ return e.mInternal!=Dimension::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,Dimension& e);
    static const std::string toString(const Dimension& e);
  
    static bool fromJSON(const Json::Value& s,Dimension& e);
    static Json::Value toJSON(const Dimension& e);
  
    static const char* getName(Dimension::DimensionInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const Dimension::DimensionInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
