#ifndef NSAPPLINKRPC_DELETECOMMAND_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DELETECOMMAND_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/DeleteCommand_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct DeleteCommand_requestMarshaller
  {
    static bool checkIntegrity(DeleteCommand_request& e);
    static bool checkIntegrityConst(const DeleteCommand_request& e);
  
    static bool fromString(const std::string& s,DeleteCommand_request& e);
    static const std::string toString(const DeleteCommand_request& e);
  
    static bool fromJSON(const Json::Value& s,DeleteCommand_request& e);
    static Json::Value toJSON(const DeleteCommand_request& e);
  };
}

#endif
