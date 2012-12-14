#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_READDIDRESPONSEMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_READDIDRESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDIDResponse.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct ReadDIDResponseMarshaller
    {
      static bool checkIntegrity(ReadDIDResponse& e);
      static bool checkIntegrityConst(const ReadDIDResponse& e);
    
      static bool fromString(const std::string& s,ReadDIDResponse& e);
      static const std::string toString(const ReadDIDResponse& e);
    
      static bool fromJSON(const Json::Value& s,ReadDIDResponse& e);
      static Json::Value toJSON(const ReadDIDResponse& e);
    };
  }
}

#endif
