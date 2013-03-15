//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_SETMEDIACLOCKTIMER_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_SETMEDIACLOCKTIMER_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/SetMediaClockTimer_request.h"


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

  struct SetMediaClockTimer_requestMarshaller
  {
    static bool checkIntegrity(SetMediaClockTimer_request& e);
    static bool checkIntegrityConst(const SetMediaClockTimer_request& e);
  
    static bool fromString(const std::string& s,SetMediaClockTimer_request& e);
    static const std::string toString(const SetMediaClockTimer_request& e);
  
    static bool fromJSON(const Json::Value& s,SetMediaClockTimer_request& e);
    static Json::Value toJSON(const SetMediaClockTimer_request& e);
  };
}

#endif
