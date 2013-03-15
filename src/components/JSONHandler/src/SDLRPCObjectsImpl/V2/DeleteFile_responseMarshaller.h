//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSSMARTDEVICELINKRPCV2_DELETEFILE_RESPONSEMARSHALLER_INCLUDE
#define NSSMARTDEVICELINKRPCV2_DELETEFILE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/SDLRPCObjects/V2/DeleteFile_response.h"


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

  struct DeleteFile_responseMarshaller
  {
    static bool checkIntegrity(DeleteFile_response& e);
    static bool checkIntegrityConst(const DeleteFile_response& e);
  
    static bool fromString(const std::string& s,DeleteFile_response& e);
    static const std::string toString(const DeleteFile_response& e);
  
    static bool fromJSON(const Json::Value& s,DeleteFile_response& e);
    static Json::Value toJSON(const DeleteFile_response& e);
  };
}

#endif
