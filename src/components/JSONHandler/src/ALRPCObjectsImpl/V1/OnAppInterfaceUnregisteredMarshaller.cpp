#include "../include/JSONHandler/ALRPCObjects/V1/OnAppInterfaceUnregistered.h"
#include "AppInterfaceUnregisteredReasonMarshaller.h"

#include "OnAppInterfaceUnregisteredMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Thu Dec 13 13:37:09 2012
  source stamp	Thu Dec 13 13:33:23 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnAppInterfaceUnregisteredMarshaller::checkIntegrity(OnAppInterfaceUnregistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppInterfaceUnregisteredMarshaller::fromString(const std::string& s,OnAppInterfaceUnregistered& e)
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


const std::string OnAppInterfaceUnregisteredMarshaller::toString(const OnAppInterfaceUnregistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppInterfaceUnregisteredMarshaller::checkIntegrityConst(const OnAppInterfaceUnregistered& s)
{
  if(!AppInterfaceUnregisteredReasonMarshaller::checkIntegrityConst(s.reason))  return false;
  return true;
}

Json::Value OnAppInterfaceUnregisteredMarshaller::toJSON(const OnAppInterfaceUnregistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnAppInterfaceUnregistered");

  Json::Value j=Json::Value(Json::objectValue);

  j["reason"]=AppInterfaceUnregisteredReasonMarshaller::toJSON(e.reason);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnAppInterfaceUnregisteredMarshaller::fromJSON(const Json::Value& js,OnAppInterfaceUnregistered& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnAppInterfaceUnregistered"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("reason"))  return false;
    {
      const Json::Value& j=json["reason"];
      if(!AppInterfaceUnregisteredReasonMarshaller::fromJSON(j,c.reason))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

