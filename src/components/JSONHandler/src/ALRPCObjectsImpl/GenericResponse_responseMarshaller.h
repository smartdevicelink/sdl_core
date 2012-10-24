#ifndef GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE
#define GENERICRESPONSE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <jsoncpp/json.h>

#include "../../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


struct GenericResponse_responseMarshaller
{
  static bool checkIntegrity(GenericResponse_response& e);
  static bool checkIntegrityConst(const GenericResponse_response& e);

  static bool fromString(const std::string& s,GenericResponse_response& e);
  static const std::string toString(const GenericResponse_response& e);

  static bool fromJSON(const Json::Value& s,GenericResponse_response& e);
  static Json::Value toJSON(const GenericResponse_response& e);
};
#endif
