#include "../include/JSONHandler/RPC2Objects/UI/SetMediaClockTimerResponse.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SetMediaClockTimerResponseMarshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;

bool SetMediaClockTimerResponseMarshaller::checkIntegrity(SetMediaClockTimerResponse& s)
{
  return checkIntegrityConst(s);
}


bool SetMediaClockTimerResponseMarshaller::fromString(const std::string& s,SetMediaClockTimerResponse& e)
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


const std::string SetMediaClockTimerResponseMarshaller::toString(const SetMediaClockTimerResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetMediaClockTimerResponseMarshaller::checkIntegrityConst(const SetMediaClockTimerResponse& s)
{
  return true;
}


Json::Value SetMediaClockTimerResponseMarshaller::toJSON(const SetMediaClockTimerResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  AppLinkRPC::Result r(static_cast<AppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["_Result"]=AppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["_Method"]=Json::Value("UI.SetMediaClockTimerResponse");

  return json;
}


bool SetMediaClockTimerResponseMarshaller::fromJSON(const Json::Value& json,SetMediaClockTimerResponse& c)
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

    Result r;
    if(!js.isMember("_Result") || !js["_Result"].isString())  return false;
    if(!js.isMember("_Method") || !js["_Method"].isString())  return false;
    if(js["_Method"].asString().compare("UI.SetMediaClockTimerResponse")) return false;

    if(!AppLinkRPC::ResultMarshaller::fromJSON(js["_Result"],r))  return false;
    c.setResult(r.get());
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
