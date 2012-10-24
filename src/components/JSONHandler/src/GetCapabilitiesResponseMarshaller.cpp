#include "JSONHandler/GetCapabilitiesResponseMarshaller.h"
#include "JSONHandler/ButtonCapabilitiesMarshaller.h"
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>

using namespace RPC2Communication;

bool GetCapabilitiesResponseMarshaller::fromString(const std::string& s,GetCapabilitiesResponse& e)
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


const std::string GetCapabilitiesResponseMarshaller::toString(const GetCapabilitiesResponse& e)
{
  Json::FastWriter writer;
  return writer.write(toJSON(e));
}


Json::Value GetCapabilitiesResponseMarshaller::toJSON(const GetCapabilitiesResponse& e)
{  
  Json::Value json(Json::objectValue);
  
  json["jsonrpc"]=Json::Value("2.0");
  
  Json::Value j=Json::Value(Json::arrayValue);
  for ( int i = 0; i < e.mCapabilities.size(); ++i )
  {
    Json::Value cpb = Json::Value(Json::objectValue);
    cpb["capabilities"] = ButtonCapabilitiesMarshaller::toJSON(e.mCapabilities[i].capability);
    cpb["button"] = e.mCapabilities[i].button;
    j.append(cpb);
  }
      
  json["result"]=j;

  json["id"] = e.getID();

  return json;
}


bool GetCapabilitiesResponseMarshaller::fromJSON(const Json::Value& json,GetCapabilitiesResponse& c)
{
  try
  {
    if(!json.isObject())  return false;
    
    if(!json.isMember("jsonrpc"))  return false;

    if(!json.isMember("result"))  return false;
    Json::Value j=json["result"];
    if(!j.isArray())  return false;

    for ( int i = 0; i < j.size(); ++i )
    {
        GetCapabilitiesResponse::GetCapabilitiesResponseInternal cpb;
        if (!j[i].isMember("button")) return false;
        if (!ButtonCapabilitiesMarshaller::fromJSON(j[i]["capabilities"], cpb.capability)) return false;
        if (!j[i].isMember("button") || !j[i]["button"].isString()) return false;
        cpb.button = j[i]["button"].asString();
        c.mCapabilities.push_back(cpb);
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

