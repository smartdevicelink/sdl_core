//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_TIREPRESSURETELLTALEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_TIREPRESSURETELLTALEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "PerfectHashTable.h"

#include "../include/JSONHandler/SDLRPCObjects/V2/TirePressureTellTale.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
{

//! marshalling class for TirePressureTellTale

  class TirePressureTellTaleMarshaller
  {
  public:
  
    static std::string toName(const TirePressureTellTale& e) 	{ return getName(e.mInternal) ?: ""; }
  
    static bool fromName(TirePressureTellTale& e,const std::string& s)
    { 
      return (e.mInternal=getIndex(s.c_str()))!=TirePressureTellTale::INVALID_ENUM;
    }
  
    static bool checkIntegrity(TirePressureTellTale& e)		{ return e.mInternal!=TirePressureTellTale::INVALID_ENUM; } 
    static bool checkIntegrityConst(const TirePressureTellTale& e)	{ return e.mInternal!=TirePressureTellTale::INVALID_ENUM; } 
  
    static bool fromString(const std::string& s,TirePressureTellTale& e);
    static const std::string toString(const TirePressureTellTale& e);
  
    static bool fromJSON(const Json::Value& s,TirePressureTellTale& e);
    static Json::Value toJSON(const TirePressureTellTale& e);
  
    static const char* getName(TirePressureTellTale::TirePressureTellTaleInternal e)
    {
       return (e>=0 && e<3) ? mHashTable[e].name : NULL;
    }
  
    static const TirePressureTellTale::TirePressureTellTaleInternal getIndex(const char* s);
  
    static const PerfectHashTable mHashTable[3];
  };
  
}

#endif
