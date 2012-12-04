#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/ChangeLanguageRegistration.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/ChangeLanguageRegistrationMarshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Tue Dec  4 16:38:13 2012
  source stamp	Tue Dec  4 16:37:04 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;

bool ChangeLanguageRegistrationMarshaller::checkIntegrity(ChangeLanguageRegistration& s)
{
  return checkIntegrityConst(s);
}


bool ChangeLanguageRegistrationMarshaller::fromString(const std::string& s,ChangeLanguageRegistration& e)
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


const std::string ChangeLanguageRegistrationMarshaller::toString(const ChangeLanguageRegistration& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChangeLanguageRegistrationMarshaller::checkIntegrityConst(const ChangeLanguageRegistration& s)
{
  if(!NsAppLinkRPC::LanguageMarshaller::checkIntegrityConst(s.language))  return false;

  return true;
}


Json::Value ChangeLanguageRegistrationMarshaller::toJSON(const ChangeLanguageRegistration& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("TTS.ChangeLanguageRegistration");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["language"]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.language);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ChangeLanguageRegistrationMarshaller::fromJSON(const Json::Value& json,ChangeLanguageRegistration& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("TTS.ChangeLanguageRegistration"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("language") || !NsAppLinkRPC::LanguageMarshaller::fromJSON(js["language"],c.language))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
