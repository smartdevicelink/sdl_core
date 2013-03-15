//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_SPEAK_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_SPEAK_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/Speak_response.h"


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

  struct Speak_responseMarshaller
  {
    static bool checkIntegrity(Speak_response& e);
    static bool checkIntegrityConst(const Speak_response& e);
  
    static bool fromString(const std::string& s,Speak_response& e);
    static const std::string toString(const Speak_response& e);
  
    static bool fromJSON(const Json::Value& s,Speak_response& e);
    static Json::Value toJSON(const Speak_response& e);
  };
}

#endif
