#ifndef NSAPPLINKRPC_RESETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_RESETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/ResetGlobalProperties_request.h"


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

  struct ResetGlobalProperties_requestMarshaller
  {
    static bool checkIntegrity(ResetGlobalProperties_request& e);
    static bool checkIntegrityConst(const ResetGlobalProperties_request& e);
  
    static bool fromString(const std::string& s,ResetGlobalProperties_request& e);
    static const std::string toString(const ResetGlobalProperties_request& e);
  
    static bool fromJSON(const Json::Value& s,ResetGlobalProperties_request& e);
    static Json::Value toJSON(const ResetGlobalProperties_request& e);
  };
}

#endif
