#ifndef NSAPPLINKRPCV1_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_SETGLOBALPROPERTIES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/SetGlobalProperties_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
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
