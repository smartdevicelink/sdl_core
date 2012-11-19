#ifndef SENDDATAMARSHALLER_H
#define SENDDATAMARSHALLER_H

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/AppLinkCore/SendData.h"

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    struct SendDataMarshaller
    {
      static bool checkIntegrity(SendData& e);
      static bool checkIntegrityConst(const SendData& e);

      static bool fromString(const std::string& s,SendData& e);
      static const std::string toString(const SendData& e);

      static bool fromJSON(const Json::Value& s,SendData& e);
      static Json::Value toJSON(const SendData& e);
    };
  }
}

#endif // SENDDATAMARSHALLER_H
