#ifndef NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/CreateInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct CreateInteractionChoiceSet_responseMarshaller
  {
    static bool checkIntegrity(CreateInteractionChoiceSet_response& e);
    static bool checkIntegrityConst(const CreateInteractionChoiceSet_response& e);
  
    static bool fromString(const std::string& s,CreateInteractionChoiceSet_response& e);
    static const std::string toString(const CreateInteractionChoiceSet_response& e);
  
    static bool fromJSON(const Json::Value& s,CreateInteractionChoiceSet_response& e);
    static Json::Value toJSON(const CreateInteractionChoiceSet_response& e);
  };
}

#endif
