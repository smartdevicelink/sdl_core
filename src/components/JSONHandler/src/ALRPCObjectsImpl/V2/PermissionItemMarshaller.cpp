#include "../include/JSONHandler/ALRPCObjects/V2/PermissionItem.h"
#include "HMIPermissionsMarshaller.h"

#include "PermissionItemMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;


bool PermissionItemMarshaller::checkIntegrity(PermissionItem& s)
{
  return checkIntegrityConst(s);
}


bool PermissionItemMarshaller::fromString(const std::string& s,PermissionItem& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string PermissionItemMarshaller::toString(const PermissionItem& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PermissionItemMarshaller::checkIntegrityConst(const PermissionItem& s)
{
  if(!HMIPermissionsMarshaller::checkIntegrityConst(s.hmiPermissions))  return false;
  if(s.rpcName.length()>100)  return false;
  return true;
}

Json::Value PermissionItemMarshaller::toJSON(const PermissionItem& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["hmiPermissions"]=HMIPermissionsMarshaller::toJSON(e.hmiPermissions);

  json["rpcName"]=Json::Value(e.rpcName);


  return json;
}


bool PermissionItemMarshaller::fromJSON(const Json::Value& json,PermissionItem& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("hmiPermissions"))  return false;
    {
      const Json::Value& j=json["hmiPermissions"];
      if(!HMIPermissionsMarshaller::fromJSON(j,c.hmiPermissions))
        return false;
    }
    if(!json.isMember("rpcName"))  return false;
    {
      const Json::Value& j=json["rpcName"];
      if(!j.isString())  return false;
      c.rpcName=j.asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

