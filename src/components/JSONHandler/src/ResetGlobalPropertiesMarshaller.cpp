#include "JSONHandler/ResetGlobalPropertiesMarshaller.h"
#include "ALRPCObjectsImpl/GlobalPropertyMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool ResetGlobalPropertiesMarshaller::fromString(const std::string& s,ResetGlobalProperties& e)
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


const std::string ResetGlobalPropertiesMarshaller::toString(const ResetGlobalProperties& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value ResetGlobalPropertiesMarshaller::toJSON(const ResetGlobalProperties& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.ResetGlobalProperties");
  
  Json::Value j=Json::Value(Json::objectValue);
  j["property"]=GlobalPropertyMarshaller::toJSON(e.mProperty);
    
  json["params"]=j;

  json["id"] = e.getID();

  return json;
}


bool ResetGlobalPropertiesMarshaller::fromJSON(const Json::Value& json,ResetGlobalProperties& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("UI.ResetGlobalProperties"))  
        return false;

    if(!json.isMember("params"))  return false;
    Json::Value j=json["params"];
    if(!j.isObject())  return false;

    if(!j.isMember("property"))  return false;
    if(!GlobalPropertyMarshaller::fromJSON(j["property"], c.mProperty)) return false;

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

