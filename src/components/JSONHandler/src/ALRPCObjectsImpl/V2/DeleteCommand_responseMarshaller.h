#ifndef NSAPPLINKRPCV2_DELETECOMMAND_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DELETECOMMAND_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/DeleteCommand_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
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
