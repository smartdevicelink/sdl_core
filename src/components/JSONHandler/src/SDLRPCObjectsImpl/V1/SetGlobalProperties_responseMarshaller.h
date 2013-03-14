#ifndef NSAPPLINKRPC_SETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_SETGLOBALPROPERTIES_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/SetGlobalProperties_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

  struct SetGlobalProperties_responseMarshaller
  {
    static bool checkIntegrity(SetGlobalProperties_response& e);
    static bool checkIntegrityConst(const SetGlobalProperties_response& e);
  
    static bool fromString(const std::string& s,SetGlobalProperties_response& e);
    static const std::string toString(const SetGlobalProperties_response& e);
  
    static bool fromJSON(const Json::Value& s,SetGlobalProperties_response& e);
    static Json::Value toJSON(const SetGlobalProperties_response& e);
  };
}

#endif
