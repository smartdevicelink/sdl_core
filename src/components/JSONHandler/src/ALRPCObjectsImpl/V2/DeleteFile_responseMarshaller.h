#ifndef NSAPPLINKRPCV2_DELETEFILE_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV2_DELETEFILE_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V2/DeleteFile_response.h"


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
