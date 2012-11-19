#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "../src/ALRPCObjectsImpl/StartTimeMarshaller.h"
#include "../src/ALRPCObjectsImpl/UpdateModeMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SetMediaClockTimerMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Mon Nov 19 10:37:39 2012
  source stamp	Mon Nov 19 10:17:20 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

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
  if(s.startTime && (!NsAppLinkRPC::StartTimeMarshaller::checkIntegrityConst(s.startTime[0])))  return false;

  if(!NsAppLinkRPC::UpdateModeMarshaller::checkIntegrityConst(s.updateMode))  return false;

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
    json["params"]["startTime"]=NsAppLinkRPC::StartTimeMarshaller::toJSON(e.startTime[0]);;
  json["params"]["updateMode"]=NsAppLinkRPC::UpdateModeMarshaller::toJSON(e.updateMode);;
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
      c.startTime=new NsAppLinkRPC::StartTime();
      if(!NsAppLinkRPC::StartTimeMarshaller::fromJSON(js["startTime"],c.startTime[0]))  return false;
    }

    if(!js.isMember("updateMode") || !NsAppLinkRPC::UpdateModeMarshaller::fromJSON(js["updateMode"],c.updateMode))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
