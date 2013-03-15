//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_DISPLAYCAPABILITIESMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_DISPLAYCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/DisplayCapabilities.h"


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

  struct DisplayCapabilitiesMarshaller
  {
    static bool checkIntegrity(DisplayCapabilities& e);
    static bool checkIntegrityConst(const DisplayCapabilities& e);
  
    static bool fromString(const std::string& s,DisplayCapabilities& e);
    static const std::string toString(const DisplayCapabilities& e);
  
    static bool fromJSON(const Json::Value& s,DisplayCapabilities& e);
    static Json::Value toJSON(const DisplayCapabilities& e);
  };
}

#endif
