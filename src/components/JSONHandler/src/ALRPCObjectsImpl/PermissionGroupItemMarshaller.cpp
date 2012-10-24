#include "../../include/JSONHandler/ALRPCObjects/PermissionGroupItem.h"
#include "PermissionStatusMarshaller.h"

#include "PermissionGroupItemMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


bool PermissionGroupItemMarshaller::checkIntegrity(PermissionGroupItem& s)
{
  return checkIntegrityConst(s);
}


bool PermissionGroupItemMarshaller::fromString(const std::string& s,PermissionGroupItem& e)
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


const std::string PermissionGroupItemMarshaller::toString(const PermissionGroupItem& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PermissionGroupItemMarshaller::checkIntegrityConst(const PermissionGroupItem& s)
{
  if(s.PermissionGroupName.length()>100)  return false;
  if(!PermissionStatusMarshaller::checkIntegrityConst(s.PermissionGroupStatus))  return false;
  return true;
}

Json::Value PermissionGroupItemMarshaller::toJSON(const PermissionGroupItem& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["PermissionGroupName"]=Json::Value(e.PermissionGroupName);

  json["PermissionGroupStatus"]=PermissionStatusMarshaller::toJSON(e.PermissionGroupStatus);


  return json;
}


bool PermissionGroupItemMarshaller::fromJSON(const Json::Value& json,PermissionGroupItem& c)
{
  try
  {
    if(!json.isObject())  return false;

    if(!json.isMember("PermissionGroupName"))  return false;
    {
      const Json::Value& j=json["PermissionGroupName"];
      if(!j.isString())  return false;
      c.PermissionGroupName=j.asString();
    }
    if(!json.isMember("PermissionGroupStatus"))  return false;
    {
      const Json::Value& j=json["PermissionGroupStatus"];
      if(!PermissionStatusMarshaller::fromJSON(j,c.PermissionGroupStatus))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

