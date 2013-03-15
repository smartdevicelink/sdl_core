//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_REGISTERAPPINTERFACE_REQUESTMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_REGISTERAPPINTERFACE_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/RegisterAppInterface_request.h"


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

  struct RegisterAppInterface_requestMarshaller
  {
    static bool checkIntegrity(RegisterAppInterface_request& e);
    static bool checkIntegrityConst(const RegisterAppInterface_request& e);
  
    static bool fromString(const std::string& s,RegisterAppInterface_request& e);
    static const std::string toString(const RegisterAppInterface_request& e);
  
    static bool fromJSON(const Json::Value& s,RegisterAppInterface_request& e);
    static Json::Value toJSON(const RegisterAppInterface_request& e);
  };
}

#endif
