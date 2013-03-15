//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_CHOICEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_CHOICEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/Choice.h"


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

  struct ChoiceMarshaller
  {
    static bool checkIntegrity(Choice& e);
    static bool checkIntegrityConst(const Choice& e);
  
    static bool fromString(const std::string& s,Choice& e);
    static const std::string toString(const Choice& e);
  
    static bool fromJSON(const Json::Value& s,Choice& e);
    static Json::Value toJSON(const Choice& e);
  };
}

#endif
