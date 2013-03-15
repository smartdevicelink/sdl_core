//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_VEHICLETYPEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_VEHICLETYPEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/VehicleType.h"


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

  struct VehicleTypeMarshaller
  {
    static bool checkIntegrity(VehicleType& e);
    static bool checkIntegrityConst(const VehicleType& e);
  
    static bool fromString(const std::string& s,VehicleType& e);
    static const std::string toString(const VehicleType& e);
  
    static bool fromJSON(const Json::Value& s,VehicleType& e);
    static Json::Value toJSON(const VehicleType& e);
  };
}

#endif
