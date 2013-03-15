//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_SHOW_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_SHOW_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/Show_response.h"


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

  struct Show_responseMarshaller
  {
    static bool checkIntegrity(Show_response& e);
    static bool checkIntegrityConst(const Show_response& e);
  
    static bool fromString(const std::string& s,Show_response& e);
    static const std::string toString(const Show_response& e);
  
    static bool fromJSON(const Json::Value& s,Show_response& e);
    static Json::Value toJSON(const Show_response& e);
  };
}

#endif
