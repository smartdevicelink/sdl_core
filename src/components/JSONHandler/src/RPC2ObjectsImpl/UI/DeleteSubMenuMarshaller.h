#ifndef RPC2COMMUNICATION_UI_DELETESUBMENUMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_DELETESUBMENUMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/DeleteSubMenu.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct DeleteSubMenuMarshaller
    {
      static bool checkIntegrity(DeleteSubMenu& e);
      static bool checkIntegrityConst(const DeleteSubMenu& e);
    
      static bool fromString(const std::string& s,DeleteSubMenu& e);
      static const std::string toString(const DeleteSubMenu& e);
    
      static bool fromJSON(const Json::Value& s,DeleteSubMenu& e);
      static Json::Value toJSON(const DeleteSubMenu& e);
    };
  }
}

#endif
