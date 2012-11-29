#ifndef NSAPPLINKRPC_LISTFILES_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPC_LISTFILES_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/ListFiles_request.h"


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

  struct ListFiles_requestMarshaller
  {
    static bool checkIntegrity(ListFiles_request& e);
    static bool checkIntegrityConst(const ListFiles_request& e);
  
    static bool fromString(const std::string& s,ListFiles_request& e);
    static const std::string toString(const ListFiles_request& e);
  
    static bool fromJSON(const Json::Value& s,ListFiles_request& e);
    static Json::Value toJSON(const ListFiles_request& e);
  };
}

#endif
