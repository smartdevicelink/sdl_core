//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPC_ADDCOMMAND_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPC_ADDCOMMAND_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/AddCommand_response.h"


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

  struct AddCommand_responseMarshaller
  {
    static bool checkIntegrity(AddCommand_response& e);
    static bool checkIntegrityConst(const AddCommand_response& e);
  
    static bool fromString(const std::string& s,AddCommand_response& e);
    static const std::string toString(const AddCommand_response& e);
  
    static bool fromJSON(const Json::Value& s,AddCommand_response& e);
    static Json::Value toJSON(const AddCommand_response& e);
  };
}

#endif
