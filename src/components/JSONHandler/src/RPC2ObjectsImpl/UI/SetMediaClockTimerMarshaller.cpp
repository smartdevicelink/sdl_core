#include "../include/JSONHandler/RPC2Objects/UI/SetMediaClockTimer.h"
#include "../src/ALRPCObjectsImpl/StartTimeMarshaller.h"
#include "../src/ALRPCObjectsImpl/UpdateModeMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SetMediaClockTimerMarshaller.h"

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

bool SetMediaClockTimerMarshaller::checkIntegrity(SetMediaClockTimer& s)
{
  return checkIntegrityConst(s);
}


bool SetMediaClockTimerMarshaller::fromString(const std::string& s,SetMediaClockTimer& e)
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


const std::string SetMediaClockTimerMarshaller::toString(const SetMediaClockTimer& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SetMediaClockTimerMarshaller::checkIntegrityConst(const SetMediaClockTimer& s)
{
  if(s.startTime && (!StartTimeMarshaller::checkIntegrityConst(s.startTime[0])))  return false;

  if(!UpdateModeMarshaller::checkIntegrityConst(s.updateMode))  return false;

  return true;
}


Json::Value SetMediaClockTimerMarshaller::toJSON(const SetMediaClockTimer& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.SetMediaClockTimer");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  if(e.startTime)
    json["params"]["startTime"]=StartTimeMarshaller::toJSON(e.startTime[0]);;
  json["params"]["updateMode"]=UpdateModeMarshaller::toJSON(e.updateMode);;
  return json;
}


bool SetMediaClockTimerMarshaller::fromJSON(const Json::Value& json,SetMediaClockTimer& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.SetMediaClockTimer"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(c.startTime)  delete c.startTime;
    c.startTime=0;
    if(js.isMember("startTime"))
    {
      c.startTime=new StartTime();
      if(!StartTimeMarshaller::fromJSON(js["startTime"],c.startTime[0]))  return false;
    }

    if(!js.isMember("updateMode") || !UpdateModeMarshaller::fromJSON(js["updateMode"],c.updateMode))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
