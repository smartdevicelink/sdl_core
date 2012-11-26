#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppUnregistered.h"
#include "../src/ALRPCObjectsImpl/AppInterfaceUnregisteredReasonMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppUnregisteredMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Nov 20 13:32:23 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool OnAppUnregisteredMarshaller::checkIntegrity(OnAppUnregistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppUnregisteredMarshaller::fromString(const std::string& s,OnAppUnregistered& e)
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


const std::string OnAppUnregisteredMarshaller::toString(const OnAppUnregistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppUnregisteredMarshaller::checkIntegrityConst(const OnAppUnregistered& s)
{
  if(s.appName.length()>100)  return false;

  if(s.reason && (!NsAppLinkRPC::AppInterfaceUnregisteredReasonMarshaller::checkIntegrityConst(s.reason[0])))  return false;

  return true;
}


Json::Value OnAppUnregisteredMarshaller::toJSON(const OnAppUnregistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.OnAppUnregistered");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["appName"]=Json::Value(e.appName);;
  if(e.reason)
    json["params"]["reason"]=NsAppLinkRPC::AppInterfaceUnregisteredReasonMarshaller::toJSON(e.reason[0]);;
  return json;
}


bool OnAppUnregisteredMarshaller::fromJSON(const Json::Value& json,OnAppUnregistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.OnAppUnregistered"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;

    if(c.reason)  delete c.reason;
    c.reason=0;
    if(js.isMember("reason"))
    {
      c.reason=new NsAppLinkRPC::AppInterfaceUnregisteredReason();
      if(!NsAppLinkRPC::AppInterfaceUnregisteredReasonMarshaller::fromJSON(js["reason"],c.reason[0]))  return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
