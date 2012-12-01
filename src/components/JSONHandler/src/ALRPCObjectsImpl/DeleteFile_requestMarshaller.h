#ifndef NSAPPLINKRPC_DELETEFILE_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_DELETEFILE_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/DeleteFile_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

namespace NsAppLinkRPC
{

  struct DeleteFile_requestMarshaller
  {
    static bool checkIntegrity(DeleteFile_request& e);
    static bool checkIntegrityConst(const DeleteFile_request& e);
  
    static bool fromString(const std::string& s,DeleteFile_request& e);
    static const std::string toString(const DeleteFile_request& e);
  
    static bool fromJSON(const Json::Value& s,DeleteFile_request& e);
    static Json::Value toJSON(const DeleteFile_request& e);
  };
}

#endif
