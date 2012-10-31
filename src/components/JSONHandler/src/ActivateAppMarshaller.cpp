#include "JSONHandler/ActivateAppMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool ActivateAppMarshaller::fromString(const std::string& s,ActivateApp& e)
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


const std::string ActivateAppMarshaller::toString(const ActivateApp& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value ActivateAppMarshaller::toJSON(const ActivateApp& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.activateApp");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["appName"] = Json::Value(Json::arrayValue);
  j["appName"].resize(e.mAppNames.size());
  for ( int i = 0; i < e.mAppNames.size(); ++i )
  {
    j["appName"][i] = Json::Value(e.mAppNames[i]);
  }
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool ActivateAppMarshaller::fromJSON(const Json::Value& json,ActivateApp& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("AppLinkCore.activateApp"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("appName") || !j["appName"].isArray()) return false;

    const Json::Value & appnames = j["appName"];
    c.mAppNames.clear();
    for ( int i = 0; i < appnames.size(); ++i )
    {
      if (!appnames[i].isString())
        return false;
      c.mAppNames.push_back(appnames[i].asString());
    } 

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

