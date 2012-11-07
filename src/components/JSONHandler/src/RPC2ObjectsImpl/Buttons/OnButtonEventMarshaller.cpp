#include "../include/JSONHandler/RPC2Objects/Buttons/OnButtonEvent.h"
#include "../src/ALRPCObjectsImpl/ButtonNameMarshaller.h"
#include "../src/ALRPCObjectsImpl/ButtonEventModeMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "OnButtonEventMarshaller.h"

/*
  interface	RPC2Communication::Buttons
  version	1.2
  generated at	Wed Nov  7 11:25:50 2012
  source stamp	Wed Nov  7 09:31:20 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::Buttons;

bool OnButtonEventMarshaller::checkIntegrity(OnButtonEvent& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonEventMarshaller::fromString(const std::string& s,OnButtonEvent& e)
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


const std::string OnButtonEventMarshaller::toString(const OnButtonEvent& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonEventMarshaller::checkIntegrityConst(const OnButtonEvent& s)
{
  if(!ButtonNameMarshaller::checkIntegrityConst(s.name))  return false;

  if(!ButtonEventModeMarshaller::checkIntegrityConst(s.mode))  return false;

  return true;
}


Json::Value OnButtonEventMarshaller::toJSON(const OnButtonEvent& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnButtonEvent");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["name"]=ButtonNameMarshaller::toJSON(e.name);;
  json["params"]["mode"]=ButtonEventModeMarshaller::toJSON(e.mode);;
  return json;
}


bool OnButtonEventMarshaller::fromJSON(const Json::Value& json,OnButtonEvent& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("Buttons.OnButtonEvent"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("name") || !ButtonNameMarshaller::fromJSON(js["name"],c.name))  return false;

    if(!js.isMember("mode") || !ButtonEventModeMarshaller::fromJSON(js["mode"],c.mode))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
