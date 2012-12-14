#ifndef NSRPC2COMMUNICATION_PHONE_DIALNUMBERMARSHALLER_INCLUDE
#define NSRPC2COMMUNICATION_PHONE_DIALNUMBERMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Phone/DialNumber.h"

namespace NsRPC2Communication
{
  namespace Phone
  {

    struct DialNumberMarshaller
    {
      static bool checkIntegrity(DialNumber& e);
      static bool checkIntegrityConst(const DialNumber& e);
    
      static bool fromString(const std::string& s,DialNumber& e);
      static const std::string toString(const DialNumber& e);
    
      static bool fromJSON(const Json::Value& s,DialNumber& e);
      static Json::Value toJSON(const DialNumber& e);
    };
  }
}

#endif
