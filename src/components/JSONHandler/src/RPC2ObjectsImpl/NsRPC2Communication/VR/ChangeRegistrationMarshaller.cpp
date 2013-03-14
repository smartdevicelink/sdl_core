#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/ChangeRegistration.h"
#include "../src/SDLRPCObjectsImpl/V2/LanguageMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/ChangeRegistrationMarshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::VR;

bool ChangeRegistrationMarshaller::checkIntegrity(ChangeRegistration& s)
{
  return checkIntegrityConst(s);
}


bool ChangeRegistrationMarshaller::fromString(const std::string& s,ChangeRegistration& e)
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


const std::string ChangeRegistrationMarshaller::toString(const ChangeRegistration& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChangeRegistrationMarshaller::checkIntegrityConst(const ChangeRegistration& s)
{
  if(!NsSmartDeviceLinkRPCV2::LanguageMarshaller::checkIntegrityConst(s.language))  return false;

  return true;
}


Json::Value ChangeRegistrationMarshaller::toJSON(const ChangeRegistration& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("VR.ChangeRegistration");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  json["params"]["language"]=NsSmartDeviceLinkRPCV2::LanguageMarshaller::toJSON(e.language);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ChangeRegistrationMarshaller::fromJSON(const Json::Value& json,ChangeRegistration& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("VR.ChangeRegistration"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("language") || !NsSmartDeviceLinkRPCV2::LanguageMarshaller::fromJSON(js["language"],c.language))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
