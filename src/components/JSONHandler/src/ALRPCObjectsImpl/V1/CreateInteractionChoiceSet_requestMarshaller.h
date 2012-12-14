#ifndef NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/CreateInteractionChoiceSet_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Dec 14 08:11:37 2012
  source stamp	Fri Dec 14 08:11:34 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct CreateInteractionChoiceSet_requestMarshaller
  {
    static bool checkIntegrity(CreateInteractionChoiceSet_request& e);
    static bool checkIntegrityConst(const CreateInteractionChoiceSet_request& e);
  
    static bool fromString(const std::string& s,CreateInteractionChoiceSet_request& e);
    static const std::string toString(const CreateInteractionChoiceSet_request& e);
  
    static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet_request& e);
    static Json::Value toJSON(const CreateInteractionChoiceSet_request& e);
  };
}

#endif
