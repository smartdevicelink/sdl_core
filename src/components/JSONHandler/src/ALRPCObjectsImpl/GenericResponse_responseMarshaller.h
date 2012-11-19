#ifndef NSAPPLINKRPC_GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPC_GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 10:37:06 2012
  source stamp	Mon Nov 19 10:35:56 2012
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
