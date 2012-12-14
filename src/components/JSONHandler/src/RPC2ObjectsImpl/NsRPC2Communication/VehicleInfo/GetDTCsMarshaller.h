#ifndef NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCSMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_VEHICLEINFO_GETDTCSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VehicleInfo/GetDTCs.h"

namespace NsRPC2Communication
{
  namespace VehicleInfo
  {

    struct GetDTCsMarshaller
    {
      static bool checkIntegrity(GetDTCs& e);
      static bool checkIntegrityConst(const GetDTCs& e);
    
      static bool fromString(const std::string& s,GetDTCs& e);
      static const std::string toString(const GetDTCs& e);
    
      static bool fromJSON(const Json::Value& s,GetDTCs& e);
      static Json::Value toJSON(const GetDTCs& e);
    };
  }
}

#endif
