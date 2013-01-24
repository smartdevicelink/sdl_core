#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/SliderResponse.h"
#include "../src/ALRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/SliderResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool SliderResponseMarshaller::checkIntegrity(SliderResponse& s)
{
  return checkIntegrityConst(s);
}


bool SliderResponseMarshaller::fromString(const std::string& s,SliderResponse& e)
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


const std::string SliderResponseMarshaller::toString(const SliderResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SliderResponseMarshaller::checkIntegrityConst(const SliderResponse& s)
{
  if(s.sliderPosition<1 || s.sliderPosition>26)  return false;

  return true;
}


Json::Value SliderResponseMarshaller::toJSON(const SliderResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPCV2::Result r(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.SliderResponse");

  json["result"]["sliderPosition"]=Json::Value(e.sliderPosition);;
  return json;
}


bool SliderResponseMarshaller::fromJSON(const Json::Value& json,SliderResponse& c)
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
    if(js["method"].asString().compare("UI.SliderResponse")) return false;

    if(!NsAppLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("sliderPosition") || !js["sliderPosition"].isInt())  return false;
    c.sliderPosition=js["sliderPosition"].asInt();
    if(c.sliderPosition<1 || c.sliderPosition>26)  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
