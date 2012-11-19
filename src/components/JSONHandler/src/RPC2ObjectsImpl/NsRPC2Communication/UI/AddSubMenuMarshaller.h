#ifndef NSRPC2COMMUNICATION_UI_ADDSUBMENUMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_UI_ADDSUBMENUMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/AddSubMenu.h"

namespace NsRPC2Communication
{
  namespace UI
  {

    struct AddSubMenuMarshaller
    {
      static bool checkIntegrity(AddSubMenu& e);
      static bool checkIntegrityConst(const AddSubMenu& e);
    
      static bool fromString(const std::string& s,AddSubMenu& e);
      static const std::string toString(const AddSubMenu& e);
    
      static bool fromJSON(const Json::Value& s,AddSubMenu& e);
      static Json::Value toJSON(const AddSubMenu& e);
    };
  }
}

#endif
