#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/SendData.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/SendDataMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::BasicCommunication;

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

  if(s.url && (s.url[0].length()>1000))  return false;

  if(s.timeout && (s.timeout[0]<-2000000000 || s.timeout[0]>2000000000))  return false;

  return true;
}


Json::Value SendDataMarshaller::toJSON(const SendData& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("BasicCommunication.SendData");

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
  if(e.url)
    json["params"]["url"]=Json::Value(e.url[0]);;
  if(e.timeout)
    json["params"]["timeout"]=Json::Value(e.timeout[0]);;
  return json;
}


bool SendDataMarshaller::fromJSON(const Json::Value& json,SendData& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.SendData"))  return false;
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


    if(c.url)  delete c.url;
    c.url=0;
    if(js.isMember("url"))
    {
      if(!js["url"].isString())  return false;
      c.url=new std::string();
      c.url[0]=js["url"].asString();
      if(c.url[0].length()>1000)  return false;

    }

    if(c.timeout)  delete c.timeout;
    c.timeout=0;
    if(js.isMember("timeout"))
    {
      if(!js["timeout"].isInt())  return false;
      c.timeout=new int();
      c.timeout[0]=js["timeout"].asInt();
      if(c.timeout[0]<-2000000000 || c.timeout[0]>2000000000)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
