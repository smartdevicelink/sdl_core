#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>
#include "JSONHandler/ButtonEventModeMarshaller.h"
#include "JSONHandler/OnButtonEventMarshaller.h"

using namespace RPC2Communication;

bool OnButtonEventMarshaller::fromString(const std::string& s,OnButtonEvent& e)
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


const std::string OnButtonEventMarshaller::toString(const OnButtonEvent& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}

// Request: {“request”: { “correlationID”: 1234, “parameters”: {“key”:”value”}, “name”:”FunctionName”}}

Json::Value OnButtonEventMarshaller::toJSON(const OnButtonEvent& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnButtonEvent");
  
  Json::Value j=Json::Value(Json::objectValue);
  
  j["name"]=Json::Value(e.mName);
  j["mode"]=ButtonEventModeMarshaller::toJSON(e.mMode);
  
  json["params"]=j;

  return json;
}


bool OnButtonEventMarshaller::fromJSON(const Json::Value& json,OnButtonEvent& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("Buttons.OnButtonEvent"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("name"))  return false;
    if(!j.isMember("mode"))  return false;

    if(!ButtonEventModeMarshaller::fromJSON(j["mode"],c.mMode))  return false;
    if(!j["name"].isString())  return false;
    c.mName=j["name"].asString();
  } 
  catch(...)
  {
    return false;
  }

  return true;
}
