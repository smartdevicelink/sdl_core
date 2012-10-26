#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "JSONHandler/OnAppRegisteredMarshaller.h"
#include "ALRPCObjectsImpl/LanguageMarshaller.h"

using namespace RPC2Communication;

bool OnAppRegisteredMarshaller::fromString(const std::string& s,OnAppRegistered& e)
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


const std::string OnAppRegisteredMarshaller::toString(const OnAppRegistered& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}

// Request: {“request”: { “correlationID”: 1234, “parameters”: {“key”:”value”}, “name”:”FunctionName”}}

Json::Value OnAppRegisteredMarshaller::toJSON(const OnAppRegistered& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnAppRegistered");
  
  Json::Value j=Json::Value(Json::objectValue);
  
  j["appName"]=Json::Value(e.mAppName);
  j["appIcon"]=Json::Value(e.mAppIcon);

  if(e.mVrSynonyms)
  {
        unsigned int size = e.mVrSynonyms->size();
        j["vrSynonym"]=Json::Value(Json::arrayValue);
        j["vrSynonym"].resize(size);
        for(unsigned int i = 0; i < size; ++i )
            j["vrSynonym"][i] = Json::Value(e.mVrSynonyms[0][i]);
  }
  
  j["isMediaApplication"] = Json::Value(e.mIsMediaApplication);
  j["languageDesired"] = LanguageMarshaller::toJSON(e.mLanguageDesired);
  
  json["params"]=j;

  return json;
}


bool OnAppRegisteredMarshaller::fromJSON(const Json::Value& json,OnAppRegistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("Buttons.OnAppRegistered"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("appName") || !j["appName"].isString())  return false;
    c.mAppName = j["appName"].asString();

    if(!j.isMember("appIcon") || !j["appIcon"].isString()) return false;
    c.mAppIcon = j["appIcon"].asString();

    if(j.isMember("vrSynonym"))
    {
        const Json::Value& vr = j["vrSynonym"];
        if(!vr.isArray()) return false;
        delete c.mVrSynonyms;
        c.mVrSynonyms = new std::vector<std::string>(vr.size());
        for (int i = 0; i < vr.size(); ++i )
        {
            if(!vr[i].isString())
                return false;
            c.mVrSynonyms[0][i] = vr[i].asString();
        }
    }

    if(!j.isMember("isMediaApplication") || !j["isMediaApplication"].isBool()) return false;
    c.mIsMediaApplication = j["isMediaApplication"].asBool();

    if(!j.isMember("languageDesired")) return false;
    if(!LanguageMarshaller::fromJSON(j["languageDesired"], c.mLanguageDesired)) return false;
  } 
  catch(...)
  {
    return false;
  }

  return true;
}
