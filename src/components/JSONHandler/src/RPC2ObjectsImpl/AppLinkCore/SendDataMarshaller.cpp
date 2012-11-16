#include "../include/JSONHandler/RPC2Objects/AppLinkCore/SendData.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "SendDataMarshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::AppLinkCore;

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
  unsigned int i=s.data.size();
  if(i<1)  return false;
  if(i>100)  return false;

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
      j[i]=e.data[i];

    json["params"]["data"]=j;
  }
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

    if(!js.isMember("data") || !js["data"].isArray())  return false;
    {
      unsigned int i=js["data"].size();
      if(i<1)  return false;
      if(i>100)  return false;
      std::vector<std::string> z(i);
      while(i--)
          c.data[i].assign(js["data"][i].asString());
      c.data=z;
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

