#ifndef NSAPPLINKRPC_ALERT_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_ALERT_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/Alert_request.h"


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

  struct Alert_requestMarshaller
  {
    static bool checkIntegrity(Alert_request& e);
    static bool checkIntegrityConst(const Alert_request& e);
  
    static bool fromString(const std::string& s,Alert_request& e);
    static const std::string toString(const Alert_request& e);
  
    static bool fromJSON(const Json::Value& s,Alert_request& e);
    static Json::Value toJSON(const Alert_request& e);
  };
}

#endif
