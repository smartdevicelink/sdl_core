//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/OnLanguageChange.h"
#include "../src/SDLRPCObjectsImpl/V2/LanguageMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/OnLanguageChangeMarshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::TTS;

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
  if(!NsSmartDeviceLinkRPCV2::LanguageMarshaller::checkIntegrityConst(s.language))  return false;

  return true;
}


Json::Value OnLanguageChangeMarshaller::toJSON(const OnLanguageChange& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("TTS.OnLanguageChange");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["language"]=NsSmartDeviceLinkRPCV2::LanguageMarshaller::toJSON(e.language);;
  return json;
}


bool OnLanguageChangeMarshaller::fromJSON(const Json::Value& json,OnLanguageChange& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("TTS.OnLanguageChange"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("language") || !NsSmartDeviceLinkRPCV2::LanguageMarshaller::fromJSON(js["language"],c.language))  return false;

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
