#ifndef NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_DELETESUBMENURESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DeleteSubMenuResponse.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct DeleteSubMenuResponseMarshaller
    {
      static bool checkIntegrity(DeleteSubMenuResponse& e);
      static bool checkIntegrityConst(const DeleteSubMenuResponse& e);
    
      static bool fromString(const std::string& s,DeleteSubMenuResponse& e);
      static const std::string toString(const DeleteSubMenuResponse& e);
    
      static bool fromJSON(const Json::Value& s,DeleteSubMenuResponse& e);
      static Json::Value toJSON(const DeleteSubMenuResponse& e);
    };
  }
}

#endif
