#ifndef NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_CREATEINTERACTIONCHOICESET_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/CreateInteractionChoiceSet_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

namespace NsSmartDeviceLinkRPCV2
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
