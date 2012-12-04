#ifndef NSAPPLINKRPCV1_ADDSUBMENU_RESPONSEMARSHALLER_INCLUDE
#define NSAPPLINKRPCV1_ADDSUBMENU_RESPONSEMARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "../include/JSONHandler/ALRPCObjects/V1/AddSubMenu_response.h"


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

  struct AddSubMenu_responseMarshaller
  {
    static bool checkIntegrity(AddSubMenu_response& e);
    static bool checkIntegrityConst(const AddSubMenu_response& e);
  
    static bool fromString(const std::string& s,AddSubMenu_response& e);
    static const std::string toString(const AddSubMenu_response& e);
  
    static bool fromJSON(const Json::Value& s,AddSubMenu_response& e);
    static Json::Value toJSON(const AddSubMenu_response& e);
  };
}

#endif
