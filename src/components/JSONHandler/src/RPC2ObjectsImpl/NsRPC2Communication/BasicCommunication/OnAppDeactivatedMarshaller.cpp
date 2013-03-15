//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/BasicCommunication/OnAppDeactivated.h"
#include "../src/SDLRPCObjectsImpl/V2/DeactivateReasonMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/BasicCommunication/OnAppDeactivatedMarshaller.h"

/*
  interface	NsRPC2Communication::BasicCommunication
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::BasicCommunication;

bool OnAppDeactivatedMarshaller::checkIntegrity(OnAppDeactivated& s)
{
  return checkIntegrityConst(s);
}


bool OnAppDeactivatedMarshaller::fromString(const std::string& s,OnAppDeactivated& e)
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


const std::string OnAppDeactivatedMarshaller::toString(const OnAppDeactivated& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnAppDeactivatedMarshaller::checkIntegrityConst(const OnAppDeactivated& s)
{
  if(s.appName.length()>100)  return false;

  if(!NsSmartDeviceLinkRPCV2::DeactivateReasonMarshaller::checkIntegrityConst(s.reason))  return false;

  return true;
}


Json::Value OnAppDeactivatedMarshaller::toJSON(const OnAppDeactivated& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("BasicCommunication.OnAppDeactivated");

  json["params"]=Json::Value(Json::objectValue);
  json["params"]["appName"]=Json::Value(e.appName);;
  json["params"]["reason"]=NsSmartDeviceLinkRPCV2::DeactivateReasonMarshaller::toJSON(e.reason);;
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool OnAppDeactivatedMarshaller::fromJSON(const Json::Value& json,OnAppDeactivated& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("BasicCommunication.OnAppDeactivated"))  return false;
    
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(!js.isMember("appName") || !js["appName"].isString())  return false;
    c.appName=js["appName"].asString();
    if(c.appName.length()>100)  return false;
    if(!js.isMember("reason") || !NsSmartDeviceLinkRPCV2::DeactivateReasonMarshaller::fromJSON(js["reason"],c.reason))  return false;

    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
