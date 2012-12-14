#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetLanguage.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/TTS/GetLanguageMarshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;

bool GetLanguageMarshaller::checkIntegrity(GetLanguage& s)
{
  return checkIntegrityConst(s);
}


bool GetLanguageMarshaller::fromString(const std::string& s,GetLanguage& e)
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


const std::string GetLanguageMarshaller::toString(const GetLanguage& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetLanguageMarshaller::checkIntegrityConst(const GetLanguage& s)
{
  return true;
}


Json::Value GetLanguageMarshaller::toJSON(const GetLanguage& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("TTS.GetLanguage");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetLanguageMarshaller::fromJSON(const Json::Value& json,GetLanguage& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("TTS.GetLanguage"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
