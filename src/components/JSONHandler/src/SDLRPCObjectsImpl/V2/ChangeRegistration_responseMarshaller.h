#ifndef NSAPPLINKRPCV2_CHANGEREGISTRATION_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CHANGEREGISTRATION_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/ChangeRegistration_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
{

  struct ChangeRegistration_responseMarshaller
  {
    static bool checkIntegrity(ChangeRegistration_response& e);
    static bool checkIntegrityConst(const ChangeRegistration_response& e);
  
    static bool fromString(const std::string& s,ChangeRegistration_response& e);
    static const std::string toString(const ChangeRegistration_response& e);
  
    static bool fromJSON(const Json::Value& s,ChangeRegistration_response& e);
    static Json::Value toJSON(const ChangeRegistration_response& e);
  };
}

#endif
