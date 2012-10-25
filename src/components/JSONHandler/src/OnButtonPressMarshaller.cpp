#include "JSONHandler/OnButtonPressMarshaller.h"
#include "ALRPCObjectsImpl/ButtonPressModeMarshaller.h"
#include "ALRPCObjectsImpl/ButtonNameMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool OnButtonPressMarshaller::fromString(const std::string& s,OnButtonPress& e)
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


const std::string OnButtonPressMarshaller::toString(const OnButtonPress& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}

// Request: {“request”: { “correlationID”: 1234, “parameters”: {“key”:”value”}, “name”:”FunctionName”}}

Json::Value OnButtonPressMarshaller::toJSON(const OnButtonPress& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnButtonPress");
  
  Json::Value j=Json::Value(Json::objectValue);
  
  j["name"]=ButtonNameMarshaller::toJSON(e.mName);
  j["mode"]=ButtonPressModeMarshaller::toJSON(e.mMode);
  
  json["params"]=j;

  return json;
}


bool OnButtonPressMarshaller::fromJSON(const Json::Value& json,OnButtonPress& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("Buttons.OnButtonPress"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("name"))  return false;
    if(!j.isMember("mode"))  return false;

    if(!ButtonPressModeMarshaller::fromJSON(j["mode"],c.mMode))  return false;
    if(!ButtonNameMarshaller::fromJSON(j["name"], c.mName))  return false;
  } 
  catch(...)
  {
    return false;
  }

  return true;
}