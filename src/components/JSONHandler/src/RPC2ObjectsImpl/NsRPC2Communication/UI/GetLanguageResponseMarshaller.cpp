#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetLanguageResponse.h"
#include "../src/SDLRPCObjectsImpl/V2/LanguageMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/GetLanguageResponseMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool GetLanguageResponseMarshaller::checkIntegrity(GetLanguageResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetLanguageResponseMarshaller::fromString(const std::string& s,GetLanguageResponse& e)
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


const std::string GetLanguageResponseMarshaller::toString(const GetLanguageResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetLanguageResponseMarshaller::checkIntegrityConst(const GetLanguageResponse& s)
{
  if(!NsSmartDeviceLinkRPCV2::LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;

  return true;
}


Json::Value GetLanguageResponseMarshaller::toJSON(const GetLanguageResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsSmartDeviceLinkRPCV2::Result r(static_cast<NsSmartDeviceLinkRPCV2::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsSmartDeviceLinkRPCV2::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.GetLanguageResponse");

  json["result"]["hmiDisplayLanguage"]=NsSmartDeviceLinkRPCV2::LanguageMarshaller::toJSON(e.hmiDisplayLanguage);;
  return json;
}


bool GetLanguageResponseMarshaller::fromJSON(const Json::Value& json,GetLanguageResponse& c)
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

    NsSmartDeviceLinkRPCV2::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("UI.GetLanguageResponse")) return false;

    if(!NsSmartDeviceLinkRPCV2::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("hmiDisplayLanguage") || !NsSmartDeviceLinkRPCV2::LanguageMarshaller::fromJSON(js["hmiDisplayLanguage"],c.hmiDisplayLanguage))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
