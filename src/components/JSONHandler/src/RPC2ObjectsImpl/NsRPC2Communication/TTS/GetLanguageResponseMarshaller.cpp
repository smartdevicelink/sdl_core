#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetLanguageResponse.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetLanguageResponseMarshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;

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
  if(!NsAppLinkRPC::LanguageMarshaller::checkIntegrityConst(s.language))  return false;

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
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("TTS.GetLanguageResponse");

  json["result"]["language"]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.language);;
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

    NsAppLinkRPC::Result r;
    if(!js.isMember("resultCode") || !js["resultCode"].isString())  return false;
    if(!js.isMember("method") || !js["method"].isString())  return false;
    if(js["method"].asString().compare("TTS.GetLanguageResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    if(!js.isMember("language") || !NsAppLinkRPC::LanguageMarshaller::fromJSON(js["language"],c.language))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
