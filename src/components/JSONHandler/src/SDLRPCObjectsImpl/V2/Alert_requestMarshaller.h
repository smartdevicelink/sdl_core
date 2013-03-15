//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_ALERT_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_ALERT_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/Alert_request.h"


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
