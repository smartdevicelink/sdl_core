#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/ALRPCObjectsImpl/V1/GlobalPropertyMarshaller.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ResetGlobalPropertiesMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

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
      j[i]=NsAppLinkRPC::GlobalPropertyMarshaller::toJSON(e.properties[i]);

    json["params"]["properties"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
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
      std::vector<NsAppLinkRPC::GlobalProperty> z(i);
      while(i--)
        if(!NsAppLinkRPC::GlobalPropertyMarshaller::fromJSON(js["properties"][i],c.properties[i]))  return false;
      c.properties=z;
    }

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
