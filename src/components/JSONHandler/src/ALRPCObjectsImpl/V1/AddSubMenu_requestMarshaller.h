#ifndef NSAPPLINKRPCV1_ADDSUBMENU_REQUESTMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_ADDSUBMENU_REQUESTMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/AddSubMenu_request.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

namespace NsAppLinkRPCV1
{

  struct AddSubMenu_requestMarshaller
  {
    static bool checkIntegrity(AddSubMenu_request& e);
    static bool checkIntegrityConst(const AddSubMenu_request& e);
  
    static bool fromString(const std::string& s,AddSubMenu_request& e);
    static const std::string toString(const AddSubMenu_request& e);
  
    static bool fromJSON(const Json::Value& s,AddSubMenu_request& e);
    static Json::Value toJSON(const AddSubMenu_request& e);
  };
}

#endif
