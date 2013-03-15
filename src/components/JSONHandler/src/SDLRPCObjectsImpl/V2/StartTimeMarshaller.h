//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_STARTTIMEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_STARTTIMEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/StartTime.h"


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

  struct StartTimeMarshaller
  {
    static bool checkIntegrity(StartTime& e);
    static bool checkIntegrityConst(const StartTime& e);
  
    static bool fromString(const std::string& s,StartTime& e);
    static const std::string toString(const StartTime& e);
  
    static bool fromJSON(const Json::Value& s,StartTime& e);
    static Json::Value toJSON(const StartTime& e);
  };
}

#endif
