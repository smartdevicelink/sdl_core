#include "../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"
#include "PermissionItemMarshaller.h"

#include "OnPermissionsChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnPermissionsChangeMarshaller::checkIntegrity(OnPermissionsChange& s)
{
  return checkIntegrityConst(s);
}


bool OnPermissionsChangeMarshaller::fromString(const std::string& s,OnPermissionsChange& e)
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


const std::string OnPermissionsChangeMarshaller::toString(const OnPermissionsChange& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnPermissionsChangeMarshaller::checkIntegrityConst(const OnPermissionsChange& s)
{
  {
    unsigned int i=s.permissionItem.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!PermissionItemMarshaller::checkIntegrityConst(s.permissionItem[i]))   return false;
    }
  }
  return true;
}

Json::Value OnPermissionsChangeMarshaller::toJSON(const OnPermissionsChange& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnPermissionsChange");

  Json::Value j=Json::Value(Json::objectValue);

  j["permissionItem"]=Json::Value(Json::arrayValue);
  j["permissionItem"].resize(e.permissionItem.size());
  for(unsigned int i=0;i<e.permissionItem.size();i++)
    j["permissionItem"][i]=PermissionItemMarshaller::toJSON(e.permissionItem[i]);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnPermissionsChangeMarshaller::fromJSON(const Json::Value& js,OnPermissionsChange& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnPermissionsChange"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("permissionItem"))  return false;
    {
      const Json::Value& j=json["permissionItem"];
      if(!j.isArray())  return false;
      c.permissionItem.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          PermissionItem t;
          if(!PermissionItemMarshaller::fromJSON(j[i],t))
            return false;
          c.permissionItem[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

