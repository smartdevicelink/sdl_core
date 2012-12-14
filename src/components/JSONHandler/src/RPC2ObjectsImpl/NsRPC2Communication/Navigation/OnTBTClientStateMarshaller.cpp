#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Navigation/OnTBTClientState.h"
#include "../src/ALRPCObjectsImpl/V1/TBTStateMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Navigation/OnTBTClientStateMarshaller.h"

/*
  interface	NsRPC2Communication::Navigation
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Navigation;

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
  if(!NsAppLinkRPC::TBTStateMarshaller::checkIntegrityConst(s.state))  return false;

  return true;
}


Json::Value OnTBTClientStateMarshaller::toJSON(const OnTBTClientState& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Navigation.OnTBTClientState");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["state"]=NsAppLinkRPC::TBTStateMarshaller::toJSON(e.state);;
  return json;
}


bool OnTBTClientStateMarshaller::fromJSON(const Json::Value& json,OnTBTClientState& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("Navigation.OnTBTClientState"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("state") || !NsAppLinkRPC::TBTStateMarshaller::fromJSON(js["state"],c.state))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
