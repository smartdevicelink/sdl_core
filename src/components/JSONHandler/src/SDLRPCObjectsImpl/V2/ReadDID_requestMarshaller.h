//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_READDID_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_READDID_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/ReadDID_request.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

namespace NsSmartDeviceLinkRPCV2
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
