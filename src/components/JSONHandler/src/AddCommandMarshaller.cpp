#include "JSONHandler/AddCommandMarshaller.h"
#include "ALRPCObjectsImpl/MenuParamsMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool AddCommandMarshaller::fromString(const std::string& s,AddCommand& e)
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


const std::string AddCommandMarshaller::toString(const AddCommand& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value AddCommandMarshaller::toJSON(const AddCommand& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.AddCommand");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["cmdId"] = e.cmdId;
  j["menuParams"] = MenuParamsMarshaller::toJSON(e.menuParams);
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool AddCommandMarshaller::fromJSON(const Json::Value& json,AddCommand& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.AddCommand"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("cmdId") || !j["cmdId"].isInt()) return false;
    c.cmdId = j["cmdId"].asInt();

    if(!j.isMember("menuParams")) return false;
    if (!MenuParamsMarshaller::fromJSON(j["menuParams"],c.menuParams)) return false;

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

