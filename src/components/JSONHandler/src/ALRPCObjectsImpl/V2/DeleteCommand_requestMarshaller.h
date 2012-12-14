#ifndef NSAPPLINKRPCV2_DELETECOMMAND_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DELETECOMMAND_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/DeleteCommand_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
