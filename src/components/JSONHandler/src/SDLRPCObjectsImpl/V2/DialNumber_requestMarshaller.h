#ifndef NSAPPLINKRPCV2_DIALNUMBER_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DIALNUMBER_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/DialNumber_request.h"


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

  struct DialNumber_requestMarshaller
  {
    static bool checkIntegrity(DialNumber_request& e);
    static bool checkIntegrityConst(const DialNumber_request& e);
  
    static bool fromString(const std::string& s,DialNumber_request& e);
    static const std::string toString(const DialNumber_request& e);
  
    static bool fromJSON(const Json::Value& s,DialNumber_request& e);
    static Json::Value toJSON(const DialNumber_request& e);
  };
}

#endif
