#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/AlertManeuverResponse.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Navigation/AlertManeuverResponseMarshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;

bool AlertManeuverResponseMarshaller::checkIntegrity(AlertManeuverResponse& s)
{
  return checkIntegrityConst(s);
}


bool AlertManeuverResponseMarshaller::fromString(const std::string& s,AlertManeuverResponse& e)
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


const std::string AlertManeuverResponseMarshaller::toString(const AlertManeuverResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool AlertManeuverResponseMarshaller::checkIntegrityConst(const AlertManeuverResponse& s)
{
  return true;
}


Json::Value AlertManeuverResponseMarshaller::toJSON(const AlertManeuverResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("Navigation.AlertManeuverResponse");

  return json;
}


bool AlertManeuverResponseMarshaller::fromJSON(const Json::Value& json,AlertManeuverResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("result")) return false;

    Json::Value js=json["result"];
    if(!js.isObject())  return false;

    NsAppLinkRPC::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("Navigation.AlertManeuverResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
