//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_ONTBTCLIENTSTATEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_ONTBTCLIENTSTATEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/OnTBTClientState.h"


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

  struct OnTBTClientStateMarshaller
  {
    static bool checkIntegrity(OnTBTClientState& e);
    static bool checkIntegrityConst(const OnTBTClientState& e);
  
    static bool fromString(const std::string& s,OnTBTClientState& e);
    static const std::string toString(const OnTBTClientState& e);
  
    static bool fromJSON(const Json::Value& s,OnTBTClientState& e);
    static Json::Value toJSON(const OnTBTClientState& e);
  };
}

#endif
