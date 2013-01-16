#include "GetSupportedLanguagesResponseMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/LanguageMarshaller.h"

using namespace NsRPC2Communication::UI;

bool GetSupportedLanguagesResponseMarshaller::checkIntegrity(GetSupportedLanguagesResponse& s)
{
  return checkIntegrityConst(s);
}


bool GetSupportedLanguagesResponseMarshaller::fromString(const std::string& s,GetSupportedLanguagesResponse& e)
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


const std::string GetSupportedLanguagesResponseMarshaller::toString(const GetSupportedLanguagesResponse& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetSupportedLanguagesResponseMarshaller::checkIntegrityConst(const GetSupportedLanguagesResponse& s)
{
  return true;
}


Json::Value GetSupportedLanguagesResponseMarshaller::toJSON(const GetSupportedLanguagesResponse& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["id"]=Json::Value(e.getId());
  json["result"]=Json::Value(Json::objectValue);
  NsAppLinkRPC::Result r(static_cast<NsAppLinkRPC::Result::ResultInternal>(e.getResult()));
  json["result"]["resultCode"]=NsAppLinkRPC::ResultMarshaller::toJSON(r);
  json["result"]["method"]=Json::Value("UI.GetSupportedLanguagesResponse");

  {
    unsigned int i=e.languages.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::LanguageMarshaller::toJSON(e.languages[i]);

    json["result"]["languages"]=j;
  }
  
  return json;
}


bool GetSupportedLanguagesResponseMarshaller::fromJSON(const Json::Value& json,GetSupportedLanguagesResponse& c)
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
    if(js["method"].asString().compare("UI.GetSupportedLanguagesResponse")) return false;

    if(!NsAppLinkRPC::ResultMarshaller::fromJSON(js["resultCode"],r))  return false;
    c.setResult(r.get());
    
    if(!js.isMember("languages") || !js["languages"].isArray())  return false;
    {
      unsigned int i=js["languages"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.languages.resize(i);
      while(i--)
      {
        NsAppLinkRPC::Language t;
        if(!NsAppLinkRPC::LanguageMarshaller::fromJSON(js["languages"][i],t))
          return false;
         c.languages[i]=t;
      }
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
