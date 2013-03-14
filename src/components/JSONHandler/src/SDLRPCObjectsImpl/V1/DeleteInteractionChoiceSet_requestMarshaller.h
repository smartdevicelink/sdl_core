#ifndef NSAPPLINKRPC_DELETEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DELETEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V1/DeleteInteractionChoiceSet_request.h"


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

  struct DeleteInteractionChoiceSet_requestMarshaller
  {
    static bool checkIntegrity(DeleteInteractionChoiceSet_request& e);
    static bool checkIntegrityConst(const DeleteInteractionChoiceSet_request& e);
  
    static bool fromString(const std::string& s,DeleteInteractionChoiceSet_request& e);
    static const std::string toString(const DeleteInteractionChoiceSet_request& e);
  
    static bool fromJSON(const Json::Value& s,DeleteInteractionChoiceSet_request& e);
    static Json::Value toJSON(const DeleteInteractionChoiceSet_request& e);
  };
}

#endif
