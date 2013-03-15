//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_DISPLAYCAPABILITIESMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_DISPLAYCAPABILITIESMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/DisplayCapabilities.h"


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
