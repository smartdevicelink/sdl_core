#ifndef SENDDATARESPONSEMARSHALLER_H
#define SENDDATARESPONSEMARSHALLER_H

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/RPC2Objects/AppLinkCore/SendDataResponse.h"

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    struct SendDataResponseMarshaller
    {
      static bool checkIntegrity(SendDataResponse& e);
      static bool checkIntegrityConst(const SendDataResponse& e);

      static bool fromString(const std::string& s,SendDataResponse& e);
      static const std::string toString(const SendDataResponse& e);

      static bool fromJSON(const Json::Value& s,SendDataResponse& e);
      static Json::Value toJSON(const SendDataResponse& e);
    };
  }
}

#endif // SENDDATARESPONSEMARSHALLER_H
