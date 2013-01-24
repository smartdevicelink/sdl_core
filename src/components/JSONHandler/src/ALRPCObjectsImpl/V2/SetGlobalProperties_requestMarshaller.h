#ifndef NSAPPLINKRPCV2_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/SetGlobalProperties_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
