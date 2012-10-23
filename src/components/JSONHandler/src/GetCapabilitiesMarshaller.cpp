#include "JSONHandler/GetCapabilitiesMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool GetCapabilitiesMarshaller::fromString(const std::string& s,GetCapabilities& e)
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


const std::string GetCapabilitiesMarshaller::toString(const GetCapabilities& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value GetCapabilitiesMarshaller::toJSON(const GetCapabilities& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.GetCapabilities");
  
  json["id"] = e.getID();

  return json;
}


bool GetCapabilitiesMarshaller::fromJSON(const Json::Value& json,GetCapabilities& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("method") 
        || !json["method"].isString() )
        return false;
    if (json["method"].asString().compare("Buttons.GetCapabilities"))  
        return false;

    if(!json.isMember("id")) return false;

    c.setID( json["id"].asInt() );
  } 
  catch(...)
  {
    return false;
  }

  return true;
}

