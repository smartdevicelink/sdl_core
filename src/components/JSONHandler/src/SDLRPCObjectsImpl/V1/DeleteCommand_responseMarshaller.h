#ifndef NSAPPLINKRPC_DELETECOMMAND_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteCommand_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Jan 24 06:36:21 2013
  source stamp	Thu Jan 24 06:35:34 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPC
{

  struct DeleteCommand_responseMarshaller
  {
    static bool checkIntegrity(DeleteCommand_response& e);
    static bool checkIntegrityConst(const DeleteCommand_response& e);
  
    static bool fromString(const std::string& s,DeleteCommand_response& e);
    static const std::string toString(const DeleteCommand_response& e);
  
    static bool fromJSON(const Json::Value& s,DeleteCommand_response& e);
    static Json::Value toJSON(const DeleteCommand_response& e);
  };
}

#endif
