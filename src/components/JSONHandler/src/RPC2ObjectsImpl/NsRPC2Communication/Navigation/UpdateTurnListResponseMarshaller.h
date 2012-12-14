#ifndef NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_NAVIGATION_UPDATETURNLISTRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/UpdateTurnListResponse.h"

namespace NsRPC2Communication
{
  namespace Navigation
  {

    struct UpdateTurnListResponseMarshaller
    {
      static bool checkIntegrity(UpdateTurnListResponse& e);
      static bool checkIntegrityConst(const UpdateTurnListResponse& e);
    
      static bool fromString(const std::string& s,UpdateTurnListResponse& e);
      static const std::string toString(const UpdateTurnListResponse& e);
    
      static bool fromJSON(const Json::Value& s,UpdateTurnListResponse& e);
      static Json::Value toJSON(const UpdateTurnListResponse& e);
    };
  }
}

#endif
