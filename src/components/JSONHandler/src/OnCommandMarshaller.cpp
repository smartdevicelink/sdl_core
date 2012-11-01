#include "JSONHandler/OnCommandMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool OnCommandMarshaller::fromString(const std::string& s,OnCommand& e)
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


const std::string OnCommandMarshaller::toString(const OnCommand& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value OnCommandMarshaller::toJSON(const OnCommand& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.OnCommand");
  
  Json::Value j=Json::Value(Json::objectValue);
  
  j["commandId"]=Json::Value(e.commandId);
  
  json["params"]=j;

  return json;
}


bool OnCommandMarshaller::fromJSON(const Json::Value& json,OnCommand& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.OnCommand"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("commandId") || !j["commandId"].isInt())  return false;
    c.commandId = j["commandId"].asInt();
  } 
  catch(...)
  {
    return false;
  }

  return true;
}