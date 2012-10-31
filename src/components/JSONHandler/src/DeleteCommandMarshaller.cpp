#include "JSONHandler/DeleteCommandMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool DeleteCommandMarshaller::fromString(const std::string& s,DeleteCommand& e)
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


const std::string DeleteCommandMarshaller::toString(const DeleteCommand& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value DeleteCommandMarshaller::toJSON(const DeleteCommand& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.DeleteCommand");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["cmdId"] = Json::Value(e.cmdId);
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool DeleteCommandMarshaller::fromJSON(const Json::Value& json,DeleteCommand& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.DeleteCommand"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("cmdId") || !j["cmdId"].isInt()) return false;
    c.cmdId = j["cmdId"].asInt();

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

