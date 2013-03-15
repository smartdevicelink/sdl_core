//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/SetGlobalProperties_request.h"


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

  struct SetGlobalProperties_requestMarshaller
  {
    static bool checkIntegrity(SetGlobalProperties_request& e);
    static bool checkIntegrityConst(const SetGlobalProperties_request& e);
  
    static bool fromString(const std::string& s,SetGlobalProperties_request& e);
    static const std::string toString(const SetGlobalProperties_request& e);
  
    static bool fromJSON(const Json::Value& s,SetGlobalProperties_request& e);
    static Json::Value toJSON(const SetGlobalProperties_request& e);
  };
}

#endif
