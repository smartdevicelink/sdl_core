#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/ChangeLanguageRegistrationResponse.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/VR/ChangeLanguageRegistrationResponseMarshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;

bool ChangeLanguageRegistrationResponseMarshaller::checkIntegrity(ChangeLanguageRegistrationResponse& s)
{
  return checkIntegrityConst(s);
}


bool ChangeLanguageRegistrationResponseMarshaller::fromString(const std::string& s,ChangeLanguageRegistrationResponse& e)
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


const std::string ChangeLanguageRegistrationResponseMarshaller::toString(const ChangeLanguageRegistrationResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ChangeLanguageRegistrationResponseMarshaller::checkIntegrityConst(const ChangeLanguageRegistrationResponse& s)
{
  return true;
}


Json::Value ChangeLanguageRegistrationResponseMarshaller::toJSON(const ChangeLanguageRegistrationResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("VR.ChangeLanguageRegistrationResponse");

  return json;
}


bool ChangeLanguageRegistrationResponseMarshaller::fromJSON(const Json::Value& json,ChangeLanguageRegistrationResponse& c)
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
    if(js["method"].asString().compare("VR.ChangeLanguageRegistrationResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
