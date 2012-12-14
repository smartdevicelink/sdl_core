#ifndef NSAPPLINKRPCV2_READDID_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_READDID_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/ReadDID_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Dec 13 14:18:29 2012
  source stamp	Thu Dec 13 14:18:27 2012
  author	robok0der
*/

namespace NsAppLinkRPCV2
{

  struct ReadDID_requestMarshaller
  {
    static bool checkIntegrity(ReadDID_request& e);
    static bool checkIntegrityConst(const ReadDID_request& e);
  
    static bool fromString(const std::string& s,ReadDID_request& e);
    static const std::string toString(const ReadDID_request& e);
  
    static bool fromJSON(const Json::Value& s,ReadDID_request& e);
    static Json::Value toJSON(const ReadDID_request& e);
  };
}

#endif
