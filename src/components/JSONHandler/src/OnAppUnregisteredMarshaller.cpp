#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "ALRPCObjectsImpl/AppInterfaceUnregisteredReasonMarshaller.h"
#include "JSONHandler/OnAppUnregisteredMarshaller.h"

using namespace RPC2Communication;

bool OnAppUnregisteredMarshaller::fromString(const std::string& s,OnAppUnregistered& e)
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


const std::string OnAppUnregisteredMarshaller::toString(const OnAppUnregistered& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}

// Request: {“request”: { “correlationID”: 1234, “parameters”: {“key”:”value”}, “name”:”FunctionName”}}

Json::Value OnAppUnregisteredMarshaller::toJSON(const OnAppUnregistered& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.OnAppUnregistered");
  
  Json::Value j=Json::Value(Json::objectValue);
  
  j["appName"]=Json::Value(e.mAppName);
  j["reason"]=AppInterfaceUnregisteredReasonMarshaller::toJSON(e.mReason);
  
  json["params"]=j;

  return json;
}


bool OnAppUnregisteredMarshaller::fromJSON(const Json::Value& json,OnAppUnregistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("AppLinkCore.OnAppUnregistered"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("appName") || !j["appName"].isString())  return false;
    c.mAppName = j["appName"].asString();

    if(!j.isMember("reason"))  return false;
    if(!AppInterfaceUnregisteredReasonMarshaller::fromJSON(j["reason"], c.mReason)) return false;

  } 
  catch(...)
  {
    return false;
  }

  return true;
}
