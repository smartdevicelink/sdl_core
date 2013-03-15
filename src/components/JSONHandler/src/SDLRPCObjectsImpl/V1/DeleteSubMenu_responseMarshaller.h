//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_DELETESUBMENU_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_DELETESUBMENU_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteSubMenu_response.h"


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

  struct DeleteSubMenu_responseMarshaller
  {
    static bool checkIntegrity(DeleteSubMenu_response& e);
    static bool checkIntegrityConst(const DeleteSubMenu_response& e);
  
    static bool fromString(const std::string& s,DeleteSubMenu_response& e);
    static const std::string toString(const DeleteSubMenu_response& e);
  
    static bool fromJSON(const Json::Value& s,DeleteSubMenu_response& e);
    static Json::Value toJSON(const DeleteSubMenu_response& e);
  };
}

#endif
