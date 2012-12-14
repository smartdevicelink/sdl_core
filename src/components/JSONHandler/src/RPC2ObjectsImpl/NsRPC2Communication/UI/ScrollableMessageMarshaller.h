#ifndef NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_SCROLLABLEMESSAGEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ScrollableMessage.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct ScrollableMessageMarshaller
    {
      static bool checkIntegrity(ScrollableMessage& e);
      static bool checkIntegrityConst(const ScrollableMessage& e);
    
      static bool fromString(const std::string& s,ScrollableMessage& e);
      static const std::string toString(const ScrollableMessage& e);
    
      static bool fromJSON(const Json::Value& s,ScrollableMessage& e);
      static Json::Value toJSON(const ScrollableMessage& e);
    };
  }
}

#endif
