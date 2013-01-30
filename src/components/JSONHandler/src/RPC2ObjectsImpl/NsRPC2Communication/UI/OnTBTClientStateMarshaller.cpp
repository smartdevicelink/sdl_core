#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnTBTClientState.h"
#include "../src/ALRPCObjectsImpl/V2/TBTStateMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnTBTClientStateMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnTBTClientStateMarshaller::checkIntegrity(OnTBTClientState& s)
{
  return checkIntegrityConst(s);
}


bool OnTBTClientStateMarshaller::fromString(const std::string& s,OnTBTClientState& e)
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


const std::string OnTBTClientStateMarshaller::toString(const OnTBTClientState& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnTBTClientStateMarshaller::checkIntegrityConst(const OnTBTClientState& s)
{
  if(!NsAppLinkRPCV2::TBTStateMarshaller::checkIntegrityConst(s.state))  return false;

  return true;
}


Json::Value OnTBTClientStateMarshaller::toJSON(const OnTBTClientState& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnTBTClientState");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["state"]=NsAppLinkRPCV2::TBTStateMarshaller::toJSON(e.state);
  json["params"]["appId"]=Json::Value(e.appId);
  return json;
}


bool OnTBTClientStateMarshaller::fromJSON(const Json::Value& json,OnTBTClientState& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnTBTClientState"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("state") || !NsAppLinkRPCV2::TBTStateMarshaller::fromJSON(js["state"],c.state))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
