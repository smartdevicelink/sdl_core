#ifndef NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/UpdateTurnList.h"

namespace NsRPC2Communication
{
  namespace Navigation
  {

    struct UpdateTurnListMarshaller
    {
      static bool checkIntegrity(UpdateTurnList& e);
      static bool checkIntegrityConst(const UpdateTurnList& e);
    
      static bool fromString(const std::string& s,UpdateTurnList& e);
      static const std::string toString(const UpdateTurnList& e);
    
      static bool fromJSON(const Json::Value& s,UpdateTurnList& e);
      static Json::Value toJSON(const UpdateTurnList& e);
    };
  }
}

#endif
