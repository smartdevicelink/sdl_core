//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_DELETEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteInteractionChoiceSet_response.h"


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

  struct DeleteInteractionChoiceSet_responseMarshaller
  {
    static bool checkIntegrity(DeleteInteractionChoiceSet_response& e);
    static bool checkIntegrityConst(const DeleteInteractionChoiceSet_response& e);
  
    static bool fromString(const std::string& s,DeleteInteractionChoiceSet_response& e);
    static const std::string toString(const DeleteInteractionChoiceSet_response& e);
  
    static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet_response& e);
    static Json::Value toJSON(const DeleteInteractionChoiceSet_response& e);
  };
}

#endif
