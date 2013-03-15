//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/SDLRPCObjects/V2/OnPermissionsChange.h"
#include "PermissionItemMarshaller.h"

#include "OnPermissionsChangeMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	RC
*/

using namespace NsSmartDeviceLinkRPCV2;


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

  json["permissionItem"]=Json::Value(Json::arrayValue);
  json["permissionItem"].resize(e.permissionItem.size());
  for(unsigned int i=0;i<e.permissionItem.size();i++)
    json["permissionItem"][i]=PermissionItemMarshaller::toJSON(e.permissionItem[i]);

  return json;
}


bool OnPermissionsChangeMarshaller::fromJSON(const Json::Value& json,OnPermissionsChange& c)
{
  try
  {
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

