#include "../../include/JSONHandler/ALRPCObjects/OnPermissionsChange.h"
#include "PermissionGroupItemMarshaller.h"

#include "OnPermissionsChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


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
    unsigned int i=s.PermissionGroupItems.size();
    if(i>100 || i<1)  return false;
    while(i--)
    {
    if(!PermissionGroupItemMarshaller::checkIntegrityConst(s.PermissionGroupItems[i]))   return false;
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

  j["PermissionGroupItems"]=Json::Value(Json::arrayValue);
  j["PermissionGroupItems"].resize(e.PermissionGroupItems.size());
  for(unsigned int i=0;i<e.PermissionGroupItems.size();i++)
    j["PermissionGroupItems"][i]=PermissionGroupItemMarshaller::toJSON(e.PermissionGroupItems[i]);

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
    if(!json.isMember("PermissionGroupItems"))  return false;
    {
      const Json::Value& j=json["PermissionGroupItems"];
      if(!j.isArray())  return false;
      c.PermissionGroupItems.resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        {
          PermissionGroupItem t;
          if(!PermissionGroupItemMarshaller::fromJSON(j[i],t))
            return false;
          c.PermissionGroupItems[i]=t;
        }

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

