#ifndef NSAPPLINKRPC_GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/GenericResponse_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct GenericResponse_responseMarshaller
  {
    static bool checkIntegrity(GenericResponse_response& e);
    static bool checkIntegrityConst(const GenericResponse_response& e);
  
    static bool fromString(const std::string& s,GenericResponse_response& e);
    static const std::string toString(const GenericResponse_response& e);
  
    static bool fromJSON(const Json::Value& s,GenericResponse_response& e);
    static Json::Value toJSON(const GenericResponse_response& e);
  };
}

#endif
