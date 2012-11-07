#include "../include/JSONHandler/RPC2Objects/UI/ResetGlobalProperties.h"
#include "../src/ALRPCObjectsImpl/GlobalPropertyMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "ResetGlobalPropertiesMarshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::UI;

bool ResetGlobalPropertiesMarshaller::checkIntegrity(ResetGlobalProperties& s)
{
  return checkIntegrityConst(s);
}


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
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ResetGlobalPropertiesMarshaller::checkIntegrityConst(const ResetGlobalProperties& s)
{
  {
    unsigned int i=s.properties.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value ResetGlobalPropertiesMarshaller::toJSON(const ResetGlobalProperties& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.ResetGlobalProperties");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  {
    unsigned int i=e.properties.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=GlobalPropertyMarshaller::toJSON(e.properties[i]);

    json["params"]["properties"]=j;
  }
  return json;
}


bool ResetGlobalPropertiesMarshaller::fromJSON(const Json::Value& json,ResetGlobalProperties& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.ResetGlobalProperties"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("properties") || !js["properties"].isArray())  return false;
    {
      unsigned int i=js["properties"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      std::vector<GlobalProperty> z(i);
      while(i--)
        if(!GlobalPropertyMarshaller::fromJSON(js["properties"][i],c.properties[i]))  return false;
      c.properties=z;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
