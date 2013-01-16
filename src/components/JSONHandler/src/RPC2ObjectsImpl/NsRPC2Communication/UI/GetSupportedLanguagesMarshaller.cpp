#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetSupportedLanguages.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "GetSupportedLanguagesMarshaller.h"

using namespace NsRPC2Communication::UI;

bool GetSupportedLanguagesMarshaller::checkIntegrity(GetSupportedLanguages& s)
{
  return checkIntegrityConst(s);
}


bool GetSupportedLanguagesMarshaller::fromString(const std::string& s,GetSupportedLanguages& e)
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


const std::string GetSupportedLanguagesMarshaller::toString(const GetSupportedLanguages& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetSupportedLanguagesMarshaller::checkIntegrityConst(const GetSupportedLanguages& s)
{
  return true;
}


Json::Value GetSupportedLanguagesMarshaller::toJSON(const GetSupportedLanguages& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.GetSupportedLanguages");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetSupportedLanguagesMarshaller::fromJSON(const Json::Value& json,GetSupportedLanguages& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.GetSupportedLanguages"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}