#ifndef NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/CreateInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
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
