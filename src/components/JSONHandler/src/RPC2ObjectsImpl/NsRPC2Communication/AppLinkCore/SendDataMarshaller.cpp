#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/AppLinkCore/SendData.h"
#include "../src/ALRPCObjectsImpl/V1/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataMarshaller.h"

/*
  interface	NsRPC2Communication::AppLinkCore
  version	1.2
  generated at	Tue Dec  4 15:06:30 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsRPC2Communication::AppLinkCore;

bool SendDataMarshaller::checkIntegrity(SendData& s)
{
  return checkIntegrityConst(s);
}


bool SendDataMarshaller::fromString(const std::string& s,SendData& e)
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


const std::string SendDataMarshaller::toString(const SendData& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool SendDataMarshaller::checkIntegrityConst(const SendData& s)
{
  {
    unsigned int i=s.data.size();
    if(i<1)  return false;
    if(i>100)  return false;
  }

  return true;
}


Json::Value SendDataMarshaller::toJSON(const SendData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("AppLinkCore.SendData");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  {
    unsigned int i=e.data.size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.data[i]);

    json["params"]["data"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool SendDataMarshaller::fromJSON(const Json::Value& json,SendData& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("AppLinkCore.SendData"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("data") || !js["data"].isArray())
      return false;
    {
      c.data.clear();
      unsigned int i=js["data"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      c.data.resize(i);
      while(i--)
      {
        if(!js["data"][i].isString())
          return false;
        c.data[i]=js["data"][i].asString();
        
      }
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
