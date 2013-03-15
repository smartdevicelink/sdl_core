//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "../src/SDLRPCObjectsImpl/V2/ButtonNameMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ButtonEventModeMarshaller.h"
#include "../src/SDLRPCObjectsImpl/V2/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/Buttons/OnButtonEventMarshaller.h"

/*
  interface	NsRPC2Communication::Buttons
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

using namespace NsRPC2Communication::Buttons;

bool OnButtonEventMarshaller::checkIntegrity(OnButtonEvent& s)
{
  return checkIntegrityConst(s);
}


bool OnButtonEventMarshaller::fromString(const std::string& s,OnButtonEvent& e)
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


const std::string OnButtonEventMarshaller::toString(const OnButtonEvent& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool OnButtonEventMarshaller::checkIntegrityConst(const OnButtonEvent& s)
{
  if(!NsSmartDeviceLinkRPCV2::ButtonNameMarshaller::checkIntegrityConst(s.name))  return false;

  if(!NsSmartDeviceLinkRPCV2::ButtonEventModeMarshaller::checkIntegrityConst(s.mode))  return false;

  if(s.customButtonID && (s.customButtonID[0]>65536))  return false;

  return true;
}


Json::Value OnButtonEventMarshaller::toJSON(const OnButtonEvent& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("Buttons.OnButtonEvent");
  json["params"]=Json::Value(Json::objectValue);

  json["params"]["name"]=NsSmartDeviceLinkRPCV2::ButtonNameMarshaller::toJSON(e.name);;
  json["params"]["mode"]=NsSmartDeviceLinkRPCV2::ButtonEventModeMarshaller::toJSON(e.mode);;
  if(e.customButtonID)
    json["params"]["customButtonID"]=Json::Value(e.customButtonID[0]);;
  return json;
}


bool OnButtonEventMarshaller::fromJSON(const Json::Value& json,OnButtonEvent& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("Buttons.OnButtonEvent"))  return false;
    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;

    if(!js.isMember("name") || !NsSmartDeviceLinkRPCV2::ButtonNameMarshaller::fromJSON(js["name"],c.name))  return false;

    if(!js.isMember("mode") || !NsSmartDeviceLinkRPCV2::ButtonEventModeMarshaller::fromJSON(js["mode"],c.mode))  return false;

    if(c.customButtonID)  delete c.customButtonID;
    c.customButtonID=0;
    if(js.isMember("customButtonID"))
    {
      if(!js["customButtonID"].isInt())  return false;
      c.customButtonID=new unsigned int();
      c.customButtonID[0]=js["customButtonID"].asInt();
      if(c.customButtonID[0]>65536)  return false;

    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
