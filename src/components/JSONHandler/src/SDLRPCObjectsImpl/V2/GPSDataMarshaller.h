//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_GPSDATAMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_GPSDATAMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/GPSData.h"


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

  struct GPSDataMarshaller
  {
    static bool checkIntegrity(GPSData& e);
    static bool checkIntegrityConst(const GPSData& e);
  
    static bool fromString(const std::string& s,GPSData& e);
    static const std::string toString(const GPSData& e);
  
    static bool fromJSON(const Json::Value& s,GPSData& e);
    static Json::Value toJSON(const GPSData& e);
  };
}

#endif
