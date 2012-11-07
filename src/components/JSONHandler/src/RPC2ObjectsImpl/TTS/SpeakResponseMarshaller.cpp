#include "../include/JSONHandler/RPC2Objects/TTS/SpeakResponse.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SpeakResponseMarshaller.h"

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::TTS;

bool SpeakResponseMarshaller::checkIntegrity(SpeakResponse& s)
{
  return checkIntegrityConst(s);
}


bool SpeakResponseMarshaller::fromString(const std::string& s,SpeakResponse& e)
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


const std::string SpeakResponseMarshaller::toString(const SpeakResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SpeakResponseMarshaller::checkIntegrityConst(const SpeakResponse& s)
{
  return true;
}


Json::Value SpeakResponseMarshaller::toJSON(const SpeakResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  AppLinkRPC::Result r(static_cast<AppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["_Result"]=AppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["_Method"]=Json::Value("TTS.SpeakResponse");

  return json;
}


bool SpeakResponseMarshaller::fromJSON(const Json::Value& json,SpeakResponse& c)
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

    Result r;
    if(!js.isMember("_Result") || !js["_Result"].isString())  return false;
    if(!js.isMember("_Method") || !js["_Method"].isString())  return false;
    if(js["_Method"].asString().compare("TTS.SpeakResponse")) return false;

    if(!AppLinkRPC::ResultMarshaller::fromJSON(js["_Result"],r))  return false;
    c.setResult(r.get());
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
