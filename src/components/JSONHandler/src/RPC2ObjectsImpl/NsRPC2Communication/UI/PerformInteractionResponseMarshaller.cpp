#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "../src/ALRPCObjectsImpl/V2/TriggerSourceMarshaller.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/PerformInteractionResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool PerformInteractionResponseMarshaller::checkIntegrity(PerformInteractionResponse& s)
{
  return checkIntegrityConst(s);
}


bool PerformInteractionResponseMarshaller::fromString(const std::string& s,PerformInteractionResponse& e)
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


const std::string PerformInteractionResponseMarshaller::toString(const PerformInteractionResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool PerformInteractionResponseMarshaller::checkIntegrityConst(const PerformInteractionResponse& s)
{
  if(s.choiceID && (s.choiceID[0]>2000000000))  return false;

  if(s.triggerSource && (!NsAppLinkRPCV2::TriggerSourceMarshaller::checkIntegrityConst(s.triggerSource[0])))  return false;

  return true;
}


Json::Value PerformInteractionResponseMarshaller::toJSON(const PerformInteractionResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPCV2::Result r(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.PerformInteractionResponse");

  if(e.choiceID)
    json["result"]["choiceID"]=Json::Value(e.choiceID[0]);;
  if(e.triggerSource)
    json["result"]["triggerSource"]=NsAppLinkRPCV2::TriggerSourceMarshaller::toJSON(e.triggerSource[0]);;
  return json;
}


bool PerformInteractionResponseMarshaller::fromJSON(const Json::Value& json,PerformInteractionResponse& c)
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

    NsAppLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("UI.PerformInteractionResponse")) return false;

    if(!NsAppLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(c.choiceID)  delete c.choiceID;
    c.choiceID=0;
    if(js.isMember("choiceID"))
    {
      if(!js["choiceID"].isInt())  return false;
      c.choiceID=new unsigned int();
      c.choiceID[0]=js["choiceID"].asInt();
      if(c.choiceID[0]>2000000000)  return false;

    }

    if(c.triggerSource)  delete c.triggerSource;
    c.triggerSource=0;
    if(js.isMember("triggerSource"))
    {
      c.triggerSource=new NsAppLinkRPCV2::TriggerSource();
      if(!NsAppLinkRPCV2::TriggerSourceMarshaller::fromJSON(js["triggerSource"],c.triggerSource[0]))  return false;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
