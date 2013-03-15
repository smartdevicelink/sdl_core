//
// Copyright (c) 2013 Ford Motor Company
//

#include "../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppUnregistered.h"
#include "../src/SDLRPCObjectsImpl/V2/AppInterfaceUnregisteredReasonMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/OnAppUnregisteredMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;

bool OnAppUnregisteredMarshaller::checkIntegrity(OnAppUnregistered& s)
{
  return checkIntegrityConst(s);
}


bool OnAppUnregisteredMarshaller::fromString(const std::string& s,OnAppUnregistered& e)
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


const std::string OnAppUnregisteredMarshaller::toString(const OnAppUnregistered& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppUnregisteredMarshaller::checkIntegrityConst(const OnAppUnregistered& s)
{
  if(s.appName.length()>100)  return false;

  if(s.reason && (!NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReasonMarshaller::checkIntegrityConst(s.reason[0])))  return false;

  return true;
}


Json::Value OnAppUnregisteredMarshaller::toJSON(const OnAppUnregistered& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("BasicCommunication.OnAppUnregistered");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["appName"]=Json::Value(e.appName);;
  if(e.reason)
    json["params"]["reason"]=NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReasonMarshaller::toJSON(e.reason[0]);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool OnAppUnregisteredMarshaller::fromJSON(const Json::Value& json,OnAppUnregistered& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.OnAppUnregistered"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;

    if(c.reason)  delete c.reason;
    c.reason=0;
    if(js.isMember("reason"))
    {
      c.reason=new NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReason();
      if(!NsSmartDeviceLinkRPCV2::AppInterfaceUnregisteredReasonMarshaller::fromJSON(js["reason"],c.reason[0]))  return false;
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
