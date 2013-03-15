//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_ONDRIVERDISTRACTIONMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/OnDriverDistraction.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPC
{

  struct OnDriverDistractionMarshaller
  {
    static bool checkIntegrity(OnDriverDistraction& e);
    static bool checkIntegrityConst(const OnDriverDistraction& e);
  
    static bool fromString(const std::string& s,OnDriverDistraction& e);
    static const std::string toString(const OnDriverDistraction& e);
  
    static bool fromJSON(const Json::Value& s,OnDriverDistraction& e);
    static Json::Value toJSON(const OnDriverDistraction& e);
  };
}

#endif
