#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/OnAppRegistered.h"
#include "../src/ALRPCObjectsImpl/V2/HMIApplicationMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/OnAppRegisteredMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool OnAppRegisteredMarshaller::checkIntegrity(OnAppRegistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppRegisteredMarshaller::fromString(const std::string& s,OnAppRegistered& e)
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


const std::string OnAppRegisteredMarshaller::toString(const OnAppRegistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppRegisteredMarshaller::checkIntegrityConst(const OnAppRegistered& s)
{
  if(!NsAppLinkRPCV2::HMIApplicationMarshaller::checkIntegrityConst(s.application))  return false;

  return true;
}


Json::Value OnAppRegisteredMarshaller::toJSON(const OnAppRegistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.OnAppRegistered");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["application"]=NsAppLinkRPCV2::HMIApplicationMarshaller::toJSON(e.application);;
  return json;
}


bool OnAppRegisteredMarshaller::fromJSON(const Json::Value& json,OnAppRegistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.OnAppRegistered"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("application") || !NsAppLinkRPCV2::HMIApplicationMarshaller::fromJSON(js["application"],c.application))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
