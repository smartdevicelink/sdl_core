#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnAppDeactivated.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "OnAppDeactivatedMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 08:20:05 2012
  source stamp	Mon Nov 19 07:04:40 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnAppDeactivatedMarshaller::checkIntegrity(OnAppDeactivated& s)
{
  return checkIntegrityConst(s);
}


bool OnAppDeactivatedMarshaller::fromString(const std::string& s,OnAppDeactivated& e)
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


const std::string OnAppDeactivatedMarshaller::toString(const OnAppDeactivated& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppDeactivatedMarshaller::checkIntegrityConst(const OnAppDeactivated& s)
{
  if(s.appName.length()>100)  return false;

  return true;
}


Json::Value OnAppDeactivatedMarshaller::toJSON(const OnAppDeactivated& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnAppDeactivated");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["appName"]=Json::Value(e.appName);;
  return json;
}


bool OnAppDeactivatedMarshaller::fromJSON(const Json::Value& json,OnAppDeactivated& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnAppDeactivated"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
