//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_SINGLETIRESTATUSMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_SINGLETIRESTATUSMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/SingleTireStatus.h"


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

  struct SingleTireStatusMarshaller
  {
    static bool checkIntegrity(SingleTireStatus& e);
    static bool checkIntegrityConst(const SingleTireStatus& e);
  
    static bool fromString(const std::string& s,SingleTireStatus& e);
    static const std::string toString(const SingleTireStatus& e);
  
    static bool fromJSON(const Json::Value& s,SingleTireStatus& e);
    static Json::Value toJSON(const SingleTireStatus& e);
  };
}

#endif
