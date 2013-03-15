//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_TIRESTATUSMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_TIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/TireStatus.h"


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

  struct TireStatusMarshaller
  {
    static bool checkIntegrity(TireStatus& e);
    static bool checkIntegrityConst(const TireStatus& e);
  
    static bool fromString(const std::string& s,TireStatus& e);
    static const std::string toString(const TireStatus& e);
  
    static bool fromJSON(const Json::Value& s,TireStatus& e);
    static Json::Value toJSON(const TireStatus& e);
  };
}

#endif
