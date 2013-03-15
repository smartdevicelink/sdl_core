//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/GetAppList.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/GetAppListMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;

bool GetAppListMarshaller::checkIntegrity(GetAppList& s)
{
  return checkIntegrityConst(s);
}


bool GetAppListMarshaller::fromString(const std::string& s,GetAppList& e)
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


const std::string GetAppListMarshaller::toString(const GetAppList& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool GetAppListMarshaller::checkIntegrityConst(const GetAppList& s)
{
  return true;
}


Json::Value GetAppListMarshaller::toJSON(const GetAppList& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("BasicCommunication.GetAppList");

  json["id"]=Json::Value(e.getId());
  return json;
}


bool GetAppListMarshaller::fromJSON(const Json::Value& json,GetAppList& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.GetAppList"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
