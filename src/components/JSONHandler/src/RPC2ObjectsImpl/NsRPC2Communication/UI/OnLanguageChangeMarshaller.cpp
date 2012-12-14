#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/OnLanguageChange.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/OnLanguageChangeMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool OnLanguageChangeMarshaller::checkIntegrity(OnLanguageChange& s)
{
  return checkIntegrityConst(s);
}


bool OnLanguageChangeMarshaller::fromString(const std::string& s,OnLanguageChange& e)
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


const std::string OnLanguageChangeMarshaller::toString(const OnLanguageChange& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnLanguageChangeMarshaller::checkIntegrityConst(const OnLanguageChange& s)
{
  if(!NsAppLinkRPC::LanguageMarshaller::checkIntegrityConst(s.hmiDisplayLanguage))  return false;

  return true;
}


Json::Value OnLanguageChangeMarshaller::toJSON(const OnLanguageChange& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnLanguageChange");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["hmiDisplayLanguage"]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.hmiDisplayLanguage);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool OnLanguageChangeMarshaller::fromJSON(const Json::Value& json,OnLanguageChange& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.OnLanguageChange"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("hmiDisplayLanguage") || !NsAppLinkRPC::LanguageMarshaller::fromJSON(js["hmiDisplayLanguage"],c.hmiDisplayLanguage))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
