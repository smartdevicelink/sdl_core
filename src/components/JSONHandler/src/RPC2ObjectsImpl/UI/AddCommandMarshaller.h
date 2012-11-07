#ifndef RPC2COMMUNICATION_UI_ADDCOMMANDMARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_ADDCOMMANDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/UI/AddCommand.h"

namespace RPC2Communication
{
  namespace UI
  {

    struct AddCommandMarshaller
    {
      static bool checkIntegrity(AddCommand& e);
      static bool checkIntegrityConst(const AddCommand& e);
    
      static bool fromString(const std::string& s,AddCommand& e);
      static const std::string toString(const AddCommand& e);
    
      static bool fromJSON(const Json::Value& s,AddCommand& e);
      static Json::Value toJSON(const AddCommand& e);
    };
  }
}

#endif
