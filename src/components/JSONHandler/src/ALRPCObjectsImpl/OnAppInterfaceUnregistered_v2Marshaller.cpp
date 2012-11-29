#include "../include/JSONHandler/ALRPCObjects/OnAppInterfaceUnregistered_v2.h"
#include "AppInterfaceUnregisteredReason_v2Marshaller.h"

#include "OnAppInterfaceUnregistered_v2Marshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool OnAppInterfaceUnregistered_v2Marshaller::checkIntegrity(OnAppInterfaceUnregistered_v2& s)
{
  return checkIntegrityConst(s);
}


bool OnAppInterfaceUnregistered_v2Marshaller::fromString(const std::string& s,OnAppInterfaceUnregistered_v2& e)
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


const std::string OnAppInterfaceUnregistered_v2Marshaller::toString(const OnAppInterfaceUnregistered_v2& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppInterfaceUnregistered_v2Marshaller::checkIntegrityConst(const OnAppInterfaceUnregistered_v2& s)
{
  if(!AppInterfaceUnregisteredReason_v2Marshaller::checkIntegrityConst(s.reason))  return false;
  return true;
}

Json::Value OnAppInterfaceUnregistered_v2Marshaller::toJSON(const OnAppInterfaceUnregistered_v2& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["notification"]=Json::Value(Json::objectValue);
  json["notification"]["name"]=Json::Value("OnAppInterfaceUnregistered_v2");

  Json::Value j=Json::Value(Json::objectValue);

  j["reason"]=AppInterfaceUnregisteredReason_v2Marshaller::toJSON(e.reason);

  json["notification"]["parameters"]=j;
  return json;
}


bool OnAppInterfaceUnregistered_v2Marshaller::fromJSON(const Json::Value& js,OnAppInterfaceUnregistered_v2& c)
{
  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("notification"))  return false;

    if(!js["notification"].isObject())  return false;
    const Json::Value& j2=js["notification"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("OnAppInterfaceUnregistered_v2"))  return false;

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(!json.isMember("reason"))  return false;
    {
      const Json::Value& j=json["reason"];
      if(!AppInterfaceUnregisteredReason_v2Marshaller::fromJSON(j,c.reason))
        return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

