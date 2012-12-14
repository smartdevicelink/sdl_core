#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_READDIDMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_READDIDMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/ReadDID.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct ReadDIDMarshaller
    {
      static bool checkIntegrity(ReadDID& e);
      static bool checkIntegrityConst(const ReadDID& e);
    
      static bool fromString(const std::string& s,ReadDID& e);
      static const std::string toString(const ReadDID& e);
    
      static bool fromJSON(const Json::Value& s,ReadDID& e);
      static Json::Value toJSON(const ReadDID& e);
    };
  }
}

#endif
